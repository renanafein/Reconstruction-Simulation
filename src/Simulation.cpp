#include "Simulation.h"
#include "Auxiliary.h"
#include "Action.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

Simulation:: Simulation (const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions() {
    string text;
    ifstream myFile (configFilePath);

    while (getline(myFile, text)){
        vector<std::string> arguments = Auxiliary::parseArguments(text);
        if (arguments[0] == "settlement"){
            SettlementType type;
            if (arguments[2] == "0"){
                type = SettlementType::VILLAGE;
            }
            else if (arguments[2] == "1"){
                type = SettlementType::CITY;
            }
            else {
                type = SettlementType::METROPOLIS;
            }
            Settlement *newS = new Settlement(arguments[1], type);
            if(!addSettlement(newS)){
                std::cout << "The settlement already exists" << std::endl;
            }
            newS = nullptr;
        }
        else if (arguments[0] == "facility"){
            FacilityCategory category;
            if (arguments[2] == "0"){
                category = FacilityCategory::LIFE_QUALITY;
            }
            else if (arguments[2] == "1"){
                category = FacilityCategory::ECONOMY;
            }
            else{
                category = FacilityCategory::ENVIRONMENT;
            }
            FacilityType newF = FacilityType(arguments[1], category, std::stoi(arguments[3]), std::stoi(arguments[4]), std::stoi(arguments[5]), std::stoi(arguments[6]));
            if(!addFacility(newF)){
                std::cout << "The facility already exists" << std::endl;
            }
        }
        else if (arguments[0] == "plan"){
            if (isSettlementExists(arguments[1])) {
               addPlan(getSettlement(arguments[1]),CreatePolicy(arguments[2]));
            }
            else {
                 std::cout << "The Settlement is not exists" << std::endl;
            }
        }
    }
    myFile.close();
}
//deep copy constructor
Simulation:: Simulation(const Simulation &other) : 
isRunning(other.isRunning), 
planCounter (other.planCounter),  
actionsLog(),
plans(),
settlements(),
facilitiesOptions (other.facilitiesOptions)
{
    for(BaseAction* action: other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
    for (Settlement* sett:other.settlements)
    {
        settlements.push_back(sett->clone());
    }
     for(const Plan& plan:other.plans){
        plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
    }
}


//destructor
Simulation:: ~Simulation(){
    for(BaseAction* action :actionsLog){
        delete action;
    }
    actionsLog.clear();
    for(Settlement* settlement: settlements){
        delete settlement;
    }
    settlements.clear();
}

Simulation:: Simulation (Simulation&& other) :
    isRunning(other.isRunning), 
    planCounter (other.planCounter),  
    actionsLog(),
    plans(),
    settlements(),
    facilitiesOptions (other.facilitiesOptions) {
        for (BaseAction* actL: other.actionsLog){
            actionsLog.push_back(actL);
            actL = nullptr;
        }
        other.actionsLog.clear();
        for (Settlement* sett: other.settlements){
            settlements.push_back(sett);
            sett = nullptr;
        }
        other.settlements.clear();
        for(const Plan& plan:other.plans){
            plans.push_back(Plan(plan));
        }
        
    }

void Simulation:: clear() {
     facilitiesOptions.clear();
     for(BaseAction* action :actionsLog){
        delete action;
    }
    actionsLog.clear();
    for(Settlement* settlement: settlements){
        delete settlement;
    }
    settlements.clear();
    plans.clear();
}

Simulation& Simulation:: operator = (const Simulation& other){
    if (this != &other) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        plans.clear();
        for (Settlement* sett: this->settlements){
            delete sett;
        }
        this->settlements.clear();
        facilitiesOptions.clear();
        for (BaseAction* actL: this->actionsLog){
            delete actL;
        }
        this->actionsLog.clear();
        for (Settlement* sett: other.settlements){
            settlements.push_back(sett->clone());
        }
        for (FacilityType fac: other.facilitiesOptions) {
            facilitiesOptions.push_back(fac);
        }
        for (BaseAction* actL: other.actionsLog){
            actionsLog.push_back(actL->clone());
        }
        for(const Plan& plan:other.plans){
            plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
        }
    }
    return *this;
   
}

Simulation& Simulation:: operator = (Simulation&& other){
    if(this != &other){
        isRunning = false;
        planCounter = other.planCounter;
        facilitiesOptions.clear();
        for (FacilityType fac: other.facilitiesOptions) {
            facilitiesOptions.push_back(fac);
        }
        for (BaseAction* actL: this->actionsLog){
            delete actL;
        }
        this->actionsLog.clear();
        for (BaseAction* actL: other.actionsLog){
            actionsLog.push_back(actL);
            actL = nullptr;
        }
        other.actionsLog.clear();
        for (Settlement* sett: settlements){
            delete sett;
        }
        settlements.clear();
        for (Settlement* sett: other.settlements){
            settlements.push_back(sett);
            sett = nullptr;
        }
        other.settlements.clear();
        plans.clear();
        for(const Plan& plan:other.plans){
            plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
        }
    }
    return *this;
} 

void Simulation:: start(){
    open();
    cout << "The simulation has started" << endl;
    while (isRunning) {
        string input;
        std:: getline(std:: cin,input);
        vector<string> arguments = Auxiliary:: parseArguments(input);
        if (arguments[0] == "settlement"){
            SettlementType type;
            if (arguments[2] == "0"){
                type = SettlementType::VILLAGE;
            }
            else if (arguments[2] == "1"){
                type = SettlementType::CITY;
            }
            else {
                type = SettlementType::METROPOLIS;
            }
            AddSettlement* addS =  new AddSettlement(arguments[1], type);
            addS->act(*this);
            actionsLog.push_back(addS);
            addS = nullptr;
        }
        else if (arguments[0] == "facility"){
            FacilityCategory category;
            if (arguments[2] == "0"){
                category = FacilityCategory::LIFE_QUALITY;
            }
            else if (arguments[2] == "1"){
                category = FacilityCategory::ECONOMY;
            }
            else{
                category = FacilityCategory::ENVIRONMENT;
            }
            AddFacility* addF = new AddFacility(arguments[1], category, std::stoi(arguments[3]), std::stoi(arguments[4]), std::stoi(arguments[5]), std::stoi(arguments[6]));
            addF->act(*this);
            actionsLog.push_back(addF);
        }
        else if (arguments[0] == "plan"){
            AddPlan* addP = new AddPlan (arguments[1], arguments[2]);
            addP->act(*this);
            actionsLog.push_back(addP);
        }
        else if (arguments[0] == "step") {
            SimulateStep* newStep = new SimulateStep(stoi(arguments[1]));
            newStep->act(*this);
            actionsLog.push_back(newStep);
        }
        else if (arguments[0] == "planStatus") {
            PrintPlanStatus* printP = new PrintPlanStatus(stoi(arguments[1]));
            printP->act(*this);
            actionsLog.push_back(printP);
        }
        else if (arguments[0] == "changePolicy") {
            ChangePlanPolicy* changeP = new ChangePlanPolicy(stoi(arguments[1]), arguments[2]);
            changeP->act(*this);
            actionsLog.push_back(changeP);
        }
        else if (arguments[0] == "log") {
            PrintActionsLog* printA = new PrintActionsLog();
            printA->act(*this);
            actionsLog.push_back(printA);
        }
        else if (arguments[0] == "close") {
            Close* close = new Close();
            close->act(*this);
            actionsLog.push_back(close);
        }
        else if (arguments[0] == "backup") {
            BackupSimulation* backup = new BackupSimulation();
            backup->act(*this);
            actionsLog.push_back(backup);
        }
        else if (arguments[0] == "restore") {
            RestoreSimulation* restore = new RestoreSimulation();
            restore->act(*this);
            actionsLog.push_back(restore);
        }
    }
}

void Simulation:: addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction *action){
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement){
    if (isSettlementExists(settlement->getName())){
        return false;
    }
    else {
        settlements.push_back(settlement);
        return true;
    }
}

