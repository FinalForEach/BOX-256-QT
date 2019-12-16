#include "box256machine.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

Box256Machine::Box256Machine()
{
    reset();
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

        BOXBYTE opcodeNum = static_cast<BOXBYTE>(opCodeStr.toInt(nullptr,16));
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
        inst->instName=instName;

        if(p1=="0"){
            inst->accessParamA=AccessMethod::CONSTANT;
        }else if(p1=="@"){
            inst->accessParamA=AccessMethod::ADDRESS;
        }else if(p1=="*"){
            inst->accessParamA=AccessMethod::POINTER;
        }else{
            inst->accessParamA=AccessMethod::NONE;
            numArgs--;
        }

        if(p2=="0"){
            inst->accessParamB=AccessMethod::CONSTANT;
        }else if(p2=="@"){
            inst->accessParamB=AccessMethod::ADDRESS;
        }else if(p2=="*"){
            inst->accessParamB=AccessMethod::POINTER;
        }else{
            inst->accessParamB=AccessMethod::NONE;
            numArgs--;
        }

        if(p3=="0"){
            inst->accessParamC=AccessMethod::CONSTANT;
        }else if(p3=="@"){
            inst->accessParamC=AccessMethod::ADDRESS;
        }else if(p3=="*"){
            inst->accessParamC=AccessMethod::POINTER;
        }else{
            inst->accessParamC=AccessMethod::NONE;
            numArgs--;
        }

        inst->numArgs=numArgs;

        instMap.insert(opcodeNum,inst);
    }
    opcodeFile.close();
}
BOXBYTE Box256Machine::getOpcodeFromCommand(QString name, AccessMethod accessA, AccessMethod accessB, AccessMethod accessC)
{
    QVector<BOXBYTE> candidates;
    for(auto opcode : instMap.keys())//Get possible opcodes.
    {
        auto inst = instMap[opcode];
        if(inst==nullptr)continue;
        if(inst->instName==name){
            candidates.append(opcode);
        }
    }
    //Try strict check - exact same accesses
    for(auto opcode : candidates)
    {
        auto inst = instMap[opcode];
        if((inst->accessParamA==accessA)
            &&(inst->accessParamB==accessB)
            &&(inst->accessParamC==accessC)){
            return opcode;
        }
    }
    //Try permissive check - constants are treated as none.
    for(auto opcode : candidates)
    {
        auto inst = instMap[opcode];
        if((inst->accessParamA==accessA || (inst->accessParamA==AccessMethod::NONE && accessA==AccessMethod::CONSTANT))
            &&(inst->accessParamB==accessB || (inst->accessParamB==AccessMethod::NONE && accessB==AccessMethod::CONSTANT))
            &&(inst->accessParamC==accessC || (inst->accessParamC==AccessMethod::NONE && accessC==AccessMethod::CONSTANT))){
            return opcode;
        }
    }
    //No opcode found
    return 0;
}
void Box256Machine::writeValue(BOXBYTE wval, BOXBYTE addr)
{
    qInfo()<<"\t\t\Modified Address "<<getHexNum(addr)<<" from Value "<<getHexNum(newdata[addr])<<" to Value "<<getHexNum(wval);
    newdata[addr]=wval;
    dataJustWritten[addr]=true;
    dataJustWrittenThisThread[addr]=true;
}
void Box256Machine::writePixel(BOXBYTE wval, BOXBYTE pixAddr)
{
    pixels[pixAddr]=wval;
}
BOXBYTE Box256Machine::getPixel(BOXBYTE pixAddr) const
{
    return pixels[pixAddr];
}
BOXBYTE Box256Machine::getValue(AccessMethod valMethod, BOXBYTE getter, bool getCached) const
{
    const BOXBYTE *memory = data;
    if(!getCached){memory = newdata;}

    switch(valMethod)
    {
        case AccessMethod::CONSTANT:{
            return getter;
        }
        case AccessMethod::ADDRESS:{
            return memory[getter];
        }
        case AccessMethod::POINTER:{
            return memory[memory[getter]];
        }
        case AccessMethod::NONE:{
            return 0;
        }
    }
}
BOXBYTE Box256Machine::getPC(BOXBYTE threadNum) const
{
    return 0xFF - threadNum;
}
void Box256Machine::createThread(BOXBYTE startAddr)
{
    if(numThreads+numNewThreads==0xFF)return;//Already at max threads. Undefined behaviour.

    numNewThreads++;
    //Put point pc at start address
    writeValue(startAddr,getPC(numThreads+numNewThreads-1));
}
void Box256Machine::flushNewData()
{
    for(int i=0;i<0x100;i++)
    {
        data[i]=newdata[i];
    }
}
void Box256Machine::step()
{
    for(int b=0x0; b<0x100;b++){
        dataJustWritten[b]=false;
        dataJustWrittenThisThread[b]=false;
    }
    qInfo()<<"Stepping machine:";
    for(BOXBYTE t=0;t<numThreads;t++)
    {
        for(int b=0x0; b<0x100;b++){
            dataJustWrittenThisThread[b]=false;
        }
        currentThread=t;
        BOXBYTE pc = getValue(AccessMethod::ADDRESS,getPC(t),false);

        BOXBYTE opcode = getValue(AccessMethod::ADDRESS,pc,false);

        Box256Instruction *inst = instMap[opcode];
        if(inst==nullptr)inst=instMap[0x0];//Do NOP if no opcode found.

        qInfo()<<"\tThread"<<t<<": Executing "<<inst->instName<<", opcode: "<<getHexNum(opcode)<<" at PC="<<pc;
        inst->execute(this,pc);
        if(!dataJustWrittenThisThread[getPC(t)])//If PC not modified to immediate jump
            newdata[getPC(t)]+=4;//Increment program counter.
    }
    flushNewData();
    numThreads+=numNewThreads;
    numNewThreads=0;
    curCycle++;
}
void Box256Machine::reset()
{
    numNewThreads=0;
    currentThread=0;
    curCycle=0;
    numThreads=1;
    for(int b=0x0; b<0x100;b++)
    {
        newdata[b]=data[b]=0x0;
        pixels[b]=0x0;
        dataJustWritten[b]=false;
        dataJustWrittenThisThread[b]=false;
    }
    qInfo()<<"\nMachine reset.";
}
