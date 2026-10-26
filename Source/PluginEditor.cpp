#include "PluginEditor.h"

FlowSidechainAudioProcessorEditor::FlowSidechainAudioProcessorEditor(FlowSidechainAudioProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    const std::array<juce::String, 5> ids { "threshold", "ratio", "attack", "release", "mix" };
    const std::array<juce::String, 5> names { "THRESHOLD", "RATIO", "ATTACK", "RELEASE", "MIX" };
    for (size_t i = 0; i < knobs.size(); ++i)
    {
        knobs[i] = std::make_unique<juce::Slider>();
        knobs[i]->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knobs[i]->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 78, 20);
        knobs[i]->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff8a36));
        knobs[i]->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xff353a42));
        knobs[i]->setColour(juce::Slider::thumbColourId, juce::Colour(0xffffc078));
        knobs[i]->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xffedf0f4));
        knobs[i]->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff171a1f));
        addAndMakeVisible(*knobs[i]);
        labels[i] = std::make_unique<juce::Label>();
        labels[i]->setText(names[i], juce::dontSendNotification);
        labels[i]->setJustificationType(juce::Justification::centred);
        labels[i]->setColour(juce::Label::textColourId, juce::Colour(0xff9299a4));
        labels[i]->setFont(juce::Font(11.0f, juce::Font::bold));
        addAndMakeVisible(*labels[i]);
        attachments[i] = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, ids[i], *knobs[i]);
    }
    setResizable(false, false);
    setSize(600, 330);
}

void FlowSidechainAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff101216));
    g.setColour(juce::Colour(0xffff8a36));
    g.fillRoundedRectangle(20.0f, 20.0f, 5.0f, 37.0f, 2.0f);
    g.setColour(juce::Colour(0xfff2f3f5));
    g.setFont(juce::Font(19.0f, juce::Font::bold));
    g.drawText("FLOW", 37, 20, 100, 22, juce::Justification::left);
    g.setColour(juce::Colour(0xffffa65f));
    g.setFont(juce::Font(10.0f, juce::Font::bold));
    g.drawText("SIDECHAIN", 38, 42, 100, 14, juce::Justification::left);
    g.setColour(juce::Colour(0xff292d34));
    g.drawHorizontalLine(72, 20.0f, 580.0f);
    g.setColour(juce::Colour(0xff818995));
    g.setFont(juce::Font(10.0f));
    g.drawText("EXTERNAL KEY INPUT  /  STEREO LINK", 270, 28, 310, 20, juce::Justification::right);
    g.setColour(juce::Colour(0xff171a20));
    g.fillRoundedRectangle(20.0f, 89.0f, 560.0f, 207.0f, 10.0f);
    g.setColour(juce::Colour(0xff2b3038));
    g.drawRoundedRectangle(20.0f, 89.0f, 560.0f, 207.0f, 10.0f, 1.0f);
    g.setColour(juce::Colour(0xff666e79));
    g.setFont(juce::Font(9.5f));
    g.drawText("Route a key signal to the plug-in's Sidechain input in your DAW.", 20, 306, 560, 16, juce::Justification::centred);
}

void FlowSidechainAudioProcessorEditor::resized()
{
    const int x0 = 28, gap = 110, top = 111;
    for (int i = 0; i < 5; ++i)
    {
        labels[(size_t)i]->setBounds(x0 + i * gap, top, 96, 22);
        knobs[(size_t)i]->setBounds(x0 + i * gap, top + 22, 96, 137);
    }
}

