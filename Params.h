#pragma once
#include <iostream>
#include <vector>

#define PROTOCOL_SIZE 15

enum class DataType
{
    SAR_BEAM_CODE = 0,
    PRP_CODE,
    DELAY_CODE,
    START_DELAY_CODE,
    END_DELAY_CODE,
    LINE_NUM_CODE,
    PHASE_CODE,
    FREQ_SIGN_CODE,
    CHP_LEN_CODE,
    PULSE_TYPE_CODE,
    SIGMA_NOISE_CODE,
    RECV_ATT_CODE,
    CONTROL_CODE,
};

using range_t = std::pair<size_t, size_t>;
constexpr range_t SAR_BEAM_CODE          = {1, 25};
constexpr range_t PRP_CODE               = {100, 1000};
constexpr range_t DELAY_CODE             = {3, 34};
constexpr range_t START_DELAY_CODE       = {21, 959};
constexpr range_t END_DELAY_CODE         = {61, 999};
constexpr range_t LINE_NUM_CODE          = {1, 1048575};
constexpr range_t PHASE_CODE             = {0, 1};
constexpr range_t FREQ_SIGN_CODE         = {0, 1};
constexpr range_t CHP_LEN_CODE           = {0, 12};
constexpr range_t PULSE_TYPE_CODE        = {0, 2};
constexpr range_t SIGMA_NOISE_CODE       = {0, 127};
constexpr range_t RECV_ATT_CODE          = {0, 31};
constexpr range_t CONTROL_CODE           = {0, 1};

struct DataInstance
{
    unsigned int Value;
    DataType Type;
    range_t Interval;
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

    InfoString()
    {
        SarBeam = {0, DataType::SAR_BEAM_CODE, SAR_BEAM_CODE};
        PRP = {0, DataType::PRP_CODE, PRP_CODE};
        Delay = {0, DataType::DELAY_CODE, DELAY_CODE};
        StartDelay = {0, DataType::START_DELAY_CODE, START_DELAY_CODE};
        EndDelay = {0, DataType::END_DELAY_CODE, END_DELAY_CODE};
        LineNum = {0, DataType::LINE_NUM_CODE, LINE_NUM_CODE};
        Phase = {0, DataType::PHASE_CODE, PHASE_CODE};
        FreqSign = {0, DataType::FREQ_SIGN_CODE, FREQ_SIGN_CODE};
        ChpLen = {0, DataType::CHP_LEN_CODE, CHP_LEN_CODE};
        PulseType = {0, DataType::PULSE_TYPE_CODE, PULSE_TYPE_CODE};
        SigmaNoise = {0, DataType::SIGMA_NOISE_CODE, SIGMA_NOISE_CODE};
        ATT = {0, DataType::RECV_ATT_CODE, RECV_ATT_CODE};
        Control = {0, DataType::CONTROL_CODE, CONTROL_CODE};
    };
};

std::vector<DataInstance> GetDataInstances(InfoString * Str);