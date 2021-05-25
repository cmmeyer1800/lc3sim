#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <climits>
#include <bitset>

using namespace std;

class lc3code{
    private:
        vector<string> codeStrLst;
        bitset<16> memory[65536];
        bitset<3> nzp;
        bitset<16> r0, r1, r2, r3, r4, r5, r6, r7;
        vector<bitset<16>> registerTable;
    public:
        lc3code(){
            createRegTable();
        }
        lc3code(vector<string> code){
            codeStrLst = code;
            createRegTable();
        }
        /* The create register table function pushed the 16 bitset register values
        into the register table so that the registers can be accessed by their integer
        value. It performs as a lookup table in this way.
        */
        void createRegTable(){
        registerTable.push_back(r0);
        registerTable.push_back(r1);
        registerTable.push_back(r2);
        registerTable.push_back(r3);
        registerTable.push_back(r4);
        registerTable.push_back(r5);
        registerTable.push_back(r6);
        registerTable.push_back(r7);
        }
        /* The bitsetAdd function loops through the 16 bits in each add calls the
        fullAdder helper function.
        */
        void bitsetAdd(bitset<16>& x, bitset<16>& y){
            bool carry = false;
            for (int i = 0; i < 16; i++){
                x[i] = fullAdder(x[i], y[i], carry);
            }
        }
        /* Full Adder performs a full add on the two passed bitsets and keeps track
        of the carry. This is a helper function called by bitsetAdd
        */
        bool fullAdder(bool b1, bool b2, bool& carry){
            bool sum = (b1 ^ b2) ^ carry;
            carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
            return sum;
        }
        /* The Set CC or Conditional Code function is called on a register value
        in order to set the correct of N, Z, or P of the CC
        */
        void setCC(bitset<16> value){
            if(value.test(15)){
                nzp.reset();
                nzp.set(2);
            }
            else if(!(value.any())){
                nzp.reset();
                nzp.set(1);
            }
            else{
                nzp.reset();
                nzp.set(0);
            }
        }
        /* The load into memory function takes the bit values in our storage of code lines
        and places them into subsequent memory locations starting at the standard x3000
        */
        bool loadIntoMem(){
            int iterator = 0;
            for(int memLoc = 3000; memLoc < 3000 + codeStrLst.size(); memLoc++){
                int value = 0;
                for(int i = 0; i < codeStrLst[iterator].length(); i++){
                    if(!((codeStrLst[iterator][i]=='0') || (codeStrLst[iterator][i]=='1'))){
                        cout << "Error: code at memory location " << memLoc << " is invalid\nContains values that are not 1 or 0.\n";
                        return true;
                    }
                }
                bitset<16> bitString (codeStrLst[iterator]);
                memory[memLoc] = bitString;
                iterator++;
            }
            return false;
        }
        /* The run function while loops through the 65536 memory locations and gets the
        value at the memory location and then will check the opcode for which operation to perform.
        From their it reads the register options and the check bits in order to move data around correctly.
        */
        bool run(){
            int PC = 3000;
            while(PC < 65536){
                bitset<4> opcode;
                bitset<3> elevenNine;
                bitset<3> eightSix;
                bitset<9> eightZero;
                bitset<8> sevenZero;
                bitset<6> fiveZero;
                bitset<5> fourZero;
                bitset<3> twoZero;
                bool five;
                for(int x = 15; x >= 0; x--){
                    if(x >= 12 && x <= 15)      {opcode[x - 12] = memory[PC][x];}
                    if(x >= 9 && x <= 11)       {elevenNine[x - 9] = memory[PC][x];}
                    if(x >= 6 && x <= 8)        {eightSix[x - 6] = memory[PC][x];}
                    if(x >= 0 && x <= 8)        {eightZero[x] = memory[PC][x];}
                    if(x >= 0 && x <= 7)        {sevenZero[x] = memory[PC][x];}
                    if(x >= 0 && x <= 5)        {fiveZero[x] = memory[PC][x];}
                    if(x >= 0 && x <= 4)        {fourZero[x] = memory[PC][x];}
                    if(x >= 0 && x <= 2)        {twoZero[x] = memory[PC][x];}
                    if(x == 5)                  {five = memory[PC][x];}
                }
                PC++;
                /*  if(PC >= 3000 && PC <= 3003){
                    cout << memory[PC] << "\n";
                    cout << opcode << "\n";
                    } */
                switch(opcode.to_ulong()){
                    case(0):
                    {
                        bitset<16> offset;
                        if(eightZero.test(8)){
                            offset.set();
                        }
                        for(int i = 0; i <= 8; i++){
                            offset[i] = eightZero[i];
                        }
                        bitset<16> PCBitset((long)(PC));
                        bitsetAdd(PCBitset, offset);
                        switch(elevenNine.to_ulong()){
                            case(1):
                            {
                                if(nzp.test(0)){
                                    PC = PCBitset.to_ulong();
                                }
                                break;
                            }
                            case(2):
                            {
                                if(nzp.test(1)){
                                    PC = PCBitset.to_ulong();
                                }
                                break;
                            }
                            case(4):
                            {
                                if(nzp.test(2)){
                                    PC = PCBitset.to_ulong();
                                }
                                break;
                            }
                            default:
                                throw "Error in execution, contact author with details";
                        }
                        break;
                    }
                    case(1): // ADD
                    {
                        if(five){
                            bitset<16> imm;
                            if(fourZero.test(4)){
                                imm.set();
                            }
                            for(int i = 0; i < 5; i++){
                                imm[i] = fourZero[i];
                            }
                            bitsetAdd(imm, registerTable[eightSix.to_ulong()]);
                            registerTable[elevenNine.to_ulong()] = imm;
                            setCC(imm);
                        }
                        else{
                            bitsetAdd(registerTable[eightSix.to_ulong()], registerTable[twoZero.to_ulong()]);
                            registerTable[elevenNine.to_ulong()] = registerTable[eightSix.to_ulong()];
                        }
                        setCC(registerTable[elevenNine.to_ulong()]);
                        break;
                    }
                    case(2): // LD
                    {
                        bitset<16> offset;
                        if(eightZero.test(8)){
                            offset.set();
                        }
                        for(int i = 0; i <= 8; i++){
                            offset[i] = eightZero[i];
                        }
                        bitset<16> PCBitset((long)(PC));
                        bitsetAdd(PCBitset, offset);
                        registerTable[elevenNine.to_ulong()] = memory[PCBitset.to_ulong()];
                        setCC(registerTable[elevenNine.to_ulong()]);
                        break;
                    }
                    case(3): // ST
                    {
                        bitset<16> offset;
                        if(eightZero.test(8)){
                            offset.set();
                        }
                        for(int i = 0; i <= 8; i++){
                            offset[i] = eightZero[i];
                        }
                        bitset<16> PCBitset((long)(PC));
                        bitsetAdd(PCBitset, offset);
                        memory[PCBitset.to_ulong()] = registerTable[elevenNine.to_ulong()];
                        break;
                    }
                    case(4): // JSR
                    {
                        break;
                    }
                    case(5): // AND
                    {
                        bitset<16> temp;
                        if(five){
                            bitset<16> imm;
                            if(fourZero.test(4)){
                                imm.set();
                            }
                            for(int i = 0; i < 5; i++){
                                imm[i] = fourZero[i];
                            }
                            temp = imm & registerTable[eightSix.to_ulong()];
                            registerTable[elevenNine.to_ulong()] = temp;
                        }
                        else{
                            temp = registerTable[eightSix.to_ulong()] & registerTable[twoZero.to_ulong()];
                            registerTable[elevenNine.to_ulong()] = temp;
                        }
                        setCC(registerTable[elevenNine.to_ulong()]);
                        break;
                    }
                    case(6): // LDR
                    {
                        bitset<16> offset;
                        if(fiveZero.test(5)){
                            offset.set();
                        }
                        for(int i = 0; i <= 5; i++){
                            offset[i] = fiveZero[i];
                        }
                        bitsetAdd(offset, registerTable[eightSix.to_ulong()]);
                        registerTable[elevenNine.to_ulong()] = memory[offset.to_ulong()];
                        break;
                    }
                    case(7): // STR
                    {
                        bitset<16> offset;
                        if(fiveZero.test(5)){
                            offset.set();
                        }
                        for(int i = 0; i <= 5; i++){
                            offset[i] = fiveZero[i];
                        }
                        bitsetAdd(offset, registerTable[eightSix.to_ulong()]);
                        memory[offset.to_ulong()] = registerTable[elevenNine.to_ulong()];
                        break;
                    }
                    case(9): // NOT
                    {
                        registerTable[elevenNine.to_ulong()] = registerTable[eightSix.to_ulong()].flip();
                        setCC(registerTable[elevenNine.to_ulong()]);
                        break;
                    }
                    case(10): //LDI
                    {
                        bitset<16> offset;
                        if(eightZero.test(8)){
                            offset.set();
                        }
                        for(int i = 0; i <= 8; i++){
                            offset[i] = eightZero[i];
                        }
                        bitset<16> PCBitset((long)(PC));
                        bitsetAdd(PCBitset, offset);
                        registerTable[elevenNine.to_ulong()] = memory[memory[PCBitset.to_ulong()].to_ulong()];
                        setCC(registerTable[elevenNine.to_ulong()]);
                        break;
                    }
                    case(11): // STI
                    {
                        bitset<16> offset;
                        if(eightZero.test(8)){
                            offset.set();
                        }
                        for(int i = 0; i <= 8; i++){
                            offset[i] = eightZero[i];
                        }
                        bitset<16> PCBitset((long)(PC));
                        bitsetAdd(PCBitset, offset);
                        memory[memory[PCBitset.to_ulong()].to_ulong()] = registerTable[elevenNine.to_ulong()];
                        break;
                    }
                    case(12): // JMP
                    {
                        PC = registerTable[eightSix.to_ulong()].to_ulong();
                        break;   
                    } 
                    case(14): // LEA
                    {
                        bitset<16> offset;
                        if(eightZero.test(8)){
                            offset.set();
                        }
                        for(int i = 0; i <= 8; i++){
                            offset[i] = eightZero[i];
                        }
                        bitset<16> PCBitset((long)(PC));
                        bitsetAdd(PCBitset, offset);
                        registerTable[elevenNine.to_ulong()] = PCBitset.to_ulong();
                        setCC(registerTable[elevenNine.to_ulong()]);
                        break;
                    }
                    case(15): // TRAP
                    {
                        switch(sevenZero.to_ulong()){
                            case(21): // TRAP - OUT
                            {
                            bitset<8> asciiEight;
                            for(int i = 0; i < 8; i++){
                                asciiEight[i] = registerTable[0][i];
                            }
                            unsigned long intRepr = asciiEight.to_ulong(); 
                            unsigned char charRepr = static_cast<unsigned char>(intRepr);
                            cout << charRepr;
                            break;
                            }
                            case(25): // TRAP - HALT
                            {
                                return false;
                            }
                            default:
                                cout << "Error: Unknown TRAP instruction at memory location " << PC << "\n";
                                return true;
                        }
                        break;
                    }
                    default:
                        cout << "Error: Opcode at memory location " << PC << " is incorrect\n";
                        return true;
                }
            }
            return false;
        }
};

vector<string> getCode(string fileName){
    vector<string> code;
    fstream file;
    file.open(fileName, ios::in);
    if(!file){
        cout << "Error: File does not exist\n";
    }
    string line;
    while(getline(file, line)){
        code.push_back(line.substr(0,16));
    }
    return code;
}

bool processInput(int argCount, char **args, string *filePtr){ //return true indicates error, false indicates success
    int sizeInput;
    switch(argCount){
        case 1: // case of no filename
            {
            cout << "Error: No File name found\n";
            return true;
            }
        case 2: // good case filename passed
            {
            string tempFile(args[1]);
            *filePtr = tempFile;
            return false;
            }
        default: // case of more than 1 filename aka error
            {
            cout << "Error: To many arguments to command line call, should be 1\n";
            return true;
            }
    }
    return true;   
}

int main(int argc, char **argv){
    string fileName;
    string *filePtr = &fileName;
    if (processInput(argc, argv, filePtr)) return 1;
    vector<string> code = getCode(*filePtr);
    lc3code codeObj(code);
    if(codeObj.loadIntoMem()) return 1;
    if(codeObj.run()) return 1;
    cout << "\n";
    return 0;
}