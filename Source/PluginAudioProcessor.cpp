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
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput(stringToTitleCase(Channels::outputId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::kickId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::snareId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::tomsId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::hiHatId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::cymbalsId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::otherId), juce::AudioChannelSet::stereo(), true)
		.withOutput(stringToTitleCase(Channels::roomId), juce::AudioChannelSet::stereo(), true)
#endif
	),
	mAudioProcessorValueTreeStatePtr(std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, juce::Identifier("plugin_params"), createParameterLayout())),
	mAudioFormatManagerPtr(std::make_unique<juce::AudioFormatManager>()),
	mRoomBufferPtr(std::make_unique<juce::AudioBuffer<float>>(2, 1024))
#endif
{
	mAudioFormatManagerPtr->registerBasicFormats();
	mAudioProcessorValueTreeStatePtr->state.setProperty(PluginPresetManager::presetNameProperty, "", nullptr);
	mAudioProcessorValueTreeStatePtr->state.setProperty("version", ProjectInfo::versionString, nullptr);
	mPresetManagerPtr = std::make_unique<PluginPresetManager>(*mAudioProcessorValueTreeStatePtr.get());

	for (int channelIndex = 0; channelIndex < Channels::size; channelIndex++)
	{
		if (channelIndex != Channels::outputChannelIndex && channelIndex != Channels::roomChannelIndex)
		{
			mSynthesiserPtrVector.push_back(std::make_unique<PluginSynthesiser>());
			mSynthesiserBufferPtrVector.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 1024));
			mReverbGains.push_back(std::make_unique<juce::dsp::Gain<float>>());
			mReverbs.push_back(std::make_unique<juce::dsp::Reverb>());
			mReverbBufferPtrVector.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 1024));
		}

		mCompressors.push_back(std::make_unique<juce::dsp::Compressor<float>>());
		mCompressorGains.push_back(std::make_unique<juce::dsp::Gain<float>>());
		mCompressorDryWetMixers.push_back(std::make_unique<juce::dsp::DryWetMixer<float>>());
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
			generalMidiSnakeCaseName = stringToSnakeCase(midiName);

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
					if (!part.empty())
					{
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
					if (stringIsNumeric(parts[0]))
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
					if (stringIsNumeric(parts[0]) && stringIsNumeric(parts[1]))
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
					if (stringIsNumeric(parts[0]) && stringIsNumeric(parts[1]))
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
				auto* gainParameter = mAudioProcessorValueTreeStatePtr->getParameter(stringsJoinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::gainComponentId }));
				auto* panParameter = mAudioProcessorValueTreeStatePtr->getParameter(stringsJoinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::panComponentId }));
				auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(mAudioProcessorValueTreeStatePtr->getParameter(stringsJoinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::phaseComponentId })));

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
					*phaseParameter);
			}
		}
	}

	mAudioProcessorValueTreeStatePtr->addParameterListener(AudioParameters::roomSizeComponentId, this);
	mAudioProcessorValueTreeStatePtr->addParameterListener(AudioParameters::dampingComponentId, this);
	mAudioProcessorValueTreeStatePtr->addParameterListener(AudioParameters::widthComponentId, this);

	for (const auto& channel : Channels::channelIndexToIdMap)
	{
		const auto& channelId = channel.second;

		const auto compressionThresholdId = stringsJoinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(compressionThresholdId, this);

		const auto compressionRatioId = stringsJoinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(compressionRatioId, this);

		const auto compressionAttackId = stringsJoinAndSnakeCase({ channelId, AudioParameters::attackId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(compressionAttackId, this);

		const auto compressionReleaseId = stringsJoinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(compressionReleaseId, this);

		auto compressionGainParameterId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::gainComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(compressionGainParameterId, this);

		auto compressionDryWetParameterId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::dryWetComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(compressionDryWetParameterId, this);

		auto reverbGainParameterId = stringsJoinAndSnakeCase({ channelId, AudioParameters::reverbComponentId, AudioParameters::gainComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(reverbGainParameterId, this);

		for (const auto& equalizationTypeIdToDefaultFrequency : AudioParameters::equalizationTypeIdToDefaultFrequencyMap)
		{
			const auto& equalizationTypeId = equalizationTypeIdToDefaultFrequency.first;

			const auto eqFrequencyId = stringsJoinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::frequencyComponentId });
			mAudioProcessorValueTreeStatePtr->addParameterListener(eqFrequencyId, this);

			const auto eqQualityId = stringsJoinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::qualityComponentId });
			mAudioProcessorValueTreeStatePtr->addParameterListener(eqQualityId, this);

			const auto eqGainId = stringsJoinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::gainComponentId });
			mAudioProcessorValueTreeStatePtr->addParameterListener(eqGainId, this);
		}

		const auto channelGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
		mAudioProcessorValueTreeStatePtr->addParameterListener(channelGainId, this);
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout()
{
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterBool>(
		juce::ParameterID{ AudioParameters::multiOutComponentId, 1 },
		stringToTitleCase(AudioParameters::multiOutComponentId),
		false));

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ AudioParameters::roomSizeComponentId, 1 },
		stringToTitleCase(AudioParameters::roomSizeComponentId),
		AudioParameters::roomSizeNormalisableRange,
		AudioParameters::getDefaultValue(AudioParameters::roomSizeComponentId, AudioParameters::roomSizeDefaultValue)));

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ AudioParameters::dampingComponentId, 1 },
		stringToTitleCase(AudioParameters::dampingComponentId),
		AudioParameters::dampingNormalisableRange,
		AudioParameters::getDefaultValue(AudioParameters::dampingComponentId, AudioParameters::dampingDefaultValue)));

	layout.add(std::make_unique<juce::AudioParameterFloat>(
		juce::ParameterID{ AudioParameters::widthComponentId, 1 },
		stringToTitleCase(AudioParameters::widthComponentId),
		AudioParameters::widthNormalisableRange,
		AudioParameters::getDefaultValue(AudioParameters::widthComponentId, AudioParameters::widthDefaultValue)));

	for (const auto& pair : AudioParameters::getUniqueMidiNoteMicCombinations())
	{
		int midiNote = pair.first;
		const std::set<std::string>& micIds = pair.second;

		for (const std::string& micId : micIds)
		{
			auto gainParameterId = stringsJoinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::gainComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ gainParameterId, 1 },
				stringToTitleCase(gainParameterId),
				AudioParameters::decibelGainNormalisableRange,
				AudioParameters::getDefaultValue(gainParameterId, AudioParameters::gainDeciblesDefaultValue)));

			auto panParameterId = stringsJoinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::panComponentId });
			bool containsRight = micId.find(Samples::rightComponentId) != std::string::npos;
			bool containsLeft = micId.find(Samples::leftComponentId) != std::string::npos;
			float defaultPan = containsRight ? 1.0f : containsLeft ? -1.0f
				: 0;
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ panParameterId, 1 },
				stringToTitleCase(panParameterId),
				AudioParameters::panNormalisableRange,
				defaultPan));

			auto phaseParameterId = stringsJoinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::phaseComponentId });
			bool containsBottom = micId.find(Samples::bottomComponentId) != std::string::npos;
			layout.add(std::make_unique<juce::AudioParameterBool>(
				juce::ParameterID{ phaseParameterId, 1 },
				stringToTitleCase(phaseParameterId),
				containsBottom));
		}
	}

	for (const auto& channel : Channels::channelIndexToIdMap)
	{
		const auto& channelId = channel.second;
		const auto channelIndex = channel.first;

		const auto compressionThresholdId = stringsJoinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionThresholdId, 1 },
			stringToTitleCase(compressionThresholdId),
			AudioParameters::thresholdNormalisableRange,
			AudioParameters::getDefaultValue(compressionThresholdId, AudioParameters::thresholdDefaultValue)));

		const auto compressionRatioId = stringsJoinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionRatioId, 1 },
			stringToTitleCase(compressionRatioId),
			AudioParameters::ratioNormalisableRange,
			AudioParameters::getDefaultValue(compressionRatioId, AudioParameters::ratioDefaultValue)));

		const auto compressionAttackId = stringsJoinAndSnakeCase({ channelId, AudioParameters::attackId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionAttackId, 1 },
			stringToTitleCase(compressionAttackId),
			AudioParameters::attackNormalisableRange,
			AudioParameters::getDefaultValue(compressionAttackId, AudioParameters::attackDefaultValue)));

		const auto compressionReleaseId = stringsJoinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionReleaseId, 1 },
			stringToTitleCase(compressionReleaseId),
			AudioParameters::releaseNormalisableRange,
			AudioParameters::getDefaultValue(compressionReleaseId, AudioParameters::releaseDefaultValue)));

		const auto compressionGainId = stringsJoinAndSnakeCase({ channelId,
																AudioParameters::compressionComponentId,
																AudioParameters::gainComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionGainId, 1 },
			stringToTitleCase(compressionGainId),
			AudioParameters::decibelGainNormalisableRange,
			AudioParameters::getDefaultValue(compressionGainId, AudioParameters::gainDeciblesDefaultValue)));

		const auto compressionDryWetId = stringsJoinAndSnakeCase({ channelId,
																  AudioParameters::compressionComponentId,
																  AudioParameters::dryWetComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ compressionDryWetId, 1 },
			stringToTitleCase(compressionDryWetId),
			AudioParameters::dryWetNormalisableRange,
			AudioParameters::getDefaultValue(compressionDryWetId, AudioParameters::allWetDefaultValue)));

		if (channelIndex != Channels::outputChannelIndex && channelIndex != Channels::roomChannelIndex)
		{
			const auto reverbGainId = stringsJoinAndSnakeCase({ channelId,
												   AudioParameters::reverbComponentId,
												   AudioParameters::gainComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ reverbGainId, 1 },
				stringToTitleCase(reverbGainId),
				AudioParameters::decibelGainNormalisableRange,
				AudioParameters::getDefaultValue(reverbGainId, AudioParameters::gainDeciblesDefaultValue)));
		}

		for (const auto& equalizationTypeId : AudioParameters::equalizationTypeIdVector)
		{
			const auto equalizationFrequencyDefaultValue = AudioParameters::equalizationTypeIdToDefaultFrequencyMap.at(equalizationTypeId);

			const auto eqFrequencyId = stringsJoinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::frequencyComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqFrequencyId, 1 },
				stringToTitleCase(eqFrequencyId),
				AudioParameters::frequencyNormalisableRange,
				AudioParameters::getDefaultValue(eqFrequencyId, equalizationFrequencyDefaultValue)));

			const auto eqQualityId = stringsJoinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::qualityComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqQualityId, 1 },
				stringToTitleCase(eqQualityId),
				AudioParameters::qualityNormalisableRange,
				AudioParameters::getDefaultValue(eqQualityId, AudioParameters::qualityDefaultValue)));

			const auto eqGainId = stringsJoinAndSnakeCase({ channelId, equalizationTypeId, AudioParameters::gainComponentId });
			layout.add(std::make_unique<juce::AudioParameterFloat>(
				juce::ParameterID{ eqGainId, 1 },
				stringToTitleCase(eqGainId),
				AudioParameters::linearGainNormalisableRange,
				AudioParameters::getDefaultValue(eqGainId, AudioParameters::gainDefaultValue)));
		}

		const auto channelGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
		layout.add(std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID{ channelGainId, 1 },
			stringToTitleCase(channelGainId),
			AudioParameters::decibelGainNormalisableRange,
			AudioParameters::getDefaultValue(channelGainId, AudioParameters::gainDeciblesDefaultValue)));
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
		std::vector<int> synthesiserMidiNotesVector = synthesiser.get()->getMidiNotesVector();

		// Common "aliases", I'd prefer a better solution
		synthesiserMidiNotesVector.erase(std::remove_if(synthesiserMidiNotesVector.begin(), synthesiserMidiNotesVector.end(),
			[](int x) {
				return x == GeneralMidiPercussion::bassDrum1Note || x == GeneralMidiPercussion::electricSnareNote;
			}),
			synthesiserMidiNotesVector.end());

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
	return *mAudioProcessorValueTreeStatePtr;
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
	spec.maximumBlockSize = samplesPerBlock;		// Example block size
	spec.numChannels = getTotalNumOutputChannels(); // Stereo

	mRoomBufferPtr->setSize(2, samplesPerBlock);

	for (const auto& channel : Channels::channelIndexToIdMap)
	{
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		if (channelIndex != Channels::outputChannelIndex && channelIndex != Channels::roomChannelIndex)
		{
			mSynthesiserPtrVector[channelIndex]->setCurrentPlaybackSampleRate(sampleRate);
			mReverbBufferPtrVector[channelIndex]->setSize(2, samplesPerBlock);
			mSynthesiserBufferPtrVector[channelIndex]->setSize(2, samplesPerBlock);

			mReverbGains[channelIndex]->prepare(spec);
			mReverbs[channelIndex]->prepare(spec);

			const auto reverbGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::reverbComponentId, AudioParameters::gainComponentId });
			const float reverbGainValue = mAudioProcessorValueTreeStatePtr->getParameterAsValue(reverbGainId).getValue();
			mReverbGains[channelIndex]->setGainDecibels(reverbGainValue);

			mReverbs[channelIndex]->setParameters({ mAudioProcessorValueTreeStatePtr->getParameterAsValue(AudioParameters::roomSizeComponentId).getValue(),
												   mAudioProcessorValueTreeStatePtr->getParameterAsValue(AudioParameters::dampingComponentId).getValue(),
												   1.0f,
												   0.0f,
												   mAudioProcessorValueTreeStatePtr->getParameterAsValue(AudioParameters::widthComponentId).getValue(),
												   0.0f });
		}

		{
			const auto& compressor = mCompressors[channelIndex];
			compressor->prepare(spec);
			const auto compressionThresholdId = stringsJoinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
			compressor->setThreshold(mAudioProcessorValueTreeStatePtr->getParameterAsValue(compressionThresholdId).getValue());
			const auto compressionRatioId = stringsJoinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
			compressor->setRatio(mAudioProcessorValueTreeStatePtr->getParameterAsValue(compressionRatioId).getValue());
			const auto compressionAttackId = stringsJoinAndSnakeCase({ channelId, AudioParameters::attackId });
			compressor->setAttack(mAudioProcessorValueTreeStatePtr->getParameterAsValue(compressionAttackId).getValue());
			const auto compressionReleaseId = stringsJoinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
			compressor->setRelease(mAudioProcessorValueTreeStatePtr->getParameterAsValue(compressionReleaseId).getValue());
		}

		{
			// Compressor Gain
			const auto compressorGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::gainComponentId });
			const float compressorGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(compressorGainId).getValue();
			mCompressorGains[channelIndex]->prepare(spec);
			mCompressorGains[channelIndex]->setGainDecibels(compressorGain);
		}

		{
			// DryWet
			mCompressorDryWetMixers[channelIndex]->prepare(spec);
			const auto compressorDryWetId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::dryWetComponentId });
			const float compressorDryWetValue = mAudioProcessorValueTreeStatePtr->getParameterAsValue(compressorDryWetId).getValue();
			mCompressorDryWetMixers[channelIndex]->setWetMixProportion(compressorDryWetValue);
		}

		{
			// EQ - Low Shelf
			const auto lowShelfFrequencyId = stringsJoinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
			const float lowShelfCenterFrequency = mAudioProcessorValueTreeStatePtr->getParameterAsValue(lowShelfFrequencyId).getValue();

			const auto lowShelfQualityId = stringsJoinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId });
			const float lowShelfQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(lowShelfQualityId).getValue();

			const auto lowShelfGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::gainComponentId });
			const float lowShelfGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(lowShelfGainId).getValue();

			mLowShelfFilters[channelIndex].prepare(spec);
			*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, lowShelfCenterFrequency, lowShelfQuality, lowShelfGain);
		}

		{
			// EQ - Peak Filter
			const auto peakFilterCenterFrequencyId = stringsJoinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::frequencyComponentId });
			const float peakFilterCenterFrequency = mAudioProcessorValueTreeStatePtr->getParameterAsValue(peakFilterCenterFrequencyId).getValue();

			const auto peakFilterQualityId = stringsJoinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::qualityComponentId });
			const float peakFilterQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(peakFilterQualityId).getValue();

			const auto peakFilterGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::gainComponentId });
			const float peakFilterGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(peakFilterGainId).getValue();

			mPeakFilters[channelIndex].prepare(spec);
			*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, peakFilterCenterFrequency, peakFilterQuality, peakFilterGain);
		}

		{
			// EQ - High Shelf
			const auto highShelfFrequencyId = stringsJoinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
			const float highShelfCenterFrequency = mAudioProcessorValueTreeStatePtr->getParameterAsValue(highShelfFrequencyId).getValue();

			const auto highShelfQualityId = stringsJoinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::qualityComponentId });
			const float highShelfQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(highShelfQualityId).getValue();

			const auto highShelfGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::gainComponentId });
			const float highShelfGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(highShelfGainId).getValue();

			mHighShelfFilters[channelIndex].prepare(spec);
			*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, highShelfCenterFrequency, highShelfQuality, highShelfGain);
		}

		{
			const auto channelGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
			const float channelGainValue = mAudioProcessorValueTreeStatePtr->getParameterAsValue(channelGainId).getValue();
			mChannelGains[channelIndex]->prepare(spec);
			mChannelGains[channelIndex]->setGainDecibels(channelGainValue);
		}
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
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

