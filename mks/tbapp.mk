# -----------------------------------------------------------------------------
# MAKEFILE project
# -----------------------------------------------------------------------------
# Project targets:
# + t-bone server application is the TEE manager.
# + t-bone application library is TEE Rich client API simulation.
# + t-bone TEE library is the TEE trusted core API simulation.
# -----------------------------------------------------------------------------

PLATFORM ?= linux
LIBNAME := tbapp

# Project folders

INCD := include
SRCD := src

ROOTD ?= build
BUILD ?= $(ROOTD)/$(PLATFORM)
IMPD ?= $(BUILD)/include
OBJD ?= $(BUILD)/obj
LIBD ?= $(BUILD)/lib
BIND ?= $(BUILD)/bin

DIRS := $(IMPD) $(OBJD) $(LIBD) $(BIND)

# Project source path

vpath %.h $(INCD):$(SRCD)
vpath %.hxx $(INCD):$(SRCD)
vpath %.hpp $(INCD):$(SRCD)
vpath %.cpp $(SRCD)
vpath %.c $(SRCD)
vpath %.o $(OBJD)
vpath %.a $(LIBD)
vpath %$(EXE) $(BIND)

# Project source file list

CXXSRC := ca_context ca_entry ca_tee \
					ca_tee_connect_io ca_tee_connect_local ca_tee_connect_tcp ca_tee_connect ca_tee_settings \
					tb_messages tb_messages_string_ack tb_messages_string_req
OBJS := $(CXXSRC:%=%.o)
OBJSD := $(CXXSRC:%=%-g.o)

# Compiler and linker options

CXXINCLUDES := $(INCD)

CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC -std=c++17
ARFLAGS := rcs

# Main label

all: dirs lib$(LIBNAME).a lib$(LIBNAME)d.a

# Project file dependencies

DEPD := $(OBJD)/dep
DEPS := $(CXXSRC:%=$(DEPD)/%.d)

$(DEPD)/%.d: %.cpp | $(DEPD)
	@$(CXX) -MM -MP -MT $(OBJD)/$(basename $(<F)).o -MT $(OBJD)/$(basename $(<F))-g.o $(CXXFLAGS) $(CXXINCLUDES:%=-I %) $< > $@

$(DEPD):
	@mkdir -p $@

# Project files build rules

$(OBJD)/%-g.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -c $< -o $@

$(OBJD)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -DNDEBUG=1 -c $< -o $@

# Project targets build

$(LIBD)/lib$(LIBNAME).a: $(OBJS)
	@echo ==== Building $@ [application library] ====
	$(AR) $(ARFLAGS) $@ $^
	@echo =**= Done =**=

$(LIBD)/lib$(LIBNAME)d.a: $(OBJSD)
	@echo ==== Building $@ [application library] ====
	$(AR) $(ARFLAGS) $@ $^
	@echo =**= Done =**=

# Other project label

dirs: $(DIRS)
$(DIRS):
	@for dir in $(DIRS); do \
	  mkdir -p $$dir; \
	done

clean:
	-@$(RM) $(LIBD)/lib$(LIBNAME).a
	-@$(RM) $(LIBD)/lib$(LIBNAME)d.a
	-@$(RM) $(OBJS:%=$(OBJD)/%)
	-@$(RM) $(OBJSD:%=$(OBJD)/%)
	-@$(RM) $(DEPS)

cleanall:
	@$(RM) -rf $(DEPD)
	@$(RM) -rf $(DIRS)

.PHONY: all dirs clean cleanall

-include $(DEPS)
