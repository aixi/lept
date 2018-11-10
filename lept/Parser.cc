//
// Created by xi on 18-11-9.
//

#include <lept/Parser.h>
#include <lept/Value.h>

#include <assert.h>

namespace lept
{

Parser::Parser(std::string_view json) :
    json_(json),
    status_(Status::kNotInit),
    index_(0),
    value_(std::make_shared<Value>())
{}


bool Parser::Parse()
{
    assert(index_ == 0);
    assert(!json_.empty());
    ParseWhiteSpace();
    if (ParseValue())
    {
        ParseWhiteSpace();
        if (json_[index_] != '\0')
        {
            status_ = Status::kRootNotSingular;
        }
    }
    return status_ == Status::kOK;
}

void Parser::ParseWhiteSpace()
{
    while (json_[index_] == ' ' || json_[index_] == '\n' ||
           json_[index_] == '\r' || json_[index_] == '\t')
    {
        ++index_;
    }
}

bool Parser::ParseLiteral(std::string_view s, Value::JsonType type)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (json_[index_ + i] != s[i])
        {
            status_ = Status::kInvalidValue;
            return false;
        }
    }
    index_ += s.size();
    value_->Type(type);
    status_ = Status::kOK;
    return true;
}

bool Parser::ParseValue()
{
    switch (json_[index_])
    {
        case 't':
            return ParseLiteral("true", Value::JsonType::kTrue);
        case 'f':
            return ParseLiteral("false", Value::JsonType::kFalse);
        case 'n':
            return ParseLiteral("null", Value::JsonType::kNull);
        case '\0':
            status_ = Status::kExpectValue;
            return false;
        default:
            status_ = Status::kInvalidValue;
            return false;
    }
}


} //namespace lept
