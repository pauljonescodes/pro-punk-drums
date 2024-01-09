/*
  ==============================================================================

    ReverbComponent.h
    Created: 8 Jan 2024 8:08:47pm
    Author:  paulm

  ==============================================================================
*/

#pragma once
#include <string>
#include "JuceHeader.h"

class ReverbComponent : public juce::Component {
public:
    ReverbComponent(
        juce::AudioProcessorValueTreeState& apvts,
        const std::string roomSizeParameterId,
        const std::string dampingParameterId,
        const std::string widthParameterId,
        const std::string roomSizeTitle = "Room size",
        const std::string dampingTitle = "Damping",
        const std::string widthTitle = "Width")
    {
        mRoomSizeSliderPtr.reset(new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow));
        mRoomSizeSliderPtr->setTitle(roomSizeTitle);
        mRoomSizeSliderPtr->setScrollWheelEnabled(false);
        mRoomSizeAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            roomSizeParameterId,
            *mRoomSizeSliderPtr);

        addAndMakeVisible(mRoomSizeSliderPtr.get());

        mRoomSizeLabelPtr.reset(new juce::Label());
        mRoomSizeLabelPtr->setText(roomSizeTitle, juce::dontSendNotification);
        mRoomSizeLabelPtr->attachToComponent(mRoomSizeSliderPtr.get(), false);
        addAndMakeVisible(mRoomSizeLabelPtr.get());

        mDampingSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow));
        mDampingSliderPtr->setScrollWheelEnabled(false);
        mDampingAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            dampingParameterId,
            *mDampingSliderPtr);
        addAndMakeVisible(mDampingSliderPtr.get());

        mDampingLabelPtr.reset(new juce::Label());
        mDampingLabelPtr->setText(dampingTitle, juce::dontSendNotification);
        mDampingLabelPtr->attachToComponent(mDampingSliderPtr.get(), false);
        addAndMakeVisible(mDampingLabelPtr.get());

        mWidthSliderPtr.reset(new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow));
        mWidthSliderPtr->setScrollWheelEnabled(false);
        mWidthSliderPtr->setTitle(roomSizeTitle);
        mWidthAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            apvts,
            widthParameterId,
            *mWidthSliderPtr);
        addAndMakeVisible(mWidthSliderPtr.get());

        mWidthLabelPtr.reset(new juce::Label());
        mWidthLabelPtr->setText(widthTitle, juce::dontSendNotification);
        mWidthLabelPtr->attachToComponent(mWidthSliderPtr.get(), false);
        addAndMakeVisible(mWidthLabelPtr.get());
    }

    ~ReverbComponent()
    {
        mRoomSizeAttachmentPtr.reset();
        mDampingAttachmentPtr.reset();
        mWidthAttachmentPtr.reset();

        mRoomSizeSliderPtr.reset();
        mDampingSliderPtr.reset();
        mWidthSliderPtr.reset();

        mRoomSizeLabelPtr.reset();
        mDampingLabelPtr.reset();
        mWidthLabelPtr.reset();
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced(10);

        juce::FlexBox flexBox;
        flexBox.flexDirection = juce::FlexBox::Direction::row;
        flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;
        flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
        flexBox.alignItems = juce::FlexBox::AlignItems::stretch;
        flexBox.alignContent = juce::FlexBox::AlignContent::stretch;

        flexBox.items.add(juce::FlexItem(*mRoomSizeSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mDampingSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));
        flexBox.items.add(juce::FlexItem(*mWidthSliderPtr).withFlex(1).withMargin(juce::FlexItem::Margin(36, 0, 12, 12)));

        flexBox.performLayout(bounds);
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

private:
    std::unique_ptr<juce::GroupComponent> mGroupComponentPtr;
    std::unique_ptr<juce::Label> mRoomSizeLabelPtr, mDampingLabelPtr, mWidthLabelPtr;
    std::unique_ptr<juce::Slider> mRoomSizeSliderPtr, mDampingSliderPtr, mWidthSliderPtr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mRoomSizeAttachmentPtr, mDampingAttachmentPtr, mWidthAttachmentPtr;
};
