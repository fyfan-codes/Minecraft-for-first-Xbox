# Makefile for BlockCraft: Xbox Legacy (nxdk)
PROJECT := BlockCraft
VERSION := v0.1.0

# Set this to your nxdk root if not exported in environment
NXDK_DIR ?= $(HOME)/nxdk

# toolchain / rules from nxdk
include $(NXDK_DIR)/Makefile.common

SRCDIR := src
OBJD := obj

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(OBJD)/%.o, $(SRCS))

CXXFLAGS += -O2 -fomit-frame-pointer -march=i686 -m32 -D__XBOX__ -DNXDK -DPROJECT_VERSION=\"$(VERSION)\"
CFLAGS += $(CXXFLAGS)

all: $(PROJECT).xbe

$(OBJD)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJD)
	$(CXX) $(CXXFLAGS) -I$(SRCDIR) -I$(NXDK_DIR)/include -c $< -o $@

# link with nxdk
$(PROJECT).xbe: $(OBJS)
	$(LD) $(LDFLAGS) -T $(NXDK_DIR)/ldscripts/xbe.ld -o $@ $^ $(NXDK_DIR)/lib/xboxcrt.a -lc -lgcc

clean:
	rm -rf $(OBJD) $(PROJECT).xbe