# Container Rules

- Containers should never use logging, but use "SV_ASSERT" and "SV_STATIC_ASSERT" by including "defines.h".
- To benchmark containers' performance, you should use "benchmark" module in source dir.
