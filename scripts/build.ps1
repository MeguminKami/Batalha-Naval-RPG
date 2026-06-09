param(
    [string]$QtDir = $env:QT_DIR,
    [string]$MingwDir = $env:MINGW_DIR,
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$Preset = if ($Configuration -eq "Release") { "windows-release" } else { "windows-debug" }
$BuildFolder = if ($Configuration -eq "Release") { "windows-release" } else { "windows-debug" }

if ([string]::IsNullOrWhiteSpace($QtDir)) {
    throw "QtDir is required. Install Qt 6 MinGW and run: `$env:QT_DIR='C:\Qt\6.6.1\mingw_64'"
}

$QtConfig = Join-Path $QtDir "lib\cmake\Qt6\Qt6Config.cmake"
if (-not (Test-Path -LiteralPath $QtConfig)) {
    throw "Qt was not found at '$QtDir'. Expected: $QtConfig"
}

$DefaultMingwDir = "C:\Qt\Tools\mingw1120_64"
if ([string]::IsNullOrWhiteSpace($MingwDir) -and (Test-Path -LiteralPath (Join-Path $DefaultMingwDir "bin\c++.exe"))) {
    $MingwDir = $DefaultMingwDir
}

$CMakeArgs = @()
if (-not [string]::IsNullOrWhiteSpace($MingwDir)) {
    $CxxCompiler = Join-Path $MingwDir "bin\c++.exe"
    if (-not (Test-Path -LiteralPath $CxxCompiler)) {
        throw "MinGW compiler was not found at '$MingwDir'. Expected: $CxxCompiler"
    }

    $CachePath = Join-Path $ProjectRoot "build\$BuildFolder\CMakeCache.txt"
    if (Test-Path -LiteralPath $CachePath) {
        $CachedCompilerLine = Select-String -LiteralPath $CachePath -Pattern "^CMAKE_CXX_COMPILER:FILEPATH=(.*)$" | Select-Object -First 1
        if ($CachedCompilerLine -and ($CachedCompilerLine.Matches[0].Groups[1].Value -ne ($CxxCompiler -replace "\\", "/"))) {
            throw "CMake cache uses a different compiler. Remove 'build\$BuildFolder' and build again. Cached: $($CachedCompilerLine.Matches[0].Groups[1].Value). Expected: $CxxCompiler"
        }
    }

    $env:MINGW_DIR = $MingwDir
    $env:PATH = (Join-Path $MingwDir "bin") + [IO.Path]::PathSeparator + $env:PATH
    $CMakeArgs += "-DCMAKE_CXX_COMPILER=$CxxCompiler"
}

$env:QT_DIR = $QtDir
$env:PATH = (Join-Path $QtDir "bin") + [IO.Path]::PathSeparator + $env:PATH

Push-Location $ProjectRoot
try {
    cmake --preset $Preset @CMakeArgs
    cmake --build --preset $Preset
}
finally {
    Pop-Location
}
