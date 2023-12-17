#pragma once

#include <JuceHeader.h>

class DrumsComponent : public juce::Component, public juce::Button::Listener
{
public:
    DrumsComponent(std::vector<int> midiNotesVector);
    ~DrumsComponent() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;
    std::optional<std::function<void(juce::File)>> mOnMidiFileChoser;
    
private:
    juce::OwnedArray<juce::TextButton> mMidiNoteButtons;
    std::unique_ptr<juce::TextButton> mMidiFileButton;
    std::unique_ptr <juce::Slider> mVelocitySlider;
    std::unique_ptr <juce::Label> mVelocityLabel;
    std::unique_ptr <juce::FileChooser> mFileChooser;
    
    void buttonClicked(juce::Button* button) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DrumsComponent)
    
};
