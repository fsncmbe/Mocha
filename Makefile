TARGET_EXEC := Mocha

B_DIR := ./build
S_DIR := ./src

LIBS := -llua54 -lglfw -lgl

$(B_DIR)/$(TARGET_EXEC): $(B_DIR)/glad.o, $(B_DIR)/core.o, $(B_DIR)/utils.o
	g++ $(S_DIR)/mocha.cpp -o $(B_DIR)/$(TARGET_EXEC) -Llibs -Iinclude $(LIBS)

$(B_DIR)/glad.o:
	g++ $(S_DIR)/glad.cpp -o $(B_DIR)/glad.o -Llibs -Iinclude $(LIBS)

$(B_DIR)/core.o:
	g++ $(S_DIR)/core.cpp -o $(B_DIR)/core.o -Llibs -Iinclude $(LIBS)

$(B_DIR)/utils.o:
	g++ $(S_DIR)/utils.cpp -o $(B_DIR)/utils.o -Llibs -Iinclude $(LIBS)