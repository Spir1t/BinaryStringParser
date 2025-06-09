#pragma once
#include <vector>
#include <Params.h>

namespace HelperFunctions
{
    using namespace std;
    unsigned char InvertByte(char SignedByte);
    int GetBinarySlice(int Num, int FromBit, int ToBit);
    void WriteStringsToBinaryFile(vector<InfoString> Strings, string OutDir, string FileName);
}
