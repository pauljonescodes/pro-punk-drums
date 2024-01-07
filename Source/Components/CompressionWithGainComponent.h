/*
  ==============================================================================

    CompressionWithGainComponent.h
    Created: 7 Jan 2024 5:05:33pm
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CompressionWithGainComponent : public juce::Component {
public:
    CompressionWithGainComponent(
        const std::string title,
        juce::AudioProcessorValueTreeState& apvts,
        const std::string thresholdParameterId,
        const std::string attackParameterId,
        const std::string ratioParameterId,
        const std::string releaseParameterId,
        const std::string gainParameterId,
        const std::string thresholdTitle = "Threshold",
        const std::string attackTitle = "Attack",
        const std::string ratioTitle = "Ratio",
        const std::string releaseTitle = "Release",
        const std::string gainTitle = "Gain",
        const std::string dbSuffix = "dB",
        const std::string msSuffix = "ms")
    {
        mGroupComponentPtr.reset(new juce::GroupComponent(title, title));
        addAndMakeVisible(mGroupComponentPtr.get());

        mThresholdSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalDrag, juce::Slider::TextBoxBelow));
        mThresholdSliderPtr->setTitle(thresholdTitle);
        mThresholdSliderPtr->setTextValueSuffix(dbSuffix);
        mThresholdSliderPtr->setScrollWheelEnabled(false);
        mThresholdAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            thresholdParameterId,
            *mThresholdSliderPtr);

        addAndMakeVisible(mThresholdSliderPtr.get());

        mThresholdLabelPtr.reset(new juce::Label());
        mThresholdLabelPtr->setText(thresholdTitle, juce::dontSendNotification);
        mThresholdLabelPtr->attachToComponent(mThresholdSliderPtr.get(), false);
        addAndMakeVisible(mThresholdLabelPtr.get());

        mAttackSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow));
        mAttackSliderPtr->setScrollWheelEnabled(false);
        mAttackSliderPtr->setTextValueSuffix(msSuffix);
        mAttackAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            attackParameterId,
            *mAttackSliderPtr);
        addAndMakeVisible(mAttackSliderPtr.get());

        mAttackLabelPtr.reset(new juce::Label());
        mAttackLabelPtr->setText(attackTitle, juce::dontSendNotification);
        mAttackLabelPtr->attachToComponent(mAttackSliderPtr.get(), false);
        addAndMakeVisible(mAttackLabelPtr.get());

        mRatioSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalDrag, juce::Slider::TextBoxBelow));
        mRatioSliderPtr->setScrollWheelEnabled(false);
        mRatioSliderPtr->setTitle(ratioTitle);
        mAttackAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            ratioParameterId,
            *mRatioSliderPtr);
        addAndMakeVisible(mRatioSliderPtr.get());

        mRatioLabelPtr.reset(new juce::Label());
        mRatioLabelPtr->setText(ratioTitle, juce::dontSendNotification);
        mRatioLabelPtr->attachToComponent(mRatioSliderPtr.get(), false);
        addAndMakeVisible(mRatioLabelPtr.get());

        mReleaseSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalDrag, juce::Slider::TextBoxBelow));
        mReleaseSliderPtr->setScrollWheelEnabled(false);
        mReleaseSliderPtr->setTitle(releaseTitle);
        mReleaseAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            releaseParameterId,
            *mReleaseSliderPtr);
        addAndMakeVisible(mReleaseSliderPtr.get());

        mReleaseLabelPtr.reset(new juce::Label());
        mReleaseLabelPtr->setText(releaseTitle, juce::dontSendNotification);
        mReleaseLabelPtr->attachToComponent(mReleaseSliderPtr.get(), false);
        addAndMakeVisible(mReleaseLabelPtr.get());

        mGainSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalDrag, juce::Slider::TextBoxBelow));
        mGainSliderPtr->setScrollWheelEnabled(false);
        mGainSliderPtr->setTitle(gainTitle);
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

    ~CompressionWithGainComponent()
    {
        mThresholdAttachmentPtr.reset();
        mAttackAttachmentPtr.reset();
        mRatioAttachmentPtr.reset();
        mReleaseAttachmentPtr.reset();
        mGainAttachmentPtr.reset();

        mThresholdSliderPtr.reset();
        mAttackSliderPtr.reset();
        mRatioSliderPtr.reset();
        mReleaseSliderPtr.reset();
        mGainSliderPtr.reset();

        mThresholdLabelPtr.reset();
        mAttackLabelPtr.reset();
        mRatioLabelPtr.reset();
        mReleaseLabelPtr.reset();
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

        flexBox.items.add(juce::FlexItem(*mThresholdSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mAttackSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mRatioSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mReleaseSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mGainSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));

        flexBox.performLayout(bounds);
    }

private:
    std::unique_ptr<juce::GroupComponent> 
        mGroupComponentPtr;
    
    std::unique_ptr<juce::Label> 
        mThresholdLabelPtr, 
        mAttackLabelPtr, 
        mRatioLabelPtr, 
        mReleaseLabelPtr, 
        mGainLabelPtr;
   
    std::unique_ptr<juce::Slider> 
        mThresholdSliderPtr, 
        mAttackSliderPtr, 
        mRatioSliderPtr, 
        mReleaseSliderPtr, 
        mGainSliderPtr;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> 
        mThresholdAttachmentPtr, 
        mAttackAttachmentPtr, 
        mRatioAttachmentPtr, 
        mReleaseAttachmentPtr,
        mGainAttachmentPtr;
};