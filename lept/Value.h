//
// Created by xi on 18-11-9.
//

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H

#include <string>
#include <variant>
#include <stack>
#include <vector>
#include <stddef.h>
#include <assert.h>

namespace lept
{


class Value
{
    friend bool operator==(const Value& lhs, const Value& rhs);

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

    void Array(const std::vector<Value>& array)
    {
        assert(type_ == JsonType::kArray);
        content_ = array;
    }

    void ArrayPushBack(const Value& value)
    {
        assert(type_ == JsonType::kArray);
        std::vector<Value>& array = std::get<std::vector<Value>>(content_);
        array.push_back(value);
    }

    const std::vector<Value>& Array() const
    {
        assert(type_ == JsonType::kArray);
        return std::get<std::vector<Value>>(content_);
    }

    const Value& operator[](size_t index) const
    {
        assert(type_ == JsonType::kArray);
        return std::get<std::vector<Value>>(content_)[index];
    }

    std::string String() const
    {
        assert(type_ == JsonType::kString);
        const std::vector<char>& char_vec = std::get<std::vector<char>>(content_);
        std::string str(char_vec.begin(), char_vec.end());
        return str;
    }

    void String(const std::vector<char>& str)
    {
        assert(type_ == JsonType::kString);
        content_ = str;
    }
    
    //default copy ctor assign are OK


private:
    JsonType type_;
    std::variant<double, std::vector<char>, std::vector<Value>> content_;
}; //class Value

bool operator==(const Value& lhs, const Value& rhs);

bool operator!=(const Value& lhs, const Value& rhs);

} //namespace lept

#endif //LEPT_VALUE_H
