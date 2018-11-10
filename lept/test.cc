//
// Created by xi on 18-11-9.
//
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <lept/Parser.h>

using namespace lept;

BOOST_AUTO_TEST_CASE(testNullType)
{
    Parser null_parser("null");
    bool ret;
    ret = null_parser.ParseNull();
    BOOST_CHECK(ret);
    BOOST_CHECK(null_parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK_EQUAL(null_parser.Index(), 4);

    Parser bad_parser("nual");
    ret = bad_parser.ParseNull();
    BOOST_CHECK(!ret);
    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
    BOOST_CHECK_EQUAL(bad_parser.Index(), 0);
}

BOOST_AUTO_TEST_CASE(testTrueType)
{

    Parser true_parser("true");
    bool ret = true_parser.ParseTrue();
    BOOST_CHECK(ret);
    BOOST_CHECK(true_parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK_EQUAL(true_parser.Index(), 4);
    Parser bad_parser("trae");
    ret = bad_parser.ParseTrue();
    BOOST_CHECK(!ret);
    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
    BOOST_CHECK_EQUAL(bad_parser.Index(), 0);

}

BOOST_AUTO_TEST_CASE(testFalseType)
{
    Parser false_parser("false");
    bool ret = false_parser.ParseFalse();
    BOOST_CHECK(ret);
    BOOST_CHECK(false_parser.GetStatus() == Parser::Status::kOK);
    BOOST_CHECK_EQUAL(false_parser.Index(), 5);
    Parser bad_parser("fblse");
    ret = bad_parser.ParseFalse();
    BOOST_CHECK(!ret);
    BOOST_CHECK(bad_parser.GetStatus() == Parser::Status::kInvalidValue);
    BOOST_CHECK_EQUAL(bad_parser.Index(), 0);
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
