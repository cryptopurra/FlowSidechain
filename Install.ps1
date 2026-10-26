$ErrorActionPreference = 'Stop'
$bundle = Join-Path $PSScriptRoot 'Flow Sidechain.vst3'
if (-not (Test-Path -LiteralPath $bundle)) {
    throw 'Flow Sidechain.vst3 is missing. Extract the full download before installing.'
}
$destination = Join-Path $env:CommonProgramFiles 'VST3'
$target = Join-Path $destination 'Flow Sidechain.vst3'
$identity = [Security.Principal.WindowsIdentity]::GetCurrent()
$principal = [Security.Principal.WindowsPrincipal]::new($identity)
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    $args = '-NoProfile -ExecutionPolicy Bypass -File "' + $PSCommandPath + '"'
    Start-Process -FilePath 'powershell.exe' -ArgumentList $args -Verb RunAs -Wait
    exit
}
New-Item -ItemType Directory -Path $destination -Force | Out-Null
if (Test-Path -LiteralPath $target) { Remove-Item -LiteralPath $target -Recurse -Force }
Copy-Item -LiteralPath $bundle -Destination $destination -Recurse -Force
Write-Host 'Flow Sidechain is installed. Restart or rescan plug-ins in your DAW.'
Read-Host 'Press Enter to close'

