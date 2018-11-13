//
// Created by xi on 18-11-9.
//

#ifndef LEPT_PARSER_H
#define LEPT_PARSER_H

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
        kNumberTooBig,
        kStringMissingQuotationMark,
        kInvalidStringChar,
        kInvalidStringEscape,
        kInvalidUnicodeHex,
        kInvalidUnicodeSurrogate
    };

    explicit Parser(std::string_view json);

    bool Parse();

    void ParseWhiteSpace();

    bool ParseValue();

    bool ParseLiteral(std::string_view s, Value::JsonType type);

    bool ParseNumber();

    bool ParseString();

    bool Parse4Hex(unsigned& u);

    void EncodeUTF8(unsigned u);

    Value::JsonType Type() const
    {
        return value_->Type();
    }

    double Number() const
    {
        return value_->Number();
    };

    std::string String() const
    {
        return value_->String();
    }

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

    const std::string json_;
    Status status_;
    size_t index_;
    std::shared_ptr<Value> value_;
    std::vector<char> stack_;

}; //class Value

} // namespace lept


#endif //LEPT_PARSER_H
