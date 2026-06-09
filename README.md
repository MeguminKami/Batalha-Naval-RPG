# Batalha Naval RPG

Batalha Naval RPG is a desktop battleship RPG built with C++ and Qt 6. It mixes the classic grid-based naval battle formula with character selection, skills, ship placement, offline CPU play, and a lightweight host/join multiplayer mode.

The project is designed to be built from source with CMake and packaged on Windows as a portable game folder: a directory containing the main `.exe` plus all Qt DLLs and runtime files needed to run the game without an installer.

## Academic Context

This game was created as part of a school curricular unit. The repository is kept public as a portfolio and learning project, showing the source code, build workflow, packaging process, and project documentation.

## Features

- Classic battleship-style board gameplay
- RPG-inspired characters and skills
- Manual and randomized ship placement
- Offline mode against a CPU opponent
- Local host/join multiplayer over TCP
- Qt Widgets interface designed with Qt Designer
- Embedded images, fonts, and sound resources through Qt resource files
- Windows portable release packaging with `windeployqt`

## Tech Stack

- C++17
- Qt 6 Widgets
- Qt Multimedia
- Qt Network
- CMake
- Ninja
- MinGW-compatible C++ compiler on Windows

## Project Structure

```text
Batalha-Naval-RPG/
  assets/
    fonts/          Embedded font resources
    images/         Embedded image resources
    sounds/         Embedded sound resources
  docs/             Extra project documentation
  scripts/          PowerShell build and run helpers
  src/
    app/            Application entry point and main window logic
    audio/          Sound wrapper built on Qt Multimedia
    game/           Board, player, CPU, character, ship, skill, and placement logic
    network/        TCP client/server and protocol constants
    ui/             Qt Designer UI file
  CMakeLists.txt    Main CMake project file
  CMakePresets.json Windows Debug and Release CMake presets
```

## Requirements

Install the following tools before building:

- Qt 6 for Windows with the MinGW 64-bit kit
- Qt Multimedia module
- CMake 3.21 or newer
- Ninja
- The Qt-compatible MinGW 11.2.0 compiler available in `PATH`
- PowerShell

The original project setup uses **Desktop Qt 6.6.1 MinGW 64-bit**, so Qt `6.6.1` is the safest version for this repository.

## Installing Qt 6.6.1 From Terminal

One convenient way to install Qt from PowerShell is with `aqtinstall`.

```powershell
py -3 -m pip install --upgrade pip aqtinstall

py -3 -m aqt install-qt `
  --outputdir C:\Qt `
  windows desktop 6.6.1 win64_mingw `
  -m qtmultimedia

py -3 -m aqt install-tool `
  --outputdir C:\Qt `
  windows desktop tools_mingw90 qt.tools.win64_mingw900
```

After installation, set `QT_DIR` to the Qt kit folder:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
```

To verify that Qt is installed correctly:

```powershell
Test-Path "$env:QT_DIR\lib\cmake\Qt6\Qt6Config.cmake"
Test-Path "$env:QT_DIR\lib\cmake\Qt6Multimedia\Qt6MultimediaConfig.cmake"
Test-Path "$env:QT_DIR\bin\windeployqt.exe"
Test-Path "C:\Qt\Tools\mingw1120_64\bin\g++.exe"
```

Each command should return `True`.

## Building From Source

From the repository root, build the Debug version:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
.\scripts\build.ps1 -QtDir $env:QT_DIR
```

Build the Release version:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
.\scripts\build.ps1 -QtDir $env:QT_DIR -Configuration Release
```

The generated executables are written to:

```text
build/windows-debug/BatalhaNavalRPG.exe
build/windows-release/BatalhaNavalRPG.exe
```

## Running From Source

Run the Debug build:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
.\scripts\run.ps1 -QtDir $env:QT_DIR
```

Run the Release build:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
.\scripts\run.ps1 -QtDir $env:QT_DIR -Configuration Release
```

Run an already-built executable without rebuilding:

```powershell
.\scripts\run.ps1 -QtDir $env:QT_DIR -Configuration Release -NoBuild
```

## Creating A Portable Windows Build

A normal Qt `.exe` usually cannot be distributed alone. It needs Qt DLLs, plugins, multimedia backends, platform files, and compiler runtime DLLs beside it. Qt provides `windeployqt` to create that folder automatically.

First, build the game in Release mode:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
.\scripts\build.ps1 -QtDir $env:QT_DIR -Configuration Release
```

Create the portable folder:

```powershell
New-Item -ItemType Directory -Force .\dist\windows\BatalhaNavalRPG
Copy-Item .\build\windows-release\BatalhaNavalRPG.exe .\dist\windows\BatalhaNavalRPG\ -Force
```

Copy the Qt runtime files into the portable folder:

```powershell
& "$env:QT_DIR\bin\windeployqt.exe" `
  --release `
  --compiler-runtime `
  .\dist\windows\BatalhaNavalRPG\BatalhaNavalRPG.exe
```

