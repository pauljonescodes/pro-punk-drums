#include "OutputsComponent.h"
#include "../Configuration/Channels.h"

OutputsComponent::OutputsComponent(juce::AudioProcessorValueTreeState& apvts, std::function<void(int, float)> onDrumMidiButtonClicked)
{
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&container, false);

    for (int channelIndex = 0; channelIndex < channels::size; channelIndex++) {
        auto* component = new OutputParametersComponent(channelIndex, apvts);
        component->mOnDrumMidiButtonClicked = onDrumMidiButtonClicked;
        mComponents.add(component);
        container.addAndMakeVisible(component);
    }
}

OutputsComponent::~OutputsComponent()
{
    DBG("~OutputsComponent()");
}

void OutputsComponent::paint(juce::Graphics&)
{
}

void OutputsComponent::resized()
{
    viewport.setBounds(getLocalBounds());
    int heightPerComponent = getHeight();
    int totalHeight = heightPerComponent * mComponents.size();

    container.setBounds(0, 0, viewport.getWidth(), totalHeight);

    auto area = container.getLocalBounds();
    for (auto* component : mComponents)
    {
        component->setBounds(area.removeFromTop(heightPerComponent));
    }
}
