#include "PluginAudioProcessor.h"
#include "PluginAudioProcessorEditor.h"
#include "Configuration/Samples.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"
#include <random>
#include "Configuration/Strings.h"

PluginAudioProcessorEditor::PluginAudioProcessorEditor(PluginAudioProcessor& p)
	: AudioProcessorEditor(&p), mAudioProcessor(p)
{
	auto& apvts = mAudioProcessor.getParameterValueTreeState();

	mTabbedComponentPtr = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);

	mPresetComponentPtr.reset(new PresetComponent(mAudioProcessor.getPresetManager()));
	addAndMakeVisible(*mPresetComponentPtr);

	setSize(750, 750);
	setResizable(true, true);

	addAndMakeVisible(mTabbedComponentPtr.get());

	mDrumsComponentPtr.reset(new DrumsComponent(
		mAudioProcessor.getMidiNotesVector(), 
		apvts));
	mDrumsComponentPtr->mOnDrumMidiButtonClicked = ([this](int midiNote, float midiVelocity) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity);
		});
	mTabbedComponentPtr->addTab(Strings::drums, juce::Colours::lightgrey, mDrumsComponentPtr.get(), true);

	mSamplesComponentPtr.reset(new SamplesComponent(mAudioProcessor.getMidiNotesVector(), apvts, ([this](int midiNote, float midiVelocity, std::string micId) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity, micId);
		})));

	mOutputsComponentPtr.reset(new OutputsComponent(apvts, ([this](int midiNote, float midiVelocity) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity);
		})));

	mTabbedComponentPtr->addTab(Strings::samples, juce::Colours::lightgrey, mSamplesComponentPtr.get(), true);
	mTabbedComponentPtr->addTab(Strings::outputs, juce::Colours::lightgrey, mOutputsComponentPtr.get(), true);
}

PluginAudioProcessorEditor::~PluginAudioProcessorEditor()
{
}

void PluginAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginAudioProcessorEditor::resized()
{
	auto localBounds = getLocalBounds();

	mPresetComponentPtr->setBounds(localBounds.removeFromTop(50));

	mTabbedComponentPtr->setBounds(localBounds);
}
