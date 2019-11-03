#include "box256machine.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

Box256Machine::Box256Machine(): numThreads(1), curCycle(0)
{
    QFile opcodeFile(":/machine/opcodes.csv");
    if(!opcodeFile.open(QIODevice::ReadOnly)){
        QMessageBox::warning(nullptr,"Error","Could not load opcode file, error: "+opcodeFile.errorString());
    }

    QTextStream opcodeStream(&opcodeFile);
    auto opcodeHeader = opcodeStream.readLine().split(",");
    QMap<QString,int> fieldIndexes;
    for(int i=0;i<opcodeHeader.size();i++)
    {
        fieldIndexes.insert(opcodeHeader[i],i);
    }

    while(!opcodeStream.atEnd())
    {
        auto fields = opcodeStream.readLine().split(",");
        QString opCodeStr = fields[fieldIndexes["OPCODE"]];
        QString instName = fields[fieldIndexes["Instruction"]];
        QString p1 = fields[fieldIndexes["P1"]];
        QString p2 = fields[fieldIndexes["P2"]];
        QString p3 = fields[fieldIndexes["P3"]];
        Box256Instruction *inst = nullptr;

        int opcodeNum = opCodeStr.toInt(nullptr,16);
        int numArgs = 3;

        if(instName=="NOP"){
            inst = new Box256InstructionNOP();
        }else if(instName=="MOV"){
            inst = new Box256InstructionMOV();
        }else if(instName=="ADD"){
            inst = new Box256InstructionADD();
        }else if(instName=="SUB"){
            inst = new Box256InstructionSUB();
        }else if(instName=="JEQ"){
            inst = new Box256InstructionJEQ();
        }else if(instName=="MUL"){
            inst = new Box256InstructionMUL();
        }else if(instName=="DIV"){
            inst = new Box256InstructionDIV();
        }else if(instName=="JMP"){
            inst = new Box256InstructionJMP();
        }else if(instName=="JGR"){
            inst = new Box256InstructionJGR();
        }else if(instName=="PIX"){
            inst = new Box256InstructionPIX();
        }else if(instName=="FLP"){
            inst = new Box256InstructionFLP();
        }else if(instName=="THR"){
            inst = new Box256InstructionTHR();
        }else if(instName=="MOD"){
            inst = new Box256InstructionMOD();
        }else if(instName=="JNE"){
            inst = new Box256InstructionJNE();
        }

        if(p1=="0"){
            inst->accessParamA=AccessMethod::CONSTANT;
        }else if(p1=="@"){
            inst->accessParamA=AccessMethod::ABSOLUTE;
        }else if(p1=="*"){
            inst->accessParamA=AccessMethod::POINTER;
        }else{
            numArgs--;
        }

        if(p2=="0"){
            inst->accessParamB=AccessMethod::CONSTANT;
        }else if(p2=="@"){
            inst->accessParamB=AccessMethod::ABSOLUTE;
        }else if(p2=="*"){
            inst->accessParamB=AccessMethod::POINTER;
        }else{
            numArgs--;
        }

        if(p3=="0"){
            inst->accessParamC=AccessMethod::CONSTANT;
        }else if(p3=="@"){
            inst->accessParamC=AccessMethod::ABSOLUTE;
        }else if(p3=="*"){
            inst->accessParamC=AccessMethod::POINTER;
        }else{
            numArgs--;
        }

        inst->numArgs=numArgs;

        instMap.insert(opcodeNum,inst);
    }
}
void Box256Machine::writeValue(BOXBYTE wval, BOXBYTE addr)
{
    data[addr]=wval;
}
void Box256Machine::writePixel(BOXBYTE wval, BOXBYTE pixAddr)
{
    pixels[pixAddr]=wval;
}
BOXBYTE Box256Machine::getPixel(BOXBYTE pixAddr)
{
    return pixels[pixAddr];
}
BOXBYTE Box256Machine::getValue(AccessMethod valMethod, BOXBYTE getter)
{
    switch(valMethod)
    {
        case AccessMethod::CONSTANT:{
            return getter;
        }
        case AccessMethod::ABSOLUTE:{
            return data[getter];
        }
        case AccessMethod::POINTER:{
            return data[data[getter]];
        }
        case AccessMethod::NONE:{
            return 0;
        }
    }
}
BOXBYTE Box256Machine::getPC(BOXBYTE threadNum)
{
    return 0xFF - threadNum;
}
void Box256Machine::createThread()
{
    if(numThreads>=0xFF)return;//Already at max threads.
    numThreads++;
}
void Box256Machine::step()
{
    for(BOXBYTE t=0;t<numThreads;t++)
    {
        BOXBYTE pc = getPC(t);

        BOXBYTE opcode = getValue(AccessMethod::ABSOLUTE,pc);

        Box256Instruction *inst = instMap[opcode];
        inst->execute(this,pc);
        data[pc]++;//Increment program counter.
    }
    curCycle++;
}
void Box256Machine::reset()
{
    curCycle=0;
    numThreads=1;
}
