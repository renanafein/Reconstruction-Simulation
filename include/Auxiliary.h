#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "Settlement.h"
#include "Facility.h"

using namespace std;

class Auxiliary{
    public:
        static std::vector<std::string> parseArguments(const std::string& line);
        static const string FCtoString(FacilityCategory category);
        static const int STtoInt(SettlementType type);
        
};
