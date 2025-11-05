# Wii Alchemy Metadata Dumper

The wii version of the metadata dumper uses [Kamek](https://github.com/Treeki/Kamek) for code injection.

# Builing

0. You must assign the `ALCHEMY_KAMEK_PATH` environment variable to the path to your checkout of Kamek
1. cd into the root repository directory
2. run `make -f Wii/Kamek/Makefile GAME=<game> TARGET_GAME=<target game>` where `<game>` is an acronym for the game of interest and `<target game>` is the game + version.
e.g. `make -f Wii/Kamek/Makefile GAME=SSA TARGET_GAME=SKYSA_01_00_03`.
3. Cry at how this process is not ideal and is a bit confusing
4. profit, once you figure out how to make money off of this

## Converting the metadata into something usable

Since I couldn't bother to make file apis work, we're instead outputting via `OSReport` as hexadecimal and
then parsing the dolphin logs to make things readable. This requires some configuration changes and sadly
means that this isn't fit for a real Wii

You must set the following options in `%AppData%/Dolphin Emulator/Config/Logger.ini`:
- `OSREPORT_HLE = True`: Means that the print statements will actually be reported
- `WriteToFile = True`: Means they get written out to the log file

You can then run the logparser.py script with the following:
```
python3 logparser.py
```