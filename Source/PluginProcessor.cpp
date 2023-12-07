/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Samples.h"
#include "MultiIntensitySynthesiser.h"

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
	mFormatManager.registerBasicFormats();

	mBassDrum1Synthesiser = std::make_unique<juce::Synthesiser>();
	mElectricSnareSynthesiser = std::make_unique<juce::Synthesiser>();

	mCowbellSynthesiser = std::make_unique<MultiSampleSynthesiser>();
	mHandClapSynthesiser = std::make_unique<MultiSampleSynthesiser>();
	mTambourineSynthesiser = std::make_unique<MultiSampleSynthesiser>();
	mMaracaSynthesiser = std::make_unique<MultiSampleSynthesiser>();
	mTriangleSynthesiser = std::make_unique<MultiSampleSynthesiser>();

	mHighTomSynthesiser = std::make_unique<MultiIntensitySynthesiser>();
	mSideStickSynthesiser = std::make_unique<MultiIntensitySynthesiser>();
	mHighFloorTomSynthesiser = std::make_unique<MultiIntensitySynthesiser>();

	for (const auto& instrument : samples::instrumentVector) {
		switch (instrument) {
		case samples::InstrumentEnum::bassDrum1:
			initializeSimpleInstrument(instrument, *mBassDrum1Synthesiser);
			break;
		case samples::InstrumentEnum::electricSnare:
			initializeSimpleInstrument(instrument, *mElectricSnareSynthesiser);
			break;
		case samples::InstrumentEnum::cowbell:
			initializeVariedInstrument(instrument, *mCowbellSynthesiser);
			break;
		case samples::InstrumentEnum::handClap:
			initializeVariedInstrument(instrument, *mHandClapSynthesiser);
			break;
		case samples::InstrumentEnum::tambourine:
			initializeVariedInstrument(instrument, *mTambourineSynthesiser);
			break;
		case samples::InstrumentEnum::maraca:
			initializeVariedInstrument(instrument, *mMaracaSynthesiser);
			break;
		case samples::InstrumentEnum::triangleMute:
			initializeVariedInstrument(instrument, *mTriangleSynthesiser);
			break;
		case samples::InstrumentEnum::tomHigh:
			initializeIntensityInstrument(instrument, *mHighTomSynthesiser);
			break;
		case samples::InstrumentEnum::tomFloorHigh:
			initializeIntensityInstrument(instrument, *mHighFloorTomSynthesiser);
			break;
		case samples::InstrumentEnum::sideStick:
			initializeIntensityInstrument(instrument, *mSideStickSynthesiser);
			break;
		default:
			break;
		}
	}
}

void ABKit2AudioProcessor::initializeSimpleInstrument(samples::InstrumentEnum instrument, juce::Synthesiser& synthesizer) {
	std::string sampleName = samples::instrumentIdMap.at(instrument);

	int midiNote = samples::instrumentToGeneralMidiNoteMap.at(instrument);
	std::string resourceName = sampleName + "_wav";

	int dataSizeInBytes;

	const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
	auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
	juce::AudioFormatReader* reader = mFormatManager.createReaderFor(std::move(memoryInputStream));

	juce::BigInteger range;
	range.setRange(midiNote, 1, true);

	juce::SamplerSound* sound = new juce::SamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, 5.0);
	synthesizer.addSound(sound);
	synthesizer.addVoice(new juce::SamplerVoice());
}

void ABKit2AudioProcessor::initializeVariedInstrument(samples::InstrumentEnum instrument, MultiSampleSynthesiser& synthesizer) {
	std::string sampleName = samples::instrumentIdMap.at(instrument);

	int midiNote = samples::instrumentToGeneralMidiNoteMap.at(instrument);

	int dataSizeInBytes;

	for (int i = 0; i < samples::standardVariationsCount; i++) {
		std::string resourceName = sampleName + "_" + std::to_string(i + 1) + "_wav";

		const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
		auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
		juce::AudioFormatReader* reader = mFormatManager.createReaderFor(std::move(memoryInputStream));

		juce::BigInteger range;
		range.setRange(midiNote, 1, true);

		juce::SamplerSound* sound = new juce::SamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, 5.0);
		synthesizer.addSound(sound);
	}

	synthesizer.addVoice(new juce::SamplerVoice());
}

void ABKit2AudioProcessor::initializeIntensityInstrument(samples::InstrumentEnum instrument, MultiIntensitySynthesiser& synthesizer) {
	std::string sampleName = samples::instrumentIdMap.at(instrument);

	int midiNote = samples::instrumentToGeneralMidiNoteMap.at(instrument);

	int dataSizeInBytes;

	for (int intensityIndex = 0; intensityIndex < samples::intensityVector.size(); intensityIndex++) {
		samples::IntensityEnum intensity = samples::intensityVector[intensityIndex];

		for (int variationIndex = 0; variationIndex < samples::standardVariationsCount; variationIndex++) {
			std::string resourceName = sampleName + "_" + samples::intensityIdMap.at(intensity) + "_" + std::to_string(variationIndex + 1) + "_wav";

			const char* sourceData = BinaryData::getNamedResource(resourceName.c_str(), dataSizeInBytes);
			auto memoryInputStream = std::make_unique<juce::MemoryInputStream>(sourceData, dataSizeInBytes, false);
			juce::AudioFormatReader* reader = mFormatManager.createReaderFor(std::move(memoryInputStream));

			juce::BigInteger range;
			range.setRange(midiNote, 1, true);

			juce::SamplerSound* sound = new juce::SamplerSound(juce::String(resourceName), *reader, range, midiNote, 0.0, 0.0, 5.0);
			synthesizer.addSamplerSoundAtIntensityIndex(sound, intensityIndex);
		}

		synthesizer.addVoice(new juce::SamplerVoice());
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
	(*mBassDrum1Synthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mElectricSnareSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mCowbellSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mHandClapSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mTambourineSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mMaracaSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mTriangleSynthesiser).setCurrentPlaybackSampleRate(sampleRate);

	(*mSideStickSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mHighTomSynthesiser).setCurrentPlaybackSampleRate(sampleRate);
	(*mHighFloorTomSynthesiser).setCurrentPlaybackSampleRate(sampleRate);

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
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());


	(*mBassDrum1Synthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mElectricSnareSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mCowbellSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	(*mHandClapSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mTambourineSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mMaracaSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mTriangleSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

	(*mSideStickSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mHighTomSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
	(*mHighFloorTomSynthesiser).renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
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
void ABKit2AudioProcessor::noteOnSynthesiser(int midiNoteNumber, const float velocity)
{
	//(*mAcousticBassDrumSynthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mBassDrum1Synthesiser).noteOn(0, midiNoteNumber, 1.0f);
	(*mSideStickSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mAcousticSnareSynthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mHandClapSynthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mElectricSnareSynthesiser).noteOn(0, midiNoteNumber, 1.0f);
	(*mHighFloorTomSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mCrashCymbal1Synthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mHighTomSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mRideCymbal1Synthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mChineseCymbalSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mRideBellSynthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mTambourineSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mSplashCymbalSynthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mCowbellSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mCrashCymbal2Synthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mMaracaSynthesiser).noteOn(0, midiNoteNumber, velocity);
	//(*mHiHatSynthesiser).noteOn(0, midiNoteNumber, velocity);
	(*mTriangleSynthesiser).noteOn(0, midiNoteNumber, velocity);
}

