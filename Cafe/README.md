# Cafe Alchemy Metadata Dumper

The Cafe version of the metadata dumper uses [CemuPatchCompiler](https://github.com/Crementif/Cemu-Patch-Compiler) for code injection. 
It produces a `patch_compiled.asm` that is used as a Cemu graphics pack patch.

# Building

The building process isn't great as CemuPatchCompiler does not let us specify preprocessor arguments such as `-D` and `-I`. Because of this limitation, we have to copy files like the following as a first step:

0. Create a new temporary directory which will have copies of all the code files, e.g `compiling`
1. Copy the content of the `Source` directory into the `compiling` directory
2. Copy the content of the `Cafe/AlchemyMetadataDumper` directory into the `compiling` directory
3. Copy the build file for your game located in `Cafe/build`, e.g. `Cafe/build/build_SKYSA_WIIU_01_00_00.cpp`

We can now build it:

4. run `CemuPatchCompiler.exe <build cpp file> ./CafeBuild/ -c <module checksum>`
e.g. `CemuPatchCompiler.exe ./compiling/build_SKYSA_WIIU_01_00_00.cpp ./CafeBuild/ -c 0xC22A1809`
5. Copy `./CafeBuild/patch_compiled.asm` to a new Cemu graphics pack for your game (make a simple `rules.txt` for it).
(Note: I recommend having a command to copy the .asm file to the graphics pack directory to not have to do it manually every time you compile, or just compile it
to the graphics pack directory directly)
6. Enable it on Cemu then run the game

Notes: 
- On Cemu enable these options:
    - `Debug > Logging > Graphic pack patches` as this is useful to tell if the patch loaded fine/what went wrong
    - `Debug > Logging > Coreinit Logging (OSReport/OSConsole)` as the dumps are outputted via `OSConsole`


## Converting the metadata into something usable

Since I couldn't bother to make file apis work, we're instead outputting via `OSConsole` as hexadecimal and
then parsing the Cemu logs to make things readable.

You can run the cafe_logparser.py script with the following:
```
python3 cafe_logparser.py
```

## Notes for adding support to other games

The approach to trigger the dumping with Cafe support is different than the other platforms. On other platforms a thread is created when the game boots, it sleeps for a bit (to wait for the engine to fully setup the reflection) then it starts dumping.

This works but it's complicated for us since we are adding new instructions to the game and not making a proper .dll-like binary that runs its own independent setup code (which is where you would create the thread).

So I've decided to take a different approach: find a function of the game that is known to get called only after the reflection setup by the engine is all done, patch it to branch to our metadata-dumping code.

For example here is what I have done for Skylanders Spyro's Adventure Wii U:
```c++
PATCH_WRITE(0x0205486c, "bl _Z20MetadataDumperThreadv");
```
In this game `0x0205486c` is the address for the `tfbSprite::spriteArtImp::handleMovie` function, which gets called when the game shows the opening cutscenes. This happens first right after the legal text fades out, by that time the reflection was already fully set up, so it's a perfect candidate!

`_Z20MetadataDumperThreadv` is the mangled symbol for the `MetadataDumperThread` function of the metadata dumper.

This approach is very easy to deal with, it may not be as "clean" as making a new thread as the game will crash after `MetadataDumperThread` returns but that's not an issue.