TARGET_EXEC := Mocha

B_DIR := build
S_DIR := src
I_DIR := include src
I_DIR := $(addprefix -I, $(I_DIR))
L_DIR := libs
L_DIR := $(addprefix -L, $(L_DIR))

SRC := $(wildcard $(S_DIR)/*.cpp)

OBJS := $(patsubst $(S_DIR)/%,%,$(SRC))
OBJS := $(patsubst %,build/%,$(OBJS))
OBJS := $(patsubst %.cpp,%.o,$(OBJS))

CXX := g++

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
LIBS := -lGL -lglfw -ldl -llua54
endif
ifeq ($(UNAME), MSYS_NT-10.0-26100)
LIBS := -libglfw3.a -lgdi32 -lopengl32 -llua54.a
endif

all: $(B_DIR)/$(TARGET_EXEC)

# Get .o -> .exe
$(B_DIR)/$(TARGET_EXEC) : $(OBJS)
	$(CXX) -o $@ $^ $(L_DIR) $(LIBS)

# Get .cpp -> .o
$(B_DIR)/%.o: $(S_DIR)/%.cpp
	$(CXX) $(I_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(B_DIR)/*