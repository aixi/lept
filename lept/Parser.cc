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

bool Parser::ParseNull()
{
    if (json_[index_] != 'n' || json_[index_ + 1] != 'u' ||
        json_[index_ + 2] != 'l' || json_[index_ + 3] != 'l')
    {
        status_ = Status::kInvalidValue;
        return false;
    }
    index_ += 4;
    value_->Type(Value::JsonType::kNull);
    status_ = Status::kOK;
    return true;
}

bool Parser::ParseTrue()
{
    if (json_[index_] != 't' || json_[index_ + 1] != 'r' ||
        json_[index_ + 2] != 'u' || json_[index_ + 3] != 'e')
    {
        status_ = Status::kInvalidValue;
        return false;
    }
    index_ += 4;
    value_->Type(Value::JsonType ::kTrue);
    status_ = Status::kOK;
    return true;
}


bool Parser::ParseFalse()
{
    if (json_[index_] != 'f' || json_[index_ + 1] != 'a' ||
        json_[index_ + 2] != 'l' || json_[index_ + 3] != 's' || json_[index_ + 4] != 'e')
    {
        status_ = Status::kInvalidValue;
        return false;
    }
    index_ += 5;
    value_->Type(Value::JsonType::kFalse);
    status_ = Status::kOK;
    return true;
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
            return ParseTrue();
        case 'f':
            return ParseFalse();
        case 'n':
            return ParseNull();
        case '\0':
            status_ = Status::kExpectValue;
            return false;
        default:
            status_ = Status::kInvalidValue;
            return false;
    }
}


} //namespace lept
