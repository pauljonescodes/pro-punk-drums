/*
  ==============================================================================

	PluginPresetManager.cpp
	Created: 21 Dec 2023 3:04:08pm
	Author:  paulm

  ==============================================================================
*/

#include "PluginPresetManager.h"

const juce::File PluginPresetManager::defaultDirectory{juce::File::getSpecialLocation(
														   juce::File::SpecialLocationType::commonDocumentsDirectory)
														   .getChildFile(ProjectInfo::companyName)
														   .getChildFile(ProjectInfo::projectName)};
const juce::String PluginPresetManager::extension{"preset"};
const juce::String PluginPresetManager::presetNameProperty{"presetName"};

PluginPresetManager::PluginPresetManager(juce::AudioProcessorValueTreeState &apvts) : mValueTreeState(apvts)
{
	// Create a default Preset Directory, if it doesn't exist
	if (!defaultDirectory.exists())
	{
		const auto result = defaultDirectory.createDirectory();
		if (result.failed())
		{
			DBG("Could not create preset directory: " + result.getErrorMessage());
			jassertfalse;
		}
	}

	mValueTreeState.state.addListener(this);
	mCurrentPreset.referTo(mValueTreeState.state.getPropertyAsValue(presetNameProperty, nullptr));
}

void PluginPresetManager::savePreset(const juce::String &presetName)
{
	if (presetName.isEmpty())
		return;

	mCurrentPreset.setValue(presetName);
	const auto xml = mValueTreeState.copyState().createXml();
	const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
	if (!xml->writeTo(presetFile))
	{
		DBG("Could not create preset file: " + presetFile.getFullPathName());
		jassertfalse;
	}
}

void PluginPresetManager::deletePreset(const juce::String &presetName)
{
	if (presetName.isEmpty())
		return;

	const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
	if (!presetFile.existsAsFile())
	{
		DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
		jassertfalse;
		return;
	}
	if (!presetFile.deleteFile())
	{
		DBG("Preset file " + presetFile.getFullPathName() + " could not be deleted");
		jassertfalse;
		return;
	}
	mCurrentPreset.setValue("");
}

void PluginPresetManager::loadPreset(const juce::String &presetName)
{
	if (presetName.isEmpty())
		return;

	const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
	if (!presetFile.existsAsFile())
	{
		DBG("Preset file " + presetFile.getFullPathName() + " does not exist");
		jassertfalse;
		return;
	}
	// presetFile (XML) -> (ValueTree)
	juce::XmlDocument xmlDocument{presetFile};
	const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement());

	mValueTreeState.replaceState(valueTreeToLoad);
	mCurrentPreset.setValue(presetName);
}

void PluginPresetManager::loadPresetAt(int index)
{
	const auto allPresets = getAllPresets();
	if (index >= 0 && index < allPresets.size())
	{
		loadPreset(allPresets.getReference(index));
	}
}

int PluginPresetManager::loadNextPreset()
{
	const auto allPresets = getAllPresets();
	if (allPresets.isEmpty())
		return -1;
	const auto currentIndex = allPresets.indexOf(mCurrentPreset.toString());
	const auto nextIndex = currentIndex + 1 > (allPresets.size() - 1) ? 0 : currentIndex + 1;
	loadPreset(allPresets.getReference(nextIndex));
	return nextIndex;
}

int PluginPresetManager::loadPreviousPreset()
{
	const auto allPresets = getAllPresets();
	if (allPresets.isEmpty())
		return -1;
	const auto currentIndex = allPresets.indexOf(mCurrentPreset.toString());
	const auto previousIndex = currentIndex - 1 < 0 ? allPresets.size() - 1 : currentIndex - 1;
	loadPreset(allPresets.getReference(previousIndex));
	return previousIndex;
}

juce::StringArray PluginPresetManager::getAllPresets() const
{
	juce::StringArray presets;
	const auto fileArray = defaultDirectory.findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*." + extension);
	for (const auto &file : fileArray)
	{
		presets.add(file.getFileNameWithoutExtension());
	}

	return presets;
}

juce::String PluginPresetManager::getCurrentPreset() const
{
	return mCurrentPreset.toString();
}

int PluginPresetManager::getCurrentPresetIndex() const
{
	const auto allPresets = getAllPresets();
	return allPresets.indexOf(mCurrentPreset.toString());
}

void PluginPresetManager::valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged)
{
	mCurrentPreset.referTo(treeWhichHasBeenChanged.getPropertyAsValue(presetNameProperty, nullptr));
}
