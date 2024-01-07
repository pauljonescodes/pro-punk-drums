#include "SampleParametersComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"
#include "../Configuration/Strings.h"
#include "../Configuration/GeneralMidi.h"

SamplesParametersComponent::SamplesParametersComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts)
: mMicId(micId), mApvts(apvts)
{
    mGroupComponentPtr.reset(new juce::GroupComponent(std::to_string(midiNote) + "_label", GeneralMidiPercussion::midiNoteToNameMap.at(midiNote) + " " + PluginUtils::toTitleCase(micId)));
    addAndMakeVisible(mGroupComponentPtr.get());
    
    mNoteOnButtonPtr.reset(new juce::TextButton(std::to_string(midiNote) + " " + micId));
    mNoteOnButtonPtr->setComponentID(juce::String(midiNote));
    mNoteOnButtonPtr->addListener(this);
    addAndMakeVisible(mNoteOnButtonPtr.get());
    
    // Initialize gain slider and label
    auto gainParameterId = PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::gainComponentId });
    mRatioSliderPtr.reset(new juce::Slider(PluginUtils::toTitleCase(gainParameterId)));
    mRatioSliderPtr->setScrollWheelEnabled(false);
    mRatioSliderPtr->setTextValueSuffix(Strings::db);
    addAndMakeVisible(mRatioSliderPtr.get());
    mGainAttachmentPtr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
                                                                                   apvts,
                                                                                   juce::String(gainParameterId),
                                                                                   *mRatioSliderPtr
                                                                                   ));
    
    auto panParameterId = PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::panComponentId});
    mPanSliderPtr.reset(new juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow));
    mPanSliderPtr->setScrollWheelEnabled(false);
    mPanSliderPtr->setTextValueSuffix(" " + Strings::pan);
    addAndMakeVisible(mPanSliderPtr.get());
    mPanAttachmentPtr.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(
                                                                                  apvts,
                                                                                  juce::String(panParameterId),
                                                                                  *mPanSliderPtr
                                                                                  ));
    
    auto phaseParameterId = PluginUtils::joinAndSnakeCase({ std::to_string(midiNote), micId, AudioParameters::phaseComponentId });
    mInvertPhaseToggleButtonPtr.reset(new juce::ToggleButton(Strings::invertPhase));
    addAndMakeVisible(mInvertPhaseToggleButtonPtr.get());
    mInvertPhaseAttachmentPtr.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
                                                                                          apvts,
                                                                                          phaseParameterId,
                                                                                          *mInvertPhaseToggleButtonPtr
                                                                                          ));
}

SamplesParametersComponent::~SamplesParametersComponent()
{
    mNoteOnButtonPtr->removeListener(this);
    
    mGainAttachmentPtr.reset();
    mPanAttachmentPtr.reset();
    mInvertPhaseAttachmentPtr.reset();

    // Now it's safe to reset the sliders and buttons
    mRatioSliderPtr.reset();
    mPanSliderPtr.reset();
    mInvertPhaseToggleButtonPtr.reset();
    mNoteOnButtonPtr.reset();
    mGroupComponentPtr.reset();
}

void SamplesParametersComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SamplesParametersComponent::resized()
{
    int padding = 10;
    int toggleSize = 100;
    int labelHeight = 15;
    
    auto area = getLocalBounds().reduced(padding);
    
    mGroupComponentPtr->setBounds(getLocalBounds());
    
    mInvertPhaseToggleButtonPtr->setBounds(area.removeFromLeft(toggleSize).reduced(padding));
    mPanSliderPtr->setBounds(area.removeFromLeft(toggleSize).reduced(padding));
    
    auto buttonArea = area.removeFromRight(area.getHeight()).reduced(padding);
    
    mRatioSliderPtr->setBounds(area);
    mNoteOnButtonPtr->setBounds(buttonArea);
}

void SamplesParametersComponent::buttonClicked(juce::Button* button)
{
    const juce::String componentID = button->getComponentID();
    int midiNoteValue = componentID.getIntValue();
    
    if (mOnDrumMidiButtonClicked.has_value()) {
        mOnDrumMidiButtonClicked.value()(midiNoteValue, 0.75f, mMicId);
    }
}
