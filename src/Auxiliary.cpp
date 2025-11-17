#include "Auxiliary.h"
#include "Settlement.h"
#include "Facility.h"
#include <string>

using namespace std;
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/
std::vector<std::string> Auxiliary::parseArguments(const std::string& line) {
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument) {
        arguments.push_back(argument);
    }

    return arguments;
}

const string Auxiliary::FCtoString(FacilityCategory category) {
     string output = "env";
    if(category==FacilityCategory::ECONOMY){
        output = "eco";
    }
    else if(category==FacilityCategory::LIFE_QUALITY){
        output = "life";
    }
    return output;
}

const int Auxiliary::STtoInt(SettlementType type) {
    int prinType= -1;

    if (type == SettlementType::VILLAGE) {
        prinType = 0;
    }
    else if (type == SettlementType::METROPOLIS) {
        prinType = 2;
    }
    else if (type == SettlementType::CITY) {
        prinType = 1;
    }
    return prinType;
}