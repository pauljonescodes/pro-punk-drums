/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ABKit2AudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
	ABKit2AudioProcessorEditor(ABKit2AudioProcessor&);
	~ABKit2AudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	ABKit2AudioProcessor& audioProcessor;

	//==============================================================================
	juce::OwnedArray<juce::TextButton> mMidiNoteButtons;
	void buttonClicked(juce::Button* button) override;
	void position();
	//==============================================================================

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ABKit2AudioProcessorEditor)
};
