#include "Writer.h"
#include "HelperFunctions.h"
#include <matplot/matplot.h>
#include <vector>
#include <string>

using namespace std;

static string OutDirectory;

void DrawGraph(int DataLength, vector<int> original, vector<int> corrected, string GraphName, string FileName)
{
    using namespace matplot;
    auto x = linspace(0, DataLength - 1, DataLength);
    auto y1 = original;
    auto y2 = corrected;
    plot(x, y1)->color("red").line_width(2);
    hold(on);
    plot(x, y2)->color("blue").line_width(4);
    hold(off);
    legend("Оригинальные данные", "Исправленные данные");
    title(GraphName);
    save(OutDirectory + "/" + FileName + ".png");
}

vector<int> VectorCorrectedByConstant(vector<int> vec)
{
    if(vec.empty()) return vector<int>();
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


vector<InfoString> CorrectStrings(vector<InfoString> Strings)
{
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
    vector<int> ControlVector;

    for(InfoString InfoStr : Strings)
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
        ControlVector.push_back(InfoStr.Control.Value);
    }

    SarBeamVector = VectorCorrectedByConstant(SarBeamVector);
    PRPVector = VectorCorrectedByConstant(PRPVector);
    DelayVector = VectorCorrectedByConstant(DelayVector);
    StartDelayVector = VectorCorrectedByConstant(StartDelayVector);
    EndDelayVector = VectorCorrectedByConstant(EndDelayVector);
    LineNumVector = VectorCorrectedByIncrementing(LineNumVector);
    PhaseVector = VectorCorrectedByConstant(PhaseVector, 0);
    FreqSignVector = VectorCorrectedByBinaryAlternation(FreqSignVector);
    ChpLenVector = VectorCorrectedByConstant(ChpLenVector);
    PulseTypeVector = VectorCorrectedByConstant(PulseTypeVector);
    SigmaNoiseVector = VectorCorrectedByAverageValue(SigmaNoiseVector, 20, 3);
    RecvATTVector = VectorCorrectedByAverageValue(RecvATTVector, 10, 3);
    ControlVector = VectorCorrectedByConstant(ControlVector);
    
    vector<InfoString> Out;
    for(int i = 0; i < Strings.size(); i++)
    {
        InfoString Str;
        Str.SarBeam.Value        = SarBeamVector[i];
        Str.PRP.Value            = PRPVector[i];
        Str.Delay.Value          = DelayVector[i];
        Str.StartDelay.Value     = StartDelayVector[i];
        Str.EndDelay.Value       = EndDelayVector[i];
        Str.LineNum.Value        = LineNumVector[i];
        Str.Phase.Value          = PhaseVector[i];
        Str.FreqSign.Value       = FreqSignVector[i];
        Str.ChpLen.Value         = ChpLenVector[i];
        Str.PulseType.Value      = PulseTypeVector[i];
        Str.SigmaNoise.Value     = SigmaNoiseVector[i];
        Str.ATT.Value            = RecvATTVector[i];
        Str.Control.Value        = ControlVector[i];
        Out.push_back(Str);
    }
    return Out;
}

void MakeGraphs(vector<InfoString> OriginalStrings, vector<InfoString> CorrectedStrings, string OutDir)
{
    if(OutDir.empty()) return;
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
    for(InfoString InfoStr : OriginalStrings)
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
    }

    vector<int> SarBeamVectorCorrected;
    vector<int> PRPVectorCorrected;
    vector<int> DelayVectorCorrected;
    vector<int> StartDelayVectorCorrected;
    vector<int> EndDelayVectorCorrected;
    vector<int> LineNumVectorCorrected;
    vector<int> PhaseVectorCorrected;
    vector<int> FreqSignVectorCorrected;
    vector<int> ChpLenVectorCorrected;
    vector<int> PulseTypeVectorCorrected;
    vector<int> SigmaNoiseVectorCorrected;
    vector<int> RecvATTVectorCorrected;
    for (const InfoString& InfoStr : CorrectedStrings)
    {
        SarBeamVectorCorrected.push_back(InfoStr.SarBeam.Value);
        PRPVectorCorrected.push_back(InfoStr.PRP.Value);
        DelayVectorCorrected.push_back(InfoStr.Delay.Value);
        StartDelayVectorCorrected.push_back(InfoStr.StartDelay.Value);
        EndDelayVectorCorrected.push_back(InfoStr.EndDelay.Value);
        LineNumVectorCorrected.push_back(InfoStr.LineNum.Value);
        PhaseVectorCorrected.push_back(InfoStr.Phase.Value);
        FreqSignVectorCorrected.push_back(InfoStr.FreqSign.Value);
        ChpLenVectorCorrected.push_back(InfoStr.ChpLen.Value);
        PulseTypeVectorCorrected.push_back(InfoStr.PulseType.Value);
        SigmaNoiseVectorCorrected.push_back(InfoStr.SigmaNoise.Value);
        RecvATTVectorCorrected.push_back(InfoStr.ATT.Value);
    }

    using namespace matplot;
    auto fig = figure(true);
    fig->size(1400, 600);
    matplot::grid(true);
    gca()->minor_grid(true);

    xlabel("Номер строки");
    ylabel("Значение");

    auto y1 = SarBeamVector;
    auto y2 = SarBeamVectorCorrected;
    DrawGraph(SarBeamVector.size(), y1, y2, "Код номера луча", "Sar Beam Number");

    y1 = PRPVector;
    y2 = PRPVectorCorrected;
    DrawGraph(PRPVector.size(), y1, y2, "Период повторения", "Period Of Repetition");

    y1 = DelayVector;
    y2 = DelayVectorCorrected;
    DrawGraph(DelayVector.size(), y1, y2, "Задержка отражённого сигнала", "Delay Of Reflected Signal");

    y1 = StartDelayVector;
    y2 = StartDelayVectorCorrected;
    DrawGraph(StartDelayVector.size(), y1, y2, "Задержка начала строки", "Start Delay");

    y1 = EndDelayVector;
    y2 = EndDelayVectorCorrected;
    DrawGraph(EndDelayVector.size(), y1, y2, "Задержка конца строки", "End Delay");

    y1 = LineNumVector;
    y2 = LineNumVectorCorrected;
    DrawGraph(LineNumVector.size(), y1, y2, "Код номера строки", "Line Number");

    y1 = PhaseVector;
    y2 = PhaseVectorCorrected;
    DrawGraph(PhaseVector.size(), y1, y2, "Фаза", "Phase");

    y1 = FreqSignVector;
    y2 = FreqSignVectorCorrected;
    DrawGraph(FreqSignVector.size(), y1, y2, "Знак", "Sign");

    y1 = ChpLenVector;
    y2 = ChpLenVectorCorrected;
    DrawGraph(ChpLenVector.size(), y1, y2, "Длительность импульса", "Impulse Duration");

    y1 = PulseTypeVector;
    y2 = PulseTypeVectorCorrected;
    DrawGraph(PulseTypeVector.size(), y1, y2, "Код девиации частоты", "Frequency Deviation");

    y1 = SigmaNoiseVector;
    y2 = SigmaNoiseVectorCorrected;
    DrawGraph(SigmaNoiseVector.size(), y1, y2, "Сигма шум", "Sigma Noise");

    y1 = RecvATTVector;
    y2 = RecvATTVectorCorrected;
    DrawGraph(RecvATTVector.size(), y1, y2, "Код затухания аттенюации", "RecATT");
}

void MakeBinaryOutput(vector<InfoString> Strings, string OutDir)
{
    HelperFunctions::WriteStringsToBinaryFile(Strings, OutDir, "BinaryOutput.bin");
}

void Writer::MakeOutput(vector<InfoString> Strings, string OutDir)
{
    if(Strings.empty()) return;
    vector<InfoString> CorrectedStrings = CorrectStrings(Strings);
    OutDirectory = OutDir;
    MakeGraphs(Strings, CorrectedStrings, OutDir);
    MakeBinaryOutput(CorrectedStrings, OutDir);
}
