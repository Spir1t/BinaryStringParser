#include "HelperFunctions.h"
#include <fstream>

using namespace std;

unsigned char HelperFunctions::InvertByte(char SignedByte)
{
    //Byte structure: n8 n7 n6 n5 n4 n3 n2 n1
    //Inverted byte structure: n1 n2 n3 n4 n5 n6 n7 n8
    unsigned char Byte = static_cast<unsigned char>(SignedByte);
    int n8 = Byte >> 7;
    int n7 = (Byte >> 6) - (n8 << 1);
    int n6 = (Byte >> 5) - (n8 << 2) - (n7 << 1);
    int n5 = (Byte >> 4) - (n8 << 3) - (n7 << 2) - (n6 << 1);
    int n4 = (Byte >> 3) - (n8 << 4) - (n7 << 3) - (n6 << 2) - (n5 << 1);
    int n3 = (Byte >> 2) - (n8 << 5) - (n7 << 4) - (n6 << 3) - (n5 << 2) - (n4 << 1);
    int n2 = (Byte >> 1) - (n8 << 6) - (n7 << 5) - (n6 << 4) - (n5 << 3) - (n4 << 2) - (n3 << 1);
    int n1 = Byte - (n8 << 7) - (n7 << 6) - (n6 << 5) - (n5 << 4) - (n4 << 3) - (n3 << 2) - (n2 << 1);
    return((n1 << 7) + (n2 << 6) + (n3 << 5) + (n4 << 4) + (n5 << 3) + (n6 << 2) + (n7 << 1) + n8);
}

int HelperFunctions::GetBinarySlice(int Num, int FromBit, int ToBit)
{
    // Byte structure: n8 n7 n6 n5 n4 n3 n2 n1
    // max From is 1, Min To is 1
    int Front = (Num >> FromBit) << FromBit; 
    return((Num - Front) >> (ToBit - 1));
}

vector<unsigned char> DataInstanceToBinary(DataInstance * Inst)
{
    //The bytes are inverted
    //Value (binary): n10 n9 n8 n7 n6 n5 n4 n3 n2 n1
    //Byte 1: n1 n2 n3 n4 n5 n6 n7 n8
    //Byte 2: n9 n10 0 0 0 0 0 0
    //Note: There are not more than 20 bits in one DataInstance
    vector<unsigned char> ByteList;
    if(Inst == nullptr)
    {
        cerr << "vector<unsigned char> DataInstanceToBinary(DataInstance * Inst): Data Instance is null";
        return ByteList;
    }
    if(Inst->Type == DataType::SAR_BEAM_CODE || Inst->Type == DataType::DELAY_CODE || 
       Inst->Type == DataType::SIGMA_NOISE_CODE || Inst->Type == DataType::RECV_ATT_CODE || 
       Inst->Type == DataType::CONTROL_CODE)
    {
        unsigned char Byte = HelperFunctions::InvertByte(static_cast<unsigned char>(Inst->Value));
        ByteList.push_back(Byte);
    }
    else if(Inst->Type == DataType::PRP_CODE || Inst->Type == DataType::START_DELAY_CODE || Inst->Type == DataType::END_DELAY_CODE)
    {
        int BinarySlice2 = HelperFunctions::GetBinarySlice(Inst->Value, 16, 9);
        unsigned char Byte2 = HelperFunctions::InvertByte(static_cast<unsigned char>(BinarySlice2));
        int BinarySlice1 = HelperFunctions::GetBinarySlice(Inst->Value, 8, 1);
        unsigned char Byte1 = HelperFunctions::InvertByte(static_cast<unsigned char>(BinarySlice1));

        ByteList.push_back(Byte1);
        ByteList.push_back(Byte2);
    }
    else if(Inst->Type == DataType::LINE_NUM_CODE)
    {
        int BinarySlice3 = HelperFunctions::GetBinarySlice(Inst->Value, 24, 17);
        unsigned char Byte3 = HelperFunctions::InvertByte(static_cast<unsigned char>(BinarySlice3));
        int BinarySlice2 = HelperFunctions::GetBinarySlice(Inst->Value, 16, 9);
        unsigned char Byte2 = HelperFunctions::InvertByte(static_cast<unsigned char>(BinarySlice2));
        int BinarySlice1 = HelperFunctions::GetBinarySlice(Inst->Value, 8, 1);
        unsigned char Byte1 = HelperFunctions::InvertByte(static_cast<unsigned char>(BinarySlice1));
        ByteList.push_back(Byte1);
        ByteList.push_back(Byte2);
        ByteList.push_back(Byte3);
    }
    return ByteList;
}

void WriteDataInstanceToBinaryFile(ofstream * BinaryFile, DataInstance * DataInst)
{
    vector<unsigned char> ByteList = DataInstanceToBinary(DataInst);
    for(unsigned char Byte : ByteList)
    {
        BinaryFile->write(reinterpret_cast<char *>(&Byte), 1);
    }
}

void WriteZondingImpulseToBinaryFile(ofstream * BinaryFile, DataInstance * Phase, DataInstance * Sign, DataInstance * ChpLen, DataInstance * PulseType)
{
    int IntByte = (Phase->Value << 7) + (Sign->Value << 6) + (ChpLen->Value << 2) + (PulseType->Value);
    char Byte = static_cast<char>(IntByte);
    BinaryFile->write(&Byte, 1);
}

void HelperFunctions::WriteStringsToBinaryFile(vector<InfoString> Strings, string OutDir, string FileName)
{
    ofstream BinaryFile(OutDir + "/" + FileName, std::ios::out | std::ios::binary);
    if(!BinaryFile)
    {
        cerr << "Cannot open file " << OutDir << "/" << FileName << " for writing" << endl;
        return;
    }
    for(int i = 0; i < Strings.size(); i++)
    {
        InfoString * String = &Strings[i];
        vector<DataInstance> DataInstances = GetDataInstances(String);
        for(DataInstance Inst : DataInstances)
        {
            if(Inst.Type != DataType::PHASE_CODE && Inst.Type != DataType::FREQ_SIGN_CODE && Inst.Type != DataType::CHP_LEN_CODE && Inst.Type != DataType::PULSE_TYPE_CODE)
            {
                WriteDataInstanceToBinaryFile(&BinaryFile, &Inst);
            }
            else if(Inst.Type == DataType::PHASE_CODE)
            {
                WriteZondingImpulseToBinaryFile(&BinaryFile, &String->Phase, &String->FreqSign, &String->ChpLen, &String->PulseType);
            }
        }
    }
}