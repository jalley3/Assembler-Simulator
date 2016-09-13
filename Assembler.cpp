#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream> //open file
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <algorithm> //reverse
#include <cmath> //absolute value

std::vector<std::string> split(const std::string & s, char delim);

//class declaration for variables;
class Variables{
public :
  Variables(){};
  void setName(std::string name){name_=name;}
  void setType(std::string type){type_=type;}
  void setCounter(){counter_= numbers_[0];}
  void setCounter(long count){counter_=count;}
  void setNumbers(std::string numbers){
    std::vector<std::string>temp = split(numbers,',');
    for(int i=0; i<temp.size();i++){
      // std::cout << temp[i]<< " ";
      long test = std::stol(temp[i]);
      numbers_.push_back(test);
    }
  }
  void setNumbers (long pc){numbers_.push_back(pc);}
  std::string getName(){return name_;}
  std::string getType(){return type_;}
  long getCounter(){return counter_;}
  void showNumbers(){
    for(int i = 0; i < numbers_.size();i++){
      std::cout << numbers_[i] << " ";
    }
    std::cout << std::endl;
  }
  std::vector<long> numbers_;

private:
  std::string name_;
  std::string type_;
  long counter_;
};

//function definitions
bool isInstruction(std::string test);//check if String is a mips instruction
void instructionDecode(std::string str, long pc,
                       std::unordered_map<std::string,
                       std::string > & registers,
                       std::vector<Variables> varVec);
void variableDecode(std::string str, std::vector<Variables>);
void buildDictionary(std::unordered_map<std::string,std::string> & registers);
std::string binary (long int n, bool sign);
void hexPrint (std::string ans);

