//
// Created by xi on 18-11-9.
//
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <lept/Parser.h>

using namespace lept;

BOOST_AUTO_TEST_CASE(testNullType)
{
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
}

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
}

