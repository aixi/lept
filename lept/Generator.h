//
// Created by xi on 18-11-14.
//

#ifndef LEPT_GENERATOR_H
#define LEPT_GENERATOR_H

#include <lept/Value.h>

namespace lept
{

class Generator
{
public:
    Generator();

    std::string GenerateString(const std::string& str);

    void GenerateValue(const Value& value);

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

private:
    std::string json_;
}; //Generator

} //namespace lept

#endif //LEPT_GENERATOR_H
