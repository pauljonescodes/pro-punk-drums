#include "OutputParametersComponent.h"
#include "../Configuration/Channels.h"
#include "../Configuration/Strings.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"

OutputParametersComponent::OutputParametersComponent(int channelIndex, juce::AudioProcessorValueTreeState& apvts)
	: mApvts(apvts)
{
	const auto& channelId = channels::channelIndexToIdMap.at(channelIndex);
	const auto channelMidi = channels::channelIndexToMainGeneralMidiNote.at(channelIndex);

	mNoteOnButton.reset(new juce::TextButton(std::to_string(channelMidi) + " " + channels::channelIndexToNameMap.at(channelIndex)));
	mNoteOnButton->setComponentID(juce::String(channelMidi));
	mNoteOnButton->addListener(this);
	mMidiNoteValue = channelMidi;
	addAndMakeVisible(mNoteOnButton.get());


	mCompressionOnToggleButton.reset(new juce::ToggleButton(strings::compressor));
	mCompressionOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::compressionId, parameters::onId }),
		*mCompressionOnToggleButton
		);
	addAndMakeVisible(mCompressionOnToggleButton.get());

	mLowShelfOnToggleButton.reset(new juce::ToggleButton(strings::lowShelf));
	mLowShelfOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::onId }),
		*mLowShelfOnToggleButton
		);
	addAndMakeVisible(mLowShelfOnToggleButton.get());

	mLowShelfFrequencySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mLowShelfFrequencySlider->setTitle(strings::frequency);
	mLowShelfFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::frequencyId }),
		*mLowShelfFrequencySlider);
	addAndMakeVisible(mLowShelfFrequencySlider.get());

	mLowShelfFrequencyLabel.reset(new juce::Label());
	mLowShelfFrequencyLabel->setText(strings::frequency, juce::dontSendNotification);
	mLowShelfFrequencyLabel->attachToComponent(mLowShelfFrequencySlider.get(), false);
	addAndMakeVisible(mLowShelfFrequencyLabel.get());

	mLowShelfQualitySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mLowShelfQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::qualityId }),
		*mLowShelfQualitySlider);
	addAndMakeVisible(mLowShelfQualitySlider.get());

	mLowShelfQualityLabel.reset(new juce::Label());
	mLowShelfQualityLabel->setText(strings::quality, juce::dontSendNotification);
	mLowShelfQualityLabel->attachToComponent(mLowShelfQualitySlider.get(), false);
	addAndMakeVisible(mLowShelfQualityLabel.get());

	mLowShelfGainSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mLowShelfGainSlider->setTitle(strings::frequency);
	mLowShelfQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowShelfEqualizationTypeId, parameters::gainId }),
		*mLowShelfGainSlider);
	addAndMakeVisible(mLowShelfGainSlider.get());

	mLowShelfGainLabel.reset(new juce::Label());
	mLowShelfGainLabel->setText(strings::gain, juce::dontSendNotification);
	mLowShelfGainLabel->attachToComponent(mLowShelfGainSlider.get(), false);
	addAndMakeVisible(mLowShelfGainLabel.get());

	mPeakFilterOnToggleButton.reset(new juce::ToggleButton(strings::midPeak));
	mPeakFilterOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::onId }),
		*mPeakFilterOnToggleButton
		);
	addAndMakeVisible(mPeakFilterOnToggleButton.get());

	mPeakFilterFrequencySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mPeakFilterFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::frequencyId }),
		*mPeakFilterFrequencySlider);
	addAndMakeVisible(mPeakFilterFrequencySlider.get());

	mPeakFilterFrequencyLabel.reset(new juce::Label());
	mPeakFilterFrequencyLabel->setText(strings::frequency, juce::dontSendNotification);
	mPeakFilterFrequencyLabel->attachToComponent(mPeakFilterFrequencySlider.get(), false);
	addAndMakeVisible(mPeakFilterFrequencyLabel.get());

	mPeakFilterQualitySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mPeakFilterQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::qualityId }),
		*mPeakFilterQualitySlider);
	addAndMakeVisible(mPeakFilterQualitySlider.get());

	mPeakFilterQualityLabel.reset(new juce::Label());
	mPeakFilterQualityLabel->setText(strings::quality, juce::dontSendNotification);
	mPeakFilterQualityLabel->attachToComponent(mPeakFilterQualitySlider.get(), false);
	addAndMakeVisible(mPeakFilterQualityLabel.get());

	mPeakFilterGainSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mPeakFilterGainSlider->setTitle(strings::frequency);
	mPeakFilterQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::gainId }),
		*mPeakFilterGainSlider);
	addAndMakeVisible(mPeakFilterGainSlider.get());

	mPeakFilterGainLabel.reset(new juce::Label());
	mPeakFilterGainLabel->setText(strings::gain, juce::dontSendNotification);
	mPeakFilterGainLabel->attachToComponent(mPeakFilterGainSlider.get(), false);
	addAndMakeVisible(mPeakFilterGainLabel.get());

	mHighShelfOnToggleButton.reset(new juce::ToggleButton(strings::highShelf));
	mHighShelfOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::onId }),
		*mHighShelfOnToggleButton
		);
	addAndMakeVisible(mHighShelfOnToggleButton.get());

	mHighShelfFrequencySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mHighShelfFrequencySlider->setTitle(strings::frequency);
	mHighShelfFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::frequencyId }),
		*mHighShelfFrequencySlider);
	addAndMakeVisible(mHighShelfFrequencySlider.get());

	mHighShelfFrequencyLabel.reset(new juce::Label());
	mHighShelfFrequencyLabel->setText(strings::frequency, juce::dontSendNotification);
	mHighShelfFrequencyLabel->attachToComponent(mHighShelfFrequencySlider.get(), false);
	addAndMakeVisible(mHighShelfFrequencyLabel.get());

	mHighShelfQualitySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mHighShelfQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::qualityId }),
		*mHighShelfQualitySlider);
	addAndMakeVisible(mHighShelfQualitySlider.get());

	mHighShelfQualityLabel.reset(new juce::Label());
	mHighShelfQualityLabel->setText(strings::quality, juce::dontSendNotification);
	mHighShelfQualityLabel->attachToComponent(mHighShelfQualitySlider.get(), false);
	addAndMakeVisible(mHighShelfQualityLabel.get());

	mHighShelfGainSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mHighShelfGainSlider->setTitle(strings::frequency);
	mHighShelfQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highShelfEqualizationTypeId, parameters::gainId }),
		*mHighShelfGainSlider);
	addAndMakeVisible(mHighShelfGainSlider.get());

	mHighShelfGainLabel.reset(new juce::Label());
	mHighShelfGainLabel->setText(strings::gain, juce::dontSendNotification);
	mHighShelfGainLabel->attachToComponent(mHighShelfGainSlider.get(), false);
	addAndMakeVisible(mHighShelfGainLabel.get());

	mThresholdSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mThresholdSlider->setTitle(strings::frequency);
	mThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::thresholdId }),
		*mThresholdSlider);
	addAndMakeVisible(mThresholdSlider.get());

	mThresholdLabel.reset(new juce::Label());
	mThresholdLabel->setText(strings::threshold, juce::dontSendNotification);
	mThresholdLabel->attachToComponent(mThresholdSlider.get(), false);
	addAndMakeVisible(mThresholdLabel.get());

	mRatioSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mRatioSlider->setTitle(strings::frequency);
	mRatioAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::ratioId }),
		*mRatioSlider);
	addAndMakeVisible(mRatioSlider.get());

	mRatioLabel.reset(new juce::Label());
	mRatioLabel->setText(strings::ratio, juce::dontSendNotification);
	mRatioLabel->attachToComponent(mRatioSlider.get(), false);
	addAndMakeVisible(mRatioLabel.get());

	mAttackSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mAttackSlider->setTitle(strings::frequency);
	mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::attackId }),
		*mAttackSlider);
	addAndMakeVisible(mAttackSlider.get());

	mAttackLabel.reset(new juce::Label());
	mAttackLabel->setText(strings::attack, juce::dontSendNotification);
	mAttackLabel->attachToComponent(mAttackSlider.get(), false);
	addAndMakeVisible(mAttackLabel.get());

	mReleaseSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mReleaseSlider->setTitle(strings::frequency);
	mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::releaseId }),
		*mReleaseSlider);
	addAndMakeVisible(mReleaseSlider.get());

	mReleaseLabel.reset(new juce::Label());
	mReleaseLabel->setText(strings::release, juce::dontSendNotification);
	mReleaseLabel->attachToComponent(mReleaseSlider.get(), false);
	addAndMakeVisible(mReleaseLabel.get());

	mChannelGainSlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mChannelGainSlider->setTitle(strings::gain);
	mChannelGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::gainId }),
		*mChannelGainSlider);
	addAndMakeVisible(mChannelGainSlider.get());

	mChannelGainLabel.reset(new juce::Label());
	mChannelGainLabel->setText(strings::gain, juce::dontSendNotification);
	mChannelGainLabel->attachToComponent(mChannelGainSlider.get(), false);
	addAndMakeVisible(mChannelGainLabel.get());
}

