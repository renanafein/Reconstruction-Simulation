#include "Facility.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include <iostream>

using namespace std;

 FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
    name (name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score)
    {}

 FacilityType::FacilityType(const FacilityType &other) : 
    name(other.name), category(other.category), price(other.price), lifeQuality_score(other.lifeQuality_score), economy_score(other.economy_score), environment_score(other.environment_score) 
    {}

 const string &FacilityType::getName() const{
    return name;
 }

int FacilityType::getCost() const{
    return price;
}

int FacilityType::getLifeQualityScore() const{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const{
    return environment_score;
}

int FacilityType::getEconomyScore() const{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const{
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
    FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), 
    settlementName(settlementName), 
    status(FacilityStatus::UNDER_CONSTRUCTIONS),
    timeLeft(price)
    {}

Facility::Facility(const FacilityType &type, const string &settlementName) : 
    FacilityType(type), 
    settlementName(settlementName),
    status(FacilityStatus::UNDER_CONSTRUCTIONS),
    timeLeft(price)
    {}

const string &Facility::getSettlementName() const{
    return settlementName;
}

const int Facility::getTimeLeft() const{
    return timeLeft;
}

FacilityStatus Facility::step(){
    timeLeft--;
    if (timeLeft == 0){
        setStatus(FacilityStatus::OPERATIONAL);
    }
    return status;
}

void Facility::setStatus(FacilityStatus status){
    this->status=status;
}

const FacilityStatus& Facility::getStatus() const{
    return status;
}

const string Facility:: statusToString() const{
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS) {
        return "UNDERֹ_CONSTRUCTION";
    } 
    else { 
        return "OPERATIONAL";
    } 
}

const string Facility:: toString() const{
    return "Name:" + name + "Settelment name:" + settlementName + "Category:" + Auxiliary::FCtoString(category) + "Price:" + to_string(price) + "Life quality score:" + to_string(lifeQuality_score) + "Economy score:" + to_string(economy_score) + "Environment score:" + to_string(environment_score) + "Time left:" + to_string(timeLeft) + "Status:" + statusToString();
}