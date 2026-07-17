#pragma once

#include "core/container/string/string.hpp"
#include "core/io/stream/istream.h"
#include "core/io/stream/ostream.h"
#include <type_traits>

template<typename T>
requires std::is_trivially_copyable_v<T>
void serialize(OStream& os, const T& val);
void serialize(OStream& os, const String& val);

template<typename T>
requires std::is_trivially_copyable_v<T>
void deserialize(IStream& is, T& val);
void deserialize(IStream& is, String& val);

#include "serializer.inl"
