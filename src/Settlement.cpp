#include "Settlement.h"
#include <iostream>

using namespace std;

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {};

const string &Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

int Settlement::getLimit() const {
    if (type == SettlementType::METROPOLIS){
        return 3;
    }
    else if (type == SettlementType::CITY){
        return 2;
    } 
    else {
        return 1;
    }
}

const string Settlement::toString() const {
    string prinType;

    if (type == SettlementType::VILLAGE) {
        prinType = "Village";
    }
    else if (type == SettlementType::METROPOLIS) {
        prinType = "Metropolis";
    }
    else if (type == SettlementType::CITY) {
        prinType = "City";
    }
    return prinType;
}

Settlement* Settlement:: clone() const {
    return new Settlement(*this);
}