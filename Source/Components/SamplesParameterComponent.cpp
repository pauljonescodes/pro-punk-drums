#include "SamplesParameterComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Constants.h"
#include "../Configuration/Strings.h"
#include "../Configuration/GeneralMidi.h"

SamplesParameterComponent::SamplesParameterComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts)
: mApvts(apvts)
{
    mLabel.reset(new juce::Label(std::to_string(midiNote) + "_label", generalmidi::midiNoteToNameMap.at(midiNote) + " " + PluginUtils::capitalizeFirstLetter(micId)));
    addAndMakeVisible(mLabel.get());
    
    // Initialize gain slider and label
    auto gainParameterId = PluginUtils::getParamId(midiNote, micId, constants::gainId);
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
    
    auto panParameterId = PluginUtils::getParamId(midiNote, micId, constants::panId);
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
    
    auto phaseParameterId = PluginUtils::getParamId(midiNote, micId, constants::phaseId);
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

SamplesParameterComponent::~SamplesParameterComponent()
{
    mGainSlider = nullptr;
    mInvertPhaseToggleButton = nullptr;
}

void SamplesParameterComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SamplesParameterComponent::resized()
{
    int padding = 10;
    int toggleSize = 75;
    int labelHeight = 15; // Set the height for the label
    
    auto area = getLocalBounds().reduced(padding); // Reduce the entire area first for padding
    
    mLabel->setBounds(area.removeFromTop(labelHeight));
    mInvertPhaseToggleButton->setBounds(area.removeFromLeft(toggleSize).reduced(0, padding));
    mPanSlider->setBounds(area.removeFromLeft(toggleSize).reduced(0, padding));
    mGainSlider->setBounds(area);
}
