#########################################
# Makefile
#########################################

CXX = g++

SANITIZE := \
	-fsanitize=address \
	-fsanitize=leak \
	-fsanitize=undefined \
	-fsanitize=float-cast-overflow \
	-fno-omit-frame-pointer

CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g -Og \
	-Winit-self -Wold-style-cast -Wcast-qual -Wundef \
	-Woverloaded-virtual \
	-Wconversion \
	-Wlogical-op \
	-Wzero-as-null-pointer-constant \
	$(SANITIZE)

LINK_FLAGS := $(SANITIZE)

EXEC	:= floorplanning
SRCS	:= $(wildcard src/*.cpp)
HDRS	:= $(wildcard src/*.h)
OBJS	:= $(patsubst src/%.cpp, obj/%.o, $(SRCS))
DEPS	:= $(patsubst %.o, %.d, $(OBJS))
PROD_DIR	:= production-build
PROD_OBJS	:= $(patsubst src/%.cpp, $(PROD_DIR)/obj/%.o, $(SRCS))

# Default target
$(EXEC)	:

# Create all
all	: $(EXEC) $(PROD_DIR)/$(EXEC)

# Include existing dependancies
-include $(DEPS)

# Create dependancy files
.SUFFIXES += .d
obj/%.d: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MM -MT '$(patsubst src/%.cpp,obj/%.o,$<)' $< -MF $@

# Compilation
obj/%.o: src/%.cpp obj/%.d
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all object files to executable
$(EXEC)	: $(OBJS)
	$(CXX) $(OBJS) $(LINK_FLAGS) -o $@

## Optimized production version
production: $(PROD_DIR)/$(EXEC)
# Production compilation
$(PROD_DIR)/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -std=c++17 -Wall -Wextra -Wpedantic -DNDEBUG -O4 -c $< -o $@
# Link all object files to production executable
$(PROD_DIR)/$(EXEC)	: $(PROD_OBJS)
	$(CXX) $(PROD_OBJS) $(LINK_FLAGS) -o $@

# Documentation
doc	: $(SRCS) $(HDRS)
	doxygen

# Clean up directory
clean	: 
	rm -f $(EXEC) 
	rm -rf $(PROD_DIR)
	rm -f lib/*.o
	rm -rf obj
	rm -rf doc
	rm -rf latex
	rm -rf html


