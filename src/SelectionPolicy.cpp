#include "SelectionPolicy.h"
#include <iostream>
#include <limits.h>
#include <algorithm>

using namespace std;

//NaiveSelection
NaiveSelection::NaiveSelection() : 
    lastSelectedIndex(-1){}

NaiveSelection:: NaiveSelection(int lastSelectedIndex): 
    lastSelectedIndex(lastSelectedIndex){}

const FacilityType& NaiveSelection:: selectFacility(const vector<FacilityType>& facilitiesOptions){
     if (facilitiesOptions.size()==0) {
        std::cout << "No facilities available to select." << std::endl;
    }
    lastSelectedIndex = (lastSelectedIndex+1) % (facilitiesOptions.size());
    return facilitiesOptions[(lastSelectedIndex)];
}

const string NaiveSelection:: toString() const {
    return "nve";
}

NaiveSelection* NaiveSelection::clone() const {
    NaiveSelection* newNve (new NaiveSelection(lastSelectedIndex));
    return newNve; 
}


//BalancedSelection
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : 
    LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) 
    {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
     if (facilitiesOptions.size()==0) {
        std::cout << "No facilities available to select." << std::endl;
    }
    int bestIndex = -1;
    int min = INT_MAX;
    int maxDiff = 0;
    int diffEcoEnv = 0;
    int diffEcoLife = 0;
    int diffEnvLife = 0;
    for (int i=0; static_cast<size_t>(i) < facilitiesOptions.size(); i++){
        diffEcoEnv = std::abs ((EconomyScore + facilitiesOptions[i].getEconomyScore()) - (EnvironmentScore + facilitiesOptions[i].getEnvironmentScore()));
        diffEcoLife = std::abs ((EconomyScore + facilitiesOptions[i].getEconomyScore()) - (LifeQualityScore + facilitiesOptions[i].getLifeQualityScore()));
        diffEnvLife = std::abs ((EnvironmentScore + facilitiesOptions[i].getEnvironmentScore()) - (LifeQualityScore + facilitiesOptions[i].getLifeQualityScore())); 
        maxDiff = std::max (diffEcoEnv, diffEcoLife);
        maxDiff = std::max (maxDiff, diffEnvLife);
        if (maxDiff < min){
            min = maxDiff;
            bestIndex = i;
        }
    }
    EconomyScore += facilitiesOptions[bestIndex].getEconomyScore();
    EnvironmentScore += facilitiesOptions[bestIndex].getEnvironmentScore();
    LifeQualityScore += facilitiesOptions[bestIndex].getLifeQualityScore();
    return facilitiesOptions[bestIndex];
}

const string BalancedSelection::toString() const {
    return "bal";
}

BalancedSelection* BalancedSelection::clone() const {
    BalancedSelection* newBal (new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore));
   return newBal;
}


//EconomySelection
EconomySelection::EconomySelection():
    lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.size()==0) {
        std::cout << "No facilities available to select." << std::endl;
    }
    for (int i=0; static_cast<size_t>(i)<facilitiesOptions.size(); i++){
        lastSelectedIndex = (lastSelectedIndex+1) % (facilitiesOptions.size());
        if (facilitiesOptions[lastSelectedIndex].getCategory()==FacilityCategory::ECONOMY){
            return facilitiesOptions[lastSelectedIndex];
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const {
    return "eco";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this); 
}

//SustainabilitySelection
SustainabilitySelection::SustainabilitySelection():
    lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.size()==0) {
        std::cout << "No facilities available to select." << std::endl;
    }
    for (int i=0; static_cast<size_t>(i)<facilitiesOptions.size(); i++){
        lastSelectedIndex = (lastSelectedIndex+1) % (facilitiesOptions.size());
        if (facilitiesOptions[lastSelectedIndex].getCategory()==FacilityCategory::ENVIRONMENT){
            return facilitiesOptions[lastSelectedIndex];
        }
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    SustainabilitySelection* newEnv(new SustainabilitySelection());
    return newEnv; 
}

