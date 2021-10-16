# -----------------------------------------------------------------------------
# TEE simulator project
# -----------------------------------------------------------------------------
# Project targets:
# + t-bone server application is the TEE manager.
# + t-bone application library is TEE client application API simulation.
# + t-bone TEE library is the TEE trusted core API simulation.
# -----------------------------------------------------------------------------

PLATFORM ?= linux

SRVTEE := tbone
LIBAPP := tbapp
LIBTEE := tbtee

LIBCUW := cuw
CAUNIT := tbcau

ifneq "$(PLATFORM)" "win"
  EXE := 
else
  EXE := .exe
endif

# Project folders
ROOTD ?= build
BUILD ?= $(ROOTD)/$(PLATFORM)
IMPD := $(BUILD)/include
OBJD := $(BUILD)/obj
LIBD := $(BUILD)/lib
BIND := $(BUILD)/bin
MKSD := mks

DIRS := $(IMPD) $(OBJD) $(LIBD) $(BIND)

# Project source path

vpath %.a $(LIBD)
vpath %$(EXE) $(BIND)

# Main label

all: dirs
	$(MAKE) -f $(MKSD)/$(SRVTEE).mk
	$(MAKE) -f $(MKSD)/$(LIBTEE).mk
	$(MAKE) -f $(MKSD)/$(LIBAPP).mk

test: all lib$(LIBCUW).a
	$(MAKE) -f $(MKSD)/$(CAUNIT).mk
	cp $(BIND)/$(CAUNIT).cfg $(BIND)/tbone-client.cfg
	cp $(BIND)/$(CAUNIT)s.cfg $(BIND)/tbone-server.cfg

$(LIBD)/lib$(LIBCUW).a: ../$(LIBCUW)/Makefile
	$(MAKE) -C ../$(LIBCUW) cleanall all install PRFX=$(CURDIR)/$(BUILD)

# Others project labels

dirs: $(DIRS)
$(DIRS):
	@for dir in $(DIRS); do \
	  mkdir -p $$dir; \
	done

clean:
	@$(MAKE) -s -f $(MKSD)/$(SRVTEE).mk clean
	@$(MAKE) -s -f $(MKSD)/$(LIBTEE).mk clean
	@$(MAKE) -s -f $(MKSD)/$(LIBAPP).mk clean
	@$(MAKE) -s -f $(MKSD)/$(CAUNIT).mk clean

cleanall:
	@$(RM) -r $(BUILD)

.PHONY: all dirs clean cleanall
