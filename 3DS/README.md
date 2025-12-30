# 3DS Alchemy Metadata Dumper

The 3DS version of the metadata uses this [sample project](https://github.com/Nanquitas/Luma3DS-Plugin-sample) to compile a 3gx file which is then loaded by LumaCFW

## Prerequisites

- `devkitARM`
- `3gxtool`, I used the latest release from https://gitlab.com/thepixellizeross/3gxtool. Install this to the PATH somewhere
- `libctrpf`, can be installed from here https://gitlab.com/thepixellizeross/ctrpluginframework (read the README)

## Building and Running

1. `cd` into the `3DS` folder
2. Run `make TITLE_ID=<title id>`. List of title ids is below
3. Copy the 3gx file to `sd:/luma/plugins/<title id>/MetadataDumper-<title id>.3gx`

## Title IDs

| Game                         | ID on cartridge  | Title ID
|------------------------------|------------------|--------------------
| Skylanders Spyro's Adventure | LNA-CTR-ASPP-EUR | 000400000004C000
