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
					ca_tee_connect_io ca_tee_connect_local ca_tee_connect_tcp ca_tee_connect ca_tee_settings
OBJS := $(CXXSRC:%=%.o)
OBJSD := $(CXXSRC:%=%-d.o)

# Compiler and linker options

CXXINCLUDES := $(INCD)

CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC -std=c++17
ARFLAGS := rcs

# Main label

all: dirs lib$(LIBNAME).a lib$(LIBNAME)d.a

# Project file dependencies

$(OBJD)/ca_context.o $(OBJD)/ca_context-d.o: ca_context.cpp ca_context.hpp ca_tee.hpp tb_ptr_vector.hxx tee_client_api.h
$(OBJD)/ca_entry.o $(OBJD)/ca_entry-d.o: ca_entry.cpp ca_context.hpp ca_tee.hpp tb_errors.hpp tee_client_api.h
$(OBJD)/ca_tee.o $(OBJD)/ca_tee-d.o: ca_tee.cpp ca_tee.hpp ca_tee_connect.hpp ca_tee_settings.hpp tb_errors.hpp
$(OBJD)/ca_tee_connect_io.o $(OBJD)/ca_tee_connect_io-d.o: ca_tee_connect_io.cpp ca_tee_connect.hpp tb_network.hxx
$(OBJD)/ca_tee_connect_local.o $(OBJD)/ca_tee_connect_local-d.o: ca_tee_connect_local.cpp ca_tee_connect.hpp tb_network.hxx
$(OBJD)/ca_tee_connect_tcp.o $(OBJD)/ca_tee_connect_tcp-d.o: ca_tee_connect_tcp.cpp ca_tee_connect.hpp tb_network.hxx
$(OBJD)/ca_tee_connect.o $(OBJD)/ca_tee_connect-d.o: ca_tee_connect.cpp ca_tee_connect.hpp ca_tee_settings.hpp tb_tee_settings.hpp tb_ptr_vector.hxx tb_network.hxx
$(OBJD)/ca_tee_settings.o $(OBJD)/ca_tee_settings-d.o: ca_tee_settings.cpp ca_tee_settings.hpp tb_tee_settings.hpp tb_errors.hpp

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
