#include "box256machine.h"

#define paramA_r getRParamA(machine,pc)
#define paramB_r getRParamB(machine,pc)
#define paramC_r getRParamC(machine,pc)

#define paramA_w getWParamA(machine,pc)
#define paramB_w getWParamB(machine,pc)
#define paramC_w getWParamC(machine,pc)

Box256Instruction::Box256Instruction()
    : accessParamA(AccessMethod::NONE),accessParamB(AccessMethod::NONE), accessParamC(AccessMethod::NONE)
{
}
Box256Instruction::~Box256Instruction(){}

//NOP
Box256InstructionNOP::Box256InstructionNOP() : Box256Instruction(){}
void Box256InstructionNOP::execute(Box256Machine *machine, BOXBYTE pc){};

//MOV
Box256InstructionMOV::Box256InstructionMOV() : Box256Instruction(){}
void Box256InstructionMOV::execute(Box256Machine *machine, BOXBYTE pc)
{
    for(int i=0;i<paramC_r || i==0;i++)
    {
        machine->writeValue(paramA_r,paramB_w + i);
    }
};

//ADD
Box256InstructionADD::Box256InstructionADD() : Box256Instruction(){}
void Box256InstructionADD::execute(Box256Machine *machine, BOXBYTE pc)
{
    machine->writeValue(paramA_r+paramB_r,paramC_w);
};

//SUB
Box256InstructionSUB::Box256InstructionSUB() : Box256Instruction(){}
void Box256InstructionSUB::execute(Box256Machine *machine, BOXBYTE pc){};

//JEQ
Box256InstructionJEQ::Box256InstructionJEQ() : Box256Instruction(){}
void Box256InstructionJEQ::execute(Box256Machine *machine, BOXBYTE pc){};

//MUL
Box256InstructionMUL::Box256InstructionMUL() : Box256Instruction(){}
void Box256InstructionMUL::execute(Box256Machine *machine, BOXBYTE pc){};

//DIV
Box256InstructionDIV::Box256InstructionDIV() : Box256Instruction(){}
void Box256InstructionDIV::execute(Box256Machine *machine, BOXBYTE pc){};

//JMP
Box256InstructionJMP::Box256InstructionJMP() : Box256Instruction(){}
void Box256InstructionJMP::execute(Box256Machine *machine, BOXBYTE pc)
{
    BOXBYTE pcLoc = machine->getPC(machine->getCurThread());
    switch (accessParamA) {
    case AccessMethod::CONSTANT:
    {
        machine->writeValue(machine->getValue(AccessMethod::ADDRESS,pcLoc)+paramA_r-0x4,pcLoc);
        break;
    }
    default:
    {
        machine->writeValue(paramA_w-0x4,pcLoc);
        break;
    }
    }

}

//JGR
Box256InstructionJGR::Box256InstructionJGR() : Box256Instruction(){}
void Box256InstructionJGR::execute(Box256Machine *machine, BOXBYTE pc)
{
    if(paramA_r > paramB_r)
    {
        BOXBYTE pcLoc = machine->getPC(machine->getCurThread());
        switch (accessParamC) {
        case AccessMethod::CONSTANT:
        {
            machine->writeValue(machine->getValue(AccessMethod::ADDRESS,pcLoc)+paramC_r-0x4,pcLoc);
            break;
        }
        default:
        {
            machine->writeValue(paramC_w-0x4,pcLoc);
            break;
        }
        }
    }
}

//PIX
Box256InstructionPIX::Box256InstructionPIX() : Box256Instruction(){}
void Box256InstructionPIX::execute(Box256Machine *machine, BOXBYTE pc)
{
    machine->writePixel(paramB_r,paramA_r);
};

//FLP
Box256InstructionFLP::Box256InstructionFLP() : Box256Instruction(){}
void Box256InstructionFLP::execute(Box256Machine *machine, BOXBYTE pc){};

//THR
Box256InstructionTHR::Box256InstructionTHR() : Box256Instruction(){}
void Box256InstructionTHR::execute(Box256Machine *machine, BOXBYTE pc){};

//MOD
Box256InstructionMOD::Box256InstructionMOD() : Box256Instruction(){}
void Box256InstructionMOD::execute(Box256Machine *machine, BOXBYTE pc){};

//JNE
Box256InstructionJNE::Box256InstructionJNE() : Box256Instruction(){}
void Box256InstructionJNE::execute(Box256Machine *machine, BOXBYTE pc){};
