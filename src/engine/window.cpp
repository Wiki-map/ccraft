#include <iostream>
#include <chrono>

#include "engine/window.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <array>

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
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << "\n";
    std::cout << "Debug message (" << id << "): " <<  message << "\n";

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << "\n";

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << "\n";
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << "\n";
    std::cout << "\n";
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

    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    state.window = glfwCreateWindow(800,800,title.c_str(),NULL,NULL);
    state.width = width;
    state.height = height;
    if (state.window == NULL) {
        std::cerr<<"[ERROR]: No window..";
        exit(-1);
    }
    glfwMakeContextCurrent(state.window);

    glfwSwapInterval(0);

    int32_t gladrez = gladLoadGL(glfwGetProcAddress);
    if (gladrez == false) {
        std::cerr<<"[ERROR]: Can't load opengl..";
        exit(-1);
    }
    std::cout<<"[INFO]: Loaded opengl, version: "<< GLAD_VERSION_MAJOR(gladrez)<<","<< GLAD_VERSION_MINOR(gladrez)<<"\n";

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(state.window,WindowUpdateCallback);

    glfwSetKeyCallback(state.window,KeyCallback);
    glfwSetCursorPosCallback(state.window,CursorPositionCallback);
    glfwSetMouseButtonCallback(state.window,MouseButtonCallback);

    #ifdef ENGINE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(MessageCallback,);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    #endif

    std::cout<<"[INFO]: Enabled callbacks\n";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.FontGlobalScale = 1.5;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(state.window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(1);

    std::cout<<"[INFO]: Enabled ImGui\n";
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

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return glfwWindowShouldClose(state.window);
}

void DeleteWindow() {
    glfwTerminate();
}

float GetDeltaTime() {
    return state.time.dt / 1000.0f;
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

void ImGuiDrawOpengl() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
