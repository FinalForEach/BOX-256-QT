#ifndef BOXTYPES_H
#define BOXTYPES_H

#include <cstdint>
#include <QString>
#include <QtMath>
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

inline QString getHexNum(int i, int numLeadingZeros=0)
{
    QString hexNum = "";
    for(int x=1;x<=numLeadingZeros;x++)
    {
        if(i<qPow(16,x))
        {
            hexNum+="0";
        }
    }
    hexNum+=QString::number(i,16);
    return hexNum.toUpper();
}

#endif // BOXTYPES_H
