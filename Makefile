CPP := gcc
CXX := g++
CPP_FLAGS := -Wall -Wextra
CXX_FLAGS := -std=c++11 $(CPP_FLAGS)
OPTIMIZE_FLAGS := -O3
LD_LIBS := -lz


INCLUDES = \
	-Isrc \
	-Isrc/SequenceIO \
	-Isrc/SequenceUtils \
	-Isrc/Simulation \
	-Iext \
	-Iext/kseq.h

CPP_FLAGS += $(INCLUDES)
CXX_FLAGS += $(INCLUDES)
CXX_FLAGS += $(OPTIMIZE_FLAGS)


BUILD := build
BIN := bin
SRC := src
APPS := apps

.PHONY: all
all: trim_reads simulate

$(BUILD)/%.o: $(SRC)/%.cpp
	@echo "$(CXX) $(CXX_FLAGS) -c $< -o $@"
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) -c $< -o $@

$(BUILD)/%.o: $(APPS)/%.cpp
	@echo "$(CXX) $(CXX_FLAGS) -c $< -o $@"
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) -c $< -o $@

.PHONY: trim_reads
trim_reads: $(BIN)/trim_reads
$(BIN)/trim_reads: \
	$(BUILD)/app_trim_reads.o \
	$(BUILD)/SequenceIO/Reader.o \
	$(BUILD)/SequenceUtils/trim.o
	@echo "Compiling: $@"
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) $(LD_FLAGS) $^ $(LD_LIBS) -o $@

.PHONY: simulate
trim_reads: $(BIN)/simulate
$(BIN)/simulate: \
	$(BUILD)/app_simulate_reads.o \
	$(BUILD)/SequenceIO/Reader.o \
	$(BUILD)/Simulation/Simulate.o
	@echo "Compiling: $@"
	@mkdir -p $(@D)
	@$(CXX) $(CXX_FLAGS) $(LD_FLAGS) $^ $(LD_LIBS) -o $@
