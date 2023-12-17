#pragma once
#include <JuceHeader.h>

class SamplesParameterComponent : public juce::Component
{
public:
    SamplesParameterComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts);
    ~SamplesParameterComponent() override;
    
    
    void paint(juce::Graphics&) override;
    void resized() override;
    
    std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;
    
private:
    juce::AudioProcessorValueTreeState& mApvts;
    
    std::unique_ptr <juce::Label> mLabel;
    
    std::unique_ptr <juce::Slider> mGainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mGainAttachment;
    
    std::unique_ptr <juce::Slider> mPanSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mPanAttachment;
    
    std::unique_ptr <juce::ToggleButton> mInvertPhaseToggleButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mInvertPhaseAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplesParameterComponent)
};
