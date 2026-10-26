# Flow Sidechain

A Windows VST3 external-key compressor. It is an original implementation with a compact control panel, sidechain detection, stereo-linked gain reduction, and five host-automatable controls.

## Controls

- **Threshold**: detector level where compression starts, from -60 to 0 dB.
- **Ratio**: compression ratio, from 1:1 to 20:1.
- **Attack** and **Release**: detector and gain-smoothing times.
- **Mix**: blend between the uncompressed and compressed signal.

Route a kick, percussion, or other key signal to the plug-in's **Sidechain** input in the DAW. Without a sidechain signal the plug-in passes the input through unchanged.

## Install on Windows

Download the `FlowSidechain-VST3-Windows` artifact from the latest successful GitHub Actions run, extract it, then double-click `Install.bat`. Windows may ask for administrator approval because VST3 plug-ins are installed in the shared Steinberg folder. Restart or rescan plug-ins in the DAW.

## Build

Requires CMake 3.22+, Visual Studio 2022 with the C++ desktop workload, and network access on the first configure. JUCE 8.0.8 is fetched by CMake.

```powershell
cmake -S . -B build -A x64
cmake --build build --config Release --target FlowSidechain_VST3
```

The repository's GitHub Actions workflow builds the Windows VST3 and packages it for installation on every push and pull request.

## License

Project source: MIT. JUCE is fetched separately and remains under its own license terms. This plug-in does not include code or assets from the RONE Sidechain plug-in.

