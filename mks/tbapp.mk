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

CXXSRC := ac_context ac_entry ac_tee \
					ac_tee_connect_io ac_tee_connect_local ac_tee_connect_tcp ac_tee_connect ac_tee_settings
OBJS := $(CXXSRC:%=%.o)
OBJSD := $(CXXSRC:%=%-d.o)

# Compiler and linker options

CXXINCLUDES := $(INCD)

CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC -std=c++17
ARFLAGS := rcs

# Main label

all: dirs lib$(LIBNAME).a lib$(LIBNAME)d.a

# Project file dependencies

$(OBJD)/ac_context.o $(OBJD)/ac_context-d.o: ac_context.cpp ac_context.hpp ac_tee.hpp tb_ptr_vector.hxx tee_client_api.h
$(OBJD)/ac_entry.o $(OBJD)/ac_entry-d.o: ac_entry.cpp ac_context.hpp ac_tee.hpp tb_errors.hpp tee_client_api.h
$(OBJD)/ac_tee.o $(OBJD)/ac_tee-d.o: ac_tee.cpp ac_tee.hpp ac_tee_connect.hpp ac_tee_settings.hpp tb_errors.hpp
$(OBJD)/ac_tee_connect_io.o $(OBJD)/ac_tee_connect_io-d.o: ac_tee_connect_io.cpp ac_tee_connect.hpp
$(OBJD)/ac_tee_connect_local.o $(OBJD)/ac_tee_connect_local-d.o: ac_tee_connect_local.cpp ac_tee_connect.hpp
$(OBJD)/ac_tee_connect_tcp.o $(OBJD)/ac_tee_connect_tcp-d.o: ac_tee_connect_tcp.cpp ac_tee_connect.hpp
$(OBJD)/ac_tee_connect.o $(OBJD)/ac_tee_connect-d.o: ac_tee_connect.cpp ac_tee_connect.hpp ac_tee_settings.hpp tb_ptr_vector.hxx
$(OBJD)/ac_tee_settings.o $(OBJD)/ac_tee_settings-d.o: ac_tee_settings.cpp ac_tee_settings.hpp tb_errors.hpp

# Project files build rules

$(OBJD)/%-d.o: %.cpp
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

cleanall:
	@$(RM) -r $(DIRS)

.PHONY: all dirs clean cleanall
