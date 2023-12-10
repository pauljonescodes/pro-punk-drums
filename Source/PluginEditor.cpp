/*
 ==============================================================================
 
 This file contains the basic framework code for a JUCE plugin editor.
 
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Configuration/Samples.h"
#include "Components/SamplerComponent.h"
#include <random>

//==============================================================================
ABKit2AudioProcessorEditor::ABKit2AudioProcessorEditor(ABKit2AudioProcessor& p)
: AudioProcessorEditor(&p), mAudioProcessor(p)
{
    mTabbedComponent = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(625, 750);
    setResizable(true, true);
    
    addAndMakeVisible(mTabbedComponent.get());
    
    mSamplerComponent.reset(new SamplerComponent());
    
    auto samplerComponent = mSamplerComponent.get();
    samplerComponent->mOnDrumMidiButtonClicked = ([this](int midiNote, float midiVelocity) -> void {
        mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity);
    });
    
    mTabbedComponent->addTab(TRANS("Drums"), juce::Colours::lightgrey, samplerComponent, true);
    mTabbedComponent->addTab(TRANS("Mixer"), juce::Colours::lightgrey, new juce::Component(), true);
    
}

ABKit2AudioProcessorEditor::~ABKit2AudioProcessorEditor()
{
}

//==============================================================================
void ABKit2AudioProcessorEditor::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ABKit2AudioProcessorEditor::resized()
{
    mTabbedComponent->setBounds(getLocalBounds());
}
