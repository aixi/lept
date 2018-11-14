//
// Created by xi on 18-11-9.
//
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <lept/Generator.h>
#include <lept/Parser.h>

using namespace lept;

//BOOST_AUTO_TEST_CASE(testNullType)
//{
//    Parser null_parser("null");
//    bool ret;
//    ret = null_parser.ParseNull();
//    BOOST_CHECK(ret);
//    BOOST_CHECK(null_parser.GetStatus() == Parser::Status::kOK);
//    BOOST_CHECK_EQUAL(null_parser.Index(), 4);
//
//    Parser bad_parser("nual");
//    ret = bad_parser.ParseNull();
//    BOOST_CHECK(!ret);
//    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
//    BOOST_CHECK_EQUAL(bad_parser.Index(), 0);
//}

//BOOST_AUTO_TEST_CASE(testTrueType)
//{
//
//    Parser true_parser("true");
//    bool ret = true_parser.ParseTrue();
//    BOOST_CHECK(ret);
//    BOOST_CHECK(true_parser.GetStatus() == Parser::Status::kOK);
//    BOOST_CHECK_EQUAL(true_parser.Index(), 4);
//    Parser bad_parser("trae");
//    ret = bad_parser.ParseTrue();
//    BOOST_CHECK(!ret);
//    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
//    BOOST_CHECK_EQUAL(bad_parser.Index(), 0);
//
//}
//
//BOOST_AUTO_TEST_CASE(testFalseType)
//{
//    Parser false_parser("false");
//    bool ret = false_parser.ParseFalse();
//    BOOST_CHECK(ret);
//    BOOST_CHECK(false_parser.GetStatus() == Parser::Status::kOK);
//    BOOST_CHECK_EQUAL(false_parser.Index(), 5);
//    Parser bad_parser("fblse");
//    ret = bad_parser.ParseFalse();
//    BOOST_CHECK(!ret);
//    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
//    BOOST_CHECK_EQUAL(bad_parser.Index(), 0);
//}
//

BOOST_AUTO_TEST_CASE(testParseLiteral)
{
    std::vector<std::string> literals = {
            "null", "true", "false"
    };
    bool ret;
    for (const auto& literal : literals)
    {
        Value value;
        Parser null_parser(literal, &value);
        ret = null_parser.ParseLiteral(literal, Value::JsonType::kNull);
        BOOST_CHECK(ret);
        BOOST_CHECK(null_parser.GetStatus() == Parser::Status::kOK);
        BOOST_CHECK_EQUAL(null_parser.Index(), literal.size());
    }

    Value value1;
    Parser bad_null_parser("nual", &value1);
    ret = bad_null_parser.ParseLiteral("null", Value::JsonType::kNull);
    BOOST_CHECK(!ret);
    BOOST_CHECK(bad_null_parser.GetStatus() == Parser::Status::kInvalidValue);
    BOOST_CHECK_EQUAL(bad_null_parser.Index(), 0);

}

BOOST_AUTO_TEST_CASE(testParseNumber)
{
    std::vector<std::string> numbers = {
            "0",
            "-0",
            "-0.0",
            "1",
            "-1",
            "1.5",
            "-1.5",
            "3.1415",
            "1E10",
            "1e10",
            "1E+10",
            "1E-10",
            "-1E10",
            "-1e10",
            "-1E-10",
            "1.234E+10",
            "1.234E-10"
    };
    for (const auto& number : numbers)
    {
        Value number_value;
        Parser number_parser(number, &number_value);
        bool ret = number_parser.ParseNumber();
        BOOST_CHECK(ret);
        BOOST_CHECK(number_parser.GetStatus() == Parser::Status::kOK);
        BOOST_CHECK_CLOSE(strtod(number.c_str(), nullptr), number_parser.Number(), 0.0001);
        BOOST_CHECK_EQUAL(number_parser.Index(), number.size());
    }
}

BOOST_AUTO_TEST_CASE(testParseString)
{
    std::vector<std::string> string_expect = {
            "",
            "Hello",
            "Hello\nWorld",
            "\" \\ / \b \f \n \r \t",
//            "", '\0' need special treatment
             "\x24",
             "\xC2\xA2",
             "\xE2\x82\xAC",
//             "\xF0\x9D\x84\x9E"
    };
    std::vector<std::string> string_jsons = {
            "\"\"",
            "\"Hello\"",
            "\"Hello\\nWorld\"",
            "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"",
//            "\"\\u0000\"",
            "\"\\u0024\"",
            "\"\\u00A2\"",
            "\"\\u20AC\"",
//FIXME:            "\"\\uD834\\uDD1E\"" G clef sign U+1D11E test failed
    };
    bool ret = false;
    for (size_t i = 0; i < string_expect.size(); ++i)
    {
        Value string_value;
        Parser string_parser(string_jsons[i], &string_value);
        ret = string_parser.Parse();
        BOOST_CHECK(ret);
        BOOST_CHECK(string_parser.GetStatus() == Parser::Status::kOK);
        BOOST_CHECK(string_parser.Type() == Value::JsonType::kString);
        BOOST_CHECK_EQUAL(string_parser.String(), string_expect[i]);
        BOOST_CHECK_EQUAL(string_parser.Index(), string_jsons[i].size());
    }

}

