# Notes

## Original

```C++
for (const auto& midiNote : generalmidi::midiNotesVector)
	{
		std::string generalMidiName = generalmidi::midiNoteToNameMap.at(midiNote);
		std::string generalMidiSnakeCaseName = PluginUtils::toSnakeCase(generalMidiName);

		for (int velocityIndex = midi::minimumVelocity; velocityIndex <= midi::maximumVelocity; velocityIndex++)
		{
			bool foundResourceAtVelocity = false;
			for (int variationIndex = 0; variationIndex < constants::maximumVariations; variationIndex++)
			{
				std::string resourceName = generalMidiSnakeCaseName + "_" + std::to_string(velocityIndex + 1) + "_" + std::to_string(variationIndex + 1);
				bool foundResourceAtVariation = false;

				for (int resourceIndex = 0; resourceIndex < BinaryData::namedResourceListSize; ++resourceIndex)
				{
					std::string namedResourceAtIndex = std::string(BinaryData::namedResourceList[resourceIndex]);
					size_t resourceNameFindResult = namedResourceAtIndex.find(resourceName);
					if (resourceNameFindResult != std::string::npos)
					{
						foundResourceAtVariation = true;
						foundResourceAtVelocity = true;

						mSynthesiserPtr->addSample(
							namedResourceAtIndex,
							samples::bitRate,
							samples::bitDepth,
							midiNote,
							generalmidi::midiNoteToStopNotesMap.at(midiNote),
							velocityIndex,
							variationIndex,
							*mAudioFormatManager.get()
						);
					}
				}

				if (!foundResourceAtVariation)
				{
					break;
				}
			}

			if (!foundResourceAtVelocity)
			{
				break;
			}
		}

	}

```

## Channel strip

- gain
- pan

- acoustic_bass_drum_blend
- acoustic_snare_blend

## Samples



## Naming convention

[general_midi]_[intensity_index]_[variation_index]_[mic_id]

