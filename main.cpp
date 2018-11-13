#include <iostream>
#include <lept/Parser.h>
#include <chrono>

using namespace lept;
//void test(std::string_view sv)
//{
//    auto t1 = std::chrono::system_clock::now();
//    for (char s : sv)
//    {
//        std::cout << s;
//    }
//    auto t2 = std::chrono::system_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
//    std::cout << static_cast<double>(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den  << std::endl;
//}
//
//void test2(const std::string& str)
//{
//    auto t1 = std::chrono::system_clock::now();
//    for (char s : str)
//    {
//        std::cout << s;
//    }
//    auto t2 = std::chrono::system_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
//    std::cout << static_cast<double>(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den  << std::endl;
//
//}

int main()
{
//    std::string str("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//    std::string_view sv(str);
//    test("abc");
//    test2("abc");
    std::string json("\"\\u0000\"");
    std::string expect;
    expect.push_back('\0');
    Parser parser(json);
    parser.Parse();
    std::cout << (parser.String() == expect);
    return 0;
}