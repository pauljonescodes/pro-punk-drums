/*
  ==============================================================================

	SamplerComponent.cpp
	Created: 8 Dec 2023 10:49:23am
	Author:  paulm

  ==============================================================================
*/

#include "DrumsComponent.h"
#include "../Configuration/Samples.h"
#include "../Configuration/GeneralMidi.h"
#include "../Configuration/Strings.h"

//==============================================================================
DrumsComponent::DrumsComponent(std::vector<int> midiNotesVector)
{
	mVelocitySlider = std::make_unique<juce::Slider>();
	mVelocitySlider->setRange(0, 127, 1);
	mVelocitySlider->setValue(64);

	addAndMakeVisible(mVelocitySlider.get());

	mVelocityLabel = std::make_unique<juce::Label>();
	mVelocityLabel->setText(strings::velocity, juce::dontSendNotification);
	mVelocityLabel->attachToComponent(mVelocitySlider.get(), true);

	addAndMakeVisible(mVelocityLabel.get());

	for (int note : midiNotesVector)
	{
		juce::TextButton* button = new juce::TextButton(juce::String(note) + " " + generalmidi::midiNoteToNameMap.at(note));
		button->setComponentID(juce::String(note));
		mMidiNoteButtons.add(button);
		addAndMakeVisible(button);
		button->addListener(this);
	}

	mMidiFileButton = std::make_unique <juce::TextButton>("MIDI");
	mMidiFileButton->setComponentID(juce::String("midi_file"));
	addAndMakeVisible(mMidiFileButton.get());
	mMidiFileButton->addListener(this);

	mFileChooser = std::make_unique<juce::FileChooser>("Please select the midi you want to load...",
		juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
		"*.mid");

	resized();
}

DrumsComponent::~DrumsComponent()
{
	for (auto button : mMidiNoteButtons) {
		button = nullptr;
	}
}

//==============================================================================
void DrumsComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void DrumsComponent::resized()
{
	auto area = getLocalBounds();

	// Allocate space for the slider at the top
	auto topArea = area.removeFromTop(50); // Adjust the height as needed
	auto velocitySliderArea = topArea.removeFromRight(topArea.getWidth() - 64).reduced(50, 0);

	mVelocitySlider->setBounds(velocitySliderArea);

	auto midiFileButtonArea = topArea.reduced(10, 0); // Use the remaining part of topArea
	mMidiFileButton->setBounds(midiFileButtonArea);

	// Layout for buttons
	int numRows = std::max(1, (area.getHeight()) / 125);
	int numCols = std::max(1, area.getWidth() / 125);
	int buttonWidth = area.getWidth() / numCols;
	int buttonHeight = area.getHeight() / numRows;

	for (int i = 0; i < mMidiNoteButtons.size(); ++i)
	{
		int row = i / numCols;
		int col = i % numCols;
		mMidiNoteButtons[i]->setBounds(col * buttonWidth, row * buttonHeight + topArea.getHeight(), buttonWidth, buttonHeight);
	}
}

void DrumsComponent::buttonClicked(juce::Button* button)
{
	const juce::String componentID = button->getComponentID();

	if (mMidiFileButton->getComponentID() == componentID) {
		auto folderChooserFlags = juce::FileBrowserComponent::openMode;
		mFileChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
			{
				juce::File midiFile(chooser.getResult());
		if (mOnMidiFileChoser.has_value()) {
			mOnMidiFileChoser.value()(midiFile);
		}

			});
	}
	else {
		int midiNoteValue = componentID.getIntValue();
		float velocity = mVelocitySlider->getValue() / 127.0f;

		if (mOnDrumMidiButtonClicked.has_value()) {
			mOnDrumMidiButtonClicked.value()(midiNoteValue, velocity);
		}
	}
}

