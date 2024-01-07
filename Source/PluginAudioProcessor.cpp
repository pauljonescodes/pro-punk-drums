#include "PluginAudioProcessor.h"
#include "PluginAudioProcessorEditor.h"
#include "Configuration/Samples.h"
#include "Synthesiser/PluginSynthesiser.h"
#include "PluginUtils.h"
#include "Configuration/Midi.h"
#include "Configuration/Channels.h"
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "Configuration/Strings.h"
#include "Configuration/Parameters.h"
#include "JucePluginDefines.h"

PluginAudioProcessor::PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput(PluginUtils::toTitleCase(Channels::outputId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(Channels::kickId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(Channels::snareId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(Channels::tomsId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(Channels::hiHatId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(Channels::cymbalsId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(Channels::percussionId), juce::AudioChannelSet::stereo(), true)
#endif
	), 
	mParameterValueTreeStatePtr(std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, juce::Identifier("plugin_params"), createParameterLayout())),
	mAudioFormatManagerPtr(std::make_unique<juce::AudioFormatManager>())
#endif
{
	mAudioFormatManagerPtr->registerBasicFormats();

	mParameterValueTreeStatePtr->state.setProperty(PluginPresetManager::presetNameProperty, "", nullptr);
	mParameterValueTreeStatePtr->state.setProperty("version", ProjectInfo::versionString, nullptr);

	mPresetManagerPtr = std::make_unique<PluginPresetManager>(*mParameterValueTreeStatePtr.get());

	for (int channelIndex = 0; channelIndex < Channels::size; channelIndex++) {
		if (channelIndex != 0)
		{
			mSynthesiserPtrVector.push_back(std::make_unique<PluginSynthesiser>());
			mInternalBufferPtrVector.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 1024));
		}

		mCompressors.push_back(std::make_unique<juce::dsp::Compressor<float>>());
		mChannelGains.push_back(std::make_unique<juce::dsp::Gain<float>>());
		mLowShelfFilters.emplace_back();
		mPeakFilters.emplace_back();
		mHighShelfFilters.emplace_back();
	}

	for (int resourceIndex = 0; resourceIndex < BinaryData::namedResourceListSize; resourceIndex++)
	{
		std::string namedResource = BinaryData::namedResourceList[resourceIndex];
		std::string micId, midiName, generalMidiSnakeCaseName = "";
		int variationIndex, velocityIndex, midiNote = -1;

		for (int midiNameIndex = 0; midiNameIndex < GeneralMidiPercussion::midiNamesVector.size(); midiNameIndex++)
		{
			midiNote = -1;
			midiName = GeneralMidiPercussion::midiNamesVector.at(midiNameIndex);
			generalMidiSnakeCaseName = PluginUtils::toSnakeCase(midiName);

			std::string cleanedNamedResource = namedResource.substr(0, namedResource.size() - 4);
			size_t pos = cleanedNamedResource.find(generalMidiSnakeCaseName);
			if (pos == 0)
			{
				std::string resourceMetadata = cleanedNamedResource.substr(pos + generalMidiSnakeCaseName.length());

				std::istringstream iss(resourceMetadata);
				std::vector<std::string> parts;
				std::string part;

				while (std::getline(iss, part, '_'))
				{
					if (!part.empty()) {
						parts.push_back(part);
					}
				}

				switch (parts.size())
				{
				case 0:
					midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
					velocityIndex = 0;
					variationIndex = 0;
					break;
				case 1:
					midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
					velocityIndex = 0;
					if (PluginUtils::isNumeric(parts[0]))
					{
						variationIndex = std::stoi(parts[0].substr(0, parts[0].find("_"))) - 1;
					}
					else
					{
						variationIndex = 0;
						micId = parts[0];
					}
					break;
				case 2:
					if (PluginUtils::isNumeric(parts[0]) && PluginUtils::isNumeric(parts[1]))
					{
						midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
						velocityIndex = std::stoi(parts[0]) - 1;
						variationIndex = std::stoi(parts[1]) - 1;
					}
					else
					{
						midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
						velocityIndex = 0;
						variationIndex = std::stoi(parts[0]) - 1;
						micId = parts[1];
					}
					break;
				case 3:
					if (PluginUtils::isNumeric(parts[0]) && PluginUtils::isNumeric(parts[1]))
					{
						midiNote = GeneralMidiPercussion::midiNameToNoteMap.at(midiName);
						velocityIndex = std::stoi(parts[0]) - 1;
						variationIndex = std::stoi(parts[1]) - 1;
						micId = parts[2];
					}
					else
					{
						DBG("invalid name " + generalMidiSnakeCaseName);
						midiNameIndex = (int)GeneralMidiPercussion::midiNamesVector.size() + 1;
					}
					break;
				default:
					DBG("invalid name " + generalMidiSnakeCaseName);
					midiNameIndex = (int)GeneralMidiPercussion::midiNamesVector.size() + 1;
					break;
				}
			}

			if (midiNote != -1)
			{
				auto* gainParameter = mParameterValueTreeStatePtr->getParameter(PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::gainComponentId }));
				auto* panParameter = mParameterValueTreeStatePtr->getParameter(PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::panComponentId }));
				auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeStatePtr->getParameter(PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::phaseComponentId })));

				auto& synthesiser = mSynthesiserPtrVector.at(Channels::generalMidiNoteToChannelIndex.at(midiNote));

				synthesiser->addSample(
					namedResource,
					Samples::bitRate,
					Samples::bitDepth,
					midiNote,
					micId,
					GeneralMidiPercussion::midiNoteToStopNotesMap.at(midiNote),
					velocityIndex,
					variationIndex,
					*mAudioFormatManagerPtr.get(),
					*gainParameter,
					*panParameter,
					*phaseParameter
				);
			}
		}
	}

	for (const auto& channel : Channels::channelIndexToIdMap) {
		const auto& channelId = channel.second;

		const auto compressionThresholdId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
		mParameterValueTreeStatePtr->addParameterListener(compressionThresholdId, this);
		
		const auto compressionRatioId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
		mParameterValueTreeStatePtr->addParameterListener(compressionRatioId, this);

		const auto compressionAttackId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::attackId });
		mParameterValueTreeStatePtr->addParameterListener(compressionAttackId, this);

		const auto compressionReleaseId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
		mParameterValueTreeStatePtr->addParameterListener(compressionReleaseId, this);

		for (const auto& equalizationTypeIdToDefaultFrequency : AudioParameters::equalizationTypeIdToDefaultFrequencyMap) {
			const auto& equalizationTypeId = equalizationTypeIdToDefaultFrequency.first;
			
			const auto eqFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::frequencyComponentId });
			mParameterValueTreeStatePtr->addParameterListener(eqFrequencyId, this);

			const auto eqQualityId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::qualityComponentId });
			mParameterValueTreeStatePtr->addParameterListener(eqQualityId, this);

			const auto eqGainId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::gainComponentId });
			mParameterValueTreeStatePtr->addParameterListener(eqGainId, this);
		}

		const auto channelGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
		mParameterValueTreeStatePtr->addParameterListener(channelGainId, this);
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ AudioParameters::multiOutComponentId, 1 }, Strings::multiOut, false));

	for (const auto& pair : AudioParameters::getUniqueMidiNoteMicCombinations()) {
		int midiNote = pair.first;
		const std::set<std::string>& micIds = pair.second;

		for (const std::string& micId : micIds) {
			auto gainParameterId = PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::gainComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ gainParameterId, 1 }, 
				PluginUtils::toTitleCase(gainParameterId), 
				AudioParameters::gainNormalizableRange, 
				AudioParameters::gainDeciblesDefaultValue));

			auto panParameterId = PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::panComponentId });
			bool containsRight = micId.find(Samples::rightComponentId) != std::string::npos;
			bool containsLeft = micId.find(Samples::leftComponentId) != std::string::npos;
			float defaultPan = containsRight ? 1.0f : containsLeft ? -1.0f : 0;
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ panParameterId, 1 }, 
				PluginUtils::toTitleCase(panParameterId), 
				AudioParameters::panNormalizableRange, 
				defaultPan));

			auto phaseParameterId = PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::phaseComponentId });
			bool containsBottom = micId.find(Samples::bottomComponentId) != std::string::npos;
			layout.add(std::make_unique<juce::AudioParameterBool>(
				juce::ParameterID{ phaseParameterId, 1 }, 
				PluginUtils::toTitleCase(phaseParameterId), 
				containsBottom));
		}
	}

	for (const auto& channel : Channels::channelIndexToIdMap) {
		const auto& channelId = channel.second;				
		
		const auto compressionThresholdId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionThresholdId, 1 }, 
			PluginUtils::toTitleCase(compressionThresholdId), 
			AudioParameters::thresholdNormalizableRange, 
			AudioParameters::thresholdDefaultValue));
		
		const auto compressionRatioId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionRatioId, 1 }, 
			PluginUtils::toTitleCase(compressionRatioId), 
			AudioParameters::ratioNormalizableRange, 
			AudioParameters::ratioDefaultValue));
		
		const auto compressionAttackId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::attackId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionAttackId, 1 }, 
			PluginUtils::toTitleCase(compressionAttackId), 
			AudioParameters::attackNormalizableRange, 
			AudioParameters::attackDefaultValue));
		
		const auto compressionReleaseId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionReleaseId, 1 }, 
			PluginUtils::toTitleCase(compressionReleaseId), 
			AudioParameters::releaseNormalizableRange, 
			AudioParameters::releaseDefaultValue));

		for (const auto& equalizationTypeId : AudioParameters::equalizationTypeIdVector) {
			const auto equalizationFrequencyDefaultValue = AudioParameters::equalizationTypeIdToDefaultFrequencyMap.at(equalizationTypeId);

			const auto eqOnId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::onComponentId });
			layout.add(std::make_unique<juce::AudioParameterBool>(
				juce::ParameterID{ eqOnId, 1 }, 
				PluginUtils::toTitleCase(eqOnId), false));
			
			const auto eqFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::frequencyComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqFrequencyId, 1 },
				PluginUtils::toTitleCase(eqFrequencyId), 
				AudioParameters::frequencyNormalizableRange, 
				equalizationFrequencyDefaultValue));
			
			const auto eqQualityId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::qualityComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqQualityId, 1 },
				PluginUtils::toTitleCase(eqQualityId), 
				AudioParameters::qualityNormalizableRange, 
				AudioParameters::qualityDefaultValue));

			const auto eqGainId = PluginUtils::joinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::gainComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqGainId, 1 },
				PluginUtils::toTitleCase(eqGainId), 
				AudioParameters::eqGainNormalizableRange,
				AudioParameters::gainDefaultValue));
		}

		const auto channelGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ channelGainId, 1 }, 
			PluginUtils::toTitleCase(channelGainId), 
			AudioParameters::gainNormalizableRange, 
			AudioParameters::gainDeciblesDefaultValue));
	}

	return layout;
}

