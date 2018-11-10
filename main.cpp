#include <iostream>

#include <lept/Parser.h>

using namespace lept;

int main()
{
    Parser null("null");
    bool ret = null.Parse();
    std::cout << ret;
    return 0;
}