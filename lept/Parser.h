//
// Created by xi on 18-11-9.
//

#ifndef LEPT_PARSER_H
#define LEPT_PARSER_H

#include <string_view>
#include <memory>

#include <lept/Value.h>

namespace lept
{

class Value;

class Parser
{
public:
    enum class Status
    {
        kNotInit,
        kOK,
        kExpectValue,
        kInvalidValue,
        kRootNotSingular,
        kNumberTooBig
    };

    explicit Parser(std::string_view json);

    bool Parse();

    void ParseWhiteSpace();

    bool ParseValue();

    bool ParseLiteral(std::string_view s, Value::JsonType type);

    bool ParseNumber();

    Value::JsonType Type() const
    {
        return value_->Type();
    }

    double Number() const
    {
        return value_->Number();
    };

    Status GetStatus() const 
    {
        return status_;
    }
    
    size_t Index() const
    {
        return index_;
    }

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

private:

    const std::string_view json_;
    Status status_;
    size_t index_;
    std::shared_ptr<Value> value_;

}; //class Value

} // namespace lept


#endif //LEPT_PARSER_H