PluginAudioProcessor::~PluginAudioProcessor()
{
}

std::vector<int> PluginAudioProcessor::getMidiNotesVector()
{
	std::vector<int> midiNotesVector;

	for (const auto& synthesiser : mSynthesiserPtrVector)
	{
		auto synthesiserMidiNotesVector = synthesiser.get()->getMidiNotesVector();
		midiNotesVector.insert(midiNotesVector.end(), synthesiserMidiNotesVector.begin(), synthesiserMidiNotesVector.end());
	}

	std::sort(midiNotesVector.begin(), midiNotesVector.end());

	return midiNotesVector;
}

const juce::String PluginAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

juce::AudioProcessorValueTreeState& PluginAudioProcessor::getParameterValueTreeState() const
{
	return *mParameterValueTreeStatePtr;
}

bool PluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool PluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool PluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double PluginAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int PluginAudioProcessor::getNumPrograms()
{
	const auto numPrograms = mPresetManagerPtr->getAllPresets().size();
	DBG("getNumPrograms " + numPrograms);
	return numPrograms;
}

int PluginAudioProcessor::getCurrentProgram()
{
	DBG("getCurrentProgram " + 0);
	return mPresetManagerPtr->getCurrentPresetIndex();
}

void PluginAudioProcessor::setCurrentProgram(int index)
{
	DBG("setCurrentProgram " + index);
	mPresetManagerPtr->loadPresetAt(index);
}

