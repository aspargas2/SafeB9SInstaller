
ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_tools

export LDFLAGS := -Tlink.ld -nostartfiles -Wl,--gc-sections,-z,max-page-size=4096

rwildcard = $(foreach d, $(wildcard $1*), \
            $(filter $(subst *, %, $2), $d) \
            $(call rwildcard, $d/, $2))

PROCESSOR := ARM11

TARGET := $(shell basename $(CURDIR))

SOURCE := source
BUILD  := build

SUBARCH := -D$(PROCESSOR) -march=armv6k -mtune=mpcore -marm -mfloat-abi=hard -mfpu=vfpv2 -mtp=soft
INCDIRS := source
INCLUDE := $(foreach dir,$(INCDIRS),-I"$(shell pwd)/$(dir)")

ASFLAGS += $(SUBARCH) $(INCLUDE)
CFLAGS  += $(SUBARCH) $(INCLUDE) -flto -Os
LDFLAGS += $(SUBARCH) -Wl,-Map,$(TARGET).map -flto

OBJECTS := $(patsubst $(SOURCE)/%.S, $(BUILD)/%.o, \
           $(patsubst $(SOURCE)/%.c, $(BUILD)/%.o, \
           $(call rwildcard, $(SOURCE), *.S *.c)))

.PHONY: all
all: $(TARGET).elf

.PHONY: clean
clean:
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).map

$(TARGET).elf: $(OBJECTS) #$(OBJECTS_COMMON)
	@mkdir -p "$(@D)"
	@$(CC) $(LDFLAGS) $^ -o $@

#$(BUILD)/%.cmn.o: $(COMMON_DIR)/%.c
#	@mkdir -p "$(@D)"
#	@echo "[$(PROCESSOR)] $<"
#	@$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/%.o: $(SOURCE)/%.c
	@mkdir -p "$(@D)"
	@echo "[$(PROCESSOR)] $<"
	@$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/%.o: $(SOURCE)/%.S
	@mkdir -p "$(@D)"
	@echo "[$(PROCESSOR)] $<"
	@$(CC) -c $(ASFLAGS) -o $@ $<

include $(call rwildcard, $(BUILD), *.d)