param(
    [string]$QtDir = $env:QT_DIR,
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$Preset = if ($Configuration -eq "Release") { "windows-release" } else { "windows-debug" }

if ([string]::IsNullOrWhiteSpace($QtDir)) {
    throw "QtDir is required. Install Qt 6 MinGW and run: `$env:QT_DIR='C:\Qt\6.6.1\mingw_64'"
}

$QtConfig = Join-Path $QtDir "lib\cmake\Qt6\Qt6Config.cmake"
if (-not (Test-Path -LiteralPath $QtConfig)) {
    throw "Qt was not found at '$QtDir'. Expected: $QtConfig"
}

$env:QT_DIR = $QtDir
$env:PATH = (Join-Path $QtDir "bin") + [IO.Path]::PathSeparator + $env:PATH

Push-Location $ProjectRoot
try {
    cmake --preset $Preset
    cmake --build --preset $Preset
}
finally {
    Pop-Location
}
