#pragma once

#include "core/define/types.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include <variant>

struct KeyEvent {
  Key key;
  b32 pressed;
};

struct MouseButtonEvent {
  Button button;
  b32 pressed;
};

struct MouseMoveEvent {
  f32 x;
  f32 y;
};

struct WindowResizeEvent {
  u32 height;
  u32 width;
};

using Event =
  std::variant<KeyEvent, MouseButtonEvent, MouseMoveEvent, WindowResizeEvent>;

void handleEvent(const Event& event);