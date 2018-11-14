//
// Created by xi on 18-11-9.
//

#include <lept/Value.h>

namespace lept
{

bool operator==(const Value& lhs, const Value& rhs)
{
    if (lhs.type_ != rhs.type_)
    {
        return false;
    }
    switch (lhs.type_)
    {
        case Value::JsonType::kNull:
        case Value::JsonType::kTrue:
        case Value::JsonType::kFalse:
            return true;
        case Value::JsonType::kNumber:
            return lhs.Number() == rhs.Number();
        case Value::JsonType::kString:
            return lhs.String() == rhs.String();
        case Value::JsonType::kArray:
            return lhs.Array() == rhs.Array();
        case Value::JsonType::kObject:
            //TODO
            return false;
    }
}

bool operator!=(const Value& lhs, const Value& rhs)
{
    return !(lhs == rhs);
}

Value::Value() :
        type_(JsonType::kNull)
{}

double Value::Number() const
{
    assert(type_ == JsonType::kNumber);
    return std::get<double>(content_);
}

void Value::Number(double n)
{
    assert(type_ == JsonType::kNumber);
    content_ = n;
}

void Value::Array(const std::vector<Value>& array)
{
    assert(type_ == JsonType::kArray);
    content_ = array;
}

void Value::Object(const ObjectType& object)
{
    assert(type_ == JsonType::kObject);
    content_ = object;
}

void Value::ArrayPushBack(const Value& value)
{
    assert(type_ == JsonType::kArray);
    ArrayType& array = std::get<ArrayType>(content_);
    array.push_back(value);
}

const Value::ArrayType& Value::Array() const
{
    assert(type_ == JsonType::kArray);
    return std::get<ArrayType>(content_);
}

const Value& Value::operator[](size_t index) const
{
    assert(type_ == JsonType::kArray);
    return std::get<ArrayType>(content_)[index];
}

const std::string& Value::String() const
{
    assert(type_ == JsonType::kString);
    const std::string& str = std::get<std::string>(content_);
    return str;
}

void Value::String(const std::string& str)
{
    assert(type_ == JsonType::kString);
    content_ = str;
}


} //lept