BOOST_AUTO_TEST_CASE(testParseArray)
{
    Value value;
    std::string empty_array("[]");
    Parser parser1(empty_array, &value);
    bool ret = parser1.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser1.GetStatus() == Parser::Status::kOK);
    std::vector<Value> expect;
    BOOST_CHECK(expect == value.Array());

    Value value1;
    std::string array("[-1.5 , true, \"hello\"]");
    Parser parser2(array, &value1);
    ret = parser2.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser2.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(value1.Type() == Value::JsonType::kArray);
    std::vector<Value> json_array = value1.Array();
    BOOST_CHECK(json_array[0].Type() == Value::JsonType::kNumber);
    BOOST_CHECK_CLOSE(json_array[0].Number(), -1.5, 0.001);
    BOOST_CHECK(json_array[1].Type() == Value::JsonType::kTrue);
    BOOST_CHECK(json_array[2].Type() == Value::JsonType::kString);
    BOOST_CHECK_EQUAL(json_array[2].String(), "hello");
}

BOOST_AUTO_TEST_CASE(testNestedArray)
{
    Value value2;
    std::string nested_array("[ [],  [0], [ 0 , 1 ], [0, 1, 2]]");
    Parser parser3(nested_array, &value2);
    bool ret = parser3.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser3.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(parser3.Type() == Value::JsonType::kArray);
    const std::vector<Value>& number_array = value2.Array();
    BOOST_CHECK_EQUAL(number_array.size(), 4);
    BOOST_CHECK(value2[0].Type() == Value::JsonType::kArray);
    BOOST_CHECK(value2[1].Type() == Value::JsonType::kArray);
    BOOST_CHECK(value2[1][0].Type() == Value::JsonType::kNumber);
    BOOST_CHECK_CLOSE(value2[1][0].Number(), 0.0, 0.001);
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < i; ++j)
        {
            BOOST_CHECK(value2[i][j].Type() == Value::JsonType::kNumber);
            BOOST_CHECK_CLOSE(value2[i][j].Number(), j, 0.001);
        }
    }
}

BOOST_AUTO_TEST_CASE(testParseObject)
{
    Value value;
    std::string object_json("{}");
    Parser parser(object_json, &value);
    bool ret = parser.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(value.Type() == Value::JsonType::kObject);

    Value value1;
    object_json = "{ \"abc\" : true }";
    Parser parser1(object_json, &value1);
    ret = parser1.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser1.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(value1.Type() == Value::JsonType::kObject);
    Value true_value = value1["abc"];
    BOOST_CHECK(true_value.Type() == Value::JsonType::kTrue);

    Value value2;
    object_json = "{ \"n\" : null , \"a\" : [0, 1, 2] }";
    Parser parser2(object_json, &value2);
    ret = parser2.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser2.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(value2.Type() == Value::JsonType::kObject);
    Value null_value = value2["n"];
    BOOST_CHECK(null_value.Type() == Value::JsonType::kNull);
    Value array_value = value2["a"];
    BOOST_CHECK(array_value.Type() == Value::JsonType::kArray);
    BOOST_CHECK(array_value[0].Type() == Value::JsonType::kNumber);
    BOOST_CHECK_CLOSE(array_value[0].Number(), 0, 0.001);
    BOOST_CHECK(array_value[1].Type() == Value::JsonType::kNumber);
    BOOST_CHECK_CLOSE(array_value[1].Number(), 1, 0.001);
    BOOST_CHECK(array_value[2].Type() == Value::JsonType::kNumber);
    BOOST_CHECK_CLOSE(array_value[2].Number(), 2, 0.001);
}

BOOST_AUTO_TEST_CASE(testParseNestedObject)
{
    Value value;
    std::string nested_object_json("{\"o\" : {\"t\" : true, \"f\" : false, \"n\" : null}}");
    Parser parser(nested_object_json, &value);
    bool ret = parser.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser.GetStatus() == Parser::Status::kOK);
}

BOOST_AUTO_TEST_CASE(testParseValue)
{
    Value value1;
    std::string json("  false  \n");
    Parser parser(json, &value1);
    bool ret = parser.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK_EQUAL(parser.Index(), json.size());

    Value value2;
    std::string bad_json("  \t \r flase \n");
    Parser bad_parser(bad_json, &value2);
    ret = bad_parser.Parse();
    BOOST_CHECK(!ret);
    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
    BOOST_CHECK(bad_parser.Index() != json.size());

    Value value3;
    std::string string_json("\n\t \"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"   \t\n");
    Parser string_json_parser(string_json, &value3);
    ret = string_json_parser.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(string_json_parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(string_json_parser.Type() == Value::JsonType::kString);
    BOOST_CHECK_EQUAL(string_json_parser.String(), "\" \\ / \b \f \n \r \t");
}

BOOST_AUTO_TEST_CASE(testGenerator)
{
    Value value;
    std::string json("{ \"n\" : null , \"a\" : [0, 1, 2] }");
    Parser parser(json, &value);
    bool ret = parser.Parse();
    (void) ret;
    Generator generator;
    generator.GenerateValue(value);
    BOOST_CHECK_EQUAL(generator.GetJson(), "{\"n\":null,\"a\":[0,1,2]}");
}


