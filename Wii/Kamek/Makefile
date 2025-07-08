ifeq ($(strip $(ALCHEMY_KAMEK_PATH)),)
$(error "Please set ALCHEMY_KAMEK_PATH in your environment. export ALCHEMY_KAMEK_PATH=<path>")
endif

ifeq ($(strip $(GAME)),)
$(error "Please pass GAME when building. GAME=<game>")
endif

ifeq ($(strip $(TARGET_GAME)),)
$(error "Please pass TARGET_GAME when building. TARGET_GAME=<game>")
endif





CC                  = $(ALCHEMY_KAMEK_PATH)/cw/mwcceppc.exe
LINKER              = $(ALCHEMY_KAMEK_PATH)/Kamek/bin/Debug/net6.0/Kamek





#---------------------------------------------------------------------------------
# Sources
#---------------------------------------------------------------------------------
CPPSOURCES           =
# Kamek Loader Sources
CPPSOURCES          += $(ALCHEMY_KAMEK_PATH)/loader/kamekLoader.cpp
# Common Metadata Dumper Sources
CPPSOURCES          += Source/fileio.cpp         \
                       Source/igMetaField.cpp    \
                       Source/MetadataDumper.cpp
# Wii Metadata Dumper Sources
CPPSOURCES          += Wii/Kamek/AlchemyKamek.cpp                             \
                       Wii/Kamek/Loaders/$(GAME).cpp                          \
                       Wii/AlchemyMetadataDumper/Addresses/$(TARGET_GAME).cpp \
                       Wii/AlchemyMetadataDumper/Wiifileio.cpp                \
                       Wii/AlchemyMetadataDumper/WiiCommon.cpp                \
                       Wii/AlchemyMetadataDumper/asleep.cpp





#---------------------------------------------------------------------------------
# Includes
#---------------------------------------------------------------------------------
INCLUDES            =
# Kamek Includes
INCLUDES           += $(ALCHEMY_KAMEK_PATH)/loader $(ALCHEMY_KAMEK_PATH)/k_stdlib
# Common Metadata Dumper Includes
INCLUDES           += Source
# Wii Metadata Dumper Includes
INCLUDES           += Wii/Kamek Wii/AlchemyMetadataDumper





#---------------------------------------------------------------------------------
# Compile flags
#---------------------------------------------------------------------------------
CPPFLAGS            = -Cpp_exceptions off -RTTI off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0 -msgstyle gcc
DEFINES             = -DTARGET_WII=1 -DTARGET_GAME=$(TARGET_GAME)
INCFLAGS            = $(foreach include,$(INCLUDES), -i $(include))





#---------------------------------------------------------------------------------
# Compile commands
#---------------------------------------------------------------------------------
OFILES              = $(foreach source,$(CPPSOURCES), wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(source:.cpp=.o)))
COMPILE_COMMANDS    = $(foreach source,$(CPPSOURCES),mkdir -p wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(dir $(source))) ; $(CC) $(DEFINES) -I- $(INCFLAGS) $(CPPFLAGS) -o wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(source:.cpp=.o)) -c $(source) ;)
LINK_COMMAND        = $(LINKER) $(OFILES) -static=0x80001000 -output-code=wiibuild/loader.bin -output-riiv=wiibuild/loader.xml -valuefile=Code/loader.bin -output-map=wiibuild/file.md.map




execute-list:
	$(foreach x,$(COMPILE_COMMANDS),$(x))
	$(LINK_COMMAND)
