#ifndef BOXTYPES_H
#define BOXTYPES_H

#include <cstdint>
#include <QString>
typedef uint8_t BOXBYTE;
enum class AccessMethod
{
    CONSTANT, ADDRESS, POINTER, NONE
};
inline QString getAccessMethodName(AccessMethod a)
{
    switch(a)
    {
    case AccessMethod::CONSTANT: {return "CONSTANT";}
    case AccessMethod::ADDRESS: {return "ADDRESS";}
    case AccessMethod::POINTER: {return "POINTER";}
    case AccessMethod::NONE: {return "NONE";}
    }

}
#endif // BOXTYPES_H
