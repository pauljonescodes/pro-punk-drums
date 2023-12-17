#include "PluginAudioProcessor.h"
#include "PluginAudioProcessorEditor.h"
#include "Configuration/Samples.h"
#include "Synthesiser/PluginSynthesiser.h"
#include "PluginUtils.h"
#include "Configuration/Midi.h"
#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

//==============================================================================
PluginAudioProcessor::PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
		.withOutput("1 Kick", juce::AudioChannelSet::stereo(), true) 
		.withOutput("2 Snare", juce::AudioChannelSet::stereo(), true)
		.withOutput("3 Toms", juce::AudioChannelSet::stereo(), true)
		.withOutput("4 Hi-hat", juce::AudioChannelSet::stereo(), true)
		.withOutput("5 Cymbals", juce::AudioChannelSet::stereo(), true)
		.withOutput("6 Percussion", juce::AudioChannelSet::stereo(), true)
#endif
	), mParameterValueTreeState(std::make_unique<juce::AudioProcessorValueTreeState>(*this,
		nullptr, juce::Identifier("plugin_params"), createParameterLayout()))
#endif
{
	mAudioFormatManager = std::make_unique<juce::AudioFormatManager>();
	mAudioFormatManager->registerBasicFormats();

	mSynthesiserPtr = std::make_unique<PluginSynthesiser>();
	mInternalBufferPtr = std::make_unique<juce::AudioBuffer<float>>(1, 1024);

	for (int resourceIndex = 0; resourceIndex < BinaryData::namedResourceListSize; resourceIndex++)
	{
		std::string namedResource = BinaryData::namedResourceList[resourceIndex];
		std::string micId, midiName, generalMidiSnakeCaseName = "";
		int variationIndex, velocityIndex, midiNote = -1;

		for (int midiNameIndex = 0; midiNameIndex < generalmidi::midiNamesVector.size(); midiNameIndex++)
		{
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
						midiNameIndex = generalmidi::midiNamesVector.size() + 1;
					}
					break;
				default:
					DBG("invalid name " + generalMidiSnakeCaseName);
					midiNameIndex = generalmidi::midiNamesVector.size() + 1;
					break;
				}
			}
		}

		if (midiNote != -1)
		{
			auto* gainParameter = mParameterValueTreeState->getParameter(PluginUtils::getParamId(midiNote, micId, constants::gainId));
			auto* panParameter = mParameterValueTreeState->getParameter(PluginUtils::getParamId(midiNote, micId, constants::panId));
			auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(mParameterValueTreeState->getParameter(PluginUtils::getParamId(midiNote, micId, constants::phaseId)));

			mSynthesiserPtr->addSample(
				namedResource,
				samples::bitRate,
				samples::bitDepth,
				midiNote,
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

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	for (const auto& pair : PluginUtils::getUniqueMidiNoteMicCombinations()) {
		int midiNote = pair.first;
		std::string midiName = generalmidi::midiNoteToNameMap.at(midiNote);
		const std::set<std::string>& micIds = pair.second;

		for (const std::string& micId : micIds) {
			std::string gainName = midiName + " " + PluginUtils::capitalizeFirstLetter(micId) + " Gain";
			std::string gainParameterId = PluginUtils::getParamId(midiNote, micId, constants::gainId);
			layout.add(std::make_unique<juce::AudioParameterFloat>(gainParameterId, gainName, juce::NormalisableRange(-60.0f, 12.0f, 0.01f), 0.0f));

			std::string panName = midiName + " " + PluginUtils::capitalizeFirstLetter(micId) + " Pan";
			layout.add(std::make_unique<juce::AudioParameterFloat>(PluginUtils::getParamId(midiNote, micId, constants::panId), panName, 0.0f, 1.0f, 0.5f));

			std::string phaseName = midiName + " " + PluginUtils::capitalizeFirstLetter(micId) + " Phase";
			layout.add(std::make_unique<juce::AudioParameterBool>(PluginUtils::getParamId(midiNote, micId, constants::phaseId), phaseName, false));
		}
	}

	return layout;
}

PluginAudioProcessor::~PluginAudioProcessor()
{
}

std::vector<int> PluginAudioProcessor::getMidiNotesVector()
{
	return mSynthesiserPtr.get()->getMidiNotesVector();
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

//==============================================================================
void PluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	mInternalBufferPtr->setSize(1, samplesPerBlock);
	mSynthesiserPtr->setCurrentPlaybackSampleRate(sampleRate);
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

void PluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::MidiBuffer bufferToProcess;

	// Iterate through scheduled MIDI events
	for (auto it = scheduledMidiEvents.begin(); it != scheduledMidiEvents.end();)
	{
		if (it->first <= currentSamplePosition)
		{
			// Calculate the sample number relative to the start of the current buffer
			int sampleNumberInBuffer = static_cast<int>(it->first - currentSamplePosition + buffer.getNumSamples());
			bufferToProcess.addEvent(it->second, sampleNumberInBuffer);
			it = scheduledMidiEvents.erase(it); // Remove the event after scheduling
		}
		else
		{
			++it;
		}
	}
	currentSamplePosition += buffer.getNumSamples();

	// Merge with incoming MIDI messages
	bufferToProcess.addEvents(midiMessages, 0, buffer.getNumSamples(), 0);
	midiMessages.swapWith(bufferToProcess);

	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	mSynthesiserPtr->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
	return new PluginAudioProcessorEditor(*this);
}

//==============================================================================
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new PluginAudioProcessor();
}

//==============================================================================
void PluginAudioProcessor::noteOnSynthesisers(int midiNoteNumber, const float velocity)
{
	mSynthesiserPtr->noteOn(0, midiNoteNumber, velocity);
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

	scheduledMidiEvents.clear(); // Clear previous MIDI events
	for (int track = 0; track < midi.getNumTracks(); ++track)
	{
		auto* midiTrack = midi.getTrack(track);
		for (int event = 0; event < midiTrack->getNumEvents(); ++event)
		{
			auto midiEvent = midiTrack->getEventPointer(event);
			auto message = midiEvent->message;
			auto timestamp = static_cast<double>(message.getTimeStamp() * getSampleRate());
			scheduledMidiEvents.emplace_back(timestamp, message);
		}
	}

}


