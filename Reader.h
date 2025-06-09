#pragma once
#include "Params.h"
#include <string>
#include <vector>

namespace Reader
{
    using namespace std;

    vector<InfoString> ReadBinaryFile(string File);
}