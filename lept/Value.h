//
// Created by xi on 18-11-9.
//

#ifndef LEPT_VALUE_H
#define LEPT_VALUE_H


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

    Value(const Value&) = delete;
    Value& operator=(const Value) = delete;


private:
    JsonType type_;

}; //class Value

} //namespace lept

#endif //LEPT_VALUE_H
