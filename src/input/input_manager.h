#pragma once

#include "core/define/compiler.h"
#include "keyboard.h"
#include "mouse.h"

#include <GLFW/glfw3.h>

namespace Input {
class InputMan {
public:
  void init(GLFWwindow *window);
  void destroy();

  void newFrame();

  SV_FORCE_INLINE Keyboard &getKeyboard() { return m_keyboard; }

  SV_FORCE_INLINE Mouse &getMouse() { return m_mouse; }

private:
  Mouse m_mouse;
  Keyboard m_keyboard;
};
} // namespace Input

using InputMan = Input::InputMan;
