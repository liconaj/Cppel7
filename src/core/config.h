#ifndef CPPEL7_CONFIG_H
#define CPPEL7_CONFIG_H

#include <string>

namespace cppel7 {

struct Config
{
    std::string title {"Cppel7 Game"};
    int width {24};
    int height {16};
    int scale {4};
    bool debug {false};
};

} // namespace cppel7

#endif // CPPEL7_CONFIG_H
