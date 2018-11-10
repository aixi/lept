//
// Created by xi on 18-11-9.
//

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

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
        return num_;
    }

    void Number(double n)
    {
        num_ = n;
    }

    Value(const Value&) = delete;
    Value& operator=(const Value) = delete;


private:
    JsonType type_;
    double num_;

}; //class Value

} //namespace lept

#endif //LEPT_VALUE_H
