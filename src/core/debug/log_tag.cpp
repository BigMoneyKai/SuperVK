#include "log_tag.h"

const char *logLevelTags[] = {"[Trace]",
                              COLOR_ORANGE "[Debug]" COLOR_RESET,
                              COLOR_GREEN "[Info]" COLOR_RESET,
                              COLOR_PURPLE "[Warning]" COLOR_RESET,
                              COLOR_RED "[Error]" COLOR_RESET,
                              COLOR_DARK_RED "[Fatal]" COLOR_RESET};

const char *logCatagTags[] = {
  COLOR_GRAY "[Container]" COLOR_RESET, COLOR_GRAY "[Memory]" COLOR_RESET,
  COLOR_GRAY "[Render]" COLOR_RESET,    COLOR_GRAY "[IO]" COLOR_RESET,
  COLOR_GRAY "[Input]" COLOR_RESET,     COLOR_GRAY "[Scene]" COLOR_RESET,
  COLOR_GRAY "[Vulkan]" COLOR_RESET,    COLOR_GRAY "[Asset]" COLOR_RESET,
  COLOR_GRAY "[JobSystem]" COLOR_RESET, COLOR_GRAY "[FileSystem]" COLOR_RESET,
  COLOR_GRAY "[ECS]" COLOR_RESET,       COLOR_GRAY "[Window]" COLOR_RESET,
  COLOR_GRAY "[Platform]" COLOR_RESET,  COLOR_GRAY "[UI]" COLOR_RESET,
  COLOR_GRAY "[Unknown]" COLOR_RESET,
};
