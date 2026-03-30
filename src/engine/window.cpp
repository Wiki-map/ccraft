#include <iostream>
#include <chrono>

#include "engine/window.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

struct WindowState {
    GLFWwindow* window;
    int16_t width,height;
    struct {
        std::chrono::time_point<std::chrono::system_clock> lastframe;
        float dt;
    }time;

    #define KEYBOARD_SIZE 512
    struct {
        std::array<int8_t,KEYBOARD_SIZE> current;
        std::array<int8_t,KEYBOARD_SIZE> last;
    }keyboard;

    #define MOUSE_BUTTON_SIZE 8
    struct {
        std::array<int8_t,MOUSE_BUTTON_SIZE> current;
        std::array<int8_t,MOUSE_BUTTON_SIZE> last;
        vec2 position;
    }mouse;
};

WindowState state;

void SwapKeyBoardStates() {
    for (int i=0; i<KEYBOARD_SIZE; i++) {
        state.keyboard.last[i] = state.keyboard.current[i];
    }
}

void SwapMouseButtonStates() {
    for (int i=0; i<MOUSE_BUTTON_SIZE; i++) {
        state.mouse.last[i] = state.mouse.current[i];
    }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) state.keyboard.current[key] = 1;
    if (action == GLFW_RELEASE) state.keyboard.current[key] = 0;
}

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    state.mouse.position = {(float)xpos,(float)ypos};
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) state.mouse.current[button] = 1;
    if (action == GLFW_RELEASE) state.mouse.current[button] = 0;
}

void GLAPIENTRY MessageCallback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam ){
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),type, severity, message );
}

void WindowUpdateCallback(GLFWwindow* window, int32_t width, int32_t height) {
    state.width = width;
    state.height = height;
    glViewport(0,0,width,height);
}


void InitWindow(int32_t width,int32_t height,std::string title) {
    
    if (!glfwInit()) {
        std::cerr<<"[ERROR]: failed to init glfw\n";
        exit(-1);
    }

    state.window = glfwCreateWindow(800,800,title.c_str(),NULL,NULL);
    state.width = width;
    state.height = height;
    if (state.window == NULL) {
        fprintf(stderr,"[ERROR]: No window..");
        exit(-1);
    }
    glfwMakeContextCurrent(state.window);

    glfwSwapInterval(1);

    int32_t gladrez = gladLoadGL(glfwGetProcAddress);
    if (gladrez == false) {
        fprintf(stderr,"[ERROR]: Can't load opengl..");
        exit(-1);
    }
    std::cout<<"[INFO]: Loaded opengl, version: "<< GLAD_VERSION_MAJOR(gladrez)<<","<< GLAD_VERSION_MINOR(gladrez)<<"\n";
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(state.window,WindowUpdateCallback);

    glfwSetKeyCallback(state.window,KeyCallback);
    glfwSetCursorPosCallback(state.window,CursorPositionCallback);
    glfwSetMouseButtonCallback(state.window,MouseButtonCallback);

    glDebugMessageCallback(MessageCallback,NULL);

}

void CenterMouse() {
    glfwSetCursorPos(state.window,(double)state.width/2,(double)state.height/2);
}

bool IsWindowClosed() {
    SwapKeyBoardStates();
    SwapMouseButtonStates();

    glfwSwapBuffers(state.window);
    glfwPollEvents();

    {
        std::chrono::time_point<std::chrono::system_clock> T2 = std::chrono::system_clock::now();
        state.time.dt = std::chrono::duration_cast<std::chrono::milliseconds>(T2 - state.time.lastframe).count();
        state.time.lastframe = std::chrono::system_clock::now();
    }

    return glfwWindowShouldClose(state.window);
}

void DeleteWindow() {
    glfwTerminate();
}

float GetDeltaTime() {
    return state.time.dt;
}

vec2 GetWindowDimensions() {
    return {(float)state.width,(float)state.height};
}

int GetWindowHeight() {
    return state.height;
}

int GetWindowWidth() {
    return state.width;
}

float GetWindowAspect() {
    return (float)state.width / (float)state.height;
}

bool IsKeyDown(KeyboardKey key) {
    return state.keyboard.current[(int32_t) key];
}

bool IsKeyPresed(KeyboardKey key) {
    return state.keyboard.current[(int32_t) key] == 1 && state.keyboard.last[(int32_t) key] == 0;
}

bool IsMouseButtonDown(MouseButton button) {
    return state.mouse.current[(int32_t) button];
}

bool IsMouseButtonPresed(MouseButton button) {
    return state.mouse.current[(int32_t) button] == 1 && state.mouse.last[(int32_t) button] == 0;
}

vec2 GetMousePosition() {
    return state.mouse.position;
}