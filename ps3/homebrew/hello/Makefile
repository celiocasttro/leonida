.SUFFIXES:
ifeq ($(strip $(PSL1GHT)),)
$(error "PSL1GHT must be set in the environment.")
endif

include $(PSL1GHT)/ppu_rules

TARGET := leonida_ps3_poc
BUILD := build
SOURCE := source
INCLUDE := include
DATA := data

TITLE := Leonida PS3 Homebrew PoC
APPID := LNP300001
CONTENTID := UP0001-$(APPID)_00-0000000000000000

CFLAGS += -O2 -Wall -Wextra -std=gnu99
CFLAGS += -I$(PS3DEV)/ppu/include
LIBS += -lio -lsysutil

ifneq ($(BUILD),$(notdir $(CURDIR)))
export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCE),$(CURDIR)/$(dir))
export BUILDDIR := $(CURDIR)/$(BUILD)
export DEPSDIR := $(BUILDDIR)

CFILES := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.c)))
CXXFILES := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES := $(foreach dir,$(SOURCE),$(notdir $(wildcard $(dir)/*.S)))

ifeq ($(strip $(CXXFILES)),)
export LD := $(CC)
else
export LD := $(CXX)
endif

export OFILES := $(CFILES:.c=.o) $(CXXFILES:.cpp=.o) $(SFILES:.S=.o)
export INCLUDES := $(foreach dir,$(INCLUDE),-I$(CURDIR)/$(dir)) -I$(CURDIR)/$(BUILD)

.PHONY: all clean pkg
all: $(BUILD)

$(BUILD):
	@mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -rf $(BUILD) $(OUTPUT).elf $(OUTPUT).self $(OUTPUT).pkg $(OUTPUT).gnpdrm.pkg $(OUTPUT).fake.self

pkg: $(BUILD) $(OUTPUT).pkg

else
DEPENDS := $(OFILES:.o=.d)
$(OUTPUT).self: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)
-include $(DEPENDS)
endif
