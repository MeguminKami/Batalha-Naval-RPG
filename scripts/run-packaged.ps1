$ErrorActionPreference = "Stop"
$ProjectRoot = Split-Path -Parent $PSScriptRoot
$ZipPath = Join-Path $ProjectRoot "dist\windows\bnrpg_online.zip"
$ExtractPath = Join-Path $ProjectRoot "dist\windows\bnrpg_online"
$Executable = Join-Path $ExtractPath "bnrpg_online\src.exe"

if (-not (Test-Path -LiteralPath $ZipPath)) {
    throw "Packaged game zip was not found: $ZipPath"
}

if (-not (Test-Path -LiteralPath $Executable)) {
    New-Item -ItemType Directory -Force -Path $ExtractPath | Out-Null
    Expand-Archive -LiteralPath $ZipPath -DestinationPath $ExtractPath -Force
}

if (-not (Test-Path -LiteralPath $Executable)) {
    throw "Packaged executable was not found after extraction: $Executable"
}

& $Executable
