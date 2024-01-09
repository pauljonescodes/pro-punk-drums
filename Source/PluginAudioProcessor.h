#pragma once
#include <JuceHeader.h>
#include "Configuration/Samples.h"
#include "Synthesiser/PluginSynthesiser.h"
#include "PluginPresetManager.h"

class PluginAudioProcessor : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:

	PluginAudioProcessor();
	~PluginAudioProcessor() override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	void parameterChanged(const juce::String& parameterID, float newValue) override;

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	void noteOnSynthesisers(int midiNoteNumber, float velocity);
	void noteOnSynthesisers(int midiNoteNumber, float velocity, std::string micId);

	std::vector<int> getMidiNotesVector();
	juce::AudioProcessorValueTreeState& getParameterValueTreeState() const;

	PluginPresetManager& getPresetManager();
private:

	std::unique_ptr<juce::AudioFormatManager> mAudioFormatManagerPtr;
	std::unique_ptr<PluginPresetManager> mPresetManagerPtr;
	std::unique_ptr<juce::AudioProcessorValueTreeState> mAudioProcessorValueTreeStatePtr;

	std::vector<std::unique_ptr<PluginSynthesiser>> mSynthesiserPtrVector; // 6 synths
	std::vector<std::unique_ptr<juce::AudioBuffer<float>>> mSynthesiserBufferPtrVector; 
	
	std::vector<std::unique_ptr<juce::dsp::Gain<float>>> mReverbGains; // 6 revs
	std::vector<std::unique_ptr<juce::dsp::Reverb>> mReverbs;
	std::vector<std::unique_ptr<juce::AudioBuffer<float>>> mReverbBufferPtrVector;

	std::unique_ptr<juce::AudioBuffer<float>> mRoomBufferPtr;

	std::vector<std::unique_ptr<juce::dsp::Compressor<float>>> mCompressors; // 8 comps
	std::vector<std::unique_ptr<juce::dsp::Gain<float>>> mCompressorGains;
	std::vector<std::unique_ptr<juce::dsp::DryWetMixer<float>>> mCompressorDryWetMixers;

	std::vector<std::unique_ptr<juce::dsp::Gain<float>>> mChannelGains;

	std::vector<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
		juce::dsp::IIR::Coefficients<float>>> mLowShelfFilters;
	std::vector<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
		juce::dsp::IIR::Coefficients<float>>> mPeakFilters;
	std::vector<juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,
		juce::dsp::IIR::Coefficients<float>>> mHighShelfFilters;

	juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginAudioProcessor)
};