#if !JucePlugin_IsSynth
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

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
	{
		outputBuffer.clear(i, 0, outputBuffer.getNumSamples());
	}

	auto* multiOutParameter = dynamic_cast<juce::AudioParameterBool*>(mAudioProcessorValueTreeStatePtr->getParameter(AudioParameters::multiOutComponentId));
	bool isMultiOut = multiOutParameter->get() && totalNumOutputChannels > 2;

	mRoomBufferPtr->clear();

	for (const auto& channel : Channels::channelIndexToIdMap)
	{
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		if (channelIndex == Channels::outputChannelIndex || channelIndex == Channels::roomChannelIndex)
		{
			break;
		}

		auto& synthesiser = mSynthesiserPtrVector[channelIndex];
		auto& internalBufferPtr = mSynthesiserBufferPtrVector[channelIndex];
		internalBufferPtr->clear();
		synthesiser->renderNextBlock(*internalBufferPtr, midiMessages, 0, internalBufferPtr->getNumSamples());

		// Reverb

		const auto& reverbBufferPtr = mReverbBufferPtrVector[channelIndex];

		reverbBufferPtr->copyFrom(0, 0, internalBufferPtr->getReadPointer(0), internalBufferPtr->getNumSamples());
		reverbBufferPtr->copyFrom(1, 0, internalBufferPtr->getReadPointer(1), internalBufferPtr->getNumSamples());

		juce::dsp::AudioBlock<float> reverbBufferBlock(*reverbBufferPtr);
		juce::dsp::ProcessContextReplacing<float> reverbBufferContext(reverbBufferBlock);

		mReverbGains[channelIndex]->process(reverbBufferContext);
		mReverbs[channelIndex]->process(reverbBufferContext);

		mRoomBufferPtr->addFrom(
			0,
			0,
			reverbBufferPtr->getReadPointer(0),
			reverbBufferPtr->getNumSamples());
		mRoomBufferPtr->addFrom(
			1,
			0,
			reverbBufferPtr->getReadPointer(1),
			reverbBufferPtr->getNumSamples());

		// EQ/comp

		juce::dsp::AudioBlock<float> internalBufferBlock(*internalBufferPtr);
		juce::dsp::ProcessContextReplacing<float> internalBufferContext(internalBufferBlock);

		mCompressorDryWetMixers[channelIndex]->pushDrySamples(internalBufferBlock);

		mCompressors[channelIndex]->process(internalBufferContext);
		mCompressorGains[channelIndex]->process(internalBufferContext);

		mCompressorDryWetMixers[channelIndex]->mixWetSamples(internalBufferBlock);

		mLowShelfFilters[channelIndex].process(internalBufferContext);
		mPeakFilters[channelIndex].process(internalBufferContext);
		mHighShelfFilters[channelIndex].process(internalBufferContext);

		mChannelGains[channelIndex]->process(internalBufferContext);

		outputBuffer.addFrom(
			isMultiOut ? 2 + (channelIndex * 2) : 0,
			0,
			internalBufferPtr->getReadPointer(0),
			internalBufferPtr->getNumSamples());
		outputBuffer.addFrom(
			isMultiOut ? 2 + (channelIndex * 2) + 1 : 1,
			0,
			internalBufferPtr->getReadPointer(1),
			internalBufferPtr->getNumSamples());
	}

	juce::dsp::AudioBlock<float> roomBlock(*mRoomBufferPtr);
	juce::dsp::ProcessContextReplacing<float> roomContext(roomBlock);

	mCompressorDryWetMixers[Channels::roomChannelIndex]->pushDrySamples(roomBlock);
	mCompressors[Channels::roomChannelIndex]->process(roomContext);
	mCompressorGains[Channels::roomChannelIndex]->process(roomContext);
	mCompressorDryWetMixers[Channels::roomChannelIndex]->mixWetSamples(roomBlock);
	mLowShelfFilters[Channels::roomChannelIndex].process(roomContext);
	mPeakFilters[Channels::roomChannelIndex].process(roomContext);
	mHighShelfFilters[Channels::roomChannelIndex].process(roomContext);

	mChannelGains[Channels::roomChannelIndex]->process(roomContext);

	const auto reverbDestL = isMultiOut ? ((Channels::roomChannelIndex + 1) * 2) + 0 : 0;
	const auto reverbDestR = isMultiOut ? ((Channels::roomChannelIndex + 1) * 2) + 1 : 1;

	outputBuffer.addFrom(
		reverbDestL,
		0,
		mRoomBufferPtr->getReadPointer(0),
		mRoomBufferPtr->getNumSamples());
	outputBuffer.addFrom(
		reverbDestR,
		0,
		mRoomBufferPtr->getReadPointer(1),
		mRoomBufferPtr->getNumSamples());

	juce::dsp::AudioBlock<float> outputBlock(outputBuffer);
	juce::dsp::ProcessContextReplacing<float> outputContext(outputBlock);

	mCompressorDryWetMixers[Channels::outputChannelIndex]->pushDrySamples(outputBlock);
	mCompressors[Channels::outputChannelIndex]->process(outputContext);
	mCompressorGains[Channels::outputChannelIndex]->process(outputContext);
	mCompressorDryWetMixers[Channels::outputChannelIndex]->mixWetSamples(outputBlock);
	mLowShelfFilters[Channels::outputChannelIndex].process(outputContext);
	mPeakFilters[Channels::outputChannelIndex].process(outputContext);
	mHighShelfFilters[Channels::outputChannelIndex].process(outputContext);
	mChannelGains[Channels::outputChannelIndex]->process(outputContext);
}

