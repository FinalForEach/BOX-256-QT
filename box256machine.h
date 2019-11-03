#ifndef BOX256MACHINE_H
#define BOX256MACHINE_H

#include <QMap>

#include "boxtypes.h"
class Box256Instruction;
class Box256Machine
{
private:
    BOXBYTE data[256];
    BOXBYTE pixels[256];
    int numThreads;
    QMap<BOXBYTE,Box256Instruction*> instMap;
    int curCycle;
public:

    Box256Machine();

    void writeValue(BOXBYTE wval, BOXBYTE addr);
    void writePixel(BOXBYTE wval, BOXBYTE pixAddr);
    BOXBYTE getPixel(BOXBYTE pixAddr) const;
    BOXBYTE getValue(AccessMethod valMethod, BOXBYTE getter) const;
    BOXBYTE getPC(BOXBYTE threadNum) const;
    void createThread();

    void step();
    void reset();

    int getNumThreads() const
    {
        return numThreads;
    }
    BOXBYTE getOpcodeFromCommand(QString name, AccessMethod accessA,
                                 AccessMethod accessB=AccessMethod::NONE, AccessMethod accessC=AccessMethod::NONE);
};

class Box256Instruction
{
protected:
    BOXBYTE getParamA(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE paramA = machine->getValue(AccessMethod::ADDRESS,pc + 1);
        return machine->getValue(accessParamA,paramA);
    }
    BOXBYTE getParamB(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE paramB = machine->getValue(AccessMethod::ADDRESS,pc + 2);
        return machine->getValue(accessParamB,paramB);
    }
    BOXBYTE getParamC(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE paramC = machine->getValue(AccessMethod::ADDRESS,pc + 3);
        return machine->getValue(accessParamC,paramC);
    }

public:
    int numArgs;
    AccessMethod accessParamA;
    AccessMethod accessParamB;
    AccessMethod accessParamC;
    QString instName;
    Box256Instruction();
    virtual ~Box256Instruction();
    virtual void execute(Box256Machine *machine, BOXBYTE pc)=0;

};

class Box256InstructionNOP : public Box256Instruction{
public:
    Box256InstructionNOP();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionMOV : public Box256Instruction{
public:
    Box256InstructionMOV();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};
class Box256InstructionADD : public Box256Instruction{
public:
    Box256InstructionADD();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};
class Box256InstructionSUB : public Box256Instruction{
public:
    Box256InstructionSUB();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionJEQ : public Box256Instruction{
public:
    Box256InstructionJEQ();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionMUL : public Box256Instruction{
public:
    Box256InstructionMUL();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionDIV : public Box256Instruction{
public:
    Box256InstructionDIV();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionJMP : public Box256Instruction{
public:
    Box256InstructionJMP();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionJGR : public Box256Instruction{
public:
    Box256InstructionJGR();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionPIX : public Box256Instruction{
public:
    Box256InstructionPIX();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionFLP : public Box256Instruction{
public:
    Box256InstructionFLP();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionTHR : public Box256Instruction{
public:
    Box256InstructionTHR();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionMOD : public Box256Instruction{
public:
    Box256InstructionMOD();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};

class Box256InstructionJNE : public Box256Instruction{
public:
    Box256InstructionJNE();
    void execute(Box256Machine *machine, BOXBYTE pc) override;
};
#endif // BOX256MACHINE_H
