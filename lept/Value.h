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
    
    Value(const Value&) = delete;
    Value& operator=(const Value) = delete;


private:
    JsonType type_;
    std::variant<double, std::vector<char>> content_;
}; //class Value

} //namespace lept

#endif //LEPT_VALUE_H
