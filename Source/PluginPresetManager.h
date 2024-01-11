/*
  ==============================================================================

	PluginPresetManager.h
	Created: 21 Dec 2023 3:04:08pm
	Author:  paulm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class PluginPresetManager : juce::ValueTree::Listener
{
public:
	static const juce::File defaultDirectory;
	static const juce::String extension;
	static const juce::String presetNameProperty;

	PluginPresetManager(juce::AudioProcessorValueTreeState &);

	void savePreset(const juce::String &presetName);
	void deletePreset(const juce::String &presetName);
	void loadPreset(const juce::String &presetName);
	void loadPresetAt(int index);
	int loadNextPreset();
	int loadPreviousPreset();
	juce::StringArray getAllPresets() const;
	juce::String getCurrentPreset() const;
	int getCurrentPresetIndex() const;

private:
	void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) override;

	juce::AudioProcessorValueTreeState &mValueTreeState;
	juce::Value mCurrentPreset;
};