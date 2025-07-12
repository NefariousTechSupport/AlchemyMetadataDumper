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
LINKER              = $(ALCHEMY_KAMEK_PATH)/Kamek/bin/Debug/net6.0/Kamek.exe





#---------------------------------------------------------------------------------
# Sources
#---------------------------------------------------------------------------------
# Kamek Loader sources
LOADER_SOURCES       =
# Kamek sources
LOADER_SOURCES      += $(ALCHEMY_KAMEK_PATH)/loader/kamekLoader.cpp
# Game Loader
LOADER_SOURCES      += Wii/Kamek/AlchemyKamek.cpp                             \
                       Wii/Kamek/Loaders/$(GAME).cpp

# Payload sources
PAYLOAD_SOURCES       =
# Common Metadata Dumper Sources
PAYLOAD_SOURCES     += Source/fileio.cpp         \
                       Source/igMetaField.cpp    \
                       Source/MetadataDumper.cpp
# Wii Metadata Dumper Sources
PAYLOAD_SOURCES     += Wii/AlchemyMetadataDumper/Addresses/$(TARGET_GAME).cpp \
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
LOADER_OFILES        = $(foreach source,$(LOADER_SOURCES),  wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(source:.cpp=.o)))
PAYLOAD_OFILES       = $(foreach source,$(PAYLOAD_SOURCES), wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(source:.cpp=.o)))
COMPILE_COMMANDS     = $(foreach source,$(LOADER_SOURCES) $(PAYLOAD_SOURCES), mkdir -p wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(dir $(source))) ; $(CC) $(DEFINES) -I- $(INCFLAGS) $(CPPFLAGS) -o wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(source:.cpp=.o)) -c $(source) ;)
PAYLOAD_LINK_COMMAND = $(LINKER) $(PAYLOAD_OFILES) -dynamic -output-kamek=wiibuild/payload.bin -output-map=wiibuild/payload.map
LOADER_LINK_COMMAND  = $(LINKER) $(LOADER_OFILES)  -static=0x80001900 -output-code=wiibuild/loader.bin -output-riiv=wiibuild/loader.xml -valuefile=Code/loader.bin -output-map=wiibuild/loader.map




execute-list:
	$(foreach x,$(COMPILE_COMMANDS),$(x))
	$(LOADER_LINK_COMMAND)
	$(PAYLOAD_LINK_COMMAND)
