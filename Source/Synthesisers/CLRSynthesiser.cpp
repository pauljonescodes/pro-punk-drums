/*
  ==============================================================================

	CLROHSynthesiser.cpp
	Created: 8 Dec 2023 5:21:07pm
	Author:  paulm

  ==============================================================================
*/

#include "CLRSynthesiser.h"
#include "../Configuration/Samples.h"
#include "CLRSamplerVoice.h"

VaryingCLRSynthesiser::VaryingCLRSynthesiser()
{
	mVariationCount = 0;
	mCurrentVariationSoundIndex = 0;

	mCenterVoice = std::make_unique<PanningSamplerVoice>(0.0);
	mLeftVoice = std::make_unique<PanningSamplerVoice>(-1.0f);
	mRightVoice = std::make_unique<PanningSamplerVoice>(1);

	addVoice(mCenterVoice.get());
	addVoice(mLeftVoice.get());
	addVoice(mRightVoice.get());

	setNoteStealingEnabled(false);
}

void VaryingCLRSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	auto& leftSound = mLeftSamples[mCurrentVariationSoundIndex];
	if (leftSound->appliesToNote(midiNoteNumber) && leftSound->appliesToChannel(midiChannel))
	{
		stopVoice(mLeftVoice.get(), 1.0f, true);
		startVoice(mLeftVoice.get(), leftSound.get(), midiChannel, midiNoteNumber, velocity);
	}

	auto& centerSound = mCenterSamples[mCurrentVariationSoundIndex];
	if (centerSound->appliesToNote(midiNoteNumber) && centerSound->appliesToChannel(midiChannel))
	{
		stopVoice(mCenterVoice.get(), 1.0f, true);
		startVoice(mCenterVoice.get(), centerSound.get(), midiChannel, midiNoteNumber, velocity);
	}

	auto& rightSound = mRightSamples[mCurrentVariationSoundIndex];
	if (rightSound->appliesToNote(midiNoteNumber) && rightSound->appliesToChannel(midiChannel))
	{
		stopVoice(mRightVoice.get(), 1.0f, true);
		startVoice(mRightVoice.get(), rightSound.get(), midiChannel, midiNoteNumber, velocity);
	}

	mCurrentVariationSoundIndex = (mCurrentVariationSoundIndex + 1) % mVariationCount;
}

void VaryingCLRSynthesiser::addCLRSamplerSound(const PanningSamplerSound::Ptr& newSound, const samples::CenterLeftRightEnum clr) {
	addSound(newSound);

	switch (clr) {
	case samples::CenterLeftRightEnum::center: {
		mCenterSamples.push_back(newSound);
	}
	break;
	case samples::CenterLeftRightEnum::left: {
		mLeftSamples.push_back(newSound);
	}
	break;
	case samples::CenterLeftRightEnum::right: {
		mRightSamples.push_back(newSound);
	}
	break;
	}

	mVariationCount = std::min({ mCenterSamples.size(), mLeftSamples.size(), mRightSamples.size() });
}
