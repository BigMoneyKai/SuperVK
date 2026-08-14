#pragma once

//
// Minimal JSON DOM parser — handles the JSON subset needed for glTF 2.0.
// No external dependencies. Self-contained in the gltf/ parser directory.
//

#include <cstdlib>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

namespace gltf_json {

// ---------------------------------------------------------------------------
// JsonType
// ---------------------------------------------------------------------------
enum class JsonType { nil, boolean, number, string, array, object };

// ---------------------------------------------------------------------------
// JsonValue — a node in the JSON DOM tree
// ---------------------------------------------------------------------------
class JsonValue {
public:
    JsonType type = JsonType::nil;

    // scalar storage
    double      numValue  = 0.0;
    bool        boolValue = false;
    std::string strValue;

    // compound storage
    std::vector<JsonValue>                        arrValue;
    std::unordered_map<std::string, JsonValue>    objValue;

    // ---- constructors -------------------------------------------------------
    JsonValue() = default;

    static JsonValue makeNil()    { JsonValue v; v.type = JsonType::nil;    return v; }
    static JsonValue makeBool(bool b)   { JsonValue v; v.type = JsonType::boolean;   v.boolValue = b; return v; }
    static JsonValue makeNumber(double n) { JsonValue v; v.type = JsonType::number; v.numValue = n; return v; }
    static JsonValue makeString(const std::string& s) { JsonValue v; v.type = JsonType::string; v.strValue = s; return v; }
    static JsonValue makeArray()  { JsonValue v; v.type = JsonType::array;  return v; }
    static JsonValue makeObject() { JsonValue v; v.type = JsonType::object; return v; }

    // ---- type queries -------------------------------------------------------
    bool isNil()    const { return type == JsonType::nil; }
    bool isBool()   const { return type == JsonType::boolean; }
    bool isNumber() const { return type == JsonType::number; }
    bool isString() const { return type == JsonType::string; }
    bool isArray()  const { return type == JsonType::array; }
    bool isObject() const { return type == JsonType::object; }

    // ---- value access (caller must ensure correct type) ---------------------
    double      getDouble() const { return numValue; }
    float       getFloat()  const { return static_cast<float>(numValue); }
    int         getInt()    const { return static_cast<int>(numValue); }
    bool        getBool()   const { return boolValue; }
    const std::string& getString() const { return strValue; }

    // ---- array access -------------------------------------------------------
    size_t size() const { return isArray() ? arrValue.size() : (isObject() ? objValue.size() : 0); }

    JsonValue& operator[](size_t idx)             { return arrValue[idx]; }
    const JsonValue& operator[](size_t idx) const { return arrValue[idx]; }

    // ---- object access ------------------------------------------------------
    bool has(const std::string& key) const {
        return isObject() && objValue.find(key) != objValue.end();
    }

    JsonValue& operator[](const std::string& key) {
        return objValue[key];
    }
    const JsonValue& operator[](const std::string& key) const {
        static JsonValue nil = makeNil();
        auto it = objValue.find(key);
        return it != objValue.end() ? it->second : nil;
    }
};

// ---------------------------------------------------------------------------
// Parser
// ---------------------------------------------------------------------------
class Parser {
public:
    JsonValue parse(const std::string& json) {
        p_ = json.c_str();
        end_ = p_ + json.size();
        return parseValue();
    }

private:
    const char* p_   = nullptr;
    const char* end_ = nullptr;

    // ---- helpers ------------------------------------------------------------
    void skipWhitespace() {
        while (p_ < end_ && (*p_ == ' ' || *p_ == '\t' || *p_ == '\n' || *p_ == '\r'))
            ++p_;
    }

    // ---- parse dispatch -----------------------------------------------------
    JsonValue parseValue() {
        skipWhitespace();
        if (p_ >= end_) return JsonValue::makeNil();

        switch (*p_) {
        case '{': return parseObject();
        case '[': return parseArray();
        case '"': return parseString();
        case 't': case 'f': return parseLiteral();
        case 'n': return parseLiteral();
        default:  return parseNumber();
        }
    }

    // ---- object -------------------------------------------------------------
    JsonValue parseObject() {
        JsonValue obj = JsonValue::makeObject();
        ++p_; // skip '{'
        skipWhitespace();

        if (*p_ == '}') { ++p_; return obj; }

        for (;;) {
            skipWhitespace();
            JsonValue key = parseString(); // member name
            skipWhitespace();
            if (*p_ == ':') ++p_;
            JsonValue val = parseValue();
            obj.objValue[key.strValue] = std::move(val);

            skipWhitespace();
            if (*p_ == ',') { ++p_; continue; }
            if (*p_ == '}') { ++p_; break; }
        }
        return obj;
    }

