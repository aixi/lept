//
// Created by xi on 18-11-9.
//

#include <lept/Value.h>
#include <algorithm>
#include "Value.h"


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
            if (lhs.ObjectSize() != rhs.ObjectSize())
            {
                return false;
            }
            const auto& object_content = lhs.Object();
            for (const auto& pair : object_content)
            {
                if (pair.second != rhs[pair.first])
                {
                    return false;
                }
            }
            return true;
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

void Value::Array(const std::vector<Value>& array)
{
    assert(type_ == JsonType::kArray);
    content_ = array;
}

void Value::ArrayPushBack(const Value& value)
{
    assert(type_ == JsonType::kArray);
    ArrayType& array = std::get<ArrayType>(content_);
    array.push_back(value);
}

size_t Value::ArraySize() const
{
    assert(type_ == JsonType::kArray);
    const auto& content = std::get<ArrayType>(content_);
    return content.size();
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

void Value::Object(const ObjectType& object)
{
    assert(type_ == JsonType::kObject);
    content_ = object;
}

const Value::ObjectType& Value::Object() const
{
    assert(type_ == JsonType::kObject);
    return std::get<ObjectType >(content_);
}

size_t Value::ObjectSize() const
{
    assert(type_ == JsonType::kObject);
    const auto& content = std::get<ObjectType>(content_);
    return content.size();
}

const Value& Value::operator[](const std::string& key) const
{
    assert(type_ == JsonType::kObject);
    const ObjectType& content = std::get<ObjectType>(content_);
    //FIXME: reduce O(n) to O(1)
    auto ret = std::find_if(content.begin(), content.end(), [&key](const auto& it){ return it.first == key;});
    return ret->second;
}




} //lept

