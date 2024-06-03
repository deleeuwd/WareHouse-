#include "Action.h"
#include "Volunteer.h"
#include "WareHouse.h"
#include "Order.h"
#include "Customer.h"
#include <string>
#include <iostream>
extern WareHouse* backup;

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction:: getStatus() const{return status;}

void BaseAction:: complete(){status=ActionStatus::COMPLETED;}

void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    (*this).errorMsg=errorMsg;
    std:: cout<< "ERROR: "+errorMsg<< std:: endl;
}

string BaseAction:: toStringStatus(ActionStatus status) const {
    if (status==ActionStatus::COMPLETED){
        return "COMPLETED";
    }
    else{
        return "ERROR";
    }
}

string BaseAction:: getErrorMsg() const{return errorMsg;}




SimulateStep:: SimulateStep(int numOfSteps): numOfSteps(numOfSteps){}



void SimulateStep:: act(WareHouse &wareHouse) {
    for(int i=0; i<numOfSteps; i++){
        wareHouse.step();
    }
    complete();
}

std::string SimulateStep:: toString() const{
    return "SimulateStep  "+ std::to_string(numOfSteps)+ " "+toStringStatus(getStatus());
}

SimulateStep* SimulateStep:: clone() const{
    return new SimulateStep(*this);
}




AddOrder:: AddOrder(int id): BaseAction(), customerId(id) {}

void AddOrder::act(WareHouse &wareHouse)
{
    if (wareHouse.isCustomerExist(customerId))
    {
        if (wareHouse.getCustomer(customerId).canMakeOrder())
        {
            Order *order = new Order(wareHouse.getorderCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
            order->setStatus(OrderStatus::PENDING);
            wareHouse.addOrder(order);
            wareHouse.getCustomer(customerId).addOrder((*order).getId());
            ;
            complete();
        }
        else
        {
            error("Cannot place this order: Customer with ID " + std::to_string(customerId) + " reached the max orders limit.");
        }
    }

    else
    {
        error("Cannot place this order because customer isn't exist");
    }
}


string AddOrder::toString() const {
     return "Order "+ std::to_string(customerId)+ " "+toStringStatus(getStatus());
}
    

AddOrder *AddOrder::clone() const {
    return new AddOrder(*this);}

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders): customerName(customerName), customerType(customerType == "civilian" ? CustomerType::Civilian : CustomerType::Soldier),
distance(distance), maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) {
    if (customerType==CustomerType:: Civilian){
        wareHouse.addCustomer(new CivilianCustomer(wareHouse.getCoustomerCounter(),customerName,distance,maxOrders));
    }
    else{
        wareHouse.addCustomer(new SoldierCustomer(wareHouse.getCoustomerCounter(),customerName,distance,maxOrders));
    }
    complete();
}
AddCustomer *AddCustomer::clone() const {
    return new AddCustomer(*this);
}

string AddCustomer::toString() const {
    string type;
    if(customerType == CustomerType::Soldier){
        type="Soldier";
    }
    else{
        type= "Civilian";
    }
    return "Customer "+ customerName+ " " +type+ " " + std::to_string(distance) + " " +
    std::to_string(maxOrders)+ " "+ toStringStatus(getStatus());
} 

AddVolunteer::AddVolunteer(const string &volunteerName, const string &volunteerType, int coolDown, int maxOrders, int maxDistance, int distancePerStep): volunteerType(volunteerType), id(-1), volunteerName(volunteerName), coolDown(coolDown), timeLeft(0), maxOrders(maxOrders),
ordersLeft(0), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

void AddVolunteer:: act(WareHouse &wareHouse) {
    if (volunteerType=="CollectorVolunteer"){
        wareHouse.addVolunteer(new CollectorVolunteer(wareHouse.getvolunteerCounter(),volunteerName,coolDown));
    }
    if(volunteerType=="LimitedCollectorVolunteer"){
         wareHouse.addVolunteer(new LimitedCollectorVolunteer(wareHouse.getvolunteerCounter(),volunteerName,coolDown,maxOrders));
    }
    if (volunteerType=="DriverVolunteer")
    {
        wareHouse.addVolunteer(new DriverVolunteer(wareHouse.getvolunteerCounter(),volunteerName,maxDistance,distancePerStep));
    }
    if(volunteerType=="LimitedDriverVolunteer"){
        wareHouse.addVolunteer(new LimitedDriverVolunteer(wareHouse.getvolunteerCounter(),volunteerName,maxDistance,distancePerStep,maxOrders));
    }
    complete();
 }

 AddVolunteer *AddVolunteer::clone() const {
    return new AddVolunteer(*this);
}

string AddVolunteer::toString() const {
    return "Volunteer "+ volunteerName+" "+ toStringStatus(getStatus());
} 




PrintOrderStatus:: PrintOrderStatus(int id):BaseAction(),orderId(id){}

void PrintOrderStatus:: act(WareHouse &wareHouse) {
    if(!(wareHouse.isOrderExist(orderId))){
        error("order doesn't exist");
    }
    else{
        std::cout << wareHouse.getOrder(orderId).toString() << std::endl;
        complete();
    }
}

