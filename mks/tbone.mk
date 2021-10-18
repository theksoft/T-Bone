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
vpath %.hxx $(INCD):$(SRCD)
vpath %.hpp $(INCD):$(SRCD)
vpath %.cpp $(SRCD)
vpath %.c $(SRCD)
vpath %.o $(OBJD)
vpath %.a $(LIBD)
vpath %$(EXE) $(BIND)

# Project files

CXXSRC := ts_main ts_server_app ts_server_io ts_settings
OBJS := $(CXXSRC:%=%.o)
OBJSD := $(CXXSRC:%=%-g.o)

#Project options

CXXINCLUDES := $(INCD)

CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -fPIC -std=c++17
LDFLAGS := -fPIC
LIBFLAGS := -lconfig++ -lboost_thread -lrt -lm -pthread

# Main label

all: dirs $(APPNAME)$(EXE) $(APPNAME)-d$(EXE)

# Project file dependencies

DEPD := $(OBJD)/dep
DEPS := $(CXXSRC:%=$(DEPD)/%.d)

$(DEPD)/%.d: %.cpp | $(DEPD)
	@$(CXX) -MM -MP -MT $(OBJD)/$(basename $(<F)).o -MT $(OBJD)/$(basename $(<F))-g.o $(CXXFLAGS) $(CXXINCLUDES:%=-I %) $< > $@

$(DEPD):
	@mkdir -p $@

# Project files build rules

$(OBJD)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -DNDEBUG=1 -c $< -o $@

$(OBJD)/%-g.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CXXINCLUDES:%=-I %) -c $< -o $@

# Project targets build

$(BIND)/$(APPNAME)$(EXE): $(OBJS)
	@echo ==== Building $@ [trusted server] ====
	$(CXX) $(LDFLAGS) $^ $(LIBFLAGS) -o $@
	@echo =**= Done =**=

$(BIND)/$(APPNAME)-d$(EXE): $(OBJSD)
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
	-@$(RM) $(DEPS)

cleanall:
	@$(RM) -rf $(DEPD)
	@$(RM) -rf $(DIRS)

.PHONY: all dirs clean cleanall

-include $(DEPS)
