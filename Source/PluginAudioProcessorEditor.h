#pragma once

#include <JuceHeader.h>
#include "PluginAudioProcessor.h"
#include "PluginPresetManager.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"
#include "Components/OutputsComponent.h"
#include "Components/ReverbComponent.h"
#include "PluginLookAndFeel.h"

class PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    PluginAudioProcessorEditor(PluginAudioProcessor &);
    ~PluginAudioProcessorEditor() override;
    void paint(juce::Graphics &) override;
    void resized() override;

private:
    PluginAudioProcessor &mAudioProcessor;

    PluginLookAndFeel mLookAndFeel;

    std::unique_ptr<juce::TabbedComponent> mTabbedComponentPtr;
    std::unique_ptr<PresetComponent> mPresetComponentPtr;
    std::unique_ptr<DrumsComponent> mDrumsComponentPtr;
    std::unique_ptr<SamplesComponent> mSamplesComponentPtr;
    std::unique_ptr<OutputsComponent> mOutputsComponentPtr;
    std::unique_ptr<ReverbComponent> mReverbComponentPtr;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> mMultiOutAttachment;
    std::unique_ptr<juce::ToggleButton> mMultiOutToggleButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginAudioProcessorEditor)
};
