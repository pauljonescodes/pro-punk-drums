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

SamplesParameterComponent::SamplesParameterComponent(int midiNote, juce::AudioProcessorValueTreeState& apvts)
{
    auto gainParameterId = PluginUtils::getParamId(midiNote, "", constants::gainId);
    auto* gainParameter = apvts.getParameter(gainParameterId);
    auto* panParameter = apvts.getParameter(PluginUtils::getParamId(midiNote, "", constants::panId));
    auto* phaseParameter = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(PluginUtils::getParamId(midiNote, "", constants::phaseId)));

    // Initialize gain slider and label
    mGainSlider.reset(new juce::Slider("Gain Slider"));
    addAndMakeVisible(mGainSlider.get());
    auto& normalizableRange = gainParameter->getNormalisableRange();
    mGainSlider->setRange(normalizableRange.start, normalizableRange.end, normalizableRange.interval);
    mGainSlider->setValue(gainParameter->getValue()); \
        const std::string test = "";

    mGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        juce::String(gainParameterId),
        *mGainSlider
        );

    mGainLabel.reset(new juce::Label(gainParameter->getParameterID() + "_label", gainParameter->getName(99)));
    addAndMakeVisible(mGainLabel.get());
    mGainLabel->attachToComponent(mGainSlider.get(), true);

    // Initialize pan slider and label
    mPanSlider.reset(new juce::Slider("Pan Slider"));
    addAndMakeVisible(mPanSlider.get());
    mPanSlider->setRange(panParameter->getNormalisableRange().start, panParameter->getNormalisableRange().end);
    mPanSlider->setValue(panParameter->getValue());

    mPanLabel.reset(new juce::Label("Pan Label", "Pan"));
    addAndMakeVisible(mPanLabel.get());
    mPanLabel->attachToComponent(mPanSlider.get(), true);

    // Initialize phase toggle button and label
    mInvertPhaseToggleButton.reset(new juce::ToggleButton("Invert Phase"));
    addAndMakeVisible(mInvertPhaseToggleButton.get());
    mInvertPhaseToggleButton->setToggleState(phaseParameter->get(), juce::dontSendNotification);

    mInvertPhaseLabel.reset(new juce::Label("Phase Label", "Invert Phase"));
    addAndMakeVisible(mInvertPhaseLabel.get());
    mInvertPhaseLabel->attachToComponent(mInvertPhaseToggleButton.get(), true);
}

SamplesParameterComponent::~SamplesParameterComponent()
{
    mGainSlider = nullptr;
    mPanSlider = nullptr;
    mInvertPhaseToggleButton = nullptr;
}

void SamplesParameterComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

}

void SamplesParameterComponent::resized()
{
    auto area = getLocalBounds(); // Get the local bounds of the component

    // Define some layout parameters
    const int sliderHeight = 40;
    const int labelHeight = 20;
    const int margin = 64;
    int yPosition = margin;


    mGainSlider->setBounds(margin, yPosition, 300, sliderHeight);
    yPosition += sliderHeight + margin;

    mPanSlider->setBounds(margin, yPosition, 300, sliderHeight);
    yPosition += sliderHeight + margin;

    mInvertPhaseToggleButton->setBounds(margin, yPosition, area.getWidth() - 2 * margin, sliderHeight);
}
