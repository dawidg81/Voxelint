CXX      := g++
TARGET   := voxelint
SRC_DIR  := src
OBJ_DIR  := obj

SRCS     := $(wildcard $(SRC_DIR)/*.cpp)
OBJS     := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

CXXFLAGS := -Wall -std=c++17

# ── OS detection ─────────────────────────────────────────────────────────────
ifeq ($(OS), Windows_NT)
    TARGET   := $(TARGET).exe
    SDL_INC  := D:/MinGW/include
    SDL_LIB  := D:/MinGW/lib
    LIBS     := -lmingw32 -lSDL2main -lSDL2
    CXXFLAGS += -I$(SDL_INC)
    LDFLAGS  := -L$(SDL_LIB)
    MKDIR    := if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    RM       := del /Q
else
    LIBS     := -lSDL2main -lSDL2
    CXXFLAGS += $(shell sdl2-config --cflags)
    LDFLAGS  := $(shell sdl2-config --libs)
    MKDIR    := mkdir -p $(OBJ_DIR)
    RM       := rm -f
endif

# ── Rules ─────────────────────────────────────────────────────────────────────
.PHONY: all clean

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	$(MKDIR)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)/*.o $(TARGET)
