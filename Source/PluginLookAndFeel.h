/*
  ==============================================================================

	PluginLookAndFeel.h
	Created: 9 Jan 2024 10:30:59pm
	Author:  paulm

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define _USE_MATH_DEFINES
#include <math.h>

static juce::Colour darkestGrey = juce::Colour(34, 34, 34);
static juce::Colour darkerGrey = juce::Colour(68, 68, 68);

class PluginLookAndFeel : public juce::LookAndFeel_V4
{
public:
	PluginLookAndFeel()
	{
		setColourScheme({
			juce::Colours::black, // windowBackground - Black
			darkestGrey,		  // widgetBackground - Dark grey
			darkestGrey,		  // menuBackground - Grey
			juce::Colours::white, // outline - White
			juce::Colours::white, // defaultText - White
			juce::Colours::red,	  // defaultFill - Red
			juce::Colours::white, // highlightedText - White
			juce::Colours::red,	  // highlightedFill - Red
			juce::Colours::white  // menuText - White
		});

		setColour(juce::TabbedComponent::outlineColourId, darkestGrey);
		setColour(juce::GroupComponent::outlineColourId, darkerGrey);
		setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::black);
	}

	void drawRotarySlider(juce::Graphics &, int x, int y, int width, int height,
						  float sliderPosProportional, float rotaryStartAngle,
						  float rotaryEndAngle, juce::Slider &) override;

	void drawButtonBackground(juce::Graphics &g, juce::Button &button,
							  const juce::Colour &backgroundColour,
							  bool isMouseOverButton, bool isButtonDown) override;

	int getDefaultScrollbarWidth() override;
	bool areScrollbarButtonsVisible() override { return false; }
	void drawScrollbarButton(juce::Graphics &g, juce::ScrollBar &bar,
							 int width, int height, int buttonDirection,
							 bool isScrollbarVertical, bool isMouseOverButton,
							 bool isButtonDown) override {}

	void drawCornerResizer(juce::Graphics &g, int w, int h, bool /*isMouseOver*/, bool /*isMouseDragging*/) override;
	void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& cb) override;
	void positionComboBoxText(juce::ComboBox& cb, juce::Label& labelToPosition) override;
	void drawScrollbar(juce::Graphics& g, juce::ScrollBar& scrollbar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;
	void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;
	void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override;
	void drawTabButton(juce::TabBarButton& button, juce::Graphics& g, bool isMouseOver, bool isMouseDown) override;
};
