#pragma once
#include "PluginProcessor.h"

class FlowSidechainAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit FlowSidechainAudioProcessorEditor(FlowSidechainAudioProcessor&);
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    FlowSidechainAudioProcessor& processor;
    std::array<std::unique_ptr<juce::Slider>, 5> knobs;
    std::array<std::unique_ptr<juce::Label>, 5> labels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>, 5> attachments;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlowSidechainAudioProcessorEditor)
};

