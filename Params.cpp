#include "Params.h"

std::vector<DataInstance> GetDataInstances(InfoString * Str)
{
    std::vector<DataInstance> Out;
    Out.push_back(Str->SarBeam);
    Out.push_back(Str->PRP);
    Out.push_back(Str->Delay);
    Out.push_back(Str->StartDelay);
    Out.push_back(Str->EndDelay);
    Out.push_back(Str->LineNum);
    Out.push_back(Str->Phase);
    Out.push_back(Str->FreqSign);
    Out.push_back(Str->ChpLen);
    Out.push_back(Str->PulseType);
    Out.push_back(Str->SigmaNoise);
    Out.push_back(Str->ATT);
    Out.push_back(Str->Control);
    return Out;
}