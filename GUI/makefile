EXE=TestApp
IMGUI_PATH=imgui/
BACKEND=backend/
BUILD_PATH=build/
INCLUDE_PATH=include/

SRCS=main.cpp
SRCS+=$(IMGUI_PATH)/imgui.cpp $(IMGUI_PATH)/imgui_demo.cpp $(IMGUI_PATH)/imgui_draw.cpp $(IMGUI_PATH)/imgui_tables.cpp $(IMGUI_PATH)/imgui_widgets.cpp
SRCS+=$(IMGUI_PATH)/file_explorer.cpp $(IMGUI_PATH)/code_area.cpp $(IMGUI_PATH)/syntax_highlighter.cpp
SRCS+=$(BACKEND)/imgui_impl_glfw.cpp $(BACKEND)/imgui_impl_opengl3.cpp
OBJS=$(SRCS:%.cpp=$(BUILD_PATH)/%.o)
LIBS= 

INCLUDES=-I$(INCLUDE_PATH)

CXX=g++
CXXFLAGS=-std=c++17 -I$(IMGUI_PATH) -I$(BACKEND) -Wall -Wformat 
CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
LINUX_GL_LIBS = -lGLESv2

ECHO_MESSAGE = "Linux"
LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

CXXFLAGS += `pkg-config --cflags glfw3`
CFLAGS = $(CXXFLAGS)

all: $(BUILD_PATH)/$(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(BUILD_PATH)/$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BUILD_PATH)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_PATH)/%.o: $(IMGUI_PATH)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_PATH)/%.o: $(BACKEND)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean: 
	rm -f $(OBJS) output
	rm -rf $(BUILD_PATH)
