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

	mLabel.reset(new juce::Label(std::to_string(channelIndex) + "_label", channels::channelIndexToNameMap.at(channelIndex)));
	addAndMakeVisible(mLabel.get());

	mCompressionOnToggleButton.reset(new juce::ToggleButton("Compressor"));
	mCompressionOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::compressionId, parameters::onId }),
		*mCompressionOnToggleButton
		);
	addAndMakeVisible(mCompressionOnToggleButton.get());

	mHighPassOnToggleButton.reset(new juce::ToggleButton("High pass"));
	mHighPassOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::highPassEqualizationTypeId, parameters::onId }),
		*mHighPassOnToggleButton
		);
	addAndMakeVisible(mHighPassOnToggleButton.get());

	mPeakFilterOnToggleButton.reset(new juce::ToggleButton("Mid peak"));
	mPeakFilterOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::peakFilterEqualizationTypeId, parameters::onId }),
		*mPeakFilterOnToggleButton
		);
	addAndMakeVisible(mPeakFilterOnToggleButton.get());

	mLowPassOnToggleButton.reset(new juce::ToggleButton("Low pass"));
	mLowPassOnAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		PluginUtils::joinId({ channelId, parameters::lowPassEqualizationTypeId, parameters::onId }),
		*mLowPassOnToggleButton
		);
	addAndMakeVisible(mLowPassOnToggleButton.get());


// mHighPassFrequencySlider
// mPeakFilterFrequencySlider
// mPeakFilterQualitySlider
// mPeakFilterGainSlider
// mThresholdSlider
// mRatioSlider
// mAttackSlider
// mReleaseSlider
// mLowPassFrequencySlider
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
	auto area = getLocalBounds();

	int sectionWidth = area.getWidth() / 5;

	mLabel->setBounds(area.removeFromLeft(sectionWidth));
	mHighPassOnToggleButton->setBounds(area.removeFromLeft(sectionWidth));
	mPeakFilterOnToggleButton->setBounds(area.removeFromLeft(sectionWidth));
	mCompressionOnToggleButton->setBounds(area.removeFromLeft(sectionWidth));
	mLowPassOnToggleButton->setBounds(area.removeFromLeft(sectionWidth));

	// mHighPassFrequencySlider
	// mPeakFilterFrequencySlider
	// mPeakFilterQualitySlider
	// mPeakFilterGainSlider
	// mThresholdSlider
	// mRatioSlider
	// mAttackSlider
	// mReleaseSlider
	// mLowPassFrequencySlider
}

void OutputParametersComponent::buttonClicked(juce::Button* button)
{
}
