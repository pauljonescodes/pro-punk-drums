#pragma once
#include <JuceHeader.h>
#include "TitledFrequencyQualityGainComponent.h"
#include "CompressionWithGainComponent.h"
#include "../Configuration/Channels.h"
#include "../Configuration/Strings.h"
#include "../PluginUtils.h"
#include "../Configuration/Parameters.h"

class OutputParametersComponent : public juce::Component
{
public:
	OutputParametersComponent(juce::AudioProcessorValueTreeState &apvts) : mApvts(apvts){};

	void paint(juce::Graphics &g) override
	{
		g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
	};

	void setChannelIndex(int channelIndex)
	{
		removeAllChildren();

		const auto &channelId = Channels::channelIndexToIdMap.at(channelIndex);

		{
			const auto &lowShelfFrequencyId = stringsJoinAndSnakeCase({channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::frequencyComponentId});
			const auto &lowShelfQualityId = stringsJoinAndSnakeCase({channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::qualityComponentId});
			const auto &lowShelfGainId = stringsJoinAndSnakeCase({channelId, AudioParameters::lowShelfEqualizationTypeId, AudioParameters::gainComponentId});

			mLowShelfComponentPtr.reset(new TitledFrequencyQualityGainComponent(Strings::lowShelf, mApvts, lowShelfFrequencyId, lowShelfQualityId, lowShelfGainId));
			addAndMakeVisible(mLowShelfComponentPtr.get());
		}

		{
			const auto &midPeakFrequencyId = stringsJoinAndSnakeCase({channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::frequencyComponentId});
			const auto &midPeakQualityId = stringsJoinAndSnakeCase({channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::qualityComponentId});
			const auto &midPeakGainId = stringsJoinAndSnakeCase({channelId, AudioParameters::peakFilterEqualizationTypeId, AudioParameters::gainComponentId});

			mMidPeakComponentPtr.reset(new TitledFrequencyQualityGainComponent(Strings::midPeak, mApvts, midPeakFrequencyId, midPeakQualityId, midPeakGainId));
			addAndMakeVisible(mMidPeakComponentPtr.get());
		}

		{
			const auto &highShelfFrequencyId = stringsJoinAndSnakeCase({channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::frequencyComponentId});
			const auto &highShelfQualityId = stringsJoinAndSnakeCase({channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::qualityComponentId});
			const auto &highShelfGainId = stringsJoinAndSnakeCase({channelId, AudioParameters::highShelfEqualizationTypeId, AudioParameters::gainComponentId});

			mHighShelfComponentPtr.reset(new TitledFrequencyQualityGainComponent(Strings::highShelf, mApvts, highShelfFrequencyId, highShelfQualityId, highShelfGainId));
			addAndMakeVisible(mHighShelfComponentPtr.get());
		}

		{
			const auto &thresholdId = stringsJoinAndSnakeCase({channelId, AudioParameters::thresholdComponentId});
			const auto &ratioId = stringsJoinAndSnakeCase({channelId, AudioParameters::ratioComponentId});
			const auto &attackId = stringsJoinAndSnakeCase({channelId, AudioParameters::attackId});
			const auto &releaseId = stringsJoinAndSnakeCase({channelId, AudioParameters::releaseComponentId});
			const auto &gainId = stringsJoinAndSnakeCase({channelId, AudioParameters::compressionComponentId, AudioParameters::gainComponentId});
			const auto &dryWetId = stringsJoinAndSnakeCase({channelId, AudioParameters::compressionComponentId, AudioParameters::dryWetComponentId});

			mCompressionWithGainComponent.reset(new CompressionWithGainComponent(Strings::compressor, mApvts, thresholdId, attackId, ratioId, releaseId, gainId, dryWetId));
			addAndMakeVisible(mCompressionWithGainComponent.get());
		}

		resized();
	}

	void resized() override
	{
		auto bounds = getLocalBounds();

		juce::FlexBox flexBoxColumns;
		flexBoxColumns.flexDirection = juce::FlexBox::Direction::column;
		flexBoxColumns.flexWrap = juce::FlexBox::Wrap::noWrap;
		flexBoxColumns.justifyContent = juce::FlexBox::JustifyContent::center;
		flexBoxColumns.alignItems = juce::FlexBox::AlignItems::stretch;
		flexBoxColumns.alignContent = juce::FlexBox::AlignContent::stretch;

		juce::FlexBox flexBoxRows;
		flexBoxRows.flexDirection = juce::FlexBox::Direction::row;
		flexBoxRows.flexWrap = juce::FlexBox::Wrap::noWrap;
		flexBoxRows.justifyContent = juce::FlexBox::JustifyContent::center;
		flexBoxRows.alignItems = juce::FlexBox::AlignItems::stretch;
		flexBoxRows.alignContent = juce::FlexBox::AlignContent::stretch;

		flexBoxColumns.items.add(juce::FlexItem(*mLowShelfComponentPtr).withFlex(1));
		flexBoxColumns.items.add(juce::FlexItem(*mMidPeakComponentPtr).withFlex(1));
		flexBoxColumns.items.add(juce::FlexItem(*mHighShelfComponentPtr).withFlex(1));

		flexBoxRows.items.add(juce::FlexItem(flexBoxColumns).withFlex(3));
		flexBoxRows.items.add(juce::FlexItem(*mCompressionWithGainComponent).withFlex(2));

		flexBoxColumns.performLayout(bounds);
		flexBoxRows.performLayout(bounds);
	};

private:
	juce::AudioProcessorValueTreeState &mApvts;
	std::unique_ptr<CompressionWithGainComponent> mCompressionWithGainComponent;
	std::unique_ptr<TitledFrequencyQualityGainComponent> mLowShelfComponentPtr;
	std::unique_ptr<TitledFrequencyQualityGainComponent> mMidPeakComponentPtr;
	std::unique_ptr<TitledFrequencyQualityGainComponent> mHighShelfComponentPtr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputParametersComponent)
};
