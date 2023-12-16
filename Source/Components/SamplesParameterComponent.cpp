/*
  ==============================================================================

    SamplesParameterComponent.cpp
    Created: 16 Dec 2023 12:02:19am
    Author:  paulm

  ==============================================================================
*/

#include "SamplesParameterComponent.h"
#include "../PluginUtils.h"
#include "../Configuration/Constants.h"

SamplesParameterComponent::SamplesParameterComponent(int midiNote, std::string micId, juce::AudioProcessorValueTreeState& apvts)
    : mApvts(apvts)
{
    auto gainParameterId = PluginUtils::getParamId(midiNote, micId, constants::gainId);
    auto* gainParameter = apvts.getParameter(gainParameterId);

    // Initialize gain slider and label
    mGainSlider.reset(new juce::Slider("Gain Slider"));
    addAndMakeVisible(mGainSlider.get());
    auto& normalizableRange = gainParameter->getNormalisableRange();
    mGainSlider->setRange(normalizableRange.start, normalizableRange.end, normalizableRange.interval);
    mGainSlider->setValue(gainParameter->getValue()); 
        const std::string test = "";

    mGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        juce::String(gainParameterId),
        *mGainSlider
        );

    mGainLabel.reset(new juce::Label(gainParameter->getParameterID() + "_label", gainParameter->getName(99)));
    addAndMakeVisible(mGainLabel.get());
    mGainLabel->attachToComponent(mGainSlider.get(), true);

    auto phaseParameterId = PluginUtils::getParamId(midiNote, micId, constants::phaseId);
    auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(phaseParameterId));

    // Initialize phase toggle button and label
    mInvertPhaseToggleButton.reset(new juce::ToggleButton("Invert Phase"));
    addAndMakeVisible(mInvertPhaseToggleButton.get());
    mInvertPhaseToggleButton->setToggleState(phaseParameter->get(), juce::dontSendNotification);

    mInvertPhaseLabel.reset(new juce::Label("Phase Label", "Invert Phase"));
    addAndMakeVisible(mInvertPhaseLabel.get());
    mInvertPhaseLabel->attachToComponent(mInvertPhaseToggleButton.get(), true);

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
    auto area = getLocalBounds(); // Get the local bounds of the component
    auto sliderArea = area.removeFromTop(50).removeFromRight(area.getWidth() - 250).reduced(10, 0);
    mGainSlider->setBounds(sliderArea);

    auto buttonArea = area.removeFromTop(50); // Assuming the height of the button area is 50
    mInvertPhaseToggleButton->setBounds(0, 0 , 50, 50);
    mInvertPhaseLabel->setBounds(buttonArea);
}
