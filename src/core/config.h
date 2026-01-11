#ifndef CPPEL7_CONFIG_H
#define CPPEL7_CONFIG_H

#include <string>

#include "core/types.h"

namespace cppel7 {

struct Config
{
    std::string title {"Cppel7 Game"};
    Size width {24};
    Size height {16};
    Size scale {4};
    bool debug {false};
};

} // namespace cppel7

#endif // CPPEL7_CONFIG_H
