EXE = c++ube
SRCDIR = src
SRCS = $(SRCDIR)/main.cpp \
       $(SRCDIR)/GFX/GL/Shader.cpp \
       $(SRCDIR)/GFX/GL/FBO.cpp \
       $(SRCDIR)/GFX/GL/VBO.cpp \
       $(SRCDIR)/GFX/GL/Texture.cpp \
       $(SRCDIR)/GFX/TGA.cpp \
       $(SRCDIR)/GFX/TextLabel.cpp \
       $(SRCDIR)/GFX/TextRenderer.cpp \
       $(SRCDIR)/GFX/Window.cpp \
       $(SRCDIR)/Game/GameEngine.cpp \
       $(SRCDIR)/GFX/RenderEngine.cpp \
       $(SRCDIR)/Input/InputManager.cpp \
       $(SRCDIR)/Input/Controller/FPSController.cpp \
       $(SRCDIR)/Input/Controller/TrackballController.cpp \
       $(SRCDIR)/Models/Model.cpp

ASSIMP_PATH = Thirdparty/assimp
ASSIMP_LIB_PATH = $(ASSIMP_PATH)/lib

GCC_COLOR_MODE = auto
CXXFLAGS = -Wall -O2 -g -std=c++14 -iquote$(SRCDIR) -MMD -MP \
           -fdiagnostics-color=$(GCC_COLOR_MODE) -isystem$(ASSIMP_PATH)/include \
           -DGLM_FORCE_RADIANS

LDLIBS =
LDFLAGS = -flto

# Selects the abstraction library used to handle inputs, windowing, ...
# Valid options are glfw3 and sdl2
WINDOW_BACKEND = glfw3
WINDOW_BACKEND_LIB = $(shell pkg-config --libs $(WINDOW_BACKEND) 2> /dev/null)

ifeq ($(OS),Windows_NT)
	CC = i686-w64-mingw32-c++
	CXX = i686-w64-mingw32-c++
	CXXFLAGS += -mwindows -D_USE_MATH_DEFINES

	LDLIBS += -lglew32 $(WINDOW_BACKEND_LIB) -lopengl32 -lgdi32
	LDFLAGS += -static-libstdc++ -static-libgcc
	ASSIMP_LIB += $(ASSIMP_LIB_PATH)/libassimp.lib

	CXXFLAGS += -DOS_WIN32
else
	CC = c++
	CXX = c++

	UNAME = $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LDLIBS += $(WINDOW_BACKEND_LIB) -lGLEW -lm -lGL
		CXXFLAGS += -DOS_LINUX
		ASSIMP_LIB += $(ASSIMP_LIB_PATH)/libassimp.so
	endif
	ifeq ($(UNAME),Darwin)
		LDLIBS += -framework OpenGL $(WINDOW_BACKEND_LIB)
		CXXFLAGS += -DOS_OSX
		ASSIMP_LIB += $(ASSIMP_LIB_PATH)/libassimp.dylib
	endif
endif

CXXFLAGS += -DWINDOW_BACKEND_$(shell echo $(WINDOW_BACKEND) | tr '[:lower:]' '[:upper:]')

OUTDIR = out

# Generated variables
# OBJS = $(addprefix $(OUTDIR)/, $(SRCS:.cpp=.o))
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

.PHONY: all run clean

all: $(EXE)
$(EXE): $(OBJS) $(ASSIMP_LIB)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
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