PrintOrderStatus *PrintOrderStatus::clone() const {
    return new PrintOrderStatus(*this);
}

string PrintOrderStatus::toString() const {
   return "orderStatus "+ std::to_string(orderId)+" "+ toStringStatus(getStatus());
}

PrintCustomerStatus:: PrintCustomerStatus(int customerId):BaseAction(),customerId(customerId){}

void PrintCustomerStatus:: act(WareHouse &wareHouse){
    if(!(wareHouse.isCustomerExist(customerId))){
        error("customer doesn't exist");
    }
    else{
        Customer& customer= wareHouse.getCustomer(customerId);
        string s= "customer id: "+ std::to_string(customer.getId());
        for (int orderId: customer.getOrdersIds()){
            s += "\norder id: "+ std::to_string(orderId);
            Order& order= wareHouse.getOrder(orderId);
            if(order.getStatus()== OrderStatus:: PENDING){
                s += "\norder status:PENDING";
            }
            if(order.getStatus()== OrderStatus:: COLLECTING){
                s += "\norder status:COLLECTING";
            }
            if(order.getStatus()== OrderStatus:: DELIVERING){
                s += "\norder status:DELIVERING";
            }
            if(order.getStatus()== OrderStatus:: COMPLETED){
                s += "\norder status:COMPLETED";
            }
        }

        s+= "\n num orders left: "+std::to_string(customer.getNumOrdersLeft());
        std::cout << s << std::endl;
        complete(); 
    }
    
}

PrintCustomerStatus* PrintCustomerStatus:: clone() const{
    return new PrintCustomerStatus(*this);
}

string PrintCustomerStatus:: toString() const{
   return "CustomerStatus "+ std::to_string(customerId)+" "+ toStringStatus(getStatus());
}


PrintVolunteerStatus:: PrintVolunteerStatus(int id):BaseAction(), volunteerId(id){}

void PrintVolunteerStatus:: act(WareHouse &wareHouse){
    if(!(wareHouse.isVolunteerExist(volunteerId))){
        error("Volunteer doesn't exist");
    
    }
    else{
        auto& volunteer= wareHouse.getVolunteer(volunteerId);
        string s= "volunteer id: "+ std::to_string(volunteer.getId());
        if (volunteer.isBusy()){
            s+= " \nIs Busy: True";
            s+= "\nOrderID " + std::to_string(volunteer.getActiveOrderId());
            if (volunteer.getType()== "CollectorVolunteer" || volunteer.getType()=="LimitedCollectorVolunteer"){
                s+= "\nTime Left: "+ std::to_string(dynamic_cast<CollectorVolunteer&>(volunteer).getTimeLeft());
            }
            else{
                 s+= "\nDistamce Left: "+ std::to_string(dynamic_cast<DriverVolunteer&>(volunteer).getDistanceLeft());
            }

            }
        else{
             s = s+ "Is Busy: False"+ "\nOrderID: None"+ "\nTimeLeft or DistanceLeft: None";
        }
        if (volunteer.getType()=="LimitedCollectorVolunteer"){
            s+= "\nOrders Left: "+ std::to_string((dynamic_cast<LimitedCollectorVolunteer&>(volunteer).getNumOrdersLeft()));
        }
        else if (volunteer.getType()=="LimitedDriverVolunteer") {
            s+= "\nOrders Left: "+std::to_string((dynamic_cast<LimitedDriverVolunteer&>(volunteer).getNumOrdersLeft()));
        }
        else{
            s+= "\nOrdersLeft: No limit";
        }
        std::cout << s << std::endl;
        complete(); 
    }
    
}

PrintVolunteerStatus* PrintVolunteerStatus:: clone() const {
    return new PrintVolunteerStatus(*this);
}
        
string PrintVolunteerStatus:: toString() const {
  return "VoluntterStatus "+ std::to_string(volunteerId)+ " "+toStringStatus(getStatus()); 
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for (auto action : wareHouse.getActions()) {
        if(action!=this){
            std::cout << (*action).toString() << std::endl;
        }
      }    
    complete();
}


PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
   return "log "+ toStringStatus(getStatus());
}

Close:: Close(){}
void Close:: act(WareHouse &wareHouse){
    wareHouse.close();
    complete();
}
Close* Close:: clone() const {
    return new Close(*this);
}

string Close:: toString() const {
    return "Close";

}

BackupWareHouse::BackupWareHouse() {}

void BackupWareHouse::act(WareHouse &wareHouse) {
    if (backup != nullptr) {
        delete backup;
        backup= nullptr;
    }
    backup = new WareHouse(wareHouse); 
    complete();
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

std::string BackupWareHouse::toString() const {
    return "BackupWareHouse " +toStringStatus(getStatus());
}

RestoreWareHouse::RestoreWareHouse() {}

void RestoreWareHouse::act(WareHouse &wareHouse) {
    if (backup == nullptr) {
        error("No backup available.");
    }
    else{
        wareHouse = *backup;
        std::cout << "Warehouse status has been restored from the backup." << std::endl;
        complete();
    }
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

std::string RestoreWareHouse::toString() const {
    return "RestoreWareHouse " + toStringStatus(getStatus());
}


