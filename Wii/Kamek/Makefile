ifeq ($(strip $(ALCHEMY_KAMEK_PATH)),)
	$(error "Please set ALCHEMY_KAMEK_PATH in your environment. export ALCHEMY_KAMEK_PATH=<path>")
endif

ifeq ($(strip $(GAME)),)
	$(error "Please pass GAME when building. GAME=<game>")
endif





CC                  = $(ALCHEMY_KAMEK_PATH)/cw/mwcceppc.exe





#---------------------------------------------------------------------------------
# Sources
#---------------------------------------------------------------------------------
CPPSOURCES           =
# Kamek Loader Sources
CPPSOURCES          += $(ALCHEMY_KAMEK_PATH)/loader/kamekLoader.cpp
# Common Metadata Dumper Sources
CPPSOURCES          += Source/fileio.cpp Source/igMetaField.cpp Source/MetadataDumper.cpp
# Wii Metadata Dumper Sources
CPPSOURCES          += Wii/Kamek/AlchemyKamek.cpp Wii/Kamek/Loaders/$(GAME).cpp





#---------------------------------------------------------------------------------
# Includes
#---------------------------------------------------------------------------------
INCLUDES            =
# Kamek Includes
INCLUDES           += $(ALCHEMY_KAMEK_PATH)/loader $(ALCHEMY_KAMEK_PATH)/k_stdlib
# Common Metadata Dumper Includes
INCLUDES           += Source
# Wii Metadata Dumper Includes
INCLUDES           += Wii/Kamek





#---------------------------------------------------------------------------------
# Compile flags
#---------------------------------------------------------------------------------
CPPFLAGS            = -Cpp_exceptions off -enum int -Os -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0
DEFINES             = -DTARGET_WII=1
INCFLAGS            = $(foreach include,$(INCLUDES), -i $(include))





#---------------------------------------------------------------------------------
# Compile commands
#---------------------------------------------------------------------------------
COMMANDS            = $(foreach source,$(CPPSOURCES),mkdir -p wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(dir $(source))) ; $(CC) $(DEFINES) -I- $(INCFLAGS) $(CPPFLAGS) -o wiibuild/$(shell realpath -m --relative-base=$(ALCHEMY_KAMEK_PATH) $(source:.cpp=.o)) -c $(source) ;)





execute-list:
	$(foreach x,$(COMMANDS),$(x))
