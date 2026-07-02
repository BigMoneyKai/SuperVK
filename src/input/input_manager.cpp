#include "input_manager.h"

static InputMan* g_inputMan = nullptr;

static void keyCallback(GLFWwindow* window, i32 key, i32, i32 action, i32) {
    if(!g_inputMan) return;
    
    if(key == GLFW_KEY_W && action != GLFW_RELEASE)
        g_inputMan->getKeyboard().press(KEY_W);
    else if(key == GLFW_KEY_W && action == GLFW_RELEASE)
        g_inputMan->getKeyboard().release(KEY_W);
        
    if(key == GLFW_KEY_S && action != GLFW_RELEASE)
        g_inputMan->getKeyboard().press(KEY_S);
    else if(key == GLFW_KEY_S && action == GLFW_RELEASE)
        g_inputMan->getKeyboard().release(KEY_S);
        
    if(key == GLFW_KEY_A && action != GLFW_RELEASE)
        g_inputMan->getKeyboard().press(KEY_A);
    else if(key == GLFW_KEY_A && action == GLFW_RELEASE)
        g_inputMan->getKeyboard().release(KEY_A);
        
    if(key == GLFW_KEY_D && action != GLFW_RELEASE)
        g_inputMan->getKeyboard().press(KEY_D);
    else if(key == GLFW_KEY_D && action == GLFW_RELEASE)
        g_inputMan->getKeyboard().release(KEY_D);
}

static void buttonCallback(GLFWwindow* window, i32 button, i32 action, i32) {
    if(!g_inputMan) return;
    
    Button btn;
    switch(button) {
        case GLFW_MOUSE_BUTTON_1:
            btn = BUTTON_1;
            break;
        case GLFW_MOUSE_BUTTON_2:
            btn = BUTTON_2;
            break;
        case GLFW_MOUSE_BUTTON_3:
            btn = BUTTON_3;
            break;
        case GLFW_MOUSE_BUTTON_4:
            btn = BUTTON_4;
            break;
        default:
            return;
    }
    
    if(action != GLFW_RELEASE)
        g_inputMan->getMouse().press(btn);
    else
        g_inputMan->getMouse().release(btn);
}

static void cursorPosCallback(GLFWwindow* window, f64 x, f64 y) {
    if(!g_inputMan) return;
    g_inputMan->getMouse().move(static_cast<f32>(x), static_cast<f32>(y));
}

static void scrollCallback(GLFWwindow* window, f64 xoffset, f64 yoffset) {
    if(!g_inputMan) return;
    g_inputMan->getMouse().scroll(static_cast<f32>(yoffset));
}

void InputMan::init(GLFWwindow* window) {
    m_mouse.init();
    m_keyboard.init();
    
    g_inputMan = this;
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void InputMan::destroy() {
    m_mouse.destroy();
    m_keyboard.destroy();
    g_inputMan = nullptr;
}

void InputMan::newFrame() {
    m_mouse.newFrame();
    m_keyboard.newFrame();
}

