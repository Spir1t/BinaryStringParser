#pragma once
#include <string>
#include <vector>

namespace Generator
{
    using namespace std;
    constexpr int SIGMA_NOISE_START = 20;
    constexpr int SIGMA_NOISE_DEVIATION = 4;
    constexpr int RECV_ATT_START = 10;
    constexpr int RECV_ATT_DEVIATION = 4;
    constexpr int CORRUPTION_CHANCE = 10;
    constexpr int MAX_DATA_CORRUPTION = 10;
    constexpr int SAMPLE_SIZE = 1000;

    string GenerateBinaryFile(bool IsCorrupted, string OutDir, string FileName = "BinaryStrings.bin");
};