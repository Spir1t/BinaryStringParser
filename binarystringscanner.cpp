#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include "binarystringscanner.h"
#include <matplot/matplot.h>
#define PROTOCOL_SIZE 15

using namespace std;

struct DataInstance
{
    int Value;
    string Name;
};

struct InfoString
{
    DataInstance SarBeam;
    DataInstance PRP;
    DataInstance Delay;
    DataInstance StartDelay;
    DataInstance EndDelay;
    DataInstance LineNum;
    DataInstance Phase;
    DataInstance FreqSign;
    DataInstance ChpLen;
    DataInstance PulseType;
    DataInstance SigmaNoise;
    DataInstance ATT;
    DataInstance Control;

};

unsigned char InvertByte(char SignedByte)
{
    //Byte structure: n8 n7 n6 n5 n4 n3 n2 n1
    //Inverted byte structure: n1 n2 n3 n4 n5 n6 n7 n8
    unsigned char Byte = SignedByte;
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

int GetByteSlice(unsigned char Byte, int Start, int End)
{
    // Byte structure: n8 n7 n6 n5 n4 n3 n2 n1
    // max start is 1, Min end is 1
    int Front = (Byte >> Start) << Start;
    return((Byte - Front) >> (End - 1));
}

string toBinary(int n) {
    if (n == 0) return "0";

    string binary;
    while(n > 0)
    {
        binary = (n % 2 == 0 ? "0" : "1") + binary;
        n /= 2;
    }
    return binary;
  }

vector<InfoString> ReadBinaryFile(string BinInput)
{
    vector<InfoString> Out;

    ifstream BinaryTest;
    BinaryTest.open(BinInput, ios::binary);
    BinaryTest.seekg(0, std::ios::end);
    std::streamsize size = BinaryTest.tellg();
    BinaryTest.seekg(0, std::ios::beg);

    ifstream File;
    File.open(BinInput, ios::binary);
    for(int a = 0; a < size / PROTOCOL_SIZE; a++)
    {
        InfoString InfoStr;
        for(int i = 1; i <= PROTOCOL_SIZE; i++)
        {
            unsigned char LByte;
            File.read(reinterpret_cast<char *>(&LByte), 1);
            unsigned char Byte = InvertByte(LByte); //проблема с отрицательными чарами

            switch(i)
            {
            case 1:
                InfoStr.SarBeam.Value += GetByteSlice(Byte, 5, 1);
                break;
            case 2:
                InfoStr.PRP.Value += Byte;
                break;
            case 3:
                InfoStr.PRP.Value += GetByteSlice(Byte, 2, 1) << 8;
                break;
            case 4:
                InfoStr.Delay.Value += GetByteSlice(Byte, 6, 1);
                break;
            case 5:
                InfoStr.StartDelay.Value += Byte;
                break;
            case 6:
                InfoStr.StartDelay.Value += GetByteSlice(Byte, 2, 1) << 8;
                break;
            case 7:
                InfoStr.EndDelay.Value += Byte;
                break;
            case 8:
                InfoStr.EndDelay.Value += GetByteSlice(Byte, 2, 1) << 8;
                break;
            case 9:
                InfoStr.LineNum.Value += Byte;
                break;
            case 10:
                InfoStr.LineNum.Value += Byte << 8;
                break;
            case 11:
                InfoStr.LineNum.Value += GetByteSlice(Byte, 4, 1) << 16;
                break;
            case 12:
                InfoStr.Phase.Value += GetByteSlice(Byte, 1, 1);
                InfoStr.FreqSign.Value += GetByteSlice(Byte, 2, 2);
                InfoStr.ChpLen.Value += GetByteSlice(Byte, 6, 3);
                InfoStr.PulseType.Value += GetByteSlice(Byte, 8, 7);
                break;
            case 13:
                InfoStr.SigmaNoise.Value += GetByteSlice(Byte, 7, 1);
                break;
            case 14:
                InfoStr.ATT.Value += GetByteSlice(Byte, 5, 1);
                break;
            }

        }
        Out.push_back(InfoStr);
    }
    File.close();
    return Out;
}

vector<int> VectorCorrectedByConstant(vector<int> vec)
{
    unordered_map<int, int> freq;
    for (int val : vec)
    {
        freq[val]++;
    }

    //find constant value
    int most_common = vec[0];
    int max_count = 0;

    for(const auto& [key, count] : freq)
    {
        if(count > max_count)
        {
            max_count = count;
            most_common = key;
        }
    }

    //change deviations
    return(vector<int>(vec.size(), most_common));
}

vector<int> VectorCorrectedByConstant(vector<int> vec, const int constant)
{
    return vector<int>(vec.size(), constant);
}

vector<int> VectorCorrectedByBinaryAlternation(vector<int> vec)
{
    //BinaryAlternation - 0101010101 etc.
    vector<int> out;
    int PredictedValue = vec[0];
    for(int val : vec)
    {
        val == PredictedValue ? out.push_back(val) : out.push_back(PredictedValue);
        PredictedValue == 0 ? PredictedValue = 1 : PredictedValue = 0;
    }
    return out;
}

vector<int> VectorCorrectedByIncrementing(vector<int> vec)
{
    vector<int> out;
    int PredictedValue = vec[0];
    for(int val : vec)
    {
        val == PredictedValue ? out.push_back(val) : out.push_back(PredictedValue);
        PredictedValue++;
    }
    return out;
}

vector<int> VectorCorrectedByAverageValue(vector<int> vec, float average, float deviation)
{
    vector<int> out;
    float MaxVal = average + deviation;
    float MinVal = average - deviation;
    for(int val : vec)
    {
        (val <= MaxVal && val >= MinVal ) ? out.push_back(val) : out.push_back(average);
    }
    return out;
}

void DrawGraph(int DataLength, vector<int> original, vector<int> corrected, string GraphName, string FileName, string OutputDir)
{
    using namespace matplot;
    auto x = linspace(0, DataLength - 1, DataLength);
    auto y1 = original;
    auto y2 = corrected;
    plot(x, y2)->color("green").line_width(4);
    hold(on);
    plot(x, y1)->color("red").line_width(2);
    hold(off);
    legend("Исправленные данные", "Оригинальные данные");
    title(GraphName);
    save(OutputDir + "/" + FileName + ".png");
}

void GenerateImages(const string BinInput, const string OutputDir)
{

    ifstream file(BinInput);
    if(!file.is_open())
    {
        cout << "no" << endl;
        return;
    }
    file.close();

    vector<InfoString> InfoStringVector = ReadBinaryFile(BinInput);
    int count = 1;

    vector<int> SarBeamVector;
    vector<int> PRPVector;
    vector<int> DelayVector;
    vector<int> StartDelayVector;
    vector<int> EndDelayVector;
    vector<int> LineNumVector;
    vector<int> PhaseVector;
    vector<int> FreqSignVector;
    vector<int> ChpLenVector;
    vector<int> PulseTypeVector;
    vector<int> SigmaNoiseVector;
    vector<int> RecvATTVector;

    for(InfoString InfoStr : InfoStringVector)
    {
        SarBeamVector.push_back(InfoStr.SarBeam.Value);
        PRPVector.push_back(InfoStr.PRP.Value);
        DelayVector.push_back(InfoStr.Delay.Value);
        StartDelayVector.push_back(InfoStr.StartDelay.Value);
        EndDelayVector.push_back(InfoStr.EndDelay.Value);
        LineNumVector.push_back(InfoStr.LineNum.Value);
        PhaseVector.push_back(InfoStr.Phase.Value);
        FreqSignVector.push_back(InfoStr.FreqSign.Value);
        ChpLenVector.push_back(InfoStr.ChpLen.Value);
        PulseTypeVector.push_back(InfoStr.PulseType.Value);
        SigmaNoiseVector.push_back(InfoStr.SigmaNoise.Value);
        RecvATTVector.push_back(InfoStr.ATT.Value);

        count++;
    }

    using namespace matplot;
    auto fig = figure(true);
    fig->size(1400, 600);

    xlabel("Выборка строк");
    ylabel("Значение");

    auto y1 = SarBeamVector;
    auto y2 = VectorCorrectedByConstant(SarBeamVector);
    DrawGraph(SarBeamVector.size(), y1, y2, "Код номера луча", "Sar Beam Number", OutputDir);

    y1 = PRPVector;
    y2 = VectorCorrectedByConstant(PRPVector);
    DrawGraph(PRPVector.size(), y1, y2, "Период повторения", "Period Of Repetition", OutputDir);

    y1 = DelayVector;
    y2 = VectorCorrectedByConstant(DelayVector);
    DrawGraph(DelayVector.size(), y1, y2, "Задержка отражённого сигнала", "Delay Of Reflected Signal", OutputDir);

    y1 = StartDelayVector;
    y2 = VectorCorrectedByConstant(StartDelayVector);
    DrawGraph(StartDelayVector.size(), y1, y2, "Задержка начала строки", "Start Delay", OutputDir);

    y1 = EndDelayVector;
    y2 = VectorCorrectedByConstant(EndDelayVector);
    DrawGraph(EndDelayVector.size(), y1, y2, "Задержка конца строки", "End Delay", OutputDir);

    y1 = LineNumVector;
    y2 = VectorCorrectedByIncrementing(LineNumVector);
    DrawGraph(LineNumVector.size(), y1, y2, "Код номера строки", "Line Number", OutputDir);

    y1 = PhaseVector;
    y2 = VectorCorrectedByConstant(PhaseVector, 0);
    DrawGraph(PhaseVector.size(), y1, y2, "Фаза", "Phase", OutputDir);

    y1 = FreqSignVector;
    y2 = VectorCorrectedByBinaryAlternation(FreqSignVector);
    DrawGraph(FreqSignVector.size(), y1, y2, "Знак", "Sign", OutputDir);

    y1 = ChpLenVector;
    y2 = VectorCorrectedByConstant(ChpLenVector);
    DrawGraph(ChpLenVector.size(), y1, y2, "Длительность импульса", "Impulse Duration", OutputDir);

    y1 = PulseTypeVector;
    y2 = VectorCorrectedByConstant(PulseTypeVector);
    DrawGraph(PulseTypeVector.size(), y1, y2, "Код девиации частоты", "Frequency Deviation", OutputDir);

    y1 = SigmaNoiseVector;
    y2 = VectorCorrectedByAverageValue(SigmaNoiseVector, 20, 3);
    DrawGraph(SigmaNoiseVector.size(), y1, y2, "Сигма шум", "Sigma Noise", OutputDir);

    y1 = RecvATTVector;
    y2 = VectorCorrectedByAverageValue(RecvATTVector, 10, 3);
    DrawGraph(RecvATTVector.size(), y1, y2, "Код затухания аттенюации", "RecATT", OutputDir);
}

void BinaryStringScanner::Start(const QString InputPath, const QString OutputDir)
{
    GenerateImages(InputPath.toStdString(), OutputDir.toStdString());
}
