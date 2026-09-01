#include "ui/settings.h"

namespace UI {

void Settings::init() {
  m_colorMan.init(); // 默认 dark
  m_fontMan.init(defaultFontInfo);
  m_iconMan.init(defaultIconInfo);
  m_style.init(); // 默认圆角矩形
}

void Settings::reset() {
  m_colorMan.reset();
  m_fontMan.reset();
  m_iconMan.reset();
  m_style.reset();
}

void Settings::apply() {
  m_colorMan.apply();
  m_fontMan.apply();
  m_iconMan.apply();
  m_style.apply();
}

} // namespace UI
