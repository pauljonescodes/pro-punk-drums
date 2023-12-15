#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Configuration/Samples.h"
#include "PluginSynthesiser.h"
#include "PluginUtils.h"
#include "Configuration/Midi.h"

//==============================================================================
PluginAudioProcessor::PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), mParameterValueTreeState(std::make_unique<juce::AudioProcessorValueTreeState>(*this,
		nullptr, juce::Identifier("ABKParams"), createParameterLayout()))
#endif
{
	mAudioFormatManager = std::make_unique<juce::AudioFormatManager>();
	mAudioFormatManager->registerBasicFormats();

	mSynthesiserPtr = std::make_unique<PluginSynthesiser>();
	mInternalBufferPtr = std::make_unique<juce::AudioBuffer<float>>(1, 1024);


	for (const auto& midiNote : generalmidi::midiNotesVector)
	{
		std::string generalMidiName = generalmidi::midiNoteToNameMap.at(midiNote);
		std::string generalMidiSnakeCaseName = PluginUtils::toSnakeCase(generalMidiName);

		for (int velocityIndex = midi::minimumVelocity; velocityIndex <= midi::maximumVelocity; velocityIndex++)
		{
			bool foundResourceAtVelocity = false;
			for (int variationIndex = 0; variationIndex < constants::maximumVariations; variationIndex++)
			{
				std::string resourceName = generalMidiSnakeCaseName + "_" + std::to_string(velocityIndex + 1) + "_" + std::to_string(variationIndex + 1);
				bool foundResourceAtVariation = false;

				for (int resourceIndex = 0; resourceIndex < BinaryData::namedResourceListSize; ++resourceIndex)
				{
					std::string namedResourceAtIndex = std::string(BinaryData::namedResourceList[resourceIndex]);
					size_t resourceNameFindResult = namedResourceAtIndex.find(resourceName);
					if (resourceNameFindResult != std::string::npos)
					{
						foundResourceAtVariation = true;
						foundResourceAtVelocity = true;

						mSynthesiserPtr->addSample(
							namedResourceAtIndex,
							samples::bitRate,
							samples::bitDepth,
							midiNote,
							generalmidi::midiNoteToStopNotesMap.at(midiNote),
							velocityIndex,
							variationIndex,
							*mAudioFormatManager.get()
						);
					}
				}

				if (!foundResourceAtVariation)
				{
					break;
				}
			}

			if (!foundResourceAtVelocity)
			{
				break;
			}
		}

	}
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;


	for (const auto& element : samples::samplesVector) {
		std::string gainId = element + "_gain";
		std::string gainName = element + " Gain";
		layout.add(std::make_unique<juce::AudioParameterFloat>(gainId, gainName, 0.0f, 1.0f, 0.5f));
	}

	return layout;
}

PluginAudioProcessor::~PluginAudioProcessor()
{
}

//==============================================================================
const juce::String PluginAudioProcessor::getName() const
{
	return JucePlugin_Name;
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
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	int numSamples = buffer.getNumSamples();  // Number of samples to process
	mInternalBufferPtr->setSize(1, numSamples, true, true, true);

	mInternalBufferPtr->clear();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
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
	return new PluginProcessorEditor(*this);
}

//==============================================================================
void PluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void PluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
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

