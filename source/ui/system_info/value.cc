//
// PROJECT:         Aspia
// FILE:            ui/system_info/value.cc
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#include "base/strings/string_printf.h"
#include "base/logging.h"
#include "ui/system_info/value.h"

namespace aspia {

namespace {

constexpr uint64_t kTB = 1024ULL * 1024ULL * 1024ULL * 1024ULL;
constexpr uint64_t kGB = 1024ULL * 1024ULL * 1024ULL;
constexpr uint64_t kMB = 1024ULL * 1024ULL;
constexpr uint64_t kKB = 1024ULL;

double GetMemorySize(uint64_t size)
{
    uint64_t divider;

    if (size >= kTB)
        divider = kTB;
    else if (size >= kGB)
        divider = kGB;
    else if (size >= kMB)
        divider = kMB;
    else if (size >= kKB)
        divider = kKB;
    else
        divider = 1ULL;

    return static_cast<double>(size) / static_cast<double>(divider);
}

const char* GetMemorySizeUnit(uint64_t size)
{
    if (size >= kTB)
        return "TB";

    if (size >= kGB)
        return "GB";

    if (size >= kMB)
        return "MB";

    if (size >= kKB)
        return "kB";

    return "B";
}

} // namespace

Value::Value(Type type, ValueType&& value, std::string_view unit)
    : value_(std::move(value)),
      type_(type),
      unit_(unit)
{
    // Nothing
}

Value::Value(Type type, ValueType&& value)
    : value_(std::move(value)),
      type_(type)
{
    // Nothing
}

Value::Value(Value&& other)
    : type_(other.type_),
      value_(std::move(other.value_)),
      unit_(std::move(other.unit_))
{
    // Nothing
}

Value& Value::operator=(Value&& other)
{
    type_ = other.type_;
    value_ = std::move(other.value_);
    unit_ = std::move(other.unit_);
    return *this;
}

// static
Value Value::EmptyString()
{
    return Value(Type::STRING, std::string());
}

// static
Value Value::String(const char* value)
{
    DCHECK(value != nullptr);
    return Value(Type::STRING, std::string(value));
}

// static
Value Value::String(const std::string& value)
{
    return Value(Type::STRING, value);
}

// static
Value Value::String(std::string&& value)
{
    return Value(Type::STRING, std::move(value));
}

// static
Value Value::FormattedString(const char* format, ...)
{
    va_list args;

    va_start(args, format);
    std::string out = StringPrintfV(format, args);
    va_end(args);

    return Value(Type::STRING, std::move(out));
}

// static
Value Value::Bool(bool value)
{
    return Value(Type::BOOL, value);
}

// static
Value Value::Number(uint32_t value, std::string_view unit)
{
    return Value(Type::UINT32, value, unit);
}

// static
Value Value::Number(uint32_t value)
{
    return Value(Type::UINT32, value);
}

// static
Value Value::Number(int32_t value, std::string_view unit)
{
    return Value(Type::INT32, value, unit);
}

// static
Value Value::Number(int32_t value)
{
    return Value(Type::INT32, value);
}

// static
Value Value::Number(uint64_t value, std::string_view unit)
{
    return Value(Type::UINT64, value, unit);
}

// static
Value Value::Number(uint64_t value)
{
    return Value(Type::UINT64, value);
}

// static
Value Value::Number(int64_t value, std::string_view unit)
{
    return Value(Type::INT64, value, unit);
}

// static
Value Value::Number(int64_t value)
{
    return Value(Type::INT64, value);
}

// static
Value Value::Number(double value, std::string_view unit)
{
    return Value(Type::DOUBLE, value, unit);
}

// static
Value Value::Number(double value)
{
    return Value(Type::DOUBLE, value);
}

// static
Value Value::MemorySize(uint64_t value)
{
    return Value(Type::MEMORY_SIZE, GetMemorySize(value), GetMemorySizeUnit(value));
}

Value::Type Value::type() const
{
    return type_;
}

std::string Value::ToString() const
{
    switch (type())
    {
        case Type::STRING:
            return std::get<std::string>(value_);

        case Type::BOOL:
            return ToBool() ? "Yes" : "No";

        case Type::UINT32:
            return std::to_string(ToUint32());

        case Type::INT32:
            return std::to_string(ToInt32());

        case Type::UINT64:
            return std::to_string(ToUint64());

        case Type::INT64:
            return std::to_string(ToInt64());

        case Type::DOUBLE:
            return std::to_string(ToDouble());

        case Type::MEMORY_SIZE:
            return StringPrintf("%.2f", ToMemorySize());

        default:
            DLOG(FATAL) << "Unhandled value type: " << static_cast<int>(type());
            return std::string();
    }
}

bool Value::ToBool() const
{
    DCHECK(type() == Type::BOOL);
    return std::get<bool>(value_);
}

uint32_t Value::ToUint32() const
{
    DCHECK(type() == Type::UINT32);
    return std::get<uint32_t>(value_);
}

int32_t Value::ToInt32() const
{
    DCHECK(type() == Type::INT32);
    return std::get<int32_t>(value_);
}

uint64_t Value::ToUint64() const
{
    DCHECK(type() == Type::UINT64);
    return std::get<uint64_t>(value_);
}

int64_t Value::ToInt64() const
{
    DCHECK(type() == Type::INT64);
    return std::get<int64_t>(value_);
}

double Value::ToDouble() const
{
    DCHECK(type() == Type::DOUBLE);
    return std::get<double>(value_);
}

double Value::ToMemorySize() const
{
    DCHECK(type() == Type::MEMORY_SIZE);
    return std::get<double>(value_);
}

const std::string& Value::Unit() const
{
    return unit_;
}

bool Value::HasUnit() const
{
    return !unit_.empty();
}

} // namespace aspia