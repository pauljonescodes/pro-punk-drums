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

PluginAudioProcessor::PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput(channels::outputName, juce::AudioChannelSet::stereo(), true)
		.withOutput(channels::kickName, juce::AudioChannelSet::stereo(), true)
		.withOutput(channels::snareName, juce::AudioChannelSet::stereo(), true)
		.withOutput(channels::tomsName, juce::AudioChannelSet::stereo(), true)
		.withOutput(channels::hiHatName, juce::AudioChannelSet::stereo(), true)
		.withOutput(channels::cymbalsName, juce::AudioChannelSet::stereo(), true)
		.withOutput(channels::percussionName, juce::AudioChannelSet::stereo(), true)
#endif
	), mParameterValueTreeState(std::make_unique<juce::AudioProcessorValueTreeState>(*this,
		nullptr, juce::Identifier("plugin_params"), createParameterLayout()))
#endif
{
	mAudioFormatManager = std::make_unique<juce::AudioFormatManager>();
	mAudioFormatManager->registerBasicFormats();

	mPresetManager = std::make_unique<PluginPresetManager>(*mParameterValueTreeState.get());

	// skipping output channel
	for (int channelIndex = 1; channelIndex < channels::size; channelIndex++)
	{
		mSynthesiserPtrVector.push_back(std::make_unique<PluginSynthesiser>());
		mInternalBufferPtrVector.push_back(std::make_unique<juce::AudioBuffer<float>>(2, 1024));
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
				auto* gainParameter = mParameterValueTreeState->getParameter(PluginUtils::getMidiNoteParameterId(midiNote, micId, parameters::gainId));
				auto* panParameter = mParameterValueTreeState->getParameter(PluginUtils::getMidiNoteParameterId(midiNote, micId, parameters::panId));
				auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeState->getParameter(PluginUtils::getMidiNoteParameterId(midiNote, micId, parameters::phaseId)));

				auto& synthesiser = mSynthesiserPtrVector.at(channels::generalMidiNoteToChannelIndex.at(midiNote) - 1);

				synthesiser->addSample(
					namedResource,
					samples::bitRate,
					samples::bitDepth,
					midiNote,
					micId,
					generalmidi::midiNoteToStopNotesMap.at(midiNote),
					velocityIndex,
					variationIndex,
					*mAudioFormatManager.get(),
					*gainParameter,
					*panParameter,
					*phaseParameter
				);
			}
		}
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ parameters::multiOutId, 1 }, strings::multiOut, false));

	for (const auto& pair : PluginUtils::getUniqueMidiNoteMicCombinations()) {
		int midiNote = pair.first;
		std::string midiName = generalmidi::midiNoteToNameMap.at(midiNote);
		const std::set<std::string>& micIds = pair.second;

		for (const std::string& micId : micIds) {
			std::string gainName = midiName + " " + PluginUtils::capitalizeFirstLetter(micId) + " Gain";
			std::string gainParameterId = PluginUtils::getMidiNoteParameterId(midiNote, micId, parameters::gainId);
			layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ gainParameterId, 1 }, gainName, parameters::gainNormalizableRange, parameters::gainDefault));

			std::string panName = midiName + " " + PluginUtils::capitalizeFirstLetter(micId) + " Pan";
			std::string panParameterId = PluginUtils::getMidiNoteParameterId(midiNote, micId, parameters::panId);
			bool containsRight = micId.find(parameters::rightId) != std::string::npos;
			bool containsLeft = micId.find(parameters::leftId) != std::string::npos;
			float defaultPan = containsRight ? 1.0f : containsLeft ? -1.0f : 0;
			layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ panParameterId, 1 }, panName, parameters::panNormalizableRange, defaultPan));

			std::string phaseName = midiName + " " + PluginUtils::capitalizeFirstLetter(micId) + " Phase";
			std::string phaseParameterId = PluginUtils::getMidiNoteParameterId(midiNote, micId, parameters::phaseId);
			bool containsBottom = micId.find(parameters::bottomId) != std::string::npos;
			layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID{ phaseParameterId, 1 }, phaseName, containsBottom));
		}
	}

	for (const auto& channel : channels::channelIndexToNameMap) {
		const std::string channelName = channel.second;
		const std::string thresholdName = channelName + " " + parameters::thresholdId;
		const std::string thresholdId = PluginUtils::getChannelParameterId(channelName, parameters::thresholdId);
		layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ thresholdId, 1 }, thresholdName, parameters::thresholdNormalizableRange, parameters::thresholdDefault));

		const std::string ratioName = channelName + " " + parameters::ratioId;
		const std::string ratioId = PluginUtils::getChannelParameterId(channelName, parameters::ratioId);
		layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ ratioId, 1 }, ratioName, parameters::ratioNormalizableRange, parameters::ratioDefault));

		const std::string attackName = channelName + " " + parameters::attackId;
		const std::string attackId = PluginUtils::getChannelParameterId(channelName, parameters::attackId);
		layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ attackId, 1 }, attackName, parameters::attackNormalizableRange, parameters::attackDefault));

		const std::string releaseName = channelName + " " + parameters::releaseId;
		const std::string releaseId = PluginUtils::getChannelParameterId(channelName, parameters::releaseId);
		layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ releaseId, 1 }, releaseName, parameters::releaseNormalizableRange, parameters::releaseDefault));

		for (const auto& equalization : parameters::equalizationTypeIdToDefaultFrequencyMap) {
			const auto& equalizationTypeId = equalization.first;
			const auto equalizationDefaultValue = equalization.second;

			const auto eqGainId = PluginUtils::getEqualizationParameterId(channelName, equalizationTypeId, parameters::gainId);
			layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ eqGainId, 1 }, eqGainId, parameters::gainNormalizableRange, parameters::gainDefault));

			const auto eqFreqId = PluginUtils::getEqualizationParameterId(channelName, equalizationTypeId, parameters::frequencyId);
			layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ eqFreqId, 1 }, eqFreqId, parameters::frequencyNormalizableRange, equalizationDefaultValue));

			const auto eqQualityId = PluginUtils::getEqualizationParameterId(channelName, equalizationTypeId, parameters::qualityId);
			layout.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{ eqQualityId, 1 }, eqQualityId, parameters::qualityNormalizableRange, parameters::qualityDefaultValue));
		}
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
	return *mParameterValueTreeState;
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
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int PluginAudioProcessor::getCurrentProgram()
{
	return 0;
}

void PluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String PluginAudioProcessor::getProgramName(int index)
{
	return {};
}

void PluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}


void PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{

	for (const auto& synthesiser : mSynthesiserPtrVector)
	{
		synthesiser->setCurrentPlaybackSampleRate(sampleRate);
	}

	for (const auto& buffer : mInternalBufferPtrVector)
	{
		buffer->setSize(2, samplesPerBlock);
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

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& outputBuffer, juce::MidiBuffer& midiMessages)
{
	juce::MidiBuffer bufferToProcess;

	/// Iterate through scheduled MIDI events
	for (auto it = mScheduledMidiEvents.begin(); it != mScheduledMidiEvents.end();)
	{
		if (it->first <= mCurrentSamplePosition)
		{
			// Calculate the sample number relative to the start of the current buffer
			int sampleNumberInBuffer = static_cast<int>(it->first - mCurrentSamplePosition + outputBuffer.getNumSamples());
			bufferToProcess.addEvent(it->second, sampleNumberInBuffer);
			it = mScheduledMidiEvents.erase(it); // Remove the event after scheduling
		}
		else
		{
			++it;
		}
	}
	mCurrentSamplePosition += outputBuffer.getNumSamples();
	bufferToProcess.addEvents(midiMessages, 0, outputBuffer.getNumSamples(), 0);
	midiMessages.swapWith(bufferToProcess);

	///

	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		outputBuffer.clear(i, 0, outputBuffer.getNumSamples());
	}

	auto* multiOutParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeState->getParameter(parameters::multiOutId));
	bool isMultiOut = multiOutParameter->get() && totalNumOutputChannels > 2;

	for (auto i = 0; i < mSynthesiserPtrVector.size(); i++) {
		auto& synthesiser = mSynthesiserPtrVector[i];
		auto& internalBuffer = mInternalBufferPtrVector[i];

		internalBuffer->clear();
		synthesiser->renderNextBlock(*internalBuffer, midiMessages, 0, internalBuffer->getNumSamples());

		int leftChannelIndex = isMultiOut ? 2 + (i * 2) : 0;
		int rightChannelIndex = isMultiOut ? 2 + (i * 2) + 1 : 1;

		float* outL = outputBuffer.getWritePointer(leftChannelIndex);
		float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(rightChannelIndex) : nullptr;

		const float* inL = internalBuffer->getReadPointer(0);
		const float* inR = internalBuffer->getNumChannels() > 1 ? internalBuffer->getReadPointer(1) : nullptr;

		for (auto sample = 0; sample < outputBuffer.getNumSamples(); ++sample) 
		{
			outL[sample] += inL[sample];
			
			if (outR != nullptr && inR != nullptr) 
			{ 
				outR[sample] += inR[sample];
			}
		}
	}
}

bool PluginAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
	return new PluginAudioProcessorEditor(*this);
}


void PluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	auto state = mParameterValueTreeState.get()->copyState();
	std::unique_ptr<juce::XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
	{
		if (xmlState->hasTagName(mParameterValueTreeState.get()->state.getType()))
		{
			mParameterValueTreeState.get()->replaceState(juce::ValueTree::fromXml(*xmlState));
		}
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

void PluginAudioProcessor::loadAndPlayMidiFile(const juce::File& midiFile)
{
	juce::MidiFile midi;
	juce::FileInputStream stream(midiFile);

	if (!stream.openedOk() || !midi.readFrom(stream)) {
		DBG("Error reading MIDI file");
		return;
	}

	midi.convertTimestampTicksToSeconds();

	mScheduledMidiEvents.clear(); // Clear previous MIDI events
	for (int track = 0; track < midi.getNumTracks(); ++track)
	{
		auto* midiTrack = midi.getTrack(track);
		for (int event = 0; event < midiTrack->getNumEvents(); ++event)
		{
			auto midiEvent = midiTrack->getEventPointer(event);
			auto& message = midiEvent->message;
			auto timestamp = static_cast<double>(message.getTimeStamp() * getSampleRate());
			mScheduledMidiEvents.emplace_back(timestamp, message);
		}
	}
}

PluginPresetManager& PluginAudioProcessor::getPresetManager() {
	return *mPresetManager.get();
}