#include "PluginAudioProcessor.h"
#include "PluginAudioProcessorEditor.h"
#include "Configuration/Samples.h"
#include "Components/DrumsComponent.h"
#include "Components/SamplesComponent.h"
#include <random>
#include "Configuration/Strings.h"

PluginAudioProcessorEditor::PluginAudioProcessorEditor(PluginAudioProcessor &p)
	: AudioProcessorEditor(&p), mAudioProcessor(p)
{
	static juce::Typeface::Ptr customTypeface = juce::Typeface::createSystemTypefaceFor(
		BinaryData::SpecialEliteRegular_ttf,
		BinaryData::SpecialEliteRegular_ttfSize);

	juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypeface(customTypeface);
	setLookAndFeel(&mLookAndFeel);

	auto &apvts = mAudioProcessor.getParameterValueTreeState();

	auto *multiOutParameter = dynamic_cast<juce::AudioParameterBool *>(apvts.getParameter(AudioParameters::multiOutComponentId));
	mMultiOutToggleButton.reset(new juce::ToggleButton(Strings::multiOut));
	addAndMakeVisible(mMultiOutToggleButton.get());

	mMultiOutToggleButton->setToggleState(multiOutParameter->get(), juce::dontSendNotification);
	mMultiOutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
		apvts,
		AudioParameters::multiOutComponentId,
		*mMultiOutToggleButton);

	mTabbedComponentPtr = std::make_unique<juce::TabbedComponent>(juce::TabbedButtonBar::Orientation::TabsAtTop);

	mPresetComponentPtr.reset(new PresetComponent(mAudioProcessor.getPresetManager()));
	addAndMakeVisible(*mPresetComponentPtr);

	setSize(642, 699);
	setResizeLimits(497, 600, 9999, 9999);
	setResizable(true, true);

	addAndMakeVisible(mTabbedComponentPtr.get());

	mDrumsComponentPtr.reset(new DrumsComponent(mAudioProcessor.getMidiNotesVector()));
	mDrumsComponentPtr->mOnDrumMidiButtonClicked = ([this](int midiNote, float midiVelocity) -> void
													{ mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity); });
	mTabbedComponentPtr->addTab(Strings::drums, juce::Colours::lightgrey, mDrumsComponentPtr.get(), true);

	mSamplesComponentPtr.reset(new SamplesComponent(mAudioProcessor.getMidiNotesVector(), apvts, ([this](int midiNote, float midiVelocity, std::string micId) -> void
																								  { mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity, micId); })));

	mOutputsComponentPtr.reset(new OutputsComponent(apvts, ([this](int midiNote, float midiVelocity) -> void
															{ mAudioProcessor.noteOnSynthesisers(midiNote, midiVelocity); })));

	mReverbComponentPtr.reset(new ReverbComponent(
		apvts,
		AudioParameters::roomSizeComponentId,
		AudioParameters::dampingComponentId,
		AudioParameters::widthComponentId));

	mTabbedComponentPtr->addTab(Strings::samples, juce::Colours::lightgrey, mSamplesComponentPtr.get(), true);
	mTabbedComponentPtr->addTab(Strings::outputs, juce::Colours::lightgrey, mOutputsComponentPtr.get(), true);
	mTabbedComponentPtr->addTab(Strings::room, juce::Colours::lightgrey, mReverbComponentPtr.get(), true);
}

PluginAudioProcessorEditor::~PluginAudioProcessorEditor()
{
	mMultiOutAttachment.reset();
}

void PluginAudioProcessorEditor::paint(juce::Graphics &g)
{
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginAudioProcessorEditor::resized()
{
	auto localBounds = getLocalBounds();

	auto topAreaBounds = localBounds.removeFromTop(64).reduced(4);

	mMultiOutToggleButton->setBounds(topAreaBounds.removeFromRight(112));
	mPresetComponentPtr->setBounds(topAreaBounds);

	mTabbedComponentPtr->setBounds(localBounds);
}
