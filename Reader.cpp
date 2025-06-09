#include <fstream>
#include "Reader.h"
#include "Params.h"
#include "HelperFunctions.h"

using namespace std;

vector<InfoString> Reader::ReadBinaryFile(string File)
{
    vector<InfoString> Out;

    ifstream BinaryFile;
    BinaryFile.open(File, ios::binary);
    if(!BinaryFile)
    {
        cerr << "Reader: Cannot read input file" << endl;
        return vector<InfoString>();
    }
    BinaryFile.seekg(0, std::ios::end);
    std::streamsize size = BinaryFile.tellg();
    BinaryFile.seekg(0, std::ios::beg);

    for(int a = 0; a < size / PROTOCOL_SIZE; a++)
    {
        InfoString InfoStr;
        for(int i = 1; i <= PROTOCOL_SIZE; i++)
        {
            char LByte;
            BinaryFile.read(&LByte, 1);
            unsigned char Byte = HelperFunctions::InvertByte(LByte);

            switch(i)
            {
            case 1:
                InfoStr.SarBeam.Value += HelperFunctions::GetBinarySlice(Byte, 5, 1);
                break;
            case 2:
                InfoStr.PRP.Value += Byte;
                break;
            case 3:
                InfoStr.PRP.Value += HelperFunctions::GetBinarySlice(Byte, 2, 1) << 8;
                break;
            case 4:
                InfoStr.Delay.Value += HelperFunctions::GetBinarySlice(Byte, 6, 1);
                break;
            case 5:
                InfoStr.StartDelay.Value += Byte;
                break;
            case 6:
                InfoStr.StartDelay.Value += HelperFunctions::GetBinarySlice(Byte, 2, 1) << 8;
                break;
            case 7:
                InfoStr.EndDelay.Value += Byte;
                break;
            case 8:
                InfoStr.EndDelay.Value += HelperFunctions::GetBinarySlice(Byte, 2, 1) << 8;
                break;
            case 9:
                InfoStr.LineNum.Value += Byte;
                break;
            case 10:
                InfoStr.LineNum.Value += Byte << 8;
                break;
            case 11:
                InfoStr.LineNum.Value += HelperFunctions::GetBinarySlice(Byte, 4, 1) << 16;
                break;
            case 12:
                InfoStr.Phase.Value += HelperFunctions::GetBinarySlice(Byte, 1, 1);
                InfoStr.FreqSign.Value += HelperFunctions::GetBinarySlice(Byte, 2, 2);
                InfoStr.ChpLen.Value += HelperFunctions::GetBinarySlice(Byte, 6, 3);
                InfoStr.PulseType.Value += HelperFunctions::GetBinarySlice(Byte, 8, 7);
                break;
            case 13:
                InfoStr.SigmaNoise.Value += HelperFunctions::GetBinarySlice(Byte, 7, 1);
                break;
            case 14:
                InfoStr.ATT.Value += HelperFunctions::GetBinarySlice(Byte, 5, 1);
                break;
            }
                
        }
        Out.push_back(InfoStr);
    }
    BinaryFile.close();
    return(Out);
}
