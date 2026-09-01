#include "keyboard.h"

#include <cstring>

namespace Input {
void Keyboard::init() {
    memset(m_current, SV_FALSE, sizeof(m_current));
    memset(m_previous, SV_FALSE, sizeof(m_previous));
}

void Keyboard::destroy() {
    memset(m_current, SV_FALSE, sizeof(m_current));
    memset(m_previous, SV_FALSE, sizeof(m_previous));
}

void Keyboard::press(Key key) {
    m_current[static_cast<u32>(key)] = SV_TRUE;
}

void Keyboard::release(Key key) {
    m_current[static_cast<u32>(key)] = SV_FALSE;
}

b32 Keyboard::isPressed(Key key) const {
    return m_current[static_cast<u32>(key)] && !m_previous[static_cast<u32>(key)];
}

b32 Keyboard::isReleased(Key key) const {
    return !m_current[static_cast<u32>(key)] && m_previous[static_cast<u32>(key)];
}

b32 Keyboard::isDown(Key key) const {
    return m_current[static_cast<u32>(key)];
}

b32 Keyboard::isUp(Key key) const {
    return m_previous[static_cast<u32>(key)];
}

void Keyboard::newFrame() {
    memcpy(m_previous, m_current, sizeof(m_current));
}
} // namespace Input
