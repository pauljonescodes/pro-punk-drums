/*
 ==============================================================================

 This file contains the basic framework code for a JUCE plugin editor.

 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Configuration/Samples.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"
#include <random>

 //==============================================================================
PluginProcessorEditor::PluginProcessorEditor(PluginAudioProcessor& p)
	: AudioProcessorEditor(&p), mAudioProcessor(p)
{
	mTabbedComponent = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);

	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(750, 750);
	setResizable(true, true);

	addAndMakeVisible(mTabbedComponent.get());

	mDrumsComponent.reset(new DrumsComponent(mAudioProcessor.getMidiNotesVector()));
	auto drumsComponent = mDrumsComponent.get();
	drumsComponent->mOnDrumMidiButtonClicked = ([this](int midiNote, float midiVelocity) -> void {
		mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity);
	});
	mTabbedComponent->addTab("Drums", juce::Colours::lightgrey, drumsComponent, true);

	auto& apvts = mAudioProcessor.getParameterValueTreeState();
	mSamplesComponent.reset(new SamplesComponent(mAudioProcessor.getMidiNotesVector(), apvts));
	auto samplesComponent = mSamplesComponent.get();

	mTabbedComponent->addTab("Samples", juce::Colours::lightgrey, samplesComponent, true);
	mTabbedComponent->addTab("Mixer", juce::Colours::lightgrey, new juce::Component(), true);
}

PluginProcessorEditor::~PluginProcessorEditor()
{
}

void PluginProcessorEditor::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginProcessorEditor::resized()
{
	mTabbedComponent->setBounds(getLocalBounds());
}