    // ---- array --------------------------------------------------------------
    JsonValue parseArray() {
        JsonValue arr = JsonValue::makeArray();
        ++p_; // skip '['
        skipWhitespace();

        if (*p_ == ']') { ++p_; return arr; }

        for (;;) {
            arr.arrValue.push_back(parseValue());
            skipWhitespace();
            if (*p_ == ',') { ++p_; continue; }
            if (*p_ == ']') { ++p_; break; }
        }
        return arr;
    }

    // ---- string -------------------------------------------------------------
    JsonValue parseString() {
        ++p_; // skip opening '"'
        std::string s;
        while (p_ < end_ && *p_ != '"') {
            if (*p_ == '\\') {
                ++p_;
                if (p_ >= end_) break;
                switch (*p_) {
                case '"':  s += '"';  break;
                case '\\': s += '\\'; break;
                case '/':  s += '/';  break;
                case 'b':  s += '\b'; break;
                case 'f':  s += '\f'; break;
                case 'n':  s += '\n'; break;
                case 'r':  s += '\r'; break;
                case 't':  s += '\t'; break;
                case 'u': {
                    // \uXXXX — crude decode; good enough for basic ASCII
                    ++p_;
                    unsigned codepoint = 0;
                    for (int i = 0; i < 4 && p_ < end_; ++i, ++p_) {
                        codepoint <<= 4;
                        char c = *p_;
                        if      (c >= '0' && c <= '9') codepoint |= (c - '0');
                        else if (c >= 'a' && c <= 'f') codepoint |= (c - 'a' + 10);
                        else if (c >= 'A' && c <= 'F') codepoint |= (c - 'A' + 10);
                    }
                    --p_; // outer loop will ++p_
                    if (codepoint < 0x80) {
                        s += static_cast<char>(codepoint);
                    } else if (codepoint < 0x800) {
                        s += static_cast<char>(0xC0 | (codepoint >> 6));
                        s += static_cast<char>(0x80 | (codepoint & 0x3F));
                    } else {
                        s += static_cast<char>(0xE0 | (codepoint >> 12));
                        s += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
                        s += static_cast<char>(0x80 | (codepoint & 0x3F));
                    }
                    break;
                }
                default: s += *p_; break;
                }
            } else {
                s += *p_;
            }
            ++p_;
        }
        if (*p_ == '"') ++p_;
        return JsonValue::makeString(s);
    }

    // ---- number -------------------------------------------------------------
    JsonValue parseNumber() {
        const char* start = p_;

        // optional minus
        if (*p_ == '-') ++p_;

        // integer part
        while (p_ < end_ && *p_ >= '0' && *p_ <= '9') ++p_;

        // fractional part
        if (p_ < end_ && *p_ == '.') {
            ++p_;
            while (p_ < end_ && *p_ >= '0' && *p_ <= '9') ++p_;
        }

        // exponent part
        if (p_ < end_ && (*p_ == 'e' || *p_ == 'E')) {
            ++p_;
            if (*p_ == '+' || *p_ == '-') ++p_;
            while (p_ < end_ && *p_ >= '0' && *p_ <= '9') ++p_;
        }

        // Use strtod for accurate conversion
        char buf[128];
        size_t len = p_ - start;
        if (len >= sizeof(buf)) len = sizeof(buf) - 1;
        std::memcpy(buf, start, len);
        buf[len] = '\0';

        return JsonValue::makeNumber(std::strtod(buf, nullptr));
    }

    // ---- literal (true / false / null) --------------------------------------
    JsonValue parseLiteral() {
        if (std::strncmp(p_, "true", 4) == 0)  { p_ += 4; return JsonValue::makeBool(true); }
        if (std::strncmp(p_, "false", 5) == 0) { p_ += 5; return JsonValue::makeBool(false); }
        if (std::strncmp(p_, "null", 4) == 0)  { p_ += 4; return JsonValue::makeNil(); }
        return JsonValue::makeNil();
    }
};

// ---------------------------------------------------------------------------
// Convenience free function
// ---------------------------------------------------------------------------
inline JsonValue parse(const std::string& json) {
    Parser p;
    return p.parse(json);
}

} // namespace gltf_json
