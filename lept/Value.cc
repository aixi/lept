//
// Created by xi on 18-11-9.
//

#include <lept/Value.h>
#include "Value.h"


namespace lept
{

Value::Value() :
    type_(JsonType::kNull)
{}

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


} //lept