The portable folder will look similar to this:

```text
dist/windows/BatalhaNavalRPG/
  BatalhaNavalRPG.exe
  Qt6Core.dll
  Qt6Gui.dll
  Qt6Multimedia.dll
  Qt6Network.dll
  Qt6Widgets.dll
  platforms/
    qwindows.dll
  multimedia/
  imageformats/
  styles/
  translations/
```

To create a zip file that can be shared:

```powershell
Compress-Archive `
  -Path .\dist\windows\BatalhaNavalRPG `
  -DestinationPath .\dist\windows\BatalhaNavalRPG-portable.zip `
  -Force
```

Share `dist/windows/BatalhaNavalRPG-portable.zip` or the full `dist/windows/BatalhaNavalRPG` folder. The player should open `BatalhaNavalRPG.exe` from inside that folder.

## Forcing A Clean Remake

If the game does not open, or if CMake accidentally used the wrong compiler, delete the generated Release output and rebuild it from zero.

This project must be built with the Qt-compatible MinGW compiler:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
$env:PATH = "C:\Qt\Tools\mingw1120_64\bin;$env:QT_DIR\bin;$env:PATH"
```

Then remove only generated folders/files:

```powershell
Remove-Item .\build\windows-release -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item .\dist\windows\BatalhaNavalRPG -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item .\dist\windows\BatalhaNavalRPG-portable.zip -Force -ErrorAction SilentlyContinue
```

Rebuild and redeploy:

```powershell
.\scripts\build.ps1 -QtDir $env:QT_DIR -Configuration Release

New-Item -ItemType Directory -Force .\dist\windows\BatalhaNavalRPG
Copy-Item .\build\windows-release\BatalhaNavalRPG.exe .\dist\windows\BatalhaNavalRPG\ -Force

& "$env:QT_DIR\bin\windeployqt.exe" `
  --release `
  --compiler-runtime `
  .\dist\windows\BatalhaNavalRPG\BatalhaNavalRPG.exe

Compress-Archive `
  -Path .\dist\windows\BatalhaNavalRPG `
  -DestinationPath .\dist\windows\BatalhaNavalRPG-portable.zip `
  -Force
```

To confirm CMake used the right compiler:

```powershell
Select-String "CMAKE_CXX_COMPILER" .\build\windows-release\CMakeCache.txt
```

The output should include:

```text
C:/Qt/Tools/mingw1120_64/bin/c++.exe
```

## Running The Included Packaged Build

If `dist/windows/bnrpg_online.zip` is present, it can be extracted and started with:

```powershell
.\scripts\run-packaged.ps1
```

This helper is only for the packaged zip already stored in `dist/windows`. For a fresh build from source, use the portable build steps above.

## Gameplay Overview

1. Start the game.
2. Choose offline play or create/join an online match.
3. Select a character and enter a player name.
4. Place ships manually or randomize the layout.
5. Start the match.
6. Attack enemy board positions, use character skills, and end turns until one fleet is destroyed.

## Online Play

To test online play on one computer:

1. Start the first game instance.
2. Create a game and copy the share code.
3. Start a second game instance.
4. Join the game with the share code.
5. Ready both players.
6. Play the match and confirm turns pass between both windows.

## Troubleshooting

### `Qt6MultimediaConfig.cmake does NOT exist`

Qt Multimedia is missing. Install Qt again with the multimedia module:

```powershell
py -3 -m aqt install-qt --outputdir C:\Qt windows desktop 6.6.1 win64_mingw -m qtmultimedia
```

### `windeployqt.exe is not recognized`

Check that `QT_DIR` points to the Qt kit folder, not only to `C:\Qt`:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
Test-Path "$env:QT_DIR\bin\windeployqt.exe"
```

### `BatalhaNavalRPG.exe does not exist`

Build the Release target first:

```powershell
.\scripts\build.ps1 -QtDir $env:QT_DIR -Configuration Release
```

The executable should appear at:

```text
build/windows-release/BatalhaNavalRPG.exe
```

### The game opens from the build folder but not from the portable folder

Run `windeployqt` again on the copied executable:

```powershell
& "$env:QT_DIR\bin\windeployqt.exe" --release --compiler-runtime .\dist\windows\BatalhaNavalRPG\BatalhaNavalRPG.exe
```

Make sure the `platforms/qwindows.dll` file exists inside the portable folder.

## Development Notes

- The project uses `CMAKE_AUTOMOC`, `CMAKE_AUTOUIC`, and `CMAKE_AUTORCC`.
- UI layout is stored in `src/ui/mainwindow.ui`.
- Assets are compiled into the executable through Qt resource files.
- The main CMake target is `BatalhaNavalRPG`.
- Windows presets are defined in `CMakePresets.json`.

## License

This project is licensed under the [MIT License](LICENSE).

Third-party tools, libraries, and dependencies keep their own licenses. This includes Qt and any external software used to build or package the game.
