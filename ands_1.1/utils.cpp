#include "utils.h"

// go through each char, shift result left by 1 bit and add the current digit
int binaryToDecimal(const std::string& bin) {
    int result = 0;
    for (char c : bin) {
        result = result * 2 + (c - '0');
    }
    return result;
}