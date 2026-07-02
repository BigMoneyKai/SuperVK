#include "mouse.h"
#include "defines.h"

void Mouse::init() {
    memset(m_currButtons, SV_FALSE, BUTTON_MAX_NUM);
    memset(m_prevButtons, SV_FALSE, BUTTON_MAX_NUM);
    m_wheel = 0.0f;
    m_currPos = {0.0f, 0.0f};
    m_prevPos = {0.0f, 0.0f};
    m_delta = {0.0f, 0.0f};
}

void Mouse::destroy() {
    memset(m_currButtons, SV_FALSE, BUTTON_MAX_NUM);
    memset(m_prevButtons, SV_FALSE, BUTTON_MAX_NUM);
    m_wheel = 0.0f;
    m_currPos = {0.0f, 0.0f};
    m_prevPos = {0.0f, 0.0f};
    m_delta = {0.0f, 0.0f};
}

void Mouse::newFrame() {
    memcpy(m_prevButtons, m_currButtons, sizeof(m_currButtons));
    m_delta = m_currPos - m_prevPos;
    m_prevPos = m_currPos;
    m_wheel = 0.0f;
}

void Mouse::press(Button button) {
    m_currButtons[button] = SV_TRUE;
}

void Mouse::release(Button button) {
    m_currButtons[button] = SV_FALSE;
}

void Mouse::move(f32 x, f32 y) {
    m_currPos = {x, y};
}

void Mouse::scroll(f32 offset) {
    m_wheel += offset;
}

bool Mouse::isPressed(Button button) const {
    return !m_prevButtons[button] && m_currButtons[button];
}

bool Mouse::isReleased(Button button) const {
    return m_prevButtons[button] && !m_currButtons[button];
}

bool Mouse::isDown(Button button) const {
    return m_currButtons[button];
}