const juce::String PluginAudioProcessor::getProgramName(int index)
{
	DBG("getProgramName " + index);
	return mPresetManagerPtr->getAllPresets()[index];
}

void PluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	DBG("prepareToPlay");

	juce::dsp::ProcessSpec spec;
	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock; // Example block size
	spec.numChannels = getTotalNumOutputChannels(); // Stereo

	for (const auto& synthesiser : mSynthesiserPtrVector)
	{
		synthesiser->setCurrentPlaybackSampleRate(sampleRate);
	}

	for (const auto& buffer : mInternalBufferPtrVector)
	{
		buffer->setSize(2, samplesPerBlock);
	}

	for (const auto& channel : Channels::channelIndexToIdMap) {
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		const auto& compressor = mCompressors[channelIndex];
		compressor->prepare(spec);

		const auto compressionThresholdId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
		compressor->setThreshold(mParameterValueTreeStatePtr->getParameterAsValue(compressionThresholdId).getValue());

		const auto compressionRatioId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
		compressor->setRatio(mParameterValueTreeStatePtr->getParameterAsValue(compressionRatioId).getValue());

		const auto compressionAttackId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::attackId });
		compressor->setAttack(mParameterValueTreeStatePtr->getParameterAsValue(compressionAttackId).getValue());

		const auto compressionReleaseId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
		compressor->setRelease(mParameterValueTreeStatePtr->getParameterAsValue(compressionReleaseId).getValue());

		// EQ - Low Shelf
		const auto lowShelfFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
		const float lowShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfFrequencyId).getValue();

		const auto lowShelfQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId });
		const float lowShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfQualityId).getValue();

		const auto lowShelfGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::gainComponentId });
		const float lowShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfGainId).getValue();

		*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, lowShelfCenterFrequency, lowShelfQuality, lowShelfGain);
		mLowShelfFilters[channelIndex].prepare(spec);

		// EQ - Peak Filter
		const auto peakFilterCenterFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::frequencyComponentId });
		const float peakFilterCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterCenterFrequencyId).getValue();

		const auto peakFilterQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::qualityComponentId });
		const float peakFilterQuality = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterQualityId).getValue();

		const auto peakFilterGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::gainComponentId });
		const float peakFilterGain = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterGainId).getValue();

		*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, peakFilterCenterFrequency, peakFilterQuality, peakFilterGain);
		mPeakFilters[channelIndex].prepare(spec);

		// EQ - High Shelf
		const auto highShelfFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
		const float highShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(highShelfFrequencyId).getValue();

		const auto highShelfQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::qualityComponentId });
		const float highShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(highShelfQualityId).getValue();

		const auto highShelfGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::gainComponentId });
		const float highShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(highShelfGainId).getValue();

		*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, highShelfCenterFrequency, highShelfQuality, highShelfGain);
		mHighShelfFilters[channelIndex].prepare(spec);

		// Channel Gain
		const auto channelGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
		const float channelGain = mParameterValueTreeStatePtr->getParameterAsValue(channelGainId).getValue();
		mChannelGains[channelIndex]->setGainDecibels(channelGain);
		mChannelGains[channelIndex]->prepare(spec);
	}

}

void PluginAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& outputBuffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		outputBuffer.clear(i, 0, outputBuffer.getNumSamples());
	}

	auto* multiOutParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeStatePtr->getParameter(AudioParameters::multiOutComponentId));
	bool isMultiOut = multiOutParameter->get() && totalNumOutputChannels > 2;

	for (const auto& channel : Channels::channelIndexToIdMap) {
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		if (channelIndex == Channels::outputChannelIndex) {
			break;
		}

		auto& synthesiser = mSynthesiserPtrVector[channelIndex];
		auto& internalBufferPtr = mInternalBufferPtrVector[channelIndex];
		
		internalBufferPtr->clear();
		synthesiser->renderNextBlock(*internalBufferPtr, midiMessages, 0, internalBufferPtr->getNumSamples());

		juce::dsp::AudioBlock<float> block(*internalBufferPtr);
		juce::dsp::ProcessContextReplacing<float> context(block);

		auto& compressor = mCompressors[channelIndex];
		compressor->process(context);

		const auto lowShelfOnId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::onComponentId });
		if (mParameterValueTreeStatePtr->getParameterAsValue(lowShelfOnId).getValue()) {
			auto& highPass = mLowShelfFilters[channelIndex];
			highPass.process(context);
		}

		const auto peakFilterOnId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::onComponentId });
		if (mParameterValueTreeStatePtr->getParameterAsValue(peakFilterOnId).getValue()) {
			auto& peakFilter = mPeakFilters[channelIndex];
			peakFilter.process(context);
		}

		const auto highShelfOnId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::onComponentId });
		if (mParameterValueTreeStatePtr->getParameterAsValue(highShelfOnId).getValue()) {
			auto& lowPass = mHighShelfFilters[channelIndex];
			lowPass.process(context);
		}

		auto& channelGain = mChannelGains[channelIndex];
		channelGain->process(context);

		// send to output

		const float* inL = internalBufferPtr->getReadPointer(0);
		const float* inR = internalBufferPtr->getNumChannels() > 1 ? internalBufferPtr->getReadPointer(1) : nullptr;

		int leftChannelIndex = isMultiOut ? 2 + (channelIndex * 2) : 0;
		int rightChannelIndex = isMultiOut ? 2 + (channelIndex * 2) + 1 : 1;

		float* outL = outputBuffer.getWritePointer(leftChannelIndex);
		float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(rightChannelIndex) : nullptr;

		for (int sample = 0; sample < internalBufferPtr->getNumSamples(); ++sample)
		{
			outL[sample] += inL[sample];
			if (outR && inR)
			{
				outR[sample] += inR[sample];
			}
		}
	}

	juce::dsp::AudioBlock<float> block(outputBuffer);
	juce::dsp::ProcessContextReplacing<float> context(block);

	auto& compressor = mCompressors[Channels::size - 1];
	compressor->process(context);

	const auto lowShelfOnId = PluginUtils::joinAndSnakeCase({ Channels::outputId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::onComponentId });
	if (mParameterValueTreeStatePtr->getParameterAsValue(lowShelfOnId).getValue()) {
		auto& highPass = mLowShelfFilters[Channels::size - 1];
		highPass.process(context);
	}

	const auto peakFilterOnId = PluginUtils::joinAndSnakeCase({ Channels::outputId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::onComponentId });
	if (mParameterValueTreeStatePtr->getParameterAsValue(peakFilterOnId).getValue()) {
		auto& peakFilter = mPeakFilters[Channels::size - 1];
		peakFilter.process(context);
	}

	const auto highShelfOnId = PluginUtils::joinAndSnakeCase({ Channels::outputId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::onComponentId });
	if (mParameterValueTreeStatePtr->getParameterAsValue(highShelfOnId).getValue()) {
		auto& lowPass = mHighShelfFilters[Channels::size - 1];
		lowPass.process(context);
	}

    auto& channelGain = mChannelGains[mChannelGains.size() - 1];
    channelGain->process(context);
}

