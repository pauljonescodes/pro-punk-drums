#pragma once
#include <JuceHeader.h>

class SamplesParametersComponent : public juce::Component, public juce::Button::Listener
{
public:
    SamplesParametersComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts);
    ~SamplesParametersComponent() override;
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::optional<std::function<void(int, float, std::string)>> mOnDrumMidiButtonClicked;
    
private:
    std::string mMicId;
    
    std::unique_ptr <juce::GroupComponent> mGroupComponentPtr;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachmentPtr, mPanAttachmentPtr;
    std::unique_ptr<juce::Slider> mRatioSliderPtr, mPanSliderPtr;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mInvertPhaseAttachmentPtr;
    std::unique_ptr<juce::ToggleButton> mInvertPhaseToggleButtonPtr;

    std::unique_ptr<juce::TextButton> mNoteOnButtonPtr;
    
    juce::AudioProcessorValueTreeState& mApvts;

    void buttonClicked(juce::Button* button) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesParametersComponent)
};
