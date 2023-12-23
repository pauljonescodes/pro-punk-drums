#include "SampleParametersComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"
#include "../Configuration/Strings.h"
#include "../Configuration/GeneralMidi.h"

SamplesParametersComponent::SamplesParametersComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts)
: mApvts(apvts), mMicId(micId)
{
    mLabel.reset(new juce::Label(std::to_string(midiNote) + "_label", generalmidi::midiNoteToNameMap.at(midiNote) + " " + micId));
    addAndMakeVisible(mLabel.get());

    mNoteOnButton.reset(new juce::TextButton(std::to_string(midiNote) + " " + micId));
    mNoteOnButton->setComponentID(juce::String(midiNote));
    mNoteOnButton->addListener(this);
    addAndMakeVisible(mNoteOnButton.get());
    
    // Initialize gain slider and label
    auto gainParameterId = PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::gainId });
    auto* gainParameter = apvts.getParameter(gainParameterId);
    mGainSlider.reset(new juce::Slider("Gain Slider"));
    addAndMakeVisible(mGainSlider.get());
    auto& gainNormalizableRange = gainParameter->getNormalisableRange();
    mGainSlider->setRange(gainNormalizableRange.start, gainNormalizableRange.end, gainNormalizableRange.interval);
    mGainSlider->setValue(gainParameter->getValue());
    mGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                                                                                             apvts,
                                                                                             juce::String(gainParameterId),
                                                                                             *mGainSlider
                                                                                             );
    
    auto panParameterId = PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::panId});
    auto* panParameter = apvts.getParameter(panParameterId);
    mPanSlider.reset(new juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox));
    addAndMakeVisible(mPanSlider.get());
    auto& panNormalizableRange = panParameter->getNormalisableRange();
    mPanSlider->setRange(panNormalizableRange.start, panNormalizableRange.end, panNormalizableRange.interval);
    mPanSlider->setValue(panParameter->getValue());
    mPanAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                                                                                            apvts,
                                                                                            juce::String(panParameterId),
                                                                                            *mPanSlider
                                                                                            );
    
    auto phaseParameterId = PluginUtils::joinId({ std::to_string(midiNote), micId, parameters::phaseId });
    auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(phaseParameterId));
    mInvertPhaseToggleButton.reset(new juce::ToggleButton(strings::invertPhase));
    addAndMakeVisible(mInvertPhaseToggleButton.get());
    mInvertPhaseToggleButton->setToggleState(phaseParameter->get(), juce::dontSendNotification);
    mInvertPhaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
                                                                                                    apvts,
                                                                                                    phaseParameterId,
                                                                                                    *mInvertPhaseToggleButton
                                                                                                    );
}

SamplesParametersComponent::~SamplesParametersComponent()
{
    mGainSlider = nullptr;
    mInvertPhaseToggleButton = nullptr;
}

void SamplesParametersComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SamplesParametersComponent::resized()
{
    int padding = 10;
    int toggleSize = 75;
    int buttonWidth = 75;
    int labelHeight = 15; 

    auto area = getLocalBounds().reduced(padding); 

    mLabel->setBounds(area.removeFromTop(labelHeight));

    mInvertPhaseToggleButton->setBounds(area.removeFromLeft(toggleSize).reduced(0, padding));
    mPanSlider->setBounds(area.removeFromLeft(toggleSize).reduced(0, padding));

    auto buttonArea = area.removeFromRight(buttonWidth).reduced(padding, padding);

    mGainSlider->setBounds(area);
    mNoteOnButton->setBounds(buttonArea);
}

void SamplesParametersComponent::buttonClicked(juce::Button* button)
{
    const juce::String componentID = button->getComponentID();
    int midiNoteValue = componentID.getIntValue();

    if (mOnDrumMidiButtonClicked.has_value()) {
        mOnDrumMidiButtonClicked.value()(midiNoteValue, 0.75f, mMicId);
    }
}