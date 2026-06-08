# Batalha Naval RPG

Batalha Naval RPG is a Qt 6/C++ battleship-style RPG with offline CPU play and a lightweight host/join multiplayer mode.

## Project Layout

- `src/app` - application entry point and main window logic
- `src/game` - board, player, CPU, character, ship, skill, and placement logic
- `src/network` - TCP client/server and packet constants
- `src/audio` - Qt Multimedia sound wrapper
- `src/ui` - Qt Designer UI file
- `assets/images`, `assets/fonts`, `assets/sounds` - Qt resources embedded into the game
- `docs` - game documentation PDF
- `dist/windows` - packaged Windows build zip
- `scripts` - PowerShell helpers for build/run tasks

## Requirements

Install:

- Qt 6 for Windows with the MinGW 64-bit kit
- Qt Multimedia module
- CMake 3.21 or newer
- Ninja

The original project metadata used **Desktop Qt 6.6.1 MinGW 64-bit**, so this is the safest version to install.

After installing Qt, set `QT_DIR` to your Qt kit folder:

```powershell
$env:QT_DIR = "C:\Qt\6.6.1\mingw_64"
```

If your Qt version is different, change only the path, for example:

```powershell
$env:QT_DIR = "C:\Qt\6.8.0\mingw_64"
```

## Build From Source

From the project root:

```powershell
.\scripts\build.ps1 -QtDir $env:QT_DIR
```

Or use CMake directly:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
```

## Run From Source

```powershell
.\scripts\run.ps1 -QtDir $env:QT_DIR
```

To run without rebuilding first:

```powershell
.\scripts\run.ps1 -QtDir $env:QT_DIR -NoBuild
```

## Run The Packaged Windows Build

The repository also includes a packaged build with Qt DLLs:

```powershell
.\scripts\run-packaged.ps1
```

This extracts `dist/windows/bnrpg_online.zip` if needed and starts the packaged executable.

## Qt Creator

1. Open `CMakeLists.txt` in Qt Creator.
2. Select a Qt 6 MinGW 64-bit kit.
3. Configure the project.
4. Build and run the `BatalhaNavalRPG` target.

## Quick Smoke Tests

Offline:

1. Start the game.
2. Choose offline play.
3. Select a character and enter a name.
4. Randomize or place ships.
5. Start the match, use a skill, end the turn, and confirm the CPU takes a turn.

Online on one machine:

1. Start two game instances.
2. In the first instance, create a game and copy the share code.
3. In the second instance, join with that code.
4. Ready both players.
5. Confirm turns pass between windows and hits appear on the other board.
