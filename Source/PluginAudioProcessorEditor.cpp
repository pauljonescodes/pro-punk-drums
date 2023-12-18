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

	mDrumsComponent.reset(new DrumsComponent(mAudioProcessor.getMidiNotesVector(), apvts));
	auto drumsComponent = mDrumsComponent.get();
	drumsComponent->mOnDrumMidiButtonClicked = ([this](int midiNote, float midiVelocity) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity);
		});
	drumsComponent->mOnMidiFileChoser = ([this](juce::File midiFile) -> void {
		mAudioProcessor.loadAndPlayMidiFile(midiFile);
		});
	mTabbedComponent->addTab(strings::drums, juce::Colours::lightgrey, drumsComponent, true);

	mSamplesComponent.reset(new SamplesComponent(mAudioProcessor.getMidiNotesVector(), apvts));
	auto samplesComponent = mSamplesComponent.get();

	mTabbedComponent->addTab(strings::samples, juce::Colours::lightgrey, samplesComponent, true);
	mTabbedComponent->addTab(strings::mixer, juce::Colours::lightgrey, new juce::Component(), true);
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
