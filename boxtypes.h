#ifndef BOXTYPES_H
#define BOXTYPES_H

#include <cstdint>
typedef uint8_t BOXBYTE;
enum class AccessMethod
{
    CONSTANT, ADDRESS, POINTER, NONE
};

#endif // BOXTYPES_H
