#include <string>

struct monostate {};

class VariantValue {
public:
    enum Type { STRING, FLOAT, BOOL, MONO } type;
    std::string s;
    float f;
    bool b;

    VariantValue() : type(MONO), f(0), b(false) {}
    VariantValue(monostate) : type(MONO), f(0), b(false) {}
    VariantValue(float val) : type(FLOAT), f(val), b(false) {}
    VariantValue(bool val) : type(BOOL), f(0), b(val) {}
    VariantValue(const std::string& val) : type(STRING), s(val), f(0), b(false) {}
    VariantValue(const char* val) : type(STRING), s(val), f(0), b(false) {}
};

template<typename T> inline bool holds_alternative(const VariantValue& v);
template<> inline bool holds_alternative<float>(const VariantValue& v) { return v.type == VariantValue::FLOAT; }
template<> inline bool holds_alternative<bool>(const VariantValue& v) { return v.type == VariantValue::BOOL; }
template<> inline bool holds_alternative<std::string>(const VariantValue& v) { return v.type == VariantValue::STRING; }
template<> inline bool holds_alternative<monostate>(const VariantValue& v) { return v.type == VariantValue::MONO; }

template<typename T> inline T get(const VariantValue& v);
template<> inline float get<float>(const VariantValue& v) { return v.f; }
template<> inline bool get<bool>(const VariantValue& v) { return v.b; }
template<> inline std::string get<std::string>(const VariantValue& v) { return v.s; }
