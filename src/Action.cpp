#include "Action.h"
#include "Auxiliary.h"
#include <iostream>
#include <vector>

using namespace std;

extern Simulation* backup;

BaseAction:: BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED){}

ActionStatus BaseAction:: getStatus() const {
    return status;
}

void BaseAction:: complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction:: error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    cout << "ERROR: " + errorMsg << endl;
}

const string& BaseAction:: getErrorMsg() const {
    return errorMsg;
}

const string BaseAction:: statusToString (ActionStatus status) const {
    string output;
    if (status == ActionStatus::COMPLETED){
        output = "COMPLETED";
    }
    else {
        output = "ERROR";
    }
    return output;
}

SimulateStep:: SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep:: act(Simulation &simulation) {
    for (int i=0; i<numOfSteps; i++){
        simulation.step();
    }
    complete();
}

const string SimulateStep:: toString() const {
    return "step " + to_string(numOfSteps) + " " + statusToString(getStatus());
}

SimulateStep* SimulateStep:: clone() const {
    SimulateStep* newS (new SimulateStep(numOfSteps));
    return newS;
}


AddPlan:: AddPlan(const string &settlementName, const string &selectionPolicy) : 
    settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan:: act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName)) {
        if (selectionPolicy=="eco"||selectionPolicy=="bal"||selectionPolicy=="nve"||selectionPolicy=="env") {
            simulation.addPlan(simulation.getSettlement(settlementName), simulation.CreatePolicy(selectionPolicy));
            complete();
        }
    }
    else {
         error("cannot create this plan");
    }
}

const string AddPlan:: toString() const {
    return "plan " + settlementName + " " + selectionPolicy + " " + statusToString(getStatus());
}

AddPlan* AddPlan:: clone() const {
    AddPlan* newP (new AddPlan(settlementName, selectionPolicy));
    return newP;
}

AddSettlement:: AddSettlement(const string &settlementName,SettlementType settlementType) :
    settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement:: act(Simulation &simulation) {
    Settlement* newS = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(newS)) {
        complete();
    }
    else {
        error("settlement already exists");
    }
}

AddSettlement* AddSettlement:: clone() const {
    AddSettlement* newSet (new AddSettlement(settlementName, settlementType));
    return newSet;
}

const string AddSettlement:: toString() const {
    return "settlement " + settlementName + " " + to_string(Auxiliary::STtoInt(settlementType)) + " " + statusToString(getStatus());
}


 AddFacility:: AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) :
    facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility:: act(Simulation &simulation) {
    FacilityType newFT = FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (simulation.addFacility(newFT)) {
        complete();
    }
    else {
        error("facility already exists");
    }
}

AddFacility* AddFacility:: clone() const {
    AddFacility* newF (new AddFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
    return newF;
}

const string AddFacility:: toString() const {
    return "facility" + facilityName + " " + Auxiliary::FCtoString(facilityCategory) + " " + to_string (price) + " " + std::to_string (lifeQualityScore) + " " + std::to_string (economyScore) + " " + std::to_string (environmentScore) + " " + statusToString(getStatus());
}


PrintPlanStatus:: PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus:: act(Simulation &simulation){
    if (simulation.planExist(planId)){
        simulation.getPlan(planId).printStatus();
        complete();
    }
    else {
        error("Plan doesn't exist");
    }    
}

PrintPlanStatus* PrintPlanStatus:: clone() const {
    PrintPlanStatus* newPr (new PrintPlanStatus(planId));
    return newPr;
}

const string PrintPlanStatus:: toString() const {
    return "planStatus " + to_string(planId) + " " + statusToString(getStatus());
}


ChangePlanPolicy:: ChangePlanPolicy(const int planId, const string &newPolicy) :
    planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy:: act(Simulation &simulation) {
    if (simulation.planExist(planId) && newPolicy != simulation.getPlan(planId).getPolicy().toString()) {
        string prevPolicy = simulation.getPlan(planId).getPolicy().toString();
         SelectionPolicy * temp = nullptr;
         Plan& p = simulation.getPlan(planId);
         if(newPolicy != "bal"){
            temp = simulation.CreatePolicy(newPolicy);
         }
         else{
            int life = p.getlifeQualityScore();
            int eco = p.getEconomyScore();
            int env = p.getEnvironmentScore();
            for(Facility* f : p.getUnderConstruction()){
                life += f->getLifeQualityScore();
                eco += f->getEconomyScore();
                env += f->getEnvironmentScore();
            }
            temp = new BalancedSelection(life,eco,env);
         }
         p.setSelectionPolicy(temp);
         cout << to_string(planId) <<endl;
         cout << "previousPolicy: " + prevPolicy << endl;
         cout << "newPolicy: " +newPolicy <<endl;
         complete();
    }
    else {
        error ("Cannot change selection policy");
    }      
}

ChangePlanPolicy* ChangePlanPolicy:: clone() const {
    ChangePlanPolicy* newC (new ChangePlanPolicy(planId, newPolicy));
    return newC;
}

const string ChangePlanPolicy:: toString() const {
    return "changePolicy " + to_string(planId) + " " + newPolicy + " " + statusToString(getStatus());
}

PrintActionsLog:: PrintActionsLog() {}

void PrintActionsLog:: act(Simulation &simulation) {
    simulation.printActions();
}

PrintActionsLog* PrintActionsLog:: clone() const {
    PrintActionsLog* newPA (new PrintActionsLog());
    return newPA;
}

const string PrintActionsLog:: toString() const {
    return "log " + statusToString(getStatus());
}

Close:: Close() {}

void Close:: act(Simulation &simulation) {
    simulation.close();
}

Close* Close:: clone() const {
    return new Close(*this);
}

const string Close:: toString() const {
    return "close " + statusToString(getStatus());
}

BackupSimulation:: BackupSimulation() {}

void BackupSimulation:: act(Simulation &simulation){
    if(backup == nullptr) backup = new Simulation (simulation);
    else *backup = simulation;
    complete();
}

BackupSimulation* BackupSimulation:: clone() const {
    return new BackupSimulation(*this);
}

const string BackupSimulation:: toString() const{
    return "backup " + statusToString(getStatus());
}


RestoreSimulation:: RestoreSimulation() {}

void RestoreSimulation:: act(Simulation &simulation){
    if (backup==nullptr){
        error("No backup available");
    }
    else {
        simulation = *backup;
        complete();
    }
}

RestoreSimulation* RestoreSimulation:: clone() const {
    RestoreSimulation* newR = new RestoreSimulation();
    if (getStatus() == ActionStatus::COMPLETED) {
        newR->complete();
    }
    else{
        newR->error(getErrorMsg());
    }
    return newR;
}

const string RestoreSimulation:: toString() const{
    return "restore " + statusToString(getStatus());
}