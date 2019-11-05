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
    BOXBYTE currentThread;
public:
    int curCycle;

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
    BOXBYTE getCurThread() const
    {
        return currentThread;
    }
    BOXBYTE getOpcodeFromCommand(QString name, AccessMethod accessA,
                                 AccessMethod accessB=AccessMethod::NONE, AccessMethod accessC=AccessMethod::NONE);
};

class Box256Instruction
{
protected:
    BOXBYTE getRParamA(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + 1);
        return machine->getValue(accessParamA,param);
    }
    BOXBYTE getRParamB(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + 2);
        return machine->getValue(accessParamB,param);
    }
    BOXBYTE getRParamC(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + 3);
        return machine->getValue(accessParamC,param);
    }

    BOXBYTE getWParamA(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + 1);
        if(accessParamA==AccessMethod::ADDRESS)
            return machine->getValue(AccessMethod::CONSTANT,param);
        if(accessParamA==AccessMethod::POINTER)
            return machine->getValue(AccessMethod::ADDRESS,param);
        assert(0);//Fail if writing to a 'constant'
        return 0;
    }
    BOXBYTE getWParamB(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + 2);
        if(accessParamB==AccessMethod::ADDRESS)
            return machine->getValue(AccessMethod::CONSTANT,param);
        if(accessParamB==AccessMethod::POINTER)
            return machine->getValue(AccessMethod::ADDRESS,param);
        assert(0);//Fail if writing to a 'constant'
        return 0;
    }
    BOXBYTE getWParamC(Box256Machine *machine, BOXBYTE pc){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + 3);
        if(accessParamC==AccessMethod::ADDRESS)
            return machine->getValue(AccessMethod::CONSTANT,param);
        if(accessParamC==AccessMethod::POINTER)
            return machine->getValue(AccessMethod::ADDRESS,param);
        assert(0);//Fail if writing to a 'constant'
        return 0;
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
