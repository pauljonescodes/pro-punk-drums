/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Samples.h"
#include <random>

//==============================================================================
ABKit2AudioProcessorEditor::ABKit2AudioProcessorEditor (ABKit2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 600);
    setResizable(true, true);

    for (int note : samples::generalMidiNotesVector)
    {
        juce::TextButton* button = new juce::TextButton(juce::String(note) + " " + generalmidi::midiNoteToNameMap.at(note));
        button->setComponentID(juce::String(note));
        mMidiNoteButtons.add(button);
        addAndMakeVisible(button);

        // Optionally, add listeners or callbacks here
        button->addListener(this);
    }

    position();
}

ABKit2AudioProcessorEditor::~ABKit2AudioProcessorEditor()
{
}

//==============================================================================
void ABKit2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void ABKit2AudioProcessorEditor::resized()
{
    position();
}

//==============================================================================

void ABKit2AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    const juce::String componentID = button->getComponentID();

    // Convert the component ID (string) back to a MIDI note value (integer)
    int midiNoteValue = componentID.getIntValue();
    float randomVelocity = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    audioProcessor.noteOnSynthesiser(midiNoteValue, randomVelocity);
}

void ABKit2AudioProcessorEditor::position() {
    int numRows = 6; // You can adjust the number of rows and columns
    int numCols = 4; // as per the number of buttons you have
    int buttonWidth = getWidth() / numCols;
    int buttonHeight = getHeight() / numRows;

    for (int i = 0; i < mMidiNoteButtons.size(); ++i)
    {
        int row = i / numCols;
        int col = i % numCols;
        mMidiNoteButtons[i]->setBounds(col * buttonWidth, row * buttonHeight, buttonWidth, buttonHeight);
    }
}