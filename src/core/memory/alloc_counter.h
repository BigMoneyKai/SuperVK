#pragma once

#include "defines.h"

class AllocCounter {
public:
    static SV_FORCE_INLINE u64 add() {
        return ++m_count;
    }
    static SV_FORCE_INLINE u64 sub() {
        return --m_count;
    }
    static SV_FORCE_INLINE u64 count() {
        return m_count;
    }

private:
    static u64 m_count;
};

