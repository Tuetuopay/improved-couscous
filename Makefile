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
	   Input/Controller/FPSController.cpp \
	   Models/Model.cpp

ASSIMP_PATH = Thirdparty/assimp
ASSIMP_LIB_PATH = $(ASSIMP_PATH)/lib

GCC_COLOR_MODE = auto
CXXFLAGS = -Wall -O2 -g -std=c++14 -iquote. -MMD -MP \
           -fdiagnostics-color=$(GCC_COLOR_MODE) $(ASSIMP_PATH)/include

LDLIBS =
LDFLAGS = -flto


ifeq ($(OS),Windows_NT)
	CC = i686-w64-mingw32-c++
	CXX = i686-w64-mingw32-c++
	CXXFLAGS += -mwindows -D_USE_MATH_DEFINES

	LDLIBS += -lglew32 -lglfw3 -lopengl32 -lgdi32
	LDFLAGS += -static-libstdc++ -static-libgcc
	ASSIMP_LIB += $(ASSIMP_LIB_PATH)/libassimp.lib

	CXXFLAGS += -DOS_WIN32
else
	CC = c++
	CXX = c++

	UNAME = $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LDLIBS += -lglfw -lGLEW -lm -lGL
		CXXFLAGS += -DOS_LINUX
		ASSIMP_LIB += $(ASSIMP_LIB_PATH)/libassimp.so
	endif
	ifeq ($(UNAME),Darwin)
		LDLIBS += -framework OpenGL -lglfw3 -lglew
		CXXFLAGS += -DOS_OSX
		ASSIMP_LIB += $(ASSIMP_LIB_PATH)/libassimp.dylib
	endif
endif

OUTDIR = out

# Generated variables
# OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.o))
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.PHONY: all run clean

all: $(EXE)
$(EXE): $(OBJS) $(ASSIMP_LIB)
-include $(DEPS)

ASSIMP_JOBS = 8

Thirdparty/assimp:
	git submodule update --init --recursive

$(ASSIMP_LIB): Thirdparty/assimp
	cd Thirdparty/assimp && \
		cmake ASSIMP_BUILD_TESTS=OFF BUILD_SHARED_LIBS=OFF . && \
		make -j$(ASSIMP_JOBS)

run: $(EXE)
	./$(EXE)
clean:
	rm -f $(OBJS) $(DEPS) $(EXE)

