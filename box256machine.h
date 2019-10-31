#ifndef BOX256MACHINE_H
#define BOX256MACHINE_H

#include "boxtypes.h"
class box256machine
{
private:
    BOXBYTE data[256];
    int numThreads;
public:

    box256machine();

    void writeValue(AccessMethod valMethod, AccessMethod toMethod, BOXBYTE wval, BOXBYTE toVal);
    BOXBYTE getValue(AccessMethod valMethod, BOXBYTE getter);
    BOXBYTE getPC(BOXBYTE threadNum);
    void createThread();

    void step();

    int getNumThreads() const
    {
        return numThreads;
    }
};

#endif // BOX256MACHINE_H
