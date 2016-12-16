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

CC = c++
CXX = c++
CXXFLAGS = -Wall -O2 -g -std=c++14 -iquote. -MMD -MP -flto

LDLIBS = -framework GLUT -framework OpenGL -lglfw3 -lglew
LDFLAGS = -flto

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

