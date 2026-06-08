param(
    [string]$QtDir = $env:QT_DIR,
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    [switch]$NoBuild
)

$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$BuildFolder = if ($Configuration -eq "Release") { "windows-release" } else { "windows-debug" }
$Executable = Join-Path $ProjectRoot "build\$BuildFolder\BatalhaNavalRPG.exe"

if (-not $NoBuild) {
    & (Join-Path $PSScriptRoot "build.ps1") -QtDir $QtDir -Configuration $Configuration
}

if ([string]::IsNullOrWhiteSpace($QtDir)) {
    throw "QtDir is required so Qt DLLs can be found at runtime."
}

$env:PATH = (Join-Path $QtDir "bin") + [IO.Path]::PathSeparator + $env:PATH

if (-not (Test-Path -LiteralPath $Executable)) {
    throw "Game executable was not found: $Executable"
}

& $Executable
