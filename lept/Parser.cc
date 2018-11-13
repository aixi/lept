//
// Created by xi on 18-11-9.
//

#include <lept/Parser.h>
#include <lept/Value.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

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
    value_->Type(Value::JsonType::kNull);
    ParseWhiteSpace();
    if (ParseValue())
    {
        ParseWhiteSpace();
        if (json_[index_] != '\0')
        {
            value_->Type(Value::JsonType::kNull);
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

bool Parser::ParseNumber()
{
    size_t old_index = index_;
    if (json_[index_] == '-')
    {
        ++index_;
    }
    if (json_[index_] == '0')
    {
        ++index_;
    }
    else
    {
        if (!isdigit(json_[index_]))
        {
            status_ = Status::kInvalidValue;
            return false;
        }
        for (; isdigit(json_[index_]); ++index_);
    }
    if (json_[index_] == '.')
    {
        ++index_;
        if (!isdigit(json_[index_]))
        {
            status_ = Status::kInvalidValue;
            return false;
        }
        for (; isdigit(json_[index_]); ++index_);
    }
    if (json_[index_] == 'e' || json_[index_] == 'E')
    {
        ++index_;
        if (json_[index_] == '+' || json_[index_] == '-')
        {
            ++index_;
        }
        if (!isdigit(json_[index_]))
        {
            status_ = Status::kInvalidValue;
            return false;
        }
        for (; isdigit(json_[index_]); ++index_);
    }
    errno = 0;
    double n = strtod(json_.data() + old_index, nullptr);
    if (errno == ERANGE && (n == HUGE_VAL || n == -HUGE_VAL))
    {
        status_ = Status::kNumberTooBig;
        return false;
    }
    status_ = Status::kOK;
    value_->Type(Value::JsonType::kNumber);
    value_->Number(n);
    return true;
}

bool Parser::Parse4Hex(unsigned& u)
{
    u = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        char ch = json_[index_++];
        u <<= 4;
        if (ch >= '0' && ch <= '9')
        {
            u |= ch - '0';
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            u |= ch - ('A' - 10);
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            u |= ch - ('a' - 10);
        }
        else
        {
            return false;
        }
    }
    return true;
}

void Parser::EncodeUTF8(unsigned u)
{
    if (u <= 0x7F)
    {
        stack_.push_back(static_cast<char>(u & 0xFF));
    }
    else if (u <= 0x7FF)
    {
        stack_.push_back(static_cast<char>(0xC0 | ((u >> 6) & 0xFF)));
        stack_.push_back(static_cast<char>(0x80 | (u & 0x3F)));
    }
    else if (u <= 0xFFFF)
    {
        stack_.push_back(static_cast<char>(0xE0 | ((u >> 12) & 0xFF)));
        stack_.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3F)));
        stack_.push_back(static_cast<char>(0x80 | (u & 0x3F)));
    }
    else
    {
        assert(u <= 0x10FFFF);
        stack_.push_back(static_cast<char>(0xF0 | ((u >> 18) & 0xFF)));
        stack_.push_back(static_cast<char>(0x80 | ((u >> 12) & 0x3F)));
        stack_.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3F)));
        stack_.push_back(static_cast<char>(0xE0 | (u & 0x3F)));
    }
}

bool Parser::ParseString()
{
    unsigned u = 0, u2 = 0;
    if (json_[index_++] != '\"')
    {
        return false;
    }
    while (true)
    {
        char ch = json_[index_++];
        switch (ch)
        {
            case '\"':
                value_->Type(Value::JsonType::kString);
                value_->String(stack_);
                status_ = Status::kOK;
                return true;
            case '\\':
                switch (json_[index_++])
                {
                    case '\"':
                        stack_.push_back('\"');
                        break;
                    case '\\':
                        stack_.push_back('\\');
                        break;
                    case '/':
                        stack_.push_back('/');
                        break;
                    case 'b':
                        stack_.push_back('\b');
                        break;
                    case 'f':
                        stack_.push_back('\f');
                        break;
                    case 'n':
                        stack_.push_back('\n');
                        break;
                    case 'r':
                        stack_.push_back('\r');
                        break;
                    case 't':
                        stack_.push_back('\t');
                        break;
                    case 'u':
                        if (!Parse4Hex(u))
                        {
                            status_ = Status::kInvalidUnicodeHex;
                            return false;
                        }
                        if (u >= 0xD800 && u <= 0xD8FF)
                        {
                            if (json_[index_++] != '\\')
                            {
                                status_ = Status::kInvalidUnicodeSurrogate;
                                return false;
                            }
                            if (json_[index_++] != 'u')
                            {
                                status_ = Status::kInvalidUnicodeSurrogate;
                                return false;
                            }
                            if (!Parse4Hex(u2))
                            {
                                status_ = Status::kInvalidUnicodeHex;
                                return false;
                            }
                            if (u2 < 0xDC00 || u2 > 0xDFFF)
                            {
                                status_ = Status::kInvalidUnicodeSurrogate;
                                return false;
                            }
                            u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                        }
                        EncodeUTF8(u);
                        break;
                    default:
                        status_ = Status::kInvalidStringEscape;
                        return false;
                }
                break;
            case '\0':
                status_ = Status::kStringMissingQuotationMark;
                return false;
            default:
                if (static_cast<unsigned char>(ch) < 0x20)
                {
                    status_ = Status::kInvalidStringChar;
                    return false;
                }
                stack_.push_back(ch);
        }
    }
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
        case '"':
            return ParseString();
        case '\0':
            status_ = Status::kExpectValue;
            return false;
        default:
            return ParseNumber();
    }
}


} //namespace lept
