#include "input_manager.h"
#include "GLFW/glfw3.h"

namespace Input {
static InputMan *g_inputMan = nullptr;

static void keyCallback(GLFWwindow *window, i32 key, i32, i32 action, i32) {
  if (!g_inputMan)
    return;

  if (key == GLFW_KEY_W && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keyW);
  else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keyW);

  if (key == GLFW_KEY_S && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keyS);
  else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keyS);

  if (key == GLFW_KEY_A && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keyA);
  else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keyA);

  if (key == GLFW_KEY_D && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keyD);
  else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keyD);

  if (key == GLFW_KEY_SPACE && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keySpace);
  else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keySpace);

  if (key == GLFW_KEY_LEFT_SHIFT && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keyLShift);
  else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keyLShift);

  if (key == GLFW_KEY_ESCAPE && action != GLFW_RELEASE)
    g_inputMan->getKeyboard().press(Key::keyEsc);
  else if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    g_inputMan->getKeyboard().release(Key::keyEsc);
}

static void buttonCallback(GLFWwindow *window, i32 button, i32 action, i32) {
  if (!g_inputMan)
    return;

  Button btn;
  switch (button) {
  case GLFW_MOUSE_BUTTON_1:
    btn = Button::button1;
    break;
  case GLFW_MOUSE_BUTTON_2:
    btn = Button::button2;
    break;
  case GLFW_MOUSE_BUTTON_3:
    btn = Button::button3;
    break;
  case GLFW_MOUSE_BUTTON_4:
    btn = Button::button4;
    break;
  default:
    return;
  }

  if (action != GLFW_RELEASE)
    g_inputMan->getMouse().press(btn);
  else
    g_inputMan->getMouse().release(btn);
}

static void cursorPosCallback(GLFWwindow *window, f64 x, f64 y) {
  if (!g_inputMan)
    return;

  g_inputMan->getMouse().move(static_cast<f32>(x), static_cast<f32>(y));
}

static void scrollCallback(GLFWwindow *window, f64 xoffset, f64 yoffset) {
  if (!g_inputMan)
    return;
  g_inputMan->getMouse().scroll(static_cast<f32>(yoffset));
}

void InputMan::init(GLFWwindow *window) {
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
} // namespace Input