bool Simulation:: addFacility(FacilityType facility){
    for (FacilityType tmp: facilitiesOptions){
        if (tmp.getName() == facility.getName()){
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName){
    for (Settlement *tmp: settlements){
        if (tmp->getName() == settlementName){
            return true;
        }
    }
    return false;
}

Settlement& Simulation::getSettlement(const string &settlementName){
    for (Settlement *tmp: settlements){
        if (tmp->getName() == settlementName){
            return *tmp;
        }
    }
    return *settlements[0]; //Assumed that the settlement allways exist
}

Plan& Simulation:: getPlan(const int planID){
    return plans[planID];   
}

void Simulation:: step(){
    for (Plan& tmp: plans){
        tmp.step();
    }
}

void Simulation:: open(){
    isRunning = true;
}

SelectionPolicy* Simulation:: CreatePolicy (const string &selectionPolicy){
    if (selectionPolicy == "eco") {
         EconomySelection *policy =  new EconomySelection();
         return policy;
    }
    else if (selectionPolicy == "bal") {
        BalancedSelection *policy = new BalancedSelection(0,0,0);
        return policy;
    }
    else if (selectionPolicy == "nve") {
        NaiveSelection *policy = new NaiveSelection();
        return policy;
    }
    else if (selectionPolicy == "env") {
        SustainabilitySelection *policy = new SustainabilitySelection();
        return policy;
    }
    return nullptr;
}

void Simulation:: close() {
    isRunning = false;
    for (Plan p: plans) {
        cout << p.toString() << endl;
    }
}

bool Simulation:: planExist(int planId) {
    if (planId < planCounter) {
        return true;
    }
    return false;
}

void Simulation:: printActions() {
    for (BaseAction* tmp: actionsLog) {
        cout << tmp->toString()<< endl;
    }
}