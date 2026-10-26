#include "PluginProcessor.h"
#include "PluginEditor.h"

FlowSidechainAudioProcessor::FlowSidechainAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withInput("Sidechain", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameters()) {}

juce::AudioProcessorValueTreeState::ParameterLayout FlowSidechainAudioProcessor::createParameters()
{
    using P = juce::AudioProcessorValueTreeState;
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> p;
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"threshold", 1}, "Threshold", juce::NormalisableRange<float>{-60.0f, 0.0f, 0.1f}, -24.0f, "dB"));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"ratio", 1}, "Ratio", juce::NormalisableRange<float>{1.0f, 20.0f, 0.1f}, 4.0f, ":1"));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"attack", 1}, "Attack", juce::NormalisableRange<float>{0.1f, 100.0f, 0.01f, 0.45f}, 10.0f, "ms"));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"release", 1}, "Release", juce::NormalisableRange<float>{10.0f, 1000.0f, 0.1f, 0.45f}, 120.0f, "ms"));
    p.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"mix", 1}, "Mix", juce::NormalisableRange<float>{0.0f, 100.0f, 0.1f}, 100.0f, "%"));
    return {p.begin(), p.end()};
}

void FlowSidechainAudioProcessor::prepareToPlay(double rate, int)
{
    sampleRate = rate;
    detectorEnvelope = 0.0f;
    smoothedGain = 1.0f;
}

bool FlowSidechainAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();
    if (mainIn != mainOut || (mainIn != juce::AudioChannelSet::mono() && mainIn != juce::AudioChannelSet::stereo())) return false;
    const auto sc = layouts.getChannelSet(true, 1);
    return sc.isDisabled() || sc == juce::AudioChannelSet::mono() || sc == juce::AudioChannelSet::stereo();
}

void FlowSidechainAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const int channels = getTotalNumOutputChannels();
    for (int c = getTotalNumOutputChannels(); c < buffer.getNumChannels(); ++c) buffer.clear(c, 0, buffer.getNumSamples());
    auto sc = getBusBuffer(buffer, true, 1);
    const bool hasSidechain = sc.getNumChannels() > 0;
    const float threshold = parameters.getRawParameterValue("threshold")->load();
    const float ratio = parameters.getRawParameterValue("ratio")->load();
    const float attackMs = parameters.getRawParameterValue("attack")->load();
    const float releaseMs = parameters.getRawParameterValue("release")->load();
    const float wet = parameters.getRawParameterValue("mix")->load() * 0.01f;
    const float attack = std::exp(-1.0f / (0.001f * attackMs * static_cast<float>(sampleRate)));
    const float release = std::exp(-1.0f / (0.001f * releaseMs * static_cast<float>(sampleRate)));
    const int n = buffer.getNumSamples();
    for (int i = 0; i < n; ++i)
    {
        float detector = 0.0f;
        if (hasSidechain)
            for (int c = 0; c < sc.getNumChannels(); ++c) detector = juce::jmax(detector, std::abs(sc.getSample(c, i)));
        const float coeff = detector > detectorEnvelope ? attack : release;
        detectorEnvelope = coeff * detectorEnvelope + (1.0f - coeff) * detector;
        const float levelDb = juce::Decibels::gainToDecibels(detectorEnvelope, -100.0f);
        const float over = juce::jmax(0.0f, levelDb - threshold);
        const float gainDb = -over * (1.0f - 1.0f / ratio);
        const float targetGain = juce::Decibels::decibelsToGain(gainDb);
        const float gainCoeff = targetGain < smoothedGain ? attack : release;
        smoothedGain = gainCoeff * smoothedGain + (1.0f - gainCoeff) * targetGain;
        for (int c = 0; c < channels; ++c)
        {
            const float dry = buffer.getSample(c, i);
            const float compressed = dry * smoothedGain;
            buffer.setSample(c, i, dry + (compressed - dry) * wet);
        }
    }
}

juce::AudioProcessorEditor* FlowSidechainAudioProcessor::createEditor() { return new FlowSidechainAudioProcessorEditor(*this); }

void FlowSidechainAudioProcessor::getStateInformation(juce::MemoryBlock& dest)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, dest);
}

void FlowSidechainAudioProcessor::setStateInformation(const void* data, int size)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, size));
    if (xml && xml->hasTagName(parameters.state.getType())) parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new FlowSidechainAudioProcessor(); }

