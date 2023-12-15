/*
  ==============================================================================

    SamplerComponent.cpp
    Created: 8 Dec 2023 10:49:23am
    Author:  paulm

  ==============================================================================
*/

#include "SamplerComponent.h"
#include "../Configuration/Samples.h"
#include "../Configuration/GeneralMidi.h"

//==============================================================================
SamplerComponent::SamplerComponent()
{
    mVelocitySlider = std::make_unique<juce::Slider>();
    mVelocitySlider->setRange(0, 127, 1);
    mVelocitySlider->setValue(64);

    addAndMakeVisible(mVelocitySlider.get());
    mVelocitySlider->addListener(this);

    mVelocityLabel = std::make_unique<juce::Label>();
    mVelocityLabel->setText("Velocity", juce::dontSendNotification);
    mVelocityLabel->attachToComponent(mVelocitySlider.get(), true);
    addAndMakeVisible(mVelocityLabel.get());

    const std::vector<int> generalMidiNotesVector = {
    generalmidi::midinotes::acousticBassDrum,
    generalmidi::midinotes::sideStick,
    generalmidi::midinotes::acousticSnare,
    generalmidi::midinotes::handClap,
    generalmidi::midinotes::closedHiHat,
    generalmidi::midinotes::highFloorTom,
    generalmidi::midinotes::pedalHiHat,
    generalmidi::midinotes::openHiHat,
    generalmidi::midinotes::crashCymbal1,
    generalmidi::midinotes::highTom,
    generalmidi::midinotes::rideCymbal1,
    generalmidi::midinotes::chineseCymbal,
    generalmidi::midinotes::rideBell,
    generalmidi::midinotes::tambourine,
    generalmidi::midinotes::splashCymbal,
    generalmidi::midinotes::cowbell,
    generalmidi::midinotes::crashCymbal2,
    generalmidi::midinotes::maraca,
    generalmidi::midinotes::muteTriangle,
    generalmidi::midinotes::openTriangle
    };

    for (int note : generalMidiNotesVector)
    {
        juce::TextButton* button = new juce::TextButton(juce::String(note) + " " + generalmidi::midiNoteToNameMap.at(note));
        button->setComponentID(juce::String(note));
        mMidiNoteButtons.add(button);
        addAndMakeVisible(button);
        button->addListener(this);
    }

    resized();
}

SamplerComponent::~SamplerComponent()
{
    for (auto button : mMidiNoteButtons) {
        button = nullptr;
    }
}

//==============================================================================
void SamplerComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SamplerComponent::resized()
{
    auto area = getLocalBounds();

    // Allocate space for the slider at the top
    auto topArea = area.removeFromTop(50); // Adjust the height as needed
    auto velocitySliderArea = topArea.removeFromRight(topArea.getWidth() - 64);

    mVelocitySlider->setBounds(velocitySliderArea);

    // Layout for buttons
    int numRows = std::max(1, (area.getHeight()) / 125);
    int numCols = std::max(1, area.getWidth() / 125);
    int buttonWidth = area.getWidth() / numCols;
    int buttonHeight = area.getHeight() / numRows;

    for (int i = 0; i < mMidiNoteButtons.size(); ++i)
    {
        int row = i / numCols;
        int col = i % numCols;
        mMidiNoteButtons[i]->setBounds(col * buttonWidth, row * buttonHeight + topArea.getHeight(), buttonWidth, buttonHeight);
    }
}

void SamplerComponent::buttonClicked(juce::Button* button)
{
    const juce::String componentID = button->getComponentID();

    int midiNoteValue = componentID.getIntValue();
    float velocity = mVelocitySlider->getValue() / 127.0f;

    if (mOnDrumMidiButtonClicked.has_value()) {
        mOnDrumMidiButtonClicked.value()(midiNoteValue, velocity);
    }
}

void SamplerComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == mVelocitySlider.get())
    {
        // Handle the slider value change
        // You might want to store the current slider value in a member variable to use it in buttonClicked
    }
}