#pragma once

#include "core/define/types.h"

enum class Key {
  keyW,
  keyS,
  keyA,
  keyD,

  keyMaxNum
};

class Keyboard {
public:
  void init();
  void destroy();

  void press(Key key);
  void release(Key key);

  b32 isPressed(Key key) const;
  b32 isReleased(Key key) const;
  b32 isDown(Key key) const;
  b32 isUp(Key key) const;

  void newFrame();

private:
  b32 m_current[static_cast<u32>(Key::keyMaxNum)];
  b32 m_previous[static_cast<u32>(Key::keyMaxNum)];
};