void PluginAudioProcessor::parameterChanged(const juce::String& parameterId, float newValue)
{
	DBG("parameterChanged " + parameterId.toStdString() + " " + std::to_string(newValue));

	auto sampleRate = getSampleRate();

	for (const auto& channel : Channels::channelIndexToIdMap) {
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		const auto& compressor = mCompressors[channelIndex];

		const auto compressionThresholdId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionThresholdId.c_str()) == 0) {
			compressor->setThreshold(newValue);
		}
		
		const auto compressionRatioId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionRatioId.c_str()) == 0) {
			compressor->setRatio(newValue);
		}
		
		const auto compressionAttackId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::attackId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionAttackId.c_str()) == 0) {
			compressor->setAttack(newValue);
		}
		
		const auto compressionReleaseId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionReleaseId.c_str()) == 0) {
			compressor->setRelease(newValue);
		}

		const auto lowShelfFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
		const float lowShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfFrequencyId).getValue();
		
		const auto lowShelfQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId });
		const float lowShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfQualityId).getValue();

		const auto lowShelfGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::gainComponentId });
		const float lowShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfGainId).getValue();

		if (std::strcmp(parameterId.toRawUTF8(), lowShelfFrequencyId.c_str()) == 0) {
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, newValue),
				std::max(AudioParameters::qualityMinimumValue, lowShelfQuality),
				std::max(AudioParameters::gainDecibelsMinimumValue, lowShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), lowShelfQualityId.c_str()) == 0) {
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, lowShelfCenterFrequency),
				std::max(AudioParameters::qualityMinimumValue, newValue),
				std::max(AudioParameters::gainDecibelsMinimumValue, lowShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), lowShelfGainId.c_str()) == 0) {
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, lowShelfCenterFrequency),
				std::max(AudioParameters::qualityMinimumValue, lowShelfQuality),
				std::max(AudioParameters::gainDecibelsMinimumValue, newValue));
		}

		const auto peakFilterCenterFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::frequencyComponentId });
		const float peakFilterCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterCenterFrequencyId).getValue();

		const auto peakFilterQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::qualityComponentId });
		const float peakFilterQuality = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterQualityId).getValue();

		const auto peakFilterGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::gainComponentId });
		const float peakFilterGain = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterGainId).getValue();
		
		if (std::strcmp(parameterId.toRawUTF8(), peakFilterCenterFrequencyId.c_str()) == 0) {
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, newValue),
				std::max(AudioParameters::qualityMinimumValue, peakFilterQuality),
				std::max(AudioParameters::peakFilterGainMinimumValue, peakFilterGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), peakFilterQualityId.c_str()) == 0) {
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, peakFilterCenterFrequency),
				std::max(AudioParameters::qualityMinimumValue, newValue),
				std::max(AudioParameters::peakFilterGainMinimumValue, peakFilterGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), peakFilterGainId.c_str()) == 0) {
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, peakFilterCenterFrequency),
				std::max(AudioParameters::qualityMinimumValue, peakFilterQuality),
				std::max(AudioParameters::peakFilterGainMinimumValue, newValue));
		}

		const auto highShelfFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
		const float highShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(highShelfFrequencyId).getValue();

		const auto highShelfQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::qualityComponentId });
		const float highShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(highShelfQualityId).getValue();

		const auto highShelfGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::gainComponentId });
		const float highShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(highShelfGainId).getValue();

		if (std::strcmp(parameterId.toRawUTF8(), highShelfFrequencyId.c_str()) == 0) {
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, newValue),
				std::max(AudioParameters::qualityMinimumValue, highShelfQuality),
				std::max(AudioParameters::gainMinimumValue, highShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), highShelfQualityId.c_str()) == 0) {
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, highShelfCenterFrequency),
				std::max(AudioParameters::qualityMinimumValue, newValue),
				std::max(AudioParameters::gainMinimumValue, highShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), highShelfGainId.c_str()) == 0) {
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
				sampleRate, 
				std::max(AudioParameters::frequencyMinimumValue, highShelfCenterFrequency),
				std::max(AudioParameters::qualityMinimumValue, highShelfQuality),
				std::max(AudioParameters::gainMinimumValue, newValue));
		}

		const auto channelGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::gainComponentId });

		if (std::strcmp(parameterId.toRawUTF8(), channelGainId.c_str()) == 0) {
			mChannelGains[channelIndex]->setGainDecibels(newValue);
		}
	}
}

bool PluginAudioProcessor::hasEditor() const
{
	return true; 
}

juce::AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
	return new PluginAudioProcessorEditor(*this);
}

void PluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	auto state = mParameterValueTreeStatePtr.get()->copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	const auto valueTreeFromXml = juce::ValueTree::fromXml(*xmlState);

	if (xmlState.get() != nullptr)
	{
		mParameterValueTreeStatePtr.get()->replaceState(valueTreeFromXml);
	}
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new PluginAudioProcessor();
}

void PluginAudioProcessor::noteOnSynthesisers(int midiNoteNumber, const float velocity)
{
	for (const auto& synthesiser : mSynthesiserPtrVector)
	{
		synthesiser->noteOn(0, midiNoteNumber, velocity);
	}
}


void PluginAudioProcessor::noteOnSynthesisers(int midiNoteNumber, const float velocity, std::string micId)
{
	for (const auto& synthesiser : mSynthesiserPtrVector)
	{
		synthesiser->noteOn(0, midiNoteNumber, velocity, micId);
	}
}

PluginPresetManager& PluginAudioProcessor::getPresetManager() 
{
	return *mPresetManagerPtr.get();
}
