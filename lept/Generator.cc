//
// Created by xi on 18-11-14.
//

#include <lept/Generator.h>

namespace lept
{

Generator::Generator()
{}

void Generator::GenerateValue(const Value& value)
{
    switch (value.Type())
    {
        case Value::JsonType::kNull:
            json_.append("null");
            break;
        case Value::JsonType::kTrue:
            json_.append("true");
            break;
        case Value::JsonType::kFalse:
            json_.append("false");
            break;
        case Value::JsonType::kNumber:
        {
            char buf[32];
            sprintf(buf, "%.17g", value.Number());
            json_.append(buf);
        }
            break;
        case Value::JsonType::kString:
            json_.append(GenerateString(value.String()));
            break;
        case Value::JsonType::kArray:
        {
            json_.push_back('[');
            size_t array_len = value.ArraySize();
            for (size_t i = 0; i < array_len; ++i)
            {
                GenerateValue(value[i]);
            }
            json_.push_back(']');
        }
            break;
        case Value::JsonType::kObject:
        {
            json_.push_back('{');
            size_t object_len = value.ObjectSize();
            const auto& object_content = value.Object();
            for (size_t i = 0; i < object_len; ++i)
            {
                if (i > 0)
                {
                    json_.push_back(',');
                }
                GenerateString(object_content[i].first);
                json_.push_back(':');
                GenerateValue(object_content[i].second);
            }
            json_.push_back('}');
        }
            break;
    }
}

std::string Generator::GenerateString(const std::string& str)
{
    for (char ch : str)
    {
        switch (ch)
        {
            case '\"':
                json_.append("\\\"");
                break;
            case '\\':
                json_.append("\\\\");
                break;
            case '\b':
                json_.append("\\b");
                break;
            case '\f':
                json_.append("\\f");
                break;
            case '\n':
                json_.append("\\n");
                break;
            case '\r':
                json_.append("\\r");
                break;
            case '\t':
                json_.append("\\t");
                break;
            default:
                if (ch < 0x20)
                {
                    char buf[7] = {0};
                    sprintf(buf, "\\u%04x", ch);
                    json_.append(buf);
                }
                else
                {
                    json_.push_back(ch);
                }
        }
    }
    return str;
}

}
