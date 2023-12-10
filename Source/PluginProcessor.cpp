/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Configuration/Samples.h"
#include "Synthesisers/PanningSamplerSynthesiser.h"

//==============================================================================
ABKit2AudioProcessor::ABKit2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	mAudioFormatManager.registerBasicFormats();
	mTriangleSynthesiser = std::make_unique< MultiVoicingSynthesiser>();

	for (const auto& instrument : samples::instrumentVector) {
		std::string sampleName = samples::instrumentIdMap.at(instrument);
		int midiNote = samples::instrumentToGeneralMidiNoteMap.at(instrument);
		juce::BigInteger range;
		range.setRange(midiNote, 1, true);
		int dataSizeInBytes;

		switch (instrument) {
		case samples::InstrumentEnum::bassDrum1:
		case samples::InstrumentEnum::electricSnare:
		{
			auto synthesiser = std::make_unique<juce::Synthesiser>();

			std::string resourceName = sampleName + "_wav";

			const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
			auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
			juce::AudioFormatReader* reader = mAudioFormatManager.createReaderFor(std::move(memoryInputStream));

			double maxSampleLengthSeconds = dataSizeInBytes / (samples::bitRate * (samples::bitDepth / 8.0));
			juce::SamplerSound* sound = new juce::SamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);
			synthesiser.get()->addSound(sound);
			synthesiser.get()->addVoice(new juce::SamplerVoice());

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		case samples::InstrumentEnum::cowbell:
		case samples::InstrumentEnum::handClap:
		case samples::InstrumentEnum::tambourine:
		case samples::InstrumentEnum::maraca:
		{
			auto synthesiser = std::make_unique<PanningSamplerSynthesiser>();

			std::string sampleName = samples::instrumentIdMap.at(instrument);

			for (int i = 0; i < samples::standardVariationsCount; i++) {
				std::string resourceName = sampleName + "_" + std::to_string(i + 1) + "_wav";

				synthesiser.get()->addSample(resourceName,
					samples::bitRate,
					samples::bitDepth,
					midiNote,
					mAudioFormatManager);
			}

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		case samples::InstrumentEnum::triangleMute:
		case samples::InstrumentEnum::triangleOpen:
		{
			addMultiVoicedSounds(instrument, *mTriangleSynthesiser);
			if (instrument != samples::InstrumentEnum::triangleMute) {
				mTriangleSynthesiser.get()->addVoice(new juce::SamplerVoice());
				synthesisers.push_back(std::move(mTriangleSynthesiser));
			}
		}
		break;
		case samples::InstrumentEnum::tomHigh:
		case samples::InstrumentEnum::tomFloorHigh:
		case samples::InstrumentEnum::sideStick:
		{
			auto synthesiser = std::make_unique<PanningSamplerSynthesiser>();

			for (int intensityIndex = 0; intensityIndex < samples::intensityIdVector.size(); intensityIndex++) {
				std::string intensityId = samples::intensityIdVector[intensityIndex];

				for (int variationIndex = 0; variationIndex < samples::standardVariationsCount; variationIndex++) {
					std::string resourceName = sampleName + "_" + intensityId + "_" + std::to_string(variationIndex + 1) + "_wav";

					synthesiser->addSample(resourceName, samples::bitRate, samples::bitDepth, midiNote, "on", intensityIndex, 0, mAudioFormatManager);
				}
			}

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		case samples::InstrumentEnum::acousticBassDrum:
		{
			auto synthesiser = std::make_unique<PanningSamplerSynthesiser>();

			for (const auto& micId : samples::acousticBassDrumMicrophoneIdVector) {
				int intensityIndex = 0;
				for (const auto& intensityId : samples::intensityIdVector) {
					for (int variationIndex = 0; variationIndex < samples::standardVariationsCount; variationIndex++) {
						std::string resourceName = sampleName + "_" + intensityId + "_" + std::to_string(variationIndex + 1) + "_" + micId + "_wav";
						synthesiser->addSample(resourceName, samples::bitRate, samples::bitDepth, midiNote, micId, intensityIndex, 0, mAudioFormatManager);
					}

					++intensityIndex;
				}
			}

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		case samples::InstrumentEnum::acousticSnare:
		{
			auto synthesiser = std::make_unique<PanningSamplerSynthesiser>();

			for (const auto& micId : samples::acousticSnareMicrophoneIdVector) {
				int intensityIndex = 0;
				for (const auto& intensityId : samples::intensityIdVector) {
					for (int variationIndex = 0; variationIndex < samples::standardVariationsCount; variationIndex++) {
						std::string resourceName = sampleName + "_" + intensityId + "_" + std::to_string(variationIndex + 1) + "_" + micId + "_wav";
						synthesiser->addSample(resourceName, samples::bitRate, samples::bitDepth, midiNote, micId, intensityIndex, 0, mAudioFormatManager);
					}

					++intensityIndex;
				}
			}

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		case samples::InstrumentEnum::rideBell:
		{
			auto synthesiser = std::make_unique<PanningSamplerSynthesiser>();

			for (const auto& micId : samples::centerLeftRightIdVector) {
				int intensityIndex = 0;
				for (const auto& intensityId : { constants::mediumIntensityId, constants::hardIntensityId }) {
					for (int variationIndex = 0; variationIndex < samples::standardVariationsCount; variationIndex++) {
						std::string resourceName = sampleName + "_" + intensityId + "_" + std::to_string(variationIndex + 1) + "_" + micId + "_wav";
						synthesiser->addSample(resourceName, samples::bitRate, samples::bitDepth, midiNote, micId, intensityIndex, 0, mAudioFormatManager);
					}

					++intensityIndex;
				}
			}

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		case samples::InstrumentEnum::crashCymbal1:
		case samples::InstrumentEnum::crashCymbal2:
		case samples::InstrumentEnum::chineseCymbal:
		case samples::InstrumentEnum::splashCymbal:
		case samples::InstrumentEnum::rideCymbal1:
		{
			auto synthesiser = std::make_unique<PanningSamplerSynthesiser>();
			for (const auto& cymbalMic : samples::centerLeftRightEnumVector) {
				const auto& micId = samples::centerLeftRightEnumToIdMap.at(cymbalMic);

				for (int variationIndex = 0; variationIndex < samples::standardVariationsCount; variationIndex++) {
					std::string resourceName = sampleName + "_" + std::to_string(variationIndex + 1) + "_" + micId + "_wav";

					synthesiser.get()->addSample(resourceName,
						samples::bitRate,
						samples::bitDepth,
						midiNote,
						micId,
						samples::centerLeftRightEnumToPanMap.at(cymbalMic),
						mAudioFormatManager);
				}
			}

			synthesisers.push_back(std::move(synthesiser));
		}
		break;
		default:
			break;
		}
	}
}

void ABKit2AudioProcessor::addMultiVoicedSounds(samples::InstrumentEnum instrument, MultiVoicingSynthesiser& synthesizer) {
	std::string sampleName = samples::instrumentIdMap.at(instrument);

	int midiNote = samples::instrumentToGeneralMidiNoteMap.at(instrument);

	int dataSizeInBytes;

	for (int i = 0; i < samples::standardVariationsCount; i++) {
		std::string resourceName = sampleName + "_" + std::to_string(i + 1) + "_wav";

		const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
		auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
		juce::AudioFormatReader* reader = mAudioFormatManager.createReaderFor(std::move(memoryInputStream));

		juce::BigInteger range;
		range.setRange(midiNote, 1, true);

		double maxSampleLengthSeconds = dataSizeInBytes / (samples::bitRate * (samples::bitDepth / 8.0));
		juce::SamplerSound* sound = new juce::SamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, maxSampleLengthSeconds);
		synthesizer.addSamplerSound(sound, midiNote);
	}
}

ABKit2AudioProcessor::~ABKit2AudioProcessor()
{
}

//==============================================================================
const juce::String ABKit2AudioProcessor::getName() const
{
	return JucePlugin_Name;
	}

bool ABKit2AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool ABKit2AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool ABKit2AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double ABKit2AudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int ABKit2AudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int ABKit2AudioProcessor::getCurrentProgram()
{
	return 0;
}

void ABKit2AudioProcessor::setCurrentProgram(int index)
{
}

const juce::String ABKit2AudioProcessor::getProgramName(int index)
{
	return {};
}

void ABKit2AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void ABKit2AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	for (auto& synthesizer : synthesisers) {
		(*synthesizer).setCurrentPlaybackSampleRate(sampleRate);
	}
}

void ABKit2AudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ABKit2AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void ABKit2AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	for (auto& synthesizer : synthesisers) {
		(*synthesizer).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	}
}

//==============================================================================
bool ABKit2AudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ABKit2AudioProcessor::createEditor()
{
	return new ABKit2AudioProcessorEditor(*this);
}

//==============================================================================
void ABKit2AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void ABKit2AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new ABKit2AudioProcessor();
}

//==============================================================================
void ABKit2AudioProcessor::noteOnSynthesisers(int midiNoteNumber, const float velocity)
{
	for (auto& synthesizer : synthesisers) {
		(*synthesizer).noteOn(0, midiNoteNumber, velocity);
	}
}

