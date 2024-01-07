#include "OutputParametersComponent.h"
#include "../Configuration/Channels.h"
#include "../Configuration/Strings.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"

OutputParametersComponent::OutputParametersComponent(int channelIndex, juce::AudioProcessorValueTreeState& apvts)
	: mApvts(apvts)
{
	const auto& channelId = Channels::channelIndexToIdMap.at(channelIndex);
	const auto& channelName = PluginUtils::toTitleCase(channelId);
	const auto channelMidi = Channels::channelIndexToMainGeneralMidiNote.at(channelIndex);

	mGroupComponentPtr.reset(new juce::GroupComponent(channelName, channelName));
	addAndMakeVisible(mGroupComponentPtr.get());

	mNoteOnButtonPtr.reset(new juce::TextButton(std::to_string(channelMidi) + " " + PluginUtils::toTitleCase(Channels::channelIndexToIdMap.at(channelIndex))));
	mNoteOnButtonPtr->setComponentID(juce::String(channelMidi));
	mNoteOnButtonPtr->addListener(this);
	mMidiNoteValue = channelMidi;
	addAndMakeVisible(mNoteOnButtonPtr.get());

	const auto& lowShelfFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
	const auto& lowShelfQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId });
	const auto& lowShelfGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::gainComponentId });

	mLowShelfComponentPtr.reset(new TitledFrequencyQualityGainComponent(Strings::lowShelf, apvts, lowShelfFrequencyId, lowShelfQualityId, lowShelfGainId));
	addAndMakeVisible(mLowShelfComponentPtr.get());

	const auto& midPeakFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::frequencyComponentId });
	const auto& midPeakQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::qualityComponentId });
	const auto& midPeakGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::gainComponentId });

	mMidPeakComponentPtr.reset(new TitledFrequencyQualityGainComponent(Strings::midPeak, apvts, midPeakFrequencyId, midPeakQualityId, midPeakGainId));
	addAndMakeVisible(mMidPeakComponentPtr.get());

	const auto& highShelfFrequencyId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::frequencyComponentId });
	const auto& highShelfQualityId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::qualityComponentId });
	const auto& highShelfGainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::gainComponentId });

	mHighShelfComponentPtr.reset(new TitledFrequencyQualityGainComponent(Strings::midPeak, apvts, highShelfFrequencyId, highShelfQualityId, highShelfGainId));
	addAndMakeVisible(mHighShelfComponentPtr.get());

	const auto& thresholdId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::thresholdComponentId });
	const auto& ratioId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::ratioComponentId });
	const auto& attackId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::attackId });
	const auto& releaseId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::releaseComponentId });
	const auto& gainId = PluginUtils::joinAndSnakeCase({ channelId, AudioParameters::gainComponentId });

	mCompressionWithGainComponent.reset(new CompressionWithGainComponent(Strings::compressor, apvts, thresholdId, attackId, ratioId, releaseId, gainId));
	addAndMakeVisible(mCompressionWithGainComponent.get());
}


void OutputParametersComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void OutputParametersComponent::resized()
{
	auto bounds = getLocalBounds();
	int componentWidth = bounds.getHeight() / 3; // dividing the space into three parts

	mGroupComponentPtr->setBounds(bounds);

	juce::FlexBox flexBox;
	flexBox.flexDirection = juce::FlexBox::Direction::column;
	flexBox.flexWrap = juce::FlexBox::Wrap::noWrap;
	flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
	flexBox.alignItems = juce::FlexBox::AlignItems::stretch;
	flexBox.alignContent = juce::FlexBox::AlignContent::stretch;

	flexBox.items.add(juce::FlexItem(*mLowShelfComponentPtr).withMargin(juce::FlexItem::Margin(16, 0, 0, 0)).withFlex(1));
	flexBox.items.add(juce::FlexItem(*mMidPeakComponentPtr).withFlex(1));
	flexBox.items.add(juce::FlexItem(*mHighShelfComponentPtr).withFlex(1));
	flexBox.items.add(juce::FlexItem(*mCompressionWithGainComponent).withFlex(1));

	flexBox.performLayout(bounds);
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
