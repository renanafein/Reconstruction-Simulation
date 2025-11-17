#include "Plan.h"
#include <iostream>
#include <vector>

using namespace std;
//constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const std::vector<FacilityType> &facilityOptions) : 
    plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilities(), underConstruction(), facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) 
    {
     std::vector<Facility>facilities;
     std::vector<Facility>underConstruction;
    }
Plan::Plan(const Plan& other, const Settlement& settlement):
    plan_id(other.plan_id), 
    settlement(settlement),
    selectionPolicy(other.selectionPolicy->clone()), 
    status(other.status), 
    facilities(),
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score), 
    economy_score(other.economy_score), 
    environment_score(other.environment_score) {
        for (Facility* tmp: other.facilities){
            facilities.push_back(new Facility(*tmp));
        }
        for (Facility* tmp: other.underConstruction){
            Facility* f = new Facility (*tmp);
            underConstruction.push_back(f);
        }

}

//copy constructor
Plan:: Plan (const Plan &other) : 
    plan_id(other.plan_id), 
    settlement(other.settlement),
    selectionPolicy(other.selectionPolicy->clone()), 
    status(other.status), 
    facilities(),
    underConstruction(),
    facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score), 
    economy_score(other.economy_score), 
    environment_score(other.environment_score) {
        for (Facility* tmp: other.facilities){
            facilities.push_back(new Facility(*tmp));
        }
        for (Facility* tmp: other.underConstruction){
            Facility* f = new Facility (*tmp);
            underConstruction.push_back(f);
        }
    }
//destructor
Plan:: ~Plan() {
    if (selectionPolicy) {
        delete selectionPolicy;
        selectionPolicy = nullptr;
    }
    for (Facility* tmp: facilities) {
        if (tmp) {
            delete tmp;
            tmp = nullptr;
        }
    }
    facilities.clear();
    for (Facility* tmp: underConstruction) {
        if (tmp) {
            delete tmp;
            tmp = nullptr;
        }
    }
    underConstruction.clear();
}
//move constructor
Plan:: Plan (Plan&& other) : 
    plan_id(other.plan_id), 
    settlement(other.settlement), 
    selectionPolicy(other.selectionPolicy), 
    status(other.status),
    facilities(),
    underConstruction(),
    facilityOptions(other.facilityOptions),  
    life_quality_score(other.life_quality_score), 
    economy_score(other.economy_score), 
    environment_score(other.environment_score) {
    other.selectionPolicy = nullptr;
    for(Facility* fac:other.facilities){
        facilities.push_back(fac);
        fac = nullptr;
    }
    other.facilities.clear();
    for(Facility* UC:other.underConstruction){
        underConstruction.push_back(UC);
        UC = nullptr;
    }
    other.underConstruction.clear();
}

const int Plan:: getlifeQualityScore() const{
    return life_quality_score;
}

const int Plan:: getEconomyScore() const{
    return economy_score;
}

const int Plan:: getEnvironmentScore() const{
    return environment_score;
}

const int Plan:: getPlanId() const{
    return plan_id;
}

const string Plan:: getSettlementName() const {
    return settlement.getName();
}

const SelectionPolicy& Plan:: getPolicy() const {
    return *selectionPolicy;
}

void Plan:: setSelectionPolicy(SelectionPolicy *selectionPolicy){
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
}

void Plan:: step(){
    if (status == PlanStatus::AVALIABLE){
         while (underConstruction.size() < static_cast<std::vector<Facility*>::size_type> (settlement.getLimit()))
        {
            Facility* f = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
            addFacility(f);
        }  
    }
    for (unsigned int i = 0; i < underConstruction.size(); i++){
        if (underConstruction[i]->step() == FacilityStatus::OPERATIONAL){
            facilities.push_back(underConstruction[i]);
            life_quality_score += underConstruction[i]->getLifeQualityScore();
            economy_score += underConstruction[i]->getEconomyScore();
            environment_score += underConstruction[i]->getEnvironmentScore();
            underConstruction.erase(underConstruction.begin()+i);
            i--;
        }
    }
    if (underConstruction.size() < static_cast<std::vector<Facility*>::size_type> (settlement.getLimit())){
        status = PlanStatus::AVALIABLE;
    }
    else {
        status = PlanStatus::BUSY;
    }
}

void Plan:: printStatus() {
    cout << "PlanID: " << to_string(plan_id) << endl;
    cout << "SettlementName: " + settlement.getName() << endl;
    cout << "PlanStatus: " + statusToString() << endl;
    cout << "SelectionPolicy: " << selectionPolicy->toString() << endl;
    cout << "LifeQualityScore: " + to_string(life_quality_score) << endl;
    cout << "EconomyScore: " + to_string(economy_score) << endl;
    cout << "EnvironmentScore: " + to_string(environment_score) << endl;   
    for (Facility* tmp: underConstruction) {
        cout << "FacilityName: " + tmp->getName() << endl;
        cout << "FacilityStatus: UNDER CONSTRACTION" << endl;
    }
    for (Facility* tmp: facilities) {
        cout << "FacilityName: " + tmp->getName() << endl;
        cout << "FacilityStatus: OPERATIONAL" << endl;
    }
}


const vector<Facility*>& Plan:: getFacilities() const{
    return facilities;
}

void Plan:: addFacility(Facility* facility){
    if (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS){
        underConstruction.push_back(facility);
    }
    else{
        facilities.push_back(facility);
    }
}

const string Plan:: toString() const{
    return ("Plan ID: " + to_string(plan_id) +
            "\nsettlementName: " + settlement.getName() + 
            "\nLifeQualityScore: " + to_string(life_quality_score) +
            "\nEconomyScore: " + to_string(economy_score) + 
            "\nEnvironmentScore: " +  to_string(environment_score));
}

const string Plan:: statusToString() const{
    if (status == PlanStatus::AVALIABLE) {
        return "AVAILABLE";
    }
    else {
        return "BUSY";
    }
}

const vector<Facility*> &Plan::getUnderConstruction(){
    return underConstruction;
}
