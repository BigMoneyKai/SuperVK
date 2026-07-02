#pragma once
#include "defines.h"

enum Key {
    KEY_W,
    KEY_S,
    KEY_A,
    KEY_D,

    KEY_MAX_NUM
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
    b32 m_current[KEY_MAX_NUM];
    b32 m_previous[KEY_MAX_NUM];
};
