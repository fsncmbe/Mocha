TARGET_EXEC := Mocha

B_DIR := build
S_DIR := src
EX_DIR := externals
I_DIR := include src
I_DIR := $(addprefix -I, $(I_DIR))
L_DIR := libs
L_DIR := $(addprefix -L, $(L_DIR))

SRC := $(wildcard $(S_DIR)/*.cpp)
SRC += $(wildcard $(EX_DIR)/*.cpp)


OBJS := $(patsubst $(S_DIR)/%,%,$(SRC))
OBJS := $(patsubst $(EX_DIR)/%,%,$(OBJS))
OBJS := $(patsubst %,build/%,$(OBJS))
OBJS := $(patsubst %.cpp,%.o,$(OBJS))

CXX := g++
CXX_FLAGS := -std=c++20

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBS := -lGL -ldl -llua54 -lglfw
endif
ifeq ($(UNAME), MSYS_NT-10.0-26100)
LIBS := -libglfw3.a -lgdi32 -lopengl32 -llua54.a
endif

all: $(B_DIR)/$(TARGET_EXEC)

# Get .o -> .exe
$(B_DIR)/$(TARGET_EXEC) : $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $^ $(L_DIR) $(LIBS)

# Get .cpp -> .o for src
$(B_DIR)/%.o: $(S_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $(I_DIR) -c $< -o $@

# Get .cpp -> .o for externals
$(B_DIR)/%.o: $(EX_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) $(I_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(B_DIR)/*

.PHONY: run
run: $(B_DIR)/$(TARGET_EXEC)
	./$(B_DIR)/$(TARGET_EXEC)