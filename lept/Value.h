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
    using ArrayType = std::vector<Value>;
    using ObjectType = std::vector<std::pair<std::string, Value>>;

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

    double Number() const;

    void Number(double n);

    void Array(const std::vector<Value>& array);

    void Object(const ObjectType& object);

    void ArrayPushBack(const Value& value);

    const ArrayType& Array() const;

    const Value& operator[](size_t index) const;

    const std::string& String() const;

    void String(const std::string& str);
    
    //default copy constructor/assignment are OK
    
    
private:
    JsonType type_;
    std::variant<double, std::string, ArrayType, ObjectType> content_;
    
}; //class Value

bool operator==(const Value& lhs, const Value& rhs);

bool operator!=(const Value& lhs, const Value& rhs);

} //namespace lept

#endif //LEPT_VALUE_H
