//
// Created by xi on 18-11-9.
//

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

#include <string>
#include <variant>
#include <stddef.h>
#include <assert.h>

namespace lept
{


class Value
{
public:
    enum class JsonType
    {
        kNull,
        kTrue,
        kFalse,
        kNumber,
        kString,
        kArray,
        kObject
    };

    Value();

    void Type(JsonType type)
    {
        type_ = type;
    }

    JsonType Type() const
    {
        return type_;
    }

    double Number() const
    {
        assert(type_ == JsonType::kNumber);
        return std::get<double>(content_);
    }

    void Number(double n)
    {
        assert(type_ == JsonType::kNumber);
        content_ = n;
    }

    Value(const Value&) = delete;
    Value& operator=(const Value) = delete;


private:
    JsonType type_;
    std::variant<double> content_;

}; //class Value

} //namespace lept

#endif //LEPT_VALUE_H
