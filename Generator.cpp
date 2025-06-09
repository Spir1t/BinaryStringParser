#include <ctime>
#include <fstream>
#include "Params.h"
#include "Generator.h"
#include "HelperFunctions.h"

using namespace std;

vector<InfoString> GenerateCorrectStrings()
{
    vector<InfoString> Out;
    srand(std::time({}));

    const unsigned int SarBeamStart     = (rand() % SAR_BEAM_CODE.second)   + SAR_BEAM_CODE.first;
    const unsigned int PRPStart         = (rand() % PRP_CODE.second)        + PRP_CODE.first;
    const unsigned int DelayStart       = (rand() % DELAY_CODE.second)      + DELAY_CODE.first;
    const unsigned int StartDelayStart  = (rand() % START_DELAY_CODE.second)+ START_DELAY_CODE.first;
    const unsigned int EndDelayStart    = (rand() % (END_DELAY_CODE.second - StartDelayStart)) 
                                                                            + StartDelayStart;
    const unsigned int LineNumStart     = (rand() % LINE_NUM_CODE.second)   + LINE_NUM_CODE.first;
    const unsigned int FreqSignStart    = (rand() % FREQ_SIGN_CODE.second)  + FREQ_SIGN_CODE.first;
    const unsigned int ChpLenStart      = (rand() % CHP_LEN_CODE.second)    + CHP_LEN_CODE.first;
    const unsigned int PulseTypeStart   = (rand() % PULSE_TYPE_CODE.second) + PULSE_TYPE_CODE.first;
    const unsigned int SigmaNoiseStart  = Generator::SIGMA_NOISE_START;
    const unsigned int ATTStart         = Generator::RECV_ATT_START;
    const unsigned int ControlStart     = (rand() % CONTROL_CODE.second)    + CONTROL_CODE.first;

    int NewLineNum = LineNumStart;
    int NewFreqSign = FreqSignStart;
    for(int i = 0; i < Generator::SAMPLE_SIZE; i++)
    {
        InfoString Str;
        const unsigned int SigmaNoiseDeviation = rand() % Generator::SIGMA_NOISE_DEVIATION;
        const unsigned int ATTDeviation = rand() % Generator::RECV_ATT_DEVIATION;

        Str.SarBeam.Value     = SarBeamStart;
        Str.PRP.Value         = PRPStart;
        Str.Delay.Value       = DelayStart;
        Str.StartDelay.Value  = StartDelayStart;
        Str.EndDelay.Value    = EndDelayStart;
        Str.LineNum.Value     = NewLineNum;
        Str.Phase.Value       = 0;
        Str.FreqSign.Value    = NewFreqSign;
        Str.ChpLen.Value      = ChpLenStart;
        Str.PulseType.Value   = PulseTypeStart;
        Str.SigmaNoise.Value  = SigmaNoiseStart + SigmaNoiseDeviation;
        Str.ATT.Value         = ATTStart + ATTDeviation;
        Str.Control.Value     = ControlStart;
        Out.push_back(Str);

        NewLineNum++;
        NewFreqSign = (NewFreqSign + 1) % 2;
    }
    return Out;
}

int CorruptData(int Data)
{
    int Probability = rand() % 100;
    int Sign = (rand() % 2 == 0) ? -1 : 1;
    if(Probability < Generator::CORRUPTION_CHANCE)
    {
        int Corruption = Sign * (rand() % Generator::MAX_DATA_CORRUPTION);
        return(Data + Corruption);
    }
    return Data;
}

vector<InfoString> GenerateIncorrectStrings()
{
    vector<InfoString> Out;
    srand(std::time({}));

    const int SarBeamStart     = (rand() % SAR_BEAM_CODE.second)   + SAR_BEAM_CODE.first;
    const int PRPStart         = (rand() % PRP_CODE.second)        + PRP_CODE.first;
    const int DelayStart       = (rand() % DELAY_CODE.second)      + DELAY_CODE.first;
    const int StartDelayStart  = (rand() % START_DELAY_CODE.second)+ START_DELAY_CODE.first;
    const int EndDelayStart    = (rand() % (END_DELAY_CODE.second - StartDelayStart)) 
                                                                            + StartDelayStart;
    const int LineNumStart     = (rand() % LINE_NUM_CODE.second)   + LINE_NUM_CODE.first;
    const int FreqSignStart          = (rand() % FREQ_SIGN_CODE.second)  + FREQ_SIGN_CODE.first;
    const int ChpLenStart      = (rand() % CHP_LEN_CODE.second)    + CHP_LEN_CODE.first;
    const int PulseTypeStart   = (rand() % PULSE_TYPE_CODE.second) + PULSE_TYPE_CODE.first;
    const int SigmaNoiseStart  = Generator::SIGMA_NOISE_START;
    const int ATTStart         = Generator::RECV_ATT_START;
    const int ControlStart     = (rand() % CONTROL_CODE.second)    + CONTROL_CODE.first;

    int NewLineNum = LineNumStart;
    int NewFreqSign = FreqSignStart;
    for(int i = 0; i < Generator::SAMPLE_SIZE; i++)
    {
        InfoString Str;
        const unsigned int SigmaNoiseDeviation = rand() % Generator::SIGMA_NOISE_DEVIATION;
        const unsigned int ATTDeviation = rand() % Generator::RECV_ATT_DEVIATION;

        Str.SarBeam.Value     = CorruptData(SarBeamStart);
        Str.PRP.Value         = CorruptData(PRPStart);
        Str.Delay.Value       = CorruptData(DelayStart);
        Str.StartDelay.Value  = CorruptData(StartDelayStart);
        Str.EndDelay.Value    = CorruptData(EndDelayStart);
        Str.LineNum.Value     = CorruptData(NewLineNum);
        Str.Phase.Value       = CorruptData(0);
        Str.FreqSign.Value    = CorruptData(NewFreqSign);
        Str.ChpLen.Value      = CorruptData(ChpLenStart);
        Str.PulseType.Value   = CorruptData(PulseTypeStart);
        Str.SigmaNoise.Value  = CorruptData(SigmaNoiseStart + SigmaNoiseDeviation);
        Str.ATT.Value         = CorruptData(ATTStart + ATTDeviation);
        Str.Control.Value     = CorruptData(ControlStart);
        Out.push_back(Str);

        NewLineNum++;
        NewFreqSign = (NewFreqSign + 1) % 2;
    }
    return Out;
}

string Generator::GenerateBinaryFile(bool IsCorrupted, string OutDir, string FileName)
{
    vector<InfoString> Strings;
    IsCorrupted ? Strings = GenerateIncorrectStrings() : Strings = GenerateCorrectStrings();
    HelperFunctions::WriteStringsToBinaryFile(Strings, OutDir, FileName);
    return(OutDir + "/" + FileName);
}