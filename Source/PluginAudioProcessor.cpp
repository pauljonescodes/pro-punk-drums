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
		.withOutput(PluginUtils::toTitleCase(channels::outputId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(channels::kickId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(channels::snareId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(channels::tomsId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(channels::hiHatId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(channels::cymbalsId), juce::AudioChannelSet::stereo(), true)
		.withOutput(PluginUtils::toTitleCase(channels::percussionId), juce::AudioChannelSet::stereo(), true)
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

	for (int channelIndex = 1; channelIndex < channels::size; channelIndex++)
	{
		mSynthesiserPtrVector.push_back(std::make_unique<PluginSynthesiser>());
		mInternalBufferPtrVector.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 1024));
	}

	for (int channelIndex = 0; channelIndex < channels::size; channelIndex++) {
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

		for (int midiNameIndex = 0; midiNameIndex < generalmidi::midiNamesVector.size(); midiNameIndex++)
		{
			midiNote = -1;
			midiName = generalmidi::midiNamesVector.at(midiNameIndex);
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
					midiNote = generalmidi::midiNameToNoteMap.at(midiName);
					velocityIndex = 0;
					variationIndex = 0;
					break;
				case 1:
					midiNote = generalmidi::midiNameToNoteMap.at(midiName);
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
						midiNote = generalmidi::midiNameToNoteMap.at(midiName);
						velocityIndex = std::stoi(parts[0]) - 1;
						variationIndex = std::stoi(parts[1]) - 1;
					}
					else
					{
						midiNote = generalmidi::midiNameToNoteMap.at(midiName);
						velocityIndex = 0;
						variationIndex = std::stoi(parts[0]) - 1;
						micId = parts[1];
					}
					break;
				case 3:
					if (PluginUtils::isNumeric(parts[0]) && PluginUtils::isNumeric(parts[1]))
					{
						midiNote = generalmidi::midiNameToNoteMap.at(midiName);
						velocityIndex = std::stoi(parts[0]) - 1;
						variationIndex = std::stoi(parts[1]) - 1;
						micId = parts[2];
					}
					else
					{
						DBG("invalid name " + generalMidiSnakeCaseName);
						midiNameIndex = (int)generalmidi::midiNamesVector.size() + 1;
					}
					break;
				default:
					DBG("invalid name " + generalMidiSnakeCaseName);
					midiNameIndex = (int)generalmidi::midiNamesVector.size() + 1;
					break;
				}
			}

			if (midiNote != -1)
			{
				auto* gainParameter = mParameterValueTreeStatePtr->getParameter(PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::gainId }));
				auto* panParameter = mParameterValueTreeStatePtr->getParameter(PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::panId }));
				auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeStatePtr->getParameter(PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::phaseId })));

				auto& synthesiser = mSynthesiserPtrVector.at(channels::generalMidiNoteToChannelIndex.at(midiNote));

				synthesiser->addSample(
					namedResource,
					samples::bitRate,
					samples::bitDepth,
					midiNote,
					micId,
					generalmidi::midiNoteToStopNotesMap.at(midiNote),
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

	for (const auto& channel : channels::channelIndexToIdMap) {
		const auto& channelId = channel.second;

		const auto compressionThresholdId = PluginUtils::joinId({ channelId, parameters::thresholdId });
		mParameterValueTreeStatePtr->addParameterListener(compressionThresholdId, this);
		
		const auto compressionRatioId = PluginUtils::joinId({ channelId, parameters::ratioId });
		mParameterValueTreeStatePtr->addParameterListener(compressionRatioId, this);

		const auto compressionAttackId = PluginUtils::joinId({ channelId, parameters::attackId });
		mParameterValueTreeStatePtr->addParameterListener(compressionAttackId, this);

		const auto compressionReleaseId = PluginUtils::joinId({ channelId, parameters::releaseId });
		mParameterValueTreeStatePtr->addParameterListener(compressionReleaseId, this);

		for (const auto& equalizationTypeIdToDefaultFrequency : parameters::equalizationTypeIdToDefaultFrequencyMap) {
			const auto& equalizationTypeId = equalizationTypeIdToDefaultFrequency.first;
			
			const auto eqFrequencyId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::frequencyId });
			mParameterValueTreeStatePtr->addParameterListener(eqFrequencyId, this);

			const auto eqQualityId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::qualityId });
			mParameterValueTreeStatePtr->addParameterListener(eqQualityId, this);

			const auto eqGainId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::gainId });
			mParameterValueTreeStatePtr->addParameterListener(eqGainId, this);
		}

		const auto channelGainId = PluginUtils::joinId({ channelId, parameters::gainId });
		mParameterValueTreeStatePtr->addParameterListener(channelGainId, this);
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ parameters::multiOutId, 1 }, strings::multiOut, false));

	for (const auto& pair : PluginUtils::getUniqueMidiNoteMicCombinations()) {
		int midiNote = pair.first;
		const std::set<std::string>& micIds = pair.second;

		for (const std::string& micId : micIds) {
			auto gainParameterId = PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::gainId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ gainParameterId, 1 }, 
				PluginUtils::toTitleCase(gainParameterId), 
				parameters::gainNormalizableRange, 
				parameters::gainDefaultValue));

			auto panParameterId = PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::panId });
			bool containsRight = micId.find(samples::rightId) != std::string::npos;
			bool containsLeft = micId.find(samples::leftId) != std::string::npos;
			float defaultPan = containsRight ? 1.0f : containsLeft ? -1.0f : 0;
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ panParameterId, 1 }, 
				PluginUtils::toTitleCase(panParameterId), 
				parameters::panNormalizableRange, 
				defaultPan));

			auto phaseParameterId = PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::phaseId });
			bool containsBottom = micId.find(samples::bottomId) != std::string::npos;
			layout.add(std::make_unique<juce::AudioParameterBool>(
				juce::ParameterID{ phaseParameterId, 1 }, 
				PluginUtils::toTitleCase(phaseParameterId), 
				containsBottom));
		}
	}

	for (const auto& channel : channels::channelIndexToIdMap) {
		const auto& channelId = channel.second;				
		
		const auto compressionThresholdId = PluginUtils::joinId({ channelId, parameters::thresholdId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionThresholdId, 1 }, 
			PluginUtils::toTitleCase(compressionThresholdId), 
			parameters::thresholdNormalizableRange, 
			parameters::thresholdDefaultValue));
		
		const auto compressionRatioId = PluginUtils::joinId({ channelId, parameters::ratioId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionRatioId, 1 }, 
			PluginUtils::toTitleCase(compressionRatioId), 
			parameters::ratioNormalizableRange, 
			parameters::ratioDefaultValue));
		
		const auto compressionAttackId = PluginUtils::joinId({ channelId, parameters::attackId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionAttackId, 1 }, 
			PluginUtils::toTitleCase(compressionAttackId), 
			parameters::attackNormalizableRange, 
			parameters::attackDefaultValue));
		
		const auto compressionReleaseId = PluginUtils::joinId({ channelId, parameters::releaseId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionReleaseId, 1 }, 
			PluginUtils::toTitleCase(compressionReleaseId), 
			parameters::releaseNormalizableRange, 
			parameters::releaseDefaultValue));

		for (const auto& equalizationTypeId : parameters::equalizationTypeIdVector) {
			const auto equalizationFrequencyDefaultValue = parameters::equalizationTypeIdToDefaultFrequencyMap.at(equalizationTypeId);

			const auto eqOnId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::onId });
			layout.add(std::make_unique<juce::AudioParameterBool>(
				juce::ParameterID{ eqOnId, 1 }, 
				PluginUtils::toTitleCase(eqOnId), false));
			
			const auto eqFrequencyId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::frequencyId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqFrequencyId, 1 },
				PluginUtils::toTitleCase(eqFrequencyId), 
				parameters::frequencyNormalizableRange, 
				equalizationFrequencyDefaultValue));
			
			const auto eqQualityId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::qualityId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqQualityId, 1 },
				PluginUtils::toTitleCase(eqQualityId), 
				parameters::qualityNormalizableRange, 
				parameters::qualityDefaultValue));

			const auto eqGainId = PluginUtils::joinId({ channelId, equalizationTypeId, parameters::gainId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqGainId, 1 },
				PluginUtils::toTitleCase(eqGainId), 
				parameters::eqGainNormalizableRange,
				parameters::eqGainDefaultValue));
		}

		const auto channelGainId = PluginUtils::joinId({ channelId, parameters::gainId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ channelGainId, 1 }, 
			PluginUtils::toTitleCase(channelGainId), 
			parameters::gainNormalizableRange, 
			parameters::gainDefaultValue));
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
	spec.sampleRate = getSampleRate();
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

	for (const auto& channel : channels::channelIndexToIdMap) {
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		const auto& compressor = mCompressors[channelIndex];
		compressor->prepare(spec);

		const auto compressionThresholdId = PluginUtils::joinId({ channelId, parameters::thresholdId });
		compressor->setThreshold(mParameterValueTreeStatePtr->getParameterAsValue(compressionThresholdId).getValue());

		const auto compressionRatioId = PluginUtils::joinId({ channelId, parameters::ratioId });
		compressor->setRatio(mParameterValueTreeStatePtr->getParameterAsValue(compressionRatioId).getValue());

		const auto compressionAttackId = PluginUtils::joinId({ channelId, parameters::attackId });
		compressor->setAttack(mParameterValueTreeStatePtr->getParameterAsValue(compressionAttackId).getValue());

		const auto compressionReleaseId = PluginUtils::joinId({ channelId, parameters::releaseId });
		compressor->setRelease(mParameterValueTreeStatePtr->getParameterAsValue(compressionReleaseId).getValue());

		// EQ - Low Shelf
		const auto lowShelfFrequencyId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::frequencyId });
		const float lowShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfFrequencyId).getValue();

		const auto lowShelfQualityId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::qualityId });
		const float lowShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfQualityId).getValue();

		const auto lowShelfGainId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::gainId });
		const float lowShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfGainId).getValue();

		*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, lowShelfCenterFrequency, lowShelfQuality, lowShelfGain);
		mLowShelfFilters[channelIndex].prepare(spec);

		// EQ - Peak Filter
		const auto peakFilterCenterFrequencyId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::frequencyId });
		const float peakFilterCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterCenterFrequencyId).getValue();

		const auto peakFilterQualityId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::qualityId });
		const float peakFilterQuality = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterQualityId).getValue();

		const auto peakFilterGainId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::gainId });
		const float peakFilterGain = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterGainId).getValue();

		*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, peakFilterCenterFrequency, peakFilterQuality, peakFilterGain);
		mPeakFilters[channelIndex].prepare(spec);

		// EQ - High Shelf
		const auto highShelfFrequencyId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::frequencyId });
		const float highShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(highShelfFrequencyId).getValue();

		const auto highShelfQualityId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::qualityId });
		const float highShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(highShelfQualityId).getValue();

		const auto highShelfGainId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::gainId });
		const float highShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(highShelfGainId).getValue();

		*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, highShelfCenterFrequency, highShelfQuality, highShelfGain);
		mHighShelfFilters[channelIndex].prepare(spec);

		// Channel Gain
		const auto channelGainId = PluginUtils::joinId({ channelId, parameters::gainId });
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

	auto* multiOutParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeStatePtr->getParameter(parameters::multiOutId));
	bool isMultiOut = multiOutParameter->get() && totalNumOutputChannels > 2;

	for (const auto& channel : channels::channelIndexToIdMap) {
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		if (channelIndex == channels::outputIndex) {
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

		const auto lowShelfOnId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::onId });
		if (mParameterValueTreeStatePtr->getParameterAsValue(lowShelfOnId).getValue()) {
			auto& highPass = mLowShelfFilters[channelIndex];
			highPass.process(context);
		}

		const auto peakFilterOnId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::onId });
		if (mParameterValueTreeStatePtr->getParameterAsValue(peakFilterOnId).getValue()) {
			auto& peakFilter = mPeakFilters[channelIndex];
			peakFilter.process(context);
		}

		const auto highShelfOnId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::onId });
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

	auto& compressor = mCompressors[channels::size - 1];
	compressor->process(context);

	const auto lowShelfOnId = PluginUtils::joinId({ channels::outputId, parameters::lowShelfEqualizationTypeId, parameters::onId });
	if (mParameterValueTreeStatePtr->getParameterAsValue(lowShelfOnId).getValue()) {
		auto& highPass = mLowShelfFilters[channels::size - 1];
		highPass.process(context);
	}

	const auto peakFilterOnId = PluginUtils::joinId({ channels::outputId, parameters::peakFilterEqualizationTypeId, parameters::onId });
	if (mParameterValueTreeStatePtr->getParameterAsValue(peakFilterOnId).getValue()) {
		auto& peakFilter = mPeakFilters[channels::size - 1];
		peakFilter.process(context);
	}

	const auto highShelfOnId = PluginUtils::joinId({ channels::outputId, parameters::highShelfEqualizationTypeId, parameters::onId });
	if (mParameterValueTreeStatePtr->getParameterAsValue(highShelfOnId).getValue()) {
		auto& lowPass = mHighShelfFilters[channels::size - 1];
		lowPass.process(context);
	}

    auto& channelGain = mChannelGains[mChannelGains.size() - 1];
    channelGain->process(context);
}

void PluginAudioProcessor::parameterChanged(const juce::String& parameterId, float newValue)
{
	DBG("parameterChanged " + parameterId.toStdString() + " " + std::to_string(newValue));

	auto sampleRate = getSampleRate();

	for (const auto& channel : channels::channelIndexToIdMap) {
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		const auto& compressor = mCompressors[channelIndex];

		const auto compressionThresholdId = PluginUtils::joinId({ channelId, parameters::thresholdId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionThresholdId.c_str()) == 0) {
			compressor->setThreshold(newValue);
		}
		
		const auto compressionRatioId = PluginUtils::joinId({ channelId, parameters::ratioId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionRatioId.c_str()) == 0) {
			compressor->setRatio(newValue);
		}
		
		const auto compressionAttackId = PluginUtils::joinId({ channelId, parameters::attackId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionAttackId.c_str()) == 0) {
			compressor->setAttack(newValue);
		}
		
		const auto compressionReleaseId = PluginUtils::joinId({ channelId, parameters::releaseId });
		if (std::strcmp(parameterId.toRawUTF8(), compressionReleaseId.c_str()) == 0) {
			compressor->setRelease(newValue);
		}

		const auto lowShelfFrequencyId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::frequencyId });
		const float lowShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfFrequencyId).getValue();
		
		const auto lowShelfQualityId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::qualityId });
		const float lowShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfQualityId).getValue();

		const auto lowShelfGainId = PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::gainId });
		const float lowShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(lowShelfGainId).getValue();

		if (std::strcmp(parameterId.toRawUTF8(), lowShelfFrequencyId.c_str()) == 0) {
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, newValue),
				std::max(parameters::qualityMinimumValue, lowShelfQuality),
				std::max(parameters::gainMinimumValue, lowShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), lowShelfQualityId.c_str()) == 0) {
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, lowShelfCenterFrequency),
				std::max(parameters::qualityMinimumValue, newValue),
				std::max(parameters::gainMinimumValue, lowShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), lowShelfGainId.c_str()) == 0) {
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, lowShelfCenterFrequency),
				std::max(parameters::qualityMinimumValue, lowShelfQuality),
				std::max(parameters::gainMinimumValue, newValue));
		}

		const auto peakFilterCenterFrequencyId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::frequencyId });
		const float peakFilterCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterCenterFrequencyId).getValue();

		const auto peakFilterQualityId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::qualityId });
		const float peakFilterQuality = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterQualityId).getValue();

		const auto peakFilterGainId = PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::gainId });
		const float peakFilterGain = mParameterValueTreeStatePtr->getParameterAsValue(peakFilterGainId).getValue();
		
		if (std::strcmp(parameterId.toRawUTF8(), peakFilterCenterFrequencyId.c_str()) == 0) {
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, newValue),
				std::max(parameters::qualityMinimumValue, peakFilterQuality),
				std::max(parameters::peakFilterGainMinimumValue, peakFilterGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), peakFilterQualityId.c_str()) == 0) {
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, peakFilterCenterFrequency),
				std::max(parameters::qualityMinimumValue, newValue),
				std::max(parameters::peakFilterGainMinimumValue, peakFilterGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), peakFilterGainId.c_str()) == 0) {
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, peakFilterCenterFrequency),
				std::max(parameters::qualityMinimumValue, peakFilterQuality),
				std::max(parameters::peakFilterGainMinimumValue, newValue));
		}

		const auto highShelfFrequencyId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::frequencyId });
		const float highShelfCenterFrequency = mParameterValueTreeStatePtr->getParameterAsValue(highShelfFrequencyId).getValue();

		const auto highShelfQualityId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::qualityId });
		const float highShelfQuality = mParameterValueTreeStatePtr->getParameterAsValue(highShelfQualityId).getValue();

		const auto highShelfGainId = PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::gainId });
		const float highShelfGain = mParameterValueTreeStatePtr->getParameterAsValue(highShelfGainId).getValue();

		if (std::strcmp(parameterId.toRawUTF8(), highShelfFrequencyId.c_str()) == 0) {
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, newValue),
				std::max(parameters::qualityMinimumValue, highShelfQuality),
				std::max(parameters::eqGainMinimumValue, highShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), highShelfQualityId.c_str()) == 0) {
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, highShelfCenterFrequency),
				std::max(parameters::qualityMinimumValue, newValue),
				std::max(parameters::eqGainMinimumValue, highShelfGain));
		} else if (std::strcmp(parameterId.toRawUTF8(), highShelfGainId.c_str()) == 0) {
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
				sampleRate, 
				std::max(parameters::frequencyMinimumValue, highShelfCenterFrequency),
				std::max(parameters::qualityMinimumValue, highShelfQuality),
				std::max(parameters::eqGainMinimumValue, newValue));
		}

		const auto channelGainId = PluginUtils::joinId({ channelId, parameters::gainId });

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
