# -----------------------------------------------------------------------------
# TEE server application
# -----------------------------------------------------------------------------

PLATFORM ?= linux
APPNAME := tbone
ifneq "$(PLATFORM)" "win"
  EXE := 
else
  EXE := .exe
endif

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

CXXSRC := ts_main
OBJS := $(CXXSRC:%=%.o)
OBJSD := $(CXXSRC:%=%-d.o)

#Project options

CXXINCLUDES := $(INCD)

CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC -std=c++17
LDFLAGS := -fPIC
LIBFLAGS := -lboost_thread -lrt -lm -pthread

# Main label

all: dirs $(APPNAME)$(EXE)

# Project file dependencies

$(OBJD)/ts_main.o $(OBJD)/ts_main-d.o: ts_main.cpp

# Project files build rules

$(OBJD)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -DNDEBUG=1 -c $< -o $@

$(OBJD)/%-d.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -c $< -o $@

# Project targets build

$(BIND)/$(APPNAME)$(EXE): $(OBJS)
	@echo ==== Building $@ [trusted server] ====
	$(CXX) $(LDFLAGS) $^ $(LIBFLAGS) -o $@
	@echo =**= Done =**=

$(BIND)/$(APPNAME)d$(EXE): $(OBJSD)
	@echo ==== Building $@ [trusted server] ====
	$(CXX) $(LDFLAGS) $^ $(LIBFLAGS) -o $@
	@echo =**= Done =**=

# Other project label

dirs: $(DIRS)
$(DIRS):
	@for dir in $(DIRS); do \
	  mkdir -p $$dir/$(PLATFORM); \
	done

clean:
	-@$(RM) $(BIND)/$(APPNAME)$(EXE)
	-@$(RM) $(BIND)/$(APPNAME)d$(EXE)
	-@$(RM) $(OBJS:%=$(OBJD)/%)
	-@$(RM) $(OBJSD:%=$(OBJD)/%)

cleanall:
	@$(RM) -r $(DIRS)

.PHONY: all dirs clean cleanall
