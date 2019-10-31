#include "box256machine.h"

box256machine::box256machine(): numThreads(0)
{

}
void box256machine::writeValue(AccessMethod valMethod, AccessMethod toMethod, BOXBYTE wval, BOXBYTE toVal)
{
    BOXBYTE w = getValue(valMethod,wval);
    switch(toMethod) {
        case AccessMethod::CONSTANT: break;//Can't write to a constant!
        case AccessMethod::ABSOLUTE:
        {
            data[toVal]=w;
            break;
        }
        case AccessMethod::POINTER:
        {
            data[data[toVal]]=w;
            break;
        }
    }
}
BOXBYTE box256machine::getValue(AccessMethod valMethod, BOXBYTE getter)
{
    switch(valMethod)
    {
        case AccessMethod::CONSTANT:
        {
        return getter;
        }
        case AccessMethod::ABSOLUTE:
        {
            return data[getter];
        }
        case AccessMethod::POINTER:
        {
            return data[data[getter]];
        }
    }
}
BOXBYTE box256machine::getPC(BOXBYTE threadNum)
{
    return 0xFF - threadNum;
}
void box256machine::createThread()
{
    if(numThreads>=0xFF)return;//Already at max threads.
    numThreads++;
}
void box256machine::step()
{
    for(BOXBYTE t=0;t<numThreads;t++)
    {
        BOXBYTE pc = getPC(t);

        BOXBYTE opcode = getValue(AccessMethod::ABSOLUTE,pc);
        BOXBYTE paramA = getValue(AccessMethod::ABSOLUTE,pc+1);
        BOXBYTE paramB = getValue(AccessMethod::ABSOLUTE,pc+2);
        BOXBYTE paramC = getValue(AccessMethod::ABSOLUTE,pc+3);

        if(opcode==0x00)continue;//No-op
        if(opcode>=0x01 && opcode<=0x12)//MOV
        {
        }
        if(opcode>=0x13 && opcode<=0x1C)//ADD
        {
        }
        if(opcode>=0x1D && opcode<=0x2C)//SUB
        {
        }
        if(opcode>=0x2D && opcode<=0x3B)//JEQ
        {
        }
        if(opcode>=0x3C && opcode<=0x45)//MUL
        {
        }
        if(opcode>=0x46 && opcode<=0x53)//DIV
        {
        }
        if(opcode>=0x54 && opcode<=0x56)//JMP
        {
        }
        if(opcode>=0x57 && opcode<=0x6B)//JGR
        {
        }
        if(opcode>=0x6C && opcode<=0x74)//PIX
        {
        }
        if(opcode>=0x75 && opcode<=0x7D)//FLP
        {
        }
        if(opcode>=0x7E && opcode<=0x80)//THR
        {
        }
        if(opcode>=0x81 && opcode<=0x90)//MOD
        {
        }
        if(opcode>=0x91 && opcode<=0x9F)//JNE
        {
        }
        if(opcode>=0xA0 && opcode<=0xA1)//DIV
        {
        }
        data[pc]++;
    }
}
