#include "Order.h"

Order:: Order(int id, int customerId, int distance): id(id), customerId(customerId), distance(distance),
status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}
int Order:: getId() const {return id;}
int Order::  getCustomerId() const {return customerId;}
void Order:: setStatus(OrderStatus status){ (*this).status = status;}
void Order:: setCollectorId(int collectorId){this->collectorId=collectorId;}
void Order:: setDriverId(int driverId){this->driverId=driverId;}
int Order:: getCollectorId() const{return collectorId;}
int Order:: getDriverId() const{return driverId;}
OrderStatus Order:: getStatus() const{return status;}
const string Order:: toString() const{
    string strStatus;
    string driver= "NONE";
    int driverId = getDriverId();
    if (driverId != NO_VOLUNTEER){
        driver = std::to_string(driverId);
    }
    string collector= "NONE";
     int collectorId = getCollectorId();
    if (collectorId != NO_VOLUNTEER){
        collector = std::to_string(collectorId);
    }
    if(status == OrderStatus::PENDING){
        strStatus="PENDING";
    }
    else if(status == OrderStatus::COLLECTING){
        strStatus="COLLECTING";
    }
    else if (status == OrderStatus::DELIVERING){
        strStatus="DELIVERING";
    }
    else if (status == OrderStatus::COMPLETED){
        strStatus= "COMPLETED";
    }
    
    return "Order ID: " + std::to_string(id) +"\nStatus: " + strStatus +  "\nCustomer ID: "+std::to_string(customerId)+ "\nCollector ID: " +collector + "\nDriver ID: " + driver;
}
int Order:: getDistance() const {return distance;}

Order* Order::clone() const {
    return new Order(*this);
}

