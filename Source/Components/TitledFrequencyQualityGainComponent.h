/*
  ==============================================================================

    TitledFrequencyQualityGainComponent.h
    Created: 7 Jan 2024 3:40:48pm
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class TitledFrequencyQualityGainComponent : public juce::Component
{
public:
    TitledFrequencyQualityGainComponent(
        const std::string title,
        juce::AudioProcessorValueTreeState &apvts,
        const std::string frequencyParameterId,
        const std::string qualityParameterId,
        const std::string gainParameterId,
        const std::string frequencyTitle = "Frequency",
        const std::string qualityTitle = "Q",
        const std::string gainTitle = "Gain",
        const std::string hzSuffix = "hz",
        const std::string dbSuffix = "db")
    {
        mGroupComponentPtr.reset(new juce::GroupComponent(title, title));
        addAndMakeVisible(mGroupComponentPtr.get());

        mFrequencySliderPtr.reset(new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow));
        mFrequencySliderPtr->setTextValueSuffix(hzSuffix);
        mFrequencySliderPtr->setScrollWheelEnabled(false);
        mFrequencyAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            frequencyParameterId,
            *mFrequencySliderPtr);

        addAndMakeVisible(mFrequencySliderPtr.get());

        mFrequencyLabelPtr.reset(new juce::Label());
        mFrequencyLabelPtr->setText(frequencyTitle, juce::dontSendNotification);
        mFrequencyLabelPtr->attachToComponent(mFrequencySliderPtr.get(), false);
        addAndMakeVisible(mFrequencyLabelPtr.get());

        mQualitySliderPtr.reset(new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow));
        mQualitySliderPtr->setScrollWheelEnabled(false);
        mQualityAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            qualityParameterId,
            *mQualitySliderPtr);
        addAndMakeVisible(mQualitySliderPtr.get());

        mQualityLabelPtr.reset(new juce::Label());
        mQualityLabelPtr->setText(qualityTitle, juce::dontSendNotification);
        mQualityLabelPtr->attachToComponent(mQualitySliderPtr.get(), false);
        addAndMakeVisible(mQualityLabelPtr.get());

        mGainSliderPtr.reset(new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow));
        mGainSliderPtr->setScrollWheelEnabled(false);
        mGainAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            gainParameterId,
            *mGainSliderPtr);
        addAndMakeVisible(mGainSliderPtr.get());

        mGainLabelPtr.reset(new juce::Label());
        mGainLabelPtr->setText(gainTitle, juce::dontSendNotification);
        mGainLabelPtr->attachToComponent(mGainSliderPtr.get(), false);
        addAndMakeVisible(mGainLabelPtr.get());
    }

    ~TitledFrequencyQualityGainComponent()
    {
        mFrequencyAttachmentPtr.reset();
        mQualityAttachmentPtr.reset();
        mGainAttachmentPtr.reset();

        mFrequencySliderPtr.reset();
        mQualitySliderPtr.reset();
        mGainSliderPtr.reset();

        mFrequencyLabelPtr.reset();
        mQualityLabelPtr.reset();
        mGainLabelPtr.reset();

        mGroupComponentPtr.reset();
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(10);

        mGroupComponentPtr->setBounds(bounds);

        juce::FlexBox flexBox;
        flexBox.flexDirection = juce::FlexBox::Direction::row;
        flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;
        flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
        flexBox.alignItems = juce::FlexBox::AlignItems::stretch;
        flexBox.alignContent = juce::FlexBox::AlignContent::stretch;

        flexBox.items.add(juce::FlexItem(*mFrequencySliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mQualitySliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mGainSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));

        flexBox.performLayout(bounds);
    }

private:
    std::unique_ptr<juce::GroupComponent> mGroupComponentPtr;
    std::unique_ptr<juce::Label> mFrequencyLabelPtr, mQualityLabelPtr, mGainLabelPtr;
    std::unique_ptr<juce::Slider> mFrequencySliderPtr, mQualitySliderPtr, mGainSliderPtr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mFrequencyAttachmentPtr, mQualityAttachmentPtr, mGainAttachmentPtr;
};
