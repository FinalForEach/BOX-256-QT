#include "box256machine.h"

#define paramA getParamA(machine,pc)
#define paramB getParamB(machine,pc)
#define paramC getParamC(machine,pc)
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
void Box256InstructionMOV::execute(Box256Machine *machine, BOXBYTE pc){};

//ADD
Box256InstructionADD::Box256InstructionADD() : Box256Instruction(){}
void Box256InstructionADD::execute(Box256Machine *machine, BOXBYTE pc){};

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
void Box256InstructionJMP::execute(Box256Machine *machine, BOXBYTE pc){};

//JGR
Box256InstructionJGR::Box256InstructionJGR() : Box256Instruction(){}
void Box256InstructionJGR::execute(Box256Machine *machine, BOXBYTE pc){};

//PIX
Box256InstructionPIX::Box256InstructionPIX() : Box256Instruction(){}
void Box256InstructionPIX::execute(Box256Machine *machine, BOXBYTE pc)
{
    machine->writePixel(paramB,paramA);
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
