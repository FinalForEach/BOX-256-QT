#ifndef BOX256MACHINE_H
#define BOX256MACHINE_H

#include <QMap>

#include "boxtypes.h"
class Box256Instruction;
class Box256Machine
{
private:
    BOXBYTE newdata[256];
    BOXBYTE data[256];
    BOXBYTE pixels[256];
    BOXBYTE numThreads;
    BOXBYTE numNewThreads;
    QMap<BOXBYTE,Box256Instruction*> instMap;
    BOXBYTE currentThread;
    bool dataJustWrittenThisThread[256];
public:
    bool dataJustWritten[256];
    int curCycle;

    Box256Machine();

    void writeValue(BOXBYTE wval, BOXBYTE addr);
    void writePixel(BOXBYTE wval, BOXBYTE pixAddr);
    BOXBYTE getPixel(BOXBYTE pixAddr) const;
    BOXBYTE getValue(AccessMethod valMethod, BOXBYTE getter, bool getCached=true) const;
    BOXBYTE getPC(BOXBYTE threadNum) const;
    void createThread(BOXBYTE startAddr);

    void step();
    void reset();
    void flushNewData();

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
    BOXBYTE getRParam(Box256Machine *machine, BOXBYTE pc, BOXBYTE pcOffset, AccessMethod access){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + pcOffset,false);
        return machine->getValue(access,param);
    }
    BOXBYTE getWParam(Box256Machine *machine, BOXBYTE pc, BOXBYTE pcOffset, AccessMethod access){
        BOXBYTE param = machine->getValue(AccessMethod::ADDRESS,pc + pcOffset);
        if(access==AccessMethod::ADDRESS)
            return machine->getValue(AccessMethod::CONSTANT,param);
        if(access==AccessMethod::POINTER){
            return machine->getValue(AccessMethod::ADDRESS,param);
        }
        assert(0);//Fail if writing to a 'constant'
        return 0;
    }
    BOXBYTE getRParamA(Box256Machine *machine, BOXBYTE pc){
        return getRParam(machine,pc,1,accessParamA);
    }
    BOXBYTE getRParamB(Box256Machine *machine, BOXBYTE pc){
        return getRParam(machine,pc,2,accessParamB);
    }
    BOXBYTE getRParamC(Box256Machine *machine, BOXBYTE pc){
        return getRParam(machine,pc,3,accessParamC);
    }

    BOXBYTE getWParamA(Box256Machine *machine, BOXBYTE pc){
        return getWParam(machine,pc,1,accessParamA);
    }
    BOXBYTE getWParamB(Box256Machine *machine, BOXBYTE pc){
        return getWParam(machine,pc,2,accessParamB);
    }
    BOXBYTE getWParamC(Box256Machine *machine, BOXBYTE pc){
        return getWParam(machine,pc,3,accessParamC);
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
