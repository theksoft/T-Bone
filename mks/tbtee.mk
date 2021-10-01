# -----------------------------------------------------------------------------
# TEE trusted services library
# -----------------------------------------------------------------------------

PLATFORM ?= linux
LIBNAME := tbtee

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
vpath %.hpp $(INCD):$(SRCD)
vpath %.cpp $(SRCD)
vpath %.c $(SRCD)
vpath %.o $(OBJD)
vpath %.a $(LIBD)
vpath %$(EXE) $(BIND)

# Project files

CSRC := tc_mem
OBJS := $(CSRC:%=%.o)
OBJSD := $(CSRC:%=%-d.o)

# Compiler and linker options

CINCLUDES := $(INCD)

CFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC
ARFLAGS := rcs

# Main label

all: dirs lib$(LIBNAME).a lib$(LIBNAME)d.a

# Project file dependencies

$(OBJD)/tc_mem.o $(OBJD)/tc_mem-d.o: tc_mem.c

# Project files build rules

$(OBJD)/%.o: %.c
	$(CC) $(CFLAGS) $(CINCLUDES:%=-I %) -DNDEBUG=1 -c $< -o $@

$(OBJD)/%-d.o: %.c
	$(CC) $(CFLAGS) $(CINCLUDES:%=-I %) -c $< -o $@

# Project targets build

$(LIBD)/lib$(LIBNAME).a: $(OBJS)
	@echo ==== Building $@ [trusted library] ====
	$(AR) $(ARFLAGS) $@ $^
	@echo =**= Done =**=

$(LIBD)/lib$(LIBNAME)d.a: $(OBJSD)
	@echo ==== Building $@ [trusted library] ====
	$(AR) $(ARFLAGS) $@ $^
	@echo =**= Done =**=

# Other project labels

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