OutputParametersComponent::~OutputParametersComponent()
{
}

void OutputParametersComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void OutputParametersComponent::resized()
{
	auto bounds = getLocalBounds();
	auto row1Height = 25.0f;
	auto spacerHeight = 25.0f;
	auto rowHeight = ((bounds.getHeight() - (spacerHeight * 4)) / 4) - row1Height; // Set the height for each row
	auto rowWidth = (bounds.getWidth() / 5); // Set the height for each row

	// Create flex boxes for each row
	juce::FlexBox row1, row2, row3, row4, row5;

	// Configure the flex boxes for horizontal layout
	row1.flexDirection = juce::FlexBox::Direction::row;
	row2.flexDirection = juce::FlexBox::Direction::row;
	row3.flexDirection = juce::FlexBox::Direction::row;
	row4.flexDirection = juce::FlexBox::Direction::row;
	row5.flexDirection = juce::FlexBox::Direction::row;

	// Add components to each row as FlexItem
	row1.items.add(juce::FlexItem(*mNoteOnButton).withWidth(getWidth()).withHeight(row1Height));

	row2.items.add(juce::FlexItem(*mLowShelfOnToggleButton).withWidth(rowWidth).withHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mLowShelfFrequencySlider).withWidth(rowWidth).withHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mLowShelfQualitySlider).withWidth(rowWidth).withHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mLowShelfGainSlider).withWidth(rowWidth).withHeight(rowHeight));

	row3.items.add(juce::FlexItem(*mPeakFilterOnToggleButton).withWidth(rowWidth).withHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mPeakFilterFrequencySlider).withWidth(rowWidth).withHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mPeakFilterQualitySlider).withWidth(rowWidth).withHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mPeakFilterGainSlider).withWidth(rowWidth).withHeight(rowHeight));

	row4.items.add(juce::FlexItem(*mHighShelfOnToggleButton).withWidth(rowWidth).withHeight(rowHeight));
	row4.items.add(juce::FlexItem(*mHighShelfFrequencySlider).withWidth(rowWidth).withHeight(rowHeight));
	row4.items.add(juce::FlexItem(*mHighShelfQualitySlider).withWidth(rowWidth).withHeight(rowHeight));
	row4.items.add(juce::FlexItem(*mHighShelfGainSlider).withWidth(rowWidth).withHeight(rowHeight));

	row5.items.add(juce::FlexItem(*mCompressionOnToggleButton).withWidth(rowWidth).withHeight(rowHeight));
	row5.items.add(juce::FlexItem(*mThresholdSlider).withWidth(rowWidth).withHeight(rowHeight));
	row5.items.add(juce::FlexItem(*mAttackSlider).withWidth(rowWidth).withHeight(rowHeight));
	row5.items.add(juce::FlexItem(*mRatioSlider).withWidth(rowWidth).withHeight(rowHeight));
	row5.items.add(juce::FlexItem(*mReleaseSlider).withWidth(rowWidth).withHeight(rowHeight));
	row5.items.add(juce::FlexItem(*mChannelGainSlider).withWidth(rowWidth).withHeight(rowHeight));
	
	// Perform layout for each row
	bounds.removeFromTop(12.5);
	row1.performLayout(bounds.removeFromTop(row1Height));
	bounds.removeFromTop(spacerHeight);
	row2.performLayout(bounds.removeFromTop(rowHeight));
	bounds.removeFromTop(spacerHeight);
	row3.performLayout(bounds.removeFromTop(rowHeight));
	bounds.removeFromTop(spacerHeight);
	row4.performLayout(bounds.removeFromTop(rowHeight));
	bounds.removeFromTop(spacerHeight);
	row5.performLayout(bounds.removeFromTop(rowHeight));
}

void OutputParametersComponent::buttonClicked(juce::Button* button)
{
	mIsPlaying = !mIsPlaying;

	if (mIsPlaying && mOnDrumMidiButtonClicked.has_value()) {
		mOnDrumMidiButtonClicked.value()(mMidiNoteValue, 0.75f);
	}

	if (mIsPlaying) {
		startTimer(1000); // start timer with interval of 2000ms (2s)
	}
	else {
		stopTimer(); // stop the timer
	}
}

void OutputParametersComponent::timerCallback()
{
	if (mIsPlaying && mOnDrumMidiButtonClicked.has_value()) {
		mOnDrumMidiButtonClicked.value()(mMidiNoteValue, 0.75f);
	}
}