int main(int argc, char* argv[])
{
  std::vector<std::string> sourceCode; //to store instructions from the file

  if(argc == 0)
  {
    std::cout << "** Error: no file provided" << '\n';
  }
  if(argc > 2)
  {
    std::cout <<"** Error: Only one file accepted"<<std::endl;
  }
  //open user file
  std::ifstream inputFile(argv[1]);
  std::string lineIn;
  //look for .data to build variables
  do
  {
    std::ws(inputFile);
    std::getline(inputFile, lineIn);
    sourceCode.push_back(lineIn); //add to the list of instructions from source
  }while(inputFile.eof() == 0);
  inputFile.close();//release resources
  sourceCode.pop_back();
  std::vector<Variables> varVec;
  std::string delim = " ";
  std::string firstWord;
  long pc = 0;
  int index = 0;
  for(std::vector<std::string>::iterator it = sourceCode.begin(); it != sourceCo                                                                                                             de.end(); it++){
    firstWord = (*it).substr(0, (*it).find(delim));
    if(!isInstruction(firstWord)){
      Variables newVar;
      if(firstWord != ".data"){
        newVar.setName(firstWord);
        newVar.setType("Label");
        newVar.setCounter(pc);
        newVar.setNumbers(pc);
      }
      else{
        index = std::distance( sourceCode.begin(),it) + 1;
        for (int i = index; i < sourceCode.size();i++){
          std::vector<std::string> temp = split(sourceCode[i],' ');
          newVar.setName(temp[0]);
          newVar.setType(temp[1]);
          newVar.numbers_.clear();
          newVar.setNumbers(temp[2]);
          newVar.setCounter();
          varVec.push_back(newVar);
        }
      }
      if(index > std::distance(sourceCode.begin(),it)) break;
      varVec.push_back(newVar);
    }
    else
      pc++;
  }
  pc = 0; //reset program counter

  //now time to parse the actual instructions
  std::unordered_map<std::string, std::string> registers;
  buildDictionary (registers);
  for(std::vector<std::string>::iterator it = sourceCode.begin(); it != sourceCo                                                                                                             de.end(); it++){
    firstWord  = (*it).substr(0, (*it).find(delim));
    if(!isInstruction(firstWord)){
      variableDecode(*it, varVec);
    }
    else{
      pc++;
      instructionDecode(*it,pc,registers, varVec);
    }
  }

  return 0;
}
bool isInstruction(std::string test){
  if(test == "addu" || test == "addiu" || test == "and" || test == "beq" || test                                                                                                              == "bne"
     || test == "div" || test == "j" || test == "lw" || test == "mfhi" ||test ==                                                                                                              "mflo"
     || test == "mult" || test == "or" || test == "slt" || test == "subu" || tes                                                                                                             t == "s"
     || test == "syscall" || test == "sw"){return true;}
  else
    return false;
}
std::vector<std::string> split (const std::string & s, char delim){
  std::stringstream ss (s);
  std::string item;
  std::vector<std::string> tokens;
  while(getline(ss,item,delim)){
    tokens.push_back(item);
  }
  return tokens;
}
void buildDictionary(std::unordered_map<std::string,std::string> & registers){
  registers["$zero"] = "00000";
  registers["$at"] ="00001";
  registers["$v0"] = "00010";
  registers["$v1"] = "00011";
  registers["$a0"] = "00100";
  registers["$a1"] = "00101";
  registers["$a2"] = "00110";
  registers["$a3"] = "00111";
  registers["$t0"] = "01000";;
  registers["$t1"] = "01001";
  registers["$t2"] = "01010";
  registers["$t3"] = "01011";
  registers["$t4"] = "01100";
  registers["$t5"] = "01101";
  registers["$t6"] = "01110";
  registers["$t7"] = "01111";
  registers["$s0"] = "10000";
  registers["$s1"] = "10001";
  registers["$s2"] = "10010";
  registers["$s3"] = "10011";
  registers["$s4"] = "10100";
  registers["$s5"] = "10101";
  registers["$s6"] = "10110";
  registers["$s7"] = "10111";
  registers["$t8"] = "11000";
  registers["$t9"] = "11001";
  registers["$k0"] = "11010";
  registers["$k1"] = "11011";
  registers["$gp"] = "11100";
  registers["$sp"] = "11101";
  registers["$fp"] = "11110";
  registers["$ra"] = "11111";
}
void instructionDecode(std::string str, long pc, std::unordered_map<std::string,                                                                                                             std::string> & registers,
                       std::vector<Variables> varVec)
{
  std::string fstWrd  = (str).substr(0, (str).find(" "));
  bool sign = 0;
  //addiu
  if (fstWrd == "addiu"){
    //i type(op rs rt immed) rt rs immediate
    sign = 0;
    std::string ans = "001001";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[1]]+ registers[regs[0]];
    long int immediate = std::stol(regs[2]);
    if (immediate < 0) sign = 1;
    ans = ans + binary(std::abs(immediate), sign);

    hexPrint(ans);
    sign = 0;
  }
  //addu
  else if (fstWrd == "addu"){
    //r type(op rs rt rd sa func) rd rs rt 100001

    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[1]]+ registers[regs[2]] + registers[regs[0]] + "0                                                                                                             0000100001";

    hexPrint(ans);
  }
  //and
  else if (fstWrd == "and"){
    //r type(op rs rt rd sa func) rd rs rt 100001
    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[1]]+ registers[regs[2]] + registers[regs[0]] + "0                                                                                                             0000100100";

    hexPrint(ans);
  }
  //beq
  else if (fstWrd == "beq"){
    //i type(op rs rt immed) rs rt  immediate
    std::string ans = "000100";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[0]] + registers[regs[1]];
    int distance = 0;
    for (int i = 0; i <varVec.size(); i++){
      if(varVec[i].getName()== (regs[2]+":")){
        distance = varVec[i].getCounter() - pc;
      }
    }
    if (distance < 0) sign = 1;
    ans+= binary(std::abs(distance), sign);
    hexPrint(ans);
    sign = 0;
  }
  //bne
  else if (fstWrd == "bne"){
    //i type(op rs rt immed) rs rt  immediate
    std::string ans = "000101";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[0]] + registers[regs[1]];
    int distance = 0;
    for (int i = 0; i <varVec.size(); i++){
      if(varVec[i].getName()== (regs[2]+":")){
        distance = varVec[i].getCounter() - pc;
      }
    }
    if (distance < 0) sign = 1;
    ans+= binary(std::abs(distance),sign);
    hexPrint(ans);
    sign = 0;
  }
  //div
  else if (fstWrd == "div"){
    //r type(op rs rt rd sa func) rs rt 011010
    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[0]]+ registers[regs[1]] + "0000000000011010";
    hexPrint(ans);
  }
  //J
  else if (fstWrd == "j"){
    //J type(op immed) J immediate
    std::string ans = "0000100000000000";
    std::vector<std::string> temp = split(str,' ');
    for (int i = 0; i <varVec.size(); i++){
      if(varVec[i].getName()== (temp[1]+":")){
        ans += binary(varVec[i].getCounter(),0);
      }
    }
    hexPrint(ans);
  }
  //load word
  else if (fstWrd == "lw"){
    //i type(op rs rt immed) rt  immediate(rs)
    std::string ans = "100011";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    int immed = 0;
    std::string source;
    if (regs[1][4] == ')') source = regs[1].substr(2,2);
    else source = regs[1].substr(2,3);
    ans = ans + registers[source] + registers[regs[0]];
    for (int i = 0; i <varVec.size(); i++){
      if(varVec[i].getName()==std::string(1,regs[1][0])+":"){
        immed = varVec[i].getCounter();
      }
    }
    if (immed < 0) sign = 1;
    ans+= binary(std::abs(immed),sign);
    hexPrint(ans);
    sign = 0;
  }
  //mfhi
  else if (fstWrd == "mfhi"){
    //r type(op rs rt rd sa func) rd 010000
    std::string ans = "0000000000000000";
    std::vector<std::string> temp = split(str,' ');
    ans = ans + registers[temp[1]]  + "00000010000";
    hexPrint(ans);
  }
  //mflo
  else if (fstWrd == "mflo"){
    //r type(op rs rt rd sa func) rd 010010
    std::string ans = "0000000000000000";
    std::vector<std::string> temp = split(str,' ');
    ans = ans + registers[temp[1]]  + "00000010010";
    hexPrint(ans);
  }
  //mult
  else if (fstWrd == "mult"){
    //r type(op rs rt rd sa func) rs rt 011010
    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[0]]+ registers[regs[1]] + "0000000000011000";
    hexPrint(ans);
  }
  //or
  else if (fstWrd == "or"){
    //r type(op rs rt rd sa func) rd rs rt 100101
    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[1]]+ registers[regs[2]] + registers[regs[0]] + "0                                                                                                             0000100101";

    hexPrint(ans);
  }
  //slt
  else if (fstWrd == "slt"){
    //r type(op rs rt rd sa func) rd rs rt 101010
    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[1]]+ registers[regs[2]] + registers[regs[0]] + "0                                                                                                             0000101010";

    hexPrint(ans);
  }
  //subu
  else if (fstWrd == "subu"){
    //r type(op rs rt rd sa func) rd rs rt 100010
    std::string ans = "000000";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    ans = ans + registers[regs[1]]+ registers[regs[2]] + registers[regs[0]] + "0                                                                                                             0000100010";

    hexPrint(ans);
  }
  //store word
  else if (fstWrd == "sw"){
    //i type(op rs rt immed) rt  immediate(rs)
    std::string ans = "101011";
    std::vector<std::string> temp = split(str,' ');
    std::vector<std::string> regs = split(temp[1],',');
    int immed = 0;
    std::string source;
    if (regs[1][4] == ')') source = regs[1].substr(2,2);
    else source = regs[1].substr(2,3);
    ans = ans + registers[source] + registers[regs[0]];
    for (int i = 0; i <varVec.size(); i++){
      if(varVec[i].getName()==std::string(1,regs[1][0])+":"){
        immed = varVec[i].getCounter();
      }
    }
    if (immed < 0) sign = 1;
    ans+= binary(std::abs(immed),sign);
    hexPrint(ans);
    sign = 0;
  }
  //syscall
  else if (fstWrd == "syscall")  std::cout << "0000000c" << std::endl;
}
std::string binary( long int  n, bool sign )
{
  std::string result;
  do result.push_back( '0' + (n & 1) );
  while (n >>= 1);
  if (result.size() < 16){
    for (int i = result.size(); i < 16; i++){
      result.push_back('0');
    }
  }
  std::reverse( result.begin(), result.end() );
  if (sign == 1){
    for (int i = 0; i < result.size(); i++){
      if(result[i] == '1') result[i] = '0';
      else if (result[i] == '0') result[i] = '1';
    }
    bool carry = 1;
    for (int i = result.size()-1; i >= 0; i--){
      if (carry == 1){
        if (result[i] == '1'){
          carry = 1;
          result[i] = '0';
        }
        else if (result[i] == '0')
        {
          carry = 0;
          result[i] = '1';
        }
      }
    }
  }
  return result;
}
void hexPrint (std::string ans){
  long binaryToDec = std::stol(ans,nullptr,2);
  std::cout <<std::setbase(16)<<std::setfill('0') << std::setw(8) << binaryToDec                                                                                                              << std::endl;
}
void variableDecode(std::string str, std::vector<Variables> varVec){
  std::string fstWrd  = (str).substr(0, (str).find(" "));
  for (int i = 0; i < varVec.size(); i++){
    if (varVec[i].getName() == fstWrd){
      if (varVec[i].getType()== ".word"){
        for(int j = 0; j< varVec[i].numbers_.size(); j++)
          hexPrint(binary(varVec[i].numbers_[j],0));
      }
      else if (varVec[i].getType() == ".space"){
        for (int j = 0; j < varVec[i].getCounter(); j++)
          std::cout << "00000000" << std::endl;
      }
    }
  }
}
