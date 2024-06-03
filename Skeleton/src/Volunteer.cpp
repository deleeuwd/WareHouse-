#include "Volunteer.h"
Volunteer::Volunteer(int id, const string &name)
    : completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id), name(name) {}
    
int Volunteer:: getId() const{return id;}
const string& Volunteer::getName() const{return name;}
int Volunteer:: getActiveOrderId() const{return activeOrderId;}
int Volunteer:: getCompletedOrderId() const{return completedOrderId;}
bool Volunteer:: isBusy() const{
    return activeOrderId!=NO_ORDER;
}
int Volunteer::isCompleted()
{
    if (completedOrderId != NO_ORDER && activeOrderId == NO_ORDER)
    {
        int idOfCompleted = completedOrderId;
        completedOrderId= NO_ORDER;
        return idOfCompleted;
    }
    return -1;
} 
CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}
CollectorVolunteer* CollectorVolunteer:: clone() const {return new CollectorVolunteer(*this);}
void CollectorVolunteer:: step(){
    if(isBusy() && decreaseCoolDown()){
        completedOrderId=activeOrderId;
        activeOrderId=NO_ORDER;
    }
}
int CollectorVolunteer:: getCoolDown() const{return coolDown;}
int CollectorVolunteer:: getTimeLeft() const{return timeLeft;}
bool CollectorVolunteer:: decreaseCoolDown(){
    if(timeLeft>0){
        timeLeft--;
    }
    return timeLeft==0;
}
bool CollectorVolunteer:: hasOrdersLeft() const {return true;}
bool CollectorVolunteer:: canTakeOrder(const Order &order) const {return !isBusy();}
void CollectorVolunteer:: acceptOrder(const Order &order){
    activeOrderId = order.getId();
    timeLeft = coolDown; 
}
string CollectorVolunteer:: toString() const {return "CollectorVolunteer id: "+ std::to_string(getId())+ "\nname: " + getName()+ "\ncoolDown" +  std::to_string(coolDown);}
string CollectorVolunteer:: getType() const{return "CollectorVolunteer";}

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders)
    : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedCollectorVolunteer* LimitedCollectorVolunteer:: clone() const{return new LimitedCollectorVolunteer(*this);}
bool LimitedCollectorVolunteer:: hasOrdersLeft() const {return ordersLeft>0;}
bool LimitedCollectorVolunteer:: canTakeOrder(const Order &order) const{return !isBusy() && ordersLeft>0;}
void LimitedCollectorVolunteer:: acceptOrder(const Order &order) {
    if (LimitedCollectorVolunteer::canTakeOrder(order)){
        CollectorVolunteer:: acceptOrder(order);
        ordersLeft--; 
    }
}
int LimitedCollectorVolunteer:: getMaxOrders() const {return maxOrders;}
int LimitedCollectorVolunteer:: getNumOrdersLeft() const {return ordersLeft;}
string LimitedCollectorVolunteer:: toString() const {
    return "LimitedCollectorVolunteer id: " + std::to_string(getId())+ "\nname: " + getName()+ "\ncoolDown" +  std::to_string(getCoolDown())+ "\nmaxOrders"+ std::to_string(maxOrders);}
string LimitedCollectorVolunteer:: getType() const{return "LimitedCollectorVolunteer";}

DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}
DriverVolunteer* DriverVolunteer:: clone() const {return new DriverVolunteer(*this);}
int DriverVolunteer:: getDistanceLeft() const {return distanceLeft;}
int DriverVolunteer:: getMaxDistance() const {return maxDistance;}
int DriverVolunteer:: getDistancePerStep() const {return distancePerStep;}  
bool DriverVolunteer:: decreaseDistanceLeft() {
    distanceLeft = distanceLeft - distancePerStep;
    if (distanceLeft <= 0){
        distanceLeft = 0; 
        return true;
    }
    return false;
}
bool DriverVolunteer:: hasOrdersLeft() const {return true;}
bool DriverVolunteer:: canTakeOrder(const Order &order) const {return !isBusy() && order.getDistance() <= maxDistance;}
void DriverVolunteer:: acceptOrder(const Order &order) {
        if (isBusy() || order.getDistance() > maxDistance) {
        return;
    }
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}
void DriverVolunteer:: step() {
    if (isBusy() && decreaseDistanceLeft()) {
         completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer:: toString() const {
    return "DriverVolunteer Id " + std::to_string(getId()) + "\nname: " + getName() +"\nMax Distance: " + std::to_string(maxDistance) + "\nDistance Left: " + std::to_string(distanceLeft);
}
void DriverVolunteer:: setDisanceLeft(int distanceLeft){distanceLeft=distanceLeft;}
string DriverVolunteer:: getType() const{return "DriverVolunteer";}


LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders)
    : DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {return new LimitedDriverVolunteer(*this);}
int LimitedDriverVolunteer:: getMaxOrders() const {return maxOrders;}
int LimitedDriverVolunteer:: getNumOrdersLeft() const {return ordersLeft;}
bool LimitedDriverVolunteer:: hasOrdersLeft() const {return ordersLeft > 0;}
bool LimitedDriverVolunteer:: canTakeOrder(const Order &order) const {
     return !isBusy() && order.getDistance() <= getMaxDistance() && ordersLeft > 0;}
void LimitedDriverVolunteer:: acceptOrder(const Order &order){
    if (isBusy() || order.getDistance() > getMaxDistance() || ordersLeft <=0) {
        return;
    }
    activeOrderId = order.getId();
    setDisanceLeft(order.getDistance());
    ordersLeft--;
}
string LimitedDriverVolunteer:: toString() const {
    return "LimitedDriverVolunteer Id: " + std::to_string(getId()) + "\nname: " + getName() +
           "\nmax Distance: " + std::to_string(getMaxDistance()) + "\ndistance Left: " + std::to_string(getDistanceLeft()) +
           "max Orders: " + std::to_string(maxOrders) + "\norders Left: " + std::to_string(ordersLeft);
}
string LimitedDriverVolunteer:: getType() const{return "LimitedDriverVolunteer";}
