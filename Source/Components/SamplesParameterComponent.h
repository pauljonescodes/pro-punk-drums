#pragma once
#include <JuceHeader.h>

class SamplesParameterComponent : public juce::Component, public juce::Button::Listener
{
public:
    SamplesParameterComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts);
    ~SamplesParameterComponent() override;
    
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::optional<std::function<void(int, float, std::string)>> mOnDrumMidiButtonClicked;
    
private:
    std::string mMicId;

    juce::AudioProcessorValueTreeState& mApvts;
    
    std::unique_ptr <juce::Label> mLabel;
    
    std::unique_ptr <juce::Slider> mGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    
    std::unique_ptr <juce::Slider> mPanSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPanAttachment;
    
    std::unique_ptr <juce::ToggleButton> mInvertPhaseToggleButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mInvertPhaseAttachment;

    std::unique_ptr <juce::TextButton> mNoteOnButton;

    void buttonClicked(juce::Button* button) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesParameterComponent)
};
