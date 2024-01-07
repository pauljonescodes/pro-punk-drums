#include "DrumsComponent.h"
#include "../Configuration/Samples.h"
#include "../Configuration/GeneralMidi.h"
#include "../Configuration/Strings.h"
#include "../Configuration/Parameters.h"

DrumsComponent::DrumsComponent(
	const std::vector<int> midiNotesVector, 
	juce::AudioProcessorValueTreeState& apvts)
{
	mVelocitySlider = std::make_unique<juce::Slider>();
	mVelocitySlider->setRange(0, 127, 1);
	mVelocitySlider->setValue(100);
	addAndMakeVisible(mVelocitySlider.get());

	mVelocityLabel = std::make_unique<juce::Label>();
	mVelocityLabel->setText(Strings::velocity, juce::dontSendNotification);
	mVelocityLabel->attachToComponent(mVelocitySlider.get(), true);
	addAndMakeVisible(mVelocityLabel.get());

	for (int note : midiNotesVector)
	{
		juce::TextButton* button = new juce::TextButton(GeneralMidiPercussion::midiNoteToNameMap.at(note) + "\n" + juce::String(note).toStdString() + " " + juce::MidiMessage::getMidiNoteName(note, true, true, 4).toStdString());
		button->setComponentID(juce::String(note));
		mMidiNoteButtonsOwnedArray.add(button);
		addAndMakeVisible(button);
		button->addListener(this);
	}

	auto* multiOutParameter = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter(AudioParameters::multiOutComponentId));
	mMultiOutToggleButton.reset(new juce::ToggleButton(Strings::multiOut));
	addAndMakeVisible(mMultiOutToggleButton.get());
	mMultiOutToggleButton->setToggleState(multiOutParameter->get(), juce::dontSendNotification);
	mMultiOutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		AudioParameters::multiOutComponentId,
		*mMultiOutToggleButton
		);

	resized();
}

DrumsComponent::~DrumsComponent()
{
	mMultiOutAttachment.reset();

	mMidiNoteButtonsOwnedArray.clear(); 

	mMultiOutToggleButton.reset();
	mVelocitySlider.reset();
	mVelocityLabel.reset();
}


void DrumsComponent::paint(juce::Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void DrumsComponent::resized()
{
	const auto localBounds = getLocalBounds();
	auto sampleControls = localBounds;

	auto sampleControlHeight = 50;

	sampleControls.setHeight(sampleControlHeight);

	mMultiOutToggleButton->setBounds(sampleControls.removeFromLeft(localBounds.proportionOfWidth(0.1)));
	mVelocitySlider->setBounds(sampleControls.withTrimmedLeft(localBounds.proportionOfWidth(0.1)));

	// Layout for the MIDI note buttons
	int numRows = std::max(1, (localBounds.getHeight()) / 125);
	int numCols = std::max(1, localBounds.getWidth() / 125);
	int buttonWidth = localBounds.getWidth() / numCols;
	int buttonHeight = (localBounds.getHeight() - sampleControlHeight) / numRows;

	for (int i = 0; i < mMidiNoteButtonsOwnedArray.size(); ++i)
	{
		int row = i / numCols;
		int col = i % numCols;
		mMidiNoteButtonsOwnedArray[i]->setBounds(col * buttonWidth, row * buttonHeight + sampleControlHeight, buttonWidth, buttonHeight);
	}
}

void DrumsComponent::buttonClicked(juce::Button* button)
{
	const juce::String componentID = button->getComponentID();
	int midiNoteValue = componentID.getIntValue();
	float velocity = mVelocitySlider->getValue() / 127.0f;

	if (mOnDrumMidiButtonClicked.has_value()) {
		mOnDrumMidiButtonClicked.value()(midiNoteValue, velocity);
	}
}

