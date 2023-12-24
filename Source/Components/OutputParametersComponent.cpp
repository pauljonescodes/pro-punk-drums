#include "OutputParametersComponent.h"
#include "../Configuration/Channels.h"
#include "../Configuration/Strings.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"

OutputParametersComponent::OutputParametersComponent(int channelIndex, juce::AudioProcessorValueTreeState& apvts)
	: mApvts(apvts)
{
	const auto& channelName = channels::channelIndexToNameMap.at(channelIndex);
	const auto& channelId = channels::channelIndexToIdMap.at(channelIndex);
	const auto channelMidi = channels::channelIndexToMainGeneralMidiNote.at(channelIndex);

	auto textBoxWidth = 75;
	auto textBoxHeight = 15;

	mNoteOnButton.reset(new juce::TextButton(std::to_string(channelMidi)));
	mNoteOnButton->setComponentID(juce::String(channelMidi));
	mNoteOnButton->addListener(this);
	mMidiNoteValue = channelMidi;
	addAndMakeVisible(mNoteOnButton.get());

	mLabel.reset(new juce::Label(std::to_string(channelIndex) + "_label", channels::channelIndexToNameMap.at(channelIndex)));
	addAndMakeVisible(mLabel.get());

	mCompressionOnToggleButton.reset(new juce::ToggleButton(strings::compressor));
	mCompressionOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::compressionId, parameters::onId }),
		*mCompressionOnToggleButton
		);
	addAndMakeVisible(mCompressionOnToggleButton.get());

	mHighPassOnToggleButton.reset(new juce::ToggleButton(strings::highPass));
	mHighPassOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highPassEqualizationTypeId, parameters::onId }),
		*mHighPassOnToggleButton
		);
	addAndMakeVisible(mHighPassOnToggleButton.get());

	mHighPassFrequencySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mHighPassFrequencySlider->setTitle(strings::frequency);
	mHighPassFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highPassEqualizationTypeId, parameters::frequencyId }),
		*mHighPassFrequencySlider);
	addAndMakeVisible(mHighPassFrequencySlider.get());

	mHighPassFrequencyLabel.reset(new juce::Label());
	mHighPassFrequencyLabel->setText(strings::frequency, juce::dontSendNotification);
	mHighPassFrequencyLabel->attachToComponent(mHighPassFrequencySlider.get(), false);
	addAndMakeVisible(mHighPassFrequencyLabel.get());

	mHighPassQualitySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mHighPassQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highPassEqualizationTypeId, parameters::qualityId }),
		*mHighPassQualitySlider);
	addAndMakeVisible(mHighPassQualitySlider.get());

	mHighPassQualityLabel.reset(new juce::Label());
	mHighPassQualityLabel->setText(strings::quality, juce::dontSendNotification);
	mHighPassQualityLabel->attachToComponent(mHighPassQualitySlider.get(), false);
	addAndMakeVisible(mHighPassQualityLabel.get());

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

	mLowPassOnToggleButton.reset(new juce::ToggleButton(strings::lowPass));
	mLowPassOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowPassEqualizationTypeId, parameters::onId }),
		*mLowPassOnToggleButton
		);
	addAndMakeVisible(mLowPassOnToggleButton.get());

	mLowPassFrequencySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mLowPassFrequencySlider->setTitle(strings::frequency);
	mLowPassFrequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowPassEqualizationTypeId, parameters::frequencyId }),
		*mLowPassFrequencySlider);
	addAndMakeVisible(mLowPassFrequencySlider.get());

	mLowPassFrequencyLabel.reset(new juce::Label());
	mLowPassFrequencyLabel->setText(strings::frequency, juce::dontSendNotification);
	mLowPassFrequencyLabel->attachToComponent(mLowPassFrequencySlider.get(), false);
	addAndMakeVisible(mLowPassFrequencyLabel.get());

	mLowPassQualitySlider.reset(new juce::Slider(juce::Slider::Rotary, juce::Slider::TextBoxBelow));
	mLowPassQualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowPassEqualizationTypeId, parameters::qualityId }),
		*mLowPassQualitySlider);
	addAndMakeVisible(mLowPassQualitySlider.get());

	mLowPassQualityLabel.reset(new juce::Label());
	mLowPassQualityLabel->setText(strings::quality, juce::dontSendNotification);
	mLowPassQualityLabel->attachToComponent(mLowPassQualitySlider.get(), false);
	addAndMakeVisible(mLowPassQualityLabel.get());

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
	auto row0Height = 25.0f;
	auto row1Height = 25.0f;
	auto spacerHeight = 25.0f;
	auto rowHeight = ((bounds.getHeight() - row0Height - row1Height - spacerHeight * 3) / 3); // Set the height for each row
	auto rowWidth = (bounds.getWidth() / 5); // Set the height for each row

	// Create flex boxes for each row
	juce::FlexBox row0, row1, row2, row3, row4;

	// Configure the flex boxes for horizontal layout
	row0.flexDirection = juce::FlexBox::Direction::row;
	row1.flexDirection = juce::FlexBox::Direction::row;
	row2.flexDirection = juce::FlexBox::Direction::row;
	row3.flexDirection = juce::FlexBox::Direction::row;
	row4.flexDirection = juce::FlexBox::Direction::row;

	// Add components to each row as FlexItem
	row0.items.add(juce::FlexItem(*mLabel).withWidth(bounds.getWidth() / 2.0f).withHeight(row0Height));
	row0.items.add(juce::FlexItem(*mNoteOnButton).withWidth(bounds.getWidth() / 2.0f).withHeight(row0Height));

	row1.items.add(juce::FlexItem(*mHighPassOnToggleButton).withWidth(rowWidth).withHeight(row1Height));
	row1.items.add(juce::FlexItem(*mPeakFilterOnToggleButton).withWidth(rowWidth).withHeight(row1Height));
	row1.items.add(juce::FlexItem(*mLowPassOnToggleButton).withWidth(rowWidth).withHeight(row1Height));
	row1.items.add(juce::FlexItem(*mCompressionOnToggleButton).withWidth(rowWidth).withHeight(row1Height));

	row2.items.add(juce::FlexItem(*mHighPassFrequencySlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mPeakFilterFrequencySlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mLowPassFrequencySlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mThresholdSlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row2.items.add(juce::FlexItem(*mAttackSlider).withMinWidth(rowWidth).withMinHeight(rowHeight));

	row3.items.add(juce::FlexItem(*mHighPassQualitySlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mPeakFilterQualitySlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mLowPassQualitySlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mRatioSlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row3.items.add(juce::FlexItem(*mReleaseSlider).withMinWidth(rowWidth).withMinHeight(rowHeight));

	row4.items.add(juce::FlexItem().withMinWidth(rowWidth).withMinHeight(rowHeight));
	row4.items.add(juce::FlexItem(*mPeakFilterGainSlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	row4.items.add(juce::FlexItem().withMinWidth(rowWidth).withMinHeight(rowHeight));
	row4.items.add(juce::FlexItem(*mChannelGainSlider).withMinWidth(rowWidth).withMinHeight(rowHeight));
	
	// Perform layout for each row
	row0.performLayout(bounds.removeFromTop(row0Height));
	row1.performLayout(bounds.removeFromTop(row1Height));
	bounds.removeFromTop(spacerHeight);
	row2.performLayout(bounds.removeFromTop(rowHeight));
	bounds.removeFromTop(spacerHeight);
	row3.performLayout(bounds.removeFromTop(rowHeight));
	bounds.removeFromTop(spacerHeight);
	row4.performLayout(bounds.removeFromTop(rowHeight));
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