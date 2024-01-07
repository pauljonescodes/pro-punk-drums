#pragma once

#include <JuceHeader.h>
#include "PresetComponent.h"
#include "../PluginPresetManager.h"

class DrumsComponent : public juce::Component, public juce::Button::Listener
{
public:
    DrumsComponent(
        const std::vector<int> midiNotesVector, 
        juce::AudioProcessorValueTreeState& apvts);
    ~DrumsComponent() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;
    
private:
    juce::OwnedArray<juce::TextButton> mMidiNoteButtonsOwnedArray;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mMultiOutAttachment;
    std::unique_ptr<juce::Slider> mVelocitySlider;
    std::unique_ptr<juce::Label> mVelocityLabel;
    std::unique_ptr<juce::ToggleButton> mMultiOutToggleButton;

    void buttonClicked(juce::Button* button) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsComponent)
    
};
