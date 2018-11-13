//
// Created by xi on 18-11-9.
//
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <iostream>
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
        Parser null_parser(literal);
        ret = null_parser.ParseLiteral(literal, Value::JsonType::kNull);
        BOOST_CHECK(ret);
        BOOST_CHECK(null_parser.GetStatus() == Parser::Status::kOK);
        BOOST_CHECK_EQUAL(null_parser.Index(), literal.size());
    }

    Parser bad_null_parser("nual");
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
        Parser number_parser(number);
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
//            "",
//            "Hello",
//            "Hello\nWorld",
//            "\" \\ / \b \f \n \r \t",
//            "", '\0' need special treatment
             "\x24",
             "\xC2\xA2",
             "\xE2\x82\xAC",
//             "\xF0\x9D\x84\x9E"
    };
    std::vector<std::string> string_jsons = {
//            "\"\"",
//            "\"Hello\"",
//            "\"Hello\\nWorld\"",
//            "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"",
//            "\"\\u0000\"",
            "\"\\u0024\"",
            "\"\\u00A2\"",
            "\"\\u20AC\"",
//            "\"\\uD834\\uDD1E\"" G clef sign U+1D11E test failed
    };
    bool ret = false;
    for (size_t i = 0; i < string_expect.size(); ++i)
    {
        Parser string_parser(string_jsons[i]);
        ret = string_parser.Parse();
        BOOST_CHECK(ret);
        BOOST_CHECK(string_parser.GetStatus() == Parser::Status::kOK);
        BOOST_CHECK(string_parser.Type() == Value::JsonType::kString);
        BOOST_CHECK_EQUAL(string_parser.String(), string_expect[i]);
        BOOST_CHECK_EQUAL(string_parser.Index(), string_jsons[i].size());
    }

}

BOOST_AUTO_TEST_CASE(testParseValue)
{
    std::string json("  false  \n");
    Parser parser(json);
    bool ret = parser.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK_EQUAL(parser.Index(), json.size());

    std::string bad_json("  \t \r flase \n");
    Parser bad_parser(bad_json);
    ret = bad_parser.Parse();
    BOOST_CHECK(!ret);
    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
    BOOST_CHECK(bad_parser.Index() != json.size());

    std::string string_json("\n\t \"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"   \t\n");
    Parser string_json_parser(string_json);
    ret = string_json_parser.Parse();
    BOOST_CHECK(ret);
    BOOST_CHECK(string_json_parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK(string_json_parser.Type() == Value::JsonType::kString);
    BOOST_CHECK_EQUAL(string_json_parser.String(), "\" \\ / \b \f \n \r \t");
}

