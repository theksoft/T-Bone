# -----------------------------------------------------------------------------
# MAKEFILE project
# -----------------------------------------------------------------------------
# Project targets:
# + t-bone server application is the TEE manager.
# + t-bone application library is TEE Rich client API simulation.
# + t-bone TEE library is the TEE trusted core API simulation.
# -----------------------------------------------------------------------------

PLATFORM ?= linux
SRVTEE := tbone
LIBAPP := libtbapp
LIBTEE := libtbtee
ifneq "$(PLATFORM)" "win"
  EXE := 
else
  EXE := .exe
endif

# Project directory management
# Include header & source directories as well as temporary build directories.

INCD := include
SRCD := source
OBJD := obj
LIBD := lib
BIND := bin

DIRS := $(OBJD) $(LIBD) $(BIND)

vpath %.h $(INCD):$(SRCD)
vpath %.hpp $(INCD):$(SRCD)
vpath %.cpp $(SRCD)
vpath %.c $(SRCD)
vpath %.o $(OBJD)/$(PLATFORM)
vpath %.a $(LIBD)/$(PLATFORM)
vpath %$(EXE) $(BIND)/$(PLATFORM)

# Project source file list

CXXTSSRC := ts-main
CXXACSRC := ac-entry
CTCSRC := tc-mem

OBJSS := $(CXXTSSRC:%=%.o)
OBJSC := $(CXXACSRC:%=%.o)
OBJST := $(CTCSRC:%=%.o)

# Compiler and linker options

CXXINCLUDES := $(INCD)
CINCLUDES := $(INCD)

CFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC
ARFLAGS := rcs
LDFLAGS := -fPIC

# Main label

all: dirs $(LIBAPP).a $(LIBTEE).a $(SRVTEE)$(EXE)

# Project file dependencies

$(OBJD)/$(PLATFORM)/ts-main.o: ts-main.cpp
$(OBJD)/$(PLATFORM)/tc-mem.o: tc-mem.c
$(OBJD)/$(PLATFORM)/ac-entry.o: ac-entry.cpp

# Project files build rules

$(OBJD)/$(PLATFORM)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -c $< -o $@

$(OBJD)/$(PLATFORM)/%.o: %.c
	$(CC) $(CFLAGS) $(CINCLUDES:%=-I %) -c $< -o $@

# Project targets build

$(LIBD)/$(PLATFORM)/$(LIBAPP).a: $(OBJSC)
	@echo ==== Building $@ [application library] ====
	$(AR) $(ARFLAGS) $@ $^
	@echo =**= Done =**=

$(LIBD)/$(PLATFORM)/$(LIBTEE).a: $(OBJST)
	@echo ==== Building $@ [trusted library] ====
	$(AR) $(ARFLAGS) $@ $^
	@echo =**= Done =**=

$(BIND)/$(PLATFORM)/$(SRVTEE)$(EXE): $(OBJSS)
	@echo ==== Building $@ [trusted server] ====
	$(CXX) $(LDFLAGS) $^ -o $@
	@echo =**= Done =**=

# Other project management label

dirs: $(DIRS:%=%/$(PLATFORM))
$(DIRS:%=%/$(PLATFORM)):
	@for dir in $(DIRS); do \
	  mkdir -p $$dir/$(PLATFORM); \
	done

clean:
	@$(RM) $(foreach dir,$(DIRS),$(dir)/$(PLATFORM)/*)

cleanall:
	@$(RM) -r $(DIRS)

.PHONY: all dirs clean cleanall
