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

	mTabbedComponent = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);

	setSize(750, 750);
	setResizable(true, true);

	addAndMakeVisible(mTabbedComponent.get());

	mDrumsComponent.reset(new DrumsComponent(
		mAudioProcessor.getMidiNotesVector(), 
		apvts, 
		mAudioProcessor.getPresetManager()));
	mDrumsComponent->mOnDrumMidiButtonClicked = ([this](int midiNote, float midiVelocity) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity);
		});
	mDrumsComponent->mOnMidiFileChoser = ([this](juce::File midiFile) -> void {
		mAudioProcessor.loadAndPlayMidiFile(midiFile);
		});
	mTabbedComponent->addTab(strings::drums, juce::Colours::lightgrey, mDrumsComponent.get(), true);

	mSamplesComponent.reset(new SamplesComponent(mAudioProcessor.getMidiNotesVector(), apvts, ([this](int midiNote, float midiVelocity, std::string micId) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity, micId);
		})));

	mOutputsComponent.reset(new OutputsComponent(apvts, ([this](int midiNote, float midiVelocity, std::string micId) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity, micId);
		})));

	mTabbedComponent->addTab(strings::samples, juce::Colours::lightgrey, mSamplesComponent.get(), true);
	mTabbedComponent->addTab(strings::outputs, juce::Colours::lightgrey, mOutputsComponent.get(), true);
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
	mTabbedComponent->setBounds(getLocalBounds());
}
