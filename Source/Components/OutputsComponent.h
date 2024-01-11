#pragma once
#include <JuceHeader.h>
#include "OutputParametersComponent.h"

class OutputsComponent : public juce::Component, private juce::ComboBox::Listener, public juce::Button::Listener, private juce::Timer
{
public:
    OutputsComponent(juce::AudioProcessorValueTreeState &apvts, std::function<void(int, float)> onDrumMidiButtonClicked) : mOnDrumMidiButtonClicked(onDrumMidiButtonClicked),
                                                                                                                           mApvts(apvts)
    {
        mOutputParametersComponent.reset(new OutputParametersComponent(apvts));
        addAndMakeVisible(mOutputParametersComponent.get());

        mChannelSelector.reset(new juce::ComboBox());
        addAndMakeVisible(mChannelSelector.get());

        for (const auto &channel : Channels::channelIndexToIdMap)
        {
            mChannelSelector->addItem(stringToTitleCase(channel.second), channel.first + 1); // 0 is no selection
        }

        mChannelSelector->addListener(this);
        mChannelSelector->setSelectedItemIndex(0);

        mChannelSelectorLabelPtr.reset(new juce::Label());
        mChannelSelectorLabelPtr->setText(Strings::selectChannel, juce::dontSendNotification);
        addAndMakeVisible(*mChannelSelectorLabelPtr);
    };

    void resized() override
    {
        auto bounds = getLocalBounds();

        auto topArea = bounds.removeFromTop(144);

        topArea.removeFromRight(12);

        if (mChannelOutputGainSliderPtr != nullptr)
        {
            mChannelOutputGainSliderPtr->setBounds(topArea.removeFromRight(96).reduced(0, 24));
        }

        topArea.removeFromRight(12);

        if (mChannelReverbGainSliderPtr != nullptr)
        {
            mChannelReverbGainSliderPtr->setBounds(topArea.removeFromRight(96).reduced(0, 24));
        }

        mChannelSelectorLabelPtr->setBounds(topArea.removeFromTop(72).reduced(12, 0));

        mChannelSelector->setBounds(topArea.removeFromTop(48).reduced(12, 0));
        mOutputParametersComponent->setBounds(bounds);
    };

    void paint(juce::Graphics &g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    };

    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override
    {
        if (comboBoxThatHasChanged == mChannelSelector.get())
        {
            setChannelIndex(mChannelSelector->getSelectedItemIndex());
        }
    };

    void setChannelIndex(int channelIndex)
    {
        mChannelOutputGainAttachmentPtr.reset();
        mChannelReverbGainAttachmentPtr.reset();

        const auto &channelId = Channels::channelIndexToIdMap.at(channelIndex);
        const auto &channelName = stringToTitleCase(channelId);
        const auto channelMidi = Channels::channelIndexToGeneralMidiPerccussionNote.at(channelIndex);

        mOutputParametersComponent->setChannelIndex(channelIndex);

        mNoteOnButtonPtr.reset(new juce::TextButton(std::to_string(channelMidi) + " " + stringToTitleCase(Channels::channelIndexToIdMap.at(channelIndex))));
        mNoteOnButtonPtr->setComponentID(juce::String(channelMidi));
        mNoteOnButtonPtr->addListener(this);
        mMidiNoteValue = channelMidi;
        addAndMakeVisible(mNoteOnButtonPtr.get());

        const auto channelOutputGainParameterId = stringsJoinAndSnakeCase({channelId, AudioParameters::gainComponentId});

        mChannelOutputGainSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow));
        mChannelOutputGainSliderPtr->setScrollWheelEnabled(false);
        mChannelOutputGainAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
            mApvts,
            channelOutputGainParameterId,
            *mChannelOutputGainSliderPtr);
        addAndMakeVisible(mChannelOutputGainSliderPtr.get());

        mChannelOutputGainSliderLabelPtr.reset(new juce::Label());
        mChannelOutputGainSliderLabelPtr->setText(Strings::gain, juce::dontSendNotification);
        mChannelOutputGainSliderLabelPtr->attachToComponent(mChannelOutputGainSliderPtr.get(), false);
        addAndMakeVisible(mChannelOutputGainSliderLabelPtr.get());

        mChannelReverbGainSliderPtr.reset();
        mChannelReverbGainSliderLabelPtr.reset();

        if (channelIndex != Channels::roomChannelIndex)
        {
            const auto channelReverbGainParameterId = stringsJoinAndSnakeCase({ channelId, AudioParameters::reverbComponentId, AudioParameters::gainComponentId });
            mChannelReverbGainSliderPtr.reset(new juce::Slider(juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow));
            mChannelReverbGainSliderPtr->setScrollWheelEnabled(false);
            mChannelReverbGainAttachmentPtr = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                mApvts,
                channelReverbGainParameterId,
                *mChannelReverbGainSliderPtr);
            addAndMakeVisible(mChannelReverbGainSliderPtr.get());

            mChannelReverbGainSliderLabelPtr.reset(new juce::Label());
            mChannelReverbGainSliderLabelPtr->setText(Strings::roomSend, juce::dontSendNotification);
            mChannelReverbGainSliderLabelPtr->attachToComponent(mChannelReverbGainSliderPtr.get(), false);
            addAndMakeVisible(mChannelReverbGainSliderLabelPtr.get());
        }

        resized();
    }

private:
    juce::AudioProcessorValueTreeState &mApvts;

    std::unique_ptr<OutputParametersComponent> mOutputParametersComponent;

    std::unique_ptr<juce::ComboBox> mChannelSelector;
    std::unique_ptr<juce::Label> mChannelSelectorLabelPtr;

    std::unique_ptr<juce::Slider> mChannelOutputGainSliderPtr;
    std::unique_ptr<juce::Label> mChannelOutputGainSliderLabelPtr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mChannelOutputGainAttachmentPtr;

    std::unique_ptr<juce::Slider> mChannelReverbGainSliderPtr;
    std::unique_ptr<juce::Label> mChannelReverbGainSliderLabelPtr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mChannelReverbGainAttachmentPtr;

    std::unique_ptr<juce::TextButton> mNoteOnButtonPtr;

    bool mIsPlaying = false;
    int mMidiNoteValue = -1;

    std::optional<std::function<void(int, float)>> mOnDrumMidiButtonClicked;

    void buttonClicked(juce::Button *button) override
    {
        mIsPlaying = !mIsPlaying;

        if (mIsPlaying && mOnDrumMidiButtonClicked.has_value())
        {
            mOnDrumMidiButtonClicked.value()(mMidiNoteValue, 0.75f);
        }

        if (mIsPlaying)
        {
            startTimer(1000);
        }
        else
        {
            stopTimer();
        }
    };

    void timerCallback() override
    {
        if (mIsPlaying && mOnDrumMidiButtonClicked.has_value())
        {
            mOnDrumMidiButtonClicked.value()(mMidiNoteValue, 0.75f);
        }
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputsComponent)
};
