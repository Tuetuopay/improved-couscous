EXE = main
SRCS = main.cpp \
       GFX/GL/Shader.cpp \
       GFX/GL/FBO.cpp \
       GFX/GL/VBO.cpp \
	   GFX/GL/Texture.cpp \
	   GFX/TGA.cpp \
	   GFX/TextLabel.cpp \
	   GFX/TextRenderer.cpp \
	   Game/GameEngine.cpp \
	   GFX/RenderEngine.cpp \
	   Input/InputManager.cpp \
	   Input/Controller/FPSController.cpp

CXXFLAGS = -Wall -O2 -g -std=c++14 -iquote. -MMD -MP

LDLIBS =
LDFLAGS = -flto

ifeq ($(OS),Windows_NT)
	CC = i686-w64-mingw32-c++
	CXX = i686-w64-mingw32-c++
	CXXFLAGS += -mwindows

	LDLIBS += -lglew32 -lglfw3 -lopengl32 -lgdi32
	LDFLAGS += -static-libstdc++ -static-libgcc

	CXXFLAGS += -DOS_WIN32
else
	CC = c++
	CXX = c++

	UNAME = $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LDLIBS += -lglfw -lGLEW -lm -lGL
		CXXFLAGS += -DOS_LINUX
	endif
	ifeq ($(UNAME),Darwin)
		LDLIBS += -framework OpenGL -lglfw3 -lglew
		CXXFLAGS += -DOS_OSX
	endif
endif

OUTDIR = out

# Generated variables
# OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.o))
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.PHONY: all run clean

all: $(EXE)
$(EXE): $(OBJS)
-include $(DEPS)

run: $(EXE)
	./$(EXE)
clean:
	rm -f $(OBJS) $(DEPS) $(EXE)

