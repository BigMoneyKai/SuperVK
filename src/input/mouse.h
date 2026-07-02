#pragma once

#include "defines.h"
#include <glm/glm.hpp>
enum Button {
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,

    BUTTON_MAX_NUM
};

class Mouse {
public:
    void init();
    void destroy();

    void newFrame();

    void press(Button button);
    void release(Button button);

    void move(f32 x, f32 y);

    void scroll(f32 offset);

    bool isPressed(Button button) const;
    bool isReleased(Button button) const;
    bool isDown(Button button) const;

    SV_FORCE_INLINE f32 wheel() {
        return m_wheel;
    }
    SV_FORCE_INLINE glm::vec2 currPos() {
        return m_currPos;
    }
    SV_FORCE_INLINE glm::vec2 prevPos() {
        return m_prevPos;
    }
    SV_FORCE_INLINE glm::vec2 delta() {
        return m_delta;
    }
    SV_FORCE_INLINE const f32& wheel() const {
        return m_wheel;
    }
    SV_FORCE_INLINE const glm::vec2& currPos() const {
        return m_currPos;
    }
    SV_FORCE_INLINE const glm::vec2& prevPos() const {
        return m_prevPos;
    }
    SV_FORCE_INLINE const glm::vec2& delta() const {
        return m_delta;
    }

private:
    b32 m_currButtons[BUTTON_MAX_NUM];
    b32 m_prevButtons[BUTTON_MAX_NUM];
    f32 m_wheel;

    glm::vec2 m_currPos;
    glm::vec2 m_prevPos;
    glm::vec2 m_delta;
};

