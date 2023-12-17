#include "PluginAudioProcessor.h"
#include "PluginAudioProcessorEditor.h"
#include "Configuration/Samples.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"
#include <random>

PluginAudioProcessorEditor::PluginAudioProcessorEditor(PluginAudioProcessor& p)
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
    drumsComponent->mOnMidiFileChoser = ([this](juce::File midiFile) -> void {
        mAudioProcessor.loadAndPlayMidiFile(midiFile);
    });
    mTabbedComponent->addTab("Drums", juce::Colours::lightgrey, drumsComponent, true);
    
    auto& apvts = mAudioProcessor.getParameterValueTreeState();
    mSamplesComponent.reset(new SamplesComponent(mAudioProcessor.getMidiNotesVector(), apvts));
    auto samplesComponent = mSamplesComponent.get();
    
    mTabbedComponent->addTab("Samples", juce::Colours::lightgrey, samplesComponent, true);
    mTabbedComponent->addTab("Mixer", juce::Colours::lightgrey, new juce::Component(), true);
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