void PluginAudioProcessor::parameterChanged(const juce::String& parameterId, float newValue)
{
	DBG("parameterChanged " + parameterId.toStdString() + " " + std::to_string(newValue));

	const auto tLowShelfQualityId = stringsJoinAndSnakeCase({ "kick", AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId });
	const float tLowShelfQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(tLowShelfQualityId).getValue();
	const float tLowQuialit2 = mLowShelfFilters[0].state.getObject()->coefficients[0];
	const float tLowQuialit4 = mLowShelfFilters[0].state.getObject()->coefficients[1];
	const float tLowQuialit5 = mLowShelfFilters[0].state.getObject()->coefficients[2];
	DBG("testing" + std::to_string(tLowShelfQuality) + " " + std::to_string(tLowQuialit4));

	auto sampleRate = getSampleRate();

	if (std::strcmp(parameterId.toRawUTF8(), AudioParameters::roomSizeComponentId.c_str()) == 0)
	{
		for (const auto& reverb : mReverbs)
		{
			const auto& parameters = reverb->getParameters();
			reverb->setParameters({ newValue,
								   parameters.damping,
								   1.0f,
								   0.0f,
								   parameters.width,
								   0.0f });
		}
	}

	if (std::strcmp(parameterId.toRawUTF8(), AudioParameters::widthComponentId.c_str()) == 0)
	{
		for (const auto& reverb : mReverbs)
		{
			const auto& parameters = reverb->getParameters();
			reverb->setParameters({ parameters.roomSize,
								   parameters.damping,
								   1.0f,
								   0.0f,
								   newValue,
								   0.0f });
		}
	}

	if (std::strcmp(parameterId.toRawUTF8(), AudioParameters::dampingComponentId.c_str()) == 0)
	{
		for (const auto& reverb : mReverbs)
		{
			const auto& parameters = reverb->getParameters();
			reverb->setParameters({ parameters.roomSize,
								   newValue,
								   1.0f,
								   0.0f,
								   parameters.width,
								   0.0f });
		}
	}

	for (const auto& channel : Channels::channelIndexToIdMap)
	{
		const auto channelIndex = channel.first;
		const auto& channelId = channel.second;

		const auto& compressor = mCompressors[channelIndex];

		{
			const auto compressionGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::gainComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), compressionGainId.c_str()) == 0)
			{
				mCompressorGains[channelIndex]->setGainDecibels(newValue);
			}
		}

		{
			const auto compressionDryWetId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::dryWetComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), compressionDryWetId.c_str()) == 0)
			{
				mCompressorDryWetMixers[channelIndex]->setWetMixProportion(newValue);
			}
		}

		{
			const auto compressionThresholdId = stringsJoinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), compressionThresholdId.c_str()) == 0)
			{
				compressor->setThreshold(newValue);
			}
		}

		{
			const auto compressionRatioId = stringsJoinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), compressionRatioId.c_str()) == 0)
			{
				compressor->setRatio(newValue);
			}
		}

		{
			const auto compressionAttackId = stringsJoinAndSnakeCase({ channelId, AudioParameters::attackId });
			if (std::strcmp(parameterId.toRawUTF8(), compressionAttackId.c_str()) == 0)
			{
				compressor->setAttack(newValue);
			}
		}

		{
			const auto compressionReleaseId = stringsJoinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), compressionReleaseId.c_str()) == 0)
			{
				compressor->setRelease(newValue);
			}
		}

		if (channelIndex != Channels::outputChannelIndex && channelIndex != Channels::roomChannelIndex)
		{
			const auto reberbGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::reverbComponentId, AudioParameters::gainComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), reberbGainId.c_str()) == 0)
			{
				mReverbGains[channelIndex]->setGainDecibels(newValue);
			}
		}

		{
			const auto lowShelfFrequencyId = stringsJoinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
			float lowShelfFrequency = mAudioProcessorValueTreeStatePtr->getParameterAsValue(lowShelfFrequencyId).getValue();

			const auto lowShelfQualityId = stringsJoinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId });
			float lowShelfQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(lowShelfQualityId).getValue();

			const auto lowShelfGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::gainComponentId });
			float lowShelfGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(lowShelfGainId).getValue();

			if (lowShelfFrequency == 0)
			{
				lowShelfFrequency = AudioParameters::lowShelfFrequencyDefaultValue;
			}
			if (lowShelfQuality == 0)
			{
				lowShelfQuality = AudioParameters::qualityDefaultValue;
			}
			if (lowShelfGain == 0)
			{
				lowShelfGain = AudioParameters::gainDefaultValue;
			}

			if (std::strcmp(parameterId.toRawUTF8(), lowShelfFrequencyId.c_str()) == 0)
			{
				*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
					sampleRate,
					newValue,
					lowShelfQuality,
					lowShelfGain);
			}
			else if (std::strcmp(parameterId.toRawUTF8(), lowShelfQualityId.c_str()) == 0)
			{
				*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
					sampleRate,
					lowShelfFrequency,
					newValue,
					lowShelfGain);
			}
			else if (std::strcmp(parameterId.toRawUTF8(), lowShelfGainId.c_str()) == 0)
			{
				*mLowShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
					sampleRate,
					lowShelfFrequency,
					lowShelfQuality,
					newValue);
			}
		}

		{
			const auto peakFilterCenterFrequencyId = stringsJoinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::frequencyComponentId });
			float peakFilterFrequency = mAudioProcessorValueTreeStatePtr->getParameterAsValue(peakFilterCenterFrequencyId).getValue();

			const auto peakFilterQualityId = stringsJoinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::qualityComponentId });
			float peakFilterQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(peakFilterQualityId).getValue();

			const auto peakFilterGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::gainComponentId });
			float peakFilterGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(peakFilterGainId).getValue();

			if (peakFilterFrequency == 0)
			{
				peakFilterFrequency = AudioParameters::peakFilterFrequencyDefaultValue;
			}
			if (peakFilterQuality == 0)
			{
				peakFilterQuality = AudioParameters::qualityDefaultValue;
			}
			if (peakFilterGain == 0)
			{
				peakFilterGain = AudioParameters::gainDefaultValue;
			}

			if (std::strcmp(parameterId.toRawUTF8(), peakFilterCenterFrequencyId.c_str()) == 0)
			{
				*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
					sampleRate,
					newValue,
					peakFilterQuality,
					peakFilterGain);
			}
			else if (std::strcmp(parameterId.toRawUTF8(), peakFilterQualityId.c_str()) == 0)
			{
				*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
					sampleRate,
					peakFilterFrequency,
					newValue,
					peakFilterGain);
			}
			else if (std::strcmp(parameterId.toRawUTF8(), peakFilterGainId.c_str()) == 0)
			{
				*mPeakFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
					sampleRate,
					peakFilterFrequency,
					peakFilterQuality,
					newValue);
			}
		}

		{
			const auto highShelfFrequencyId = stringsJoinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
			float highShelfFrequency = mAudioProcessorValueTreeStatePtr->getParameterAsValue(highShelfFrequencyId).getValue();

			const auto highShelfQualityId = stringsJoinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::qualityComponentId });
			float highShelfQuality = mAudioProcessorValueTreeStatePtr->getParameterAsValue(highShelfQualityId).getValue();

			const auto highShelfGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::gainComponentId });
			float highShelfGain = mAudioProcessorValueTreeStatePtr->getParameterAsValue(highShelfGainId).getValue();

			if (highShelfFrequency == 0)
			{
				highShelfFrequency = AudioParameters::highShelfFrequencyDefaultValue;
			}
			if (highShelfQuality == 0)
			{
				highShelfQuality = AudioParameters::qualityDefaultValue;
			}
			if (highShelfGain == 0)
			{
				highShelfGain = AudioParameters::gainDefaultValue;
			}

			if (std::strcmp(parameterId.toRawUTF8(), highShelfFrequencyId.c_str()) == 0)
			{
				*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
					sampleRate,
					newValue,
					highShelfQuality,
					highShelfGain);
			}
			else if (std::strcmp(parameterId.toRawUTF8(), highShelfQualityId.c_str()) == 0)
			{
				*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
					sampleRate,
					highShelfFrequency,
					newValue,
					highShelfGain);
			}
			else if (std::strcmp(parameterId.toRawUTF8(), highShelfGainId.c_str()) == 0)
			{
				*mHighShelfFilters[channelIndex].state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
					sampleRate,
					highShelfFrequency,
					highShelfQuality,
					newValue);
			}
		}

		{
			const auto compressorGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::compressionComponentId, AudioParameters::gainComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), compressorGainId.c_str()) == 0)
			{
				mCompressorGains[channelIndex]->setGainDecibels(newValue);
			}
		}

		{
			const auto channelGainId = stringsJoinAndSnakeCase({ channelId, AudioParameters::gainComponentId });
			if (std::strcmp(parameterId.toRawUTF8(), channelGainId.c_str()) == 0)
			{
				mChannelGains[channelIndex]->setGainDecibels(newValue);
			}
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
	auto state = mAudioProcessorValueTreeStatePtr.get()->copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
	const auto valueTreeFromXml = juce::ValueTree::fromXml(*xmlState);

	if (xmlState.get() != nullptr)
	{
		mAudioProcessorValueTreeStatePtr.get()->replaceState(valueTreeFromXml);
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
