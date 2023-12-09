/*
  ==============================================================================

	CLROHSynthesiser.cpp
	Created: 8 Dec 2023 5:21:07pm
	Author:  paulm

  ==============================================================================
*/

#include "CLRSynthesiser.h"
#include "../Configuration/Samples.h"
#include "PanningSamplerVoice.h"

CLRSynthesiser::CLRSynthesiser() :
	mCenterSamples(0, std::vector<PanningSamplerSound::Ptr>()),
	mLeftSamples(0, std::vector<PanningSamplerSound::Ptr>()),
	mRightSamples(0, std::vector<PanningSamplerSound::Ptr>())
{

	mCenterVoice = std::make_unique<PanningSamplerVoice>(0.0);
	mLeftVoice = std::make_unique<PanningSamplerVoice>(-1.0f);
	mRightVoice = std::make_unique<PanningSamplerVoice>(1);

	addVoice(mCenterVoice.get());
	addVoice(mLeftVoice.get());
	addVoice(mRightVoice.get());

	setNoteStealingEnabled(false);
}

void CLRSynthesiser::noteOn(const int midiChannel, const int midiNoteNumber, const float velocity)
{
	auto hasSoundThatAppliesToNote = false;

	for (auto* sound : sounds)
	{
		if (sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
		{
			hasSoundThatAppliesToNote = true;
			break;
		}
	}

	if (hasSoundThatAppliesToNote) {
		if (mCenterSamples.first < mCenterSamples.second.size()) {
			auto& centerSound = mCenterSamples.second[mCenterSamples.first];
			if (centerSound->appliesToNote(midiNoteNumber) && centerSound->appliesToChannel(midiChannel))
			{
				stopVoice(mCenterVoice.get(), 1.0f, true);
				startVoice(mCenterVoice.get(), centerSound.get(), midiChannel, midiNoteNumber, velocity);
			}

			mCenterSamples.first = (mCenterSamples.first + 1) % mCenterSamples.second.size();
		}

		if (mLeftSamples.first < mLeftSamples.second.size()) {
			auto& leftSound = mLeftSamples.second[mLeftSamples.first];
			if (leftSound->appliesToNote(midiNoteNumber) && leftSound->appliesToChannel(midiChannel))
			{
				stopVoice(mLeftVoice.get(), 1.0f, true);
				startVoice(mLeftVoice.get(), leftSound.get(), midiChannel, midiNoteNumber, velocity);
			}

			mLeftSamples.first = (mLeftSamples.first + 1) % mLeftSamples.second.size();
		}
		
		if (mRightSamples.first < mRightSamples.second.size()) {
			auto& rightSound = mRightSamples.second[mRightSamples.first];
			if (rightSound->appliesToNote(midiNoteNumber) && rightSound->appliesToChannel(midiChannel))
			{
				stopVoice(mRightVoice.get(), 1.0f, true);
				startVoice(mRightVoice.get(), rightSound.get(), midiChannel, midiNoteNumber, velocity);
			}

			mRightSamples.first = (mRightSamples.first + 1) % mRightSamples.second.size();
		}
	}
}

void CLRSynthesiser::addCLRSamplerSound(const PanningSamplerSound::Ptr& newSound, const samples::CenterLeftRightEnum clr) {
	addSound(newSound);

	switch (clr) {
	case samples::CenterLeftRightEnum::center:
		mCenterSamples.second.push_back(newSound);
		break;
	case samples::CenterLeftRightEnum::left:
		mLeftSamples.second.push_back(newSound);
		break;
	case samples::CenterLeftRightEnum::right:
		mRightSamples.second.push_back(newSound);
		break;
	}
}
