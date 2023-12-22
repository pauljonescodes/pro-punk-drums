#pragma once

#include <JuceHeader.h>
#include "PresetComponent.h"
#include "../PluginPresetManager.h"

class DrumsComponent : public juce::Component, public juce::Button::Listener
{
public:
    DrumsComponent(
        const std::vector<int> midiNotesVector, 
        juce::AudioProcessorValueTreeState& apvts,  
        PluginPresetManager& presetManager);
    ~DrumsComponent() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;
    std::optional<std::function<void(juce::File)>> mOnMidiFileChoser;
    
private:
    juce::OwnedArray<juce::TextButton> mMidiNoteButtons;

    std::unique_ptr<PresetComponent> mPresetComponent;

    std::unique_ptr<juce::TextButton> mMidiFileButton;
    std::unique_ptr <juce::Slider> mVelocitySlider;
    std::unique_ptr <juce::Label> mVelocityLabel;
    std::unique_ptr <juce::FileChooser> mFileChooser;
    std::unique_ptr <juce::ToggleButton> mMultiOutToggleButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mMultiOutAttachment;

    void buttonClicked(juce::Button* button) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsComponent)
    
};
