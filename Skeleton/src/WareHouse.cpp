#include "WareHouse.h"
#include <algorithm>
#include "Volunteer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Order.h"
#include <utility> 
#include "Action.h"

WareHouse::WareHouse(const string &configFilePath)
    : isOpen(false),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers(),
      customerCounter(0),
      volunteerCounter(0),
      orderCounter(0),
      emptyCustomer(new CivilianCustomer(-1, "", 0, 0)),
      emptyVolunteer(new CollectorVolunteer(-1, "", 0)),
      emptyOrder(new Order(-1, 0, 0))
      {
        parse(configFilePath);
      }

void WareHouse:: start(){
    open();
    string command; 
    while(isOpen){
        std::getline(std::cin,command);
        std::stringstream ss(command);
        string word1;
        string input;
        string nothing;
        ss>>word1;
        if (word1 == "step") {
            if (ss >> input && !(ss>>nothing) && stringisNumber(input)){
                int numberOfSteps = stoi(input);
                SimulateStep* step = new SimulateStep(numberOfSteps);
                (*step).act(*this);
                addAction(step);
            }
            continue;
        }
         if (word1 == "order") {
            // order <customerId>
            if (ss >> input && !(ss>>nothing) && stringisNumber(input)){
                int id = stoi(input);
                AddOrder* addOrder = new AddOrder(id);
                (*addOrder).act(*this);
                addAction(addOrder);
            }
            continue;
        }

        if (word1 == "customer") {
            // customer <customer_name> <customer_type> <customer_distance> <max_orders>
            string name;
            string type;
            string distance;
            string maxOrder;
            if(ss >> name >> type >> distance >> maxOrder && !(ss >> nothing) && stringisNumber(distance) && stringisNumber(maxOrder)){
                AddCustomer* addCustomer = new AddCustomer(name, type, stoi(distance), stoi(maxOrder));
                (*addCustomer).act(*this);
                addAction(addCustomer);
            }
            continue;
            
        }
        
        if (word1 == "orderStatus") {
            // orderStatus <order_id>
            if(ss >> input && !(ss>>nothing) && stringisNumber(input)){
                PrintOrderStatus* orderStatus = new PrintOrderStatus(stoi(input));
                (*orderStatus).act(*this);
                addAction(orderStatus);
            }
            continue;
        }

        if (word1 == "customerStatus") {
            // customerStatus <customer_id>
            if(ss >> input && !(ss>>nothing) && stringisNumber(input)){
                PrintCustomerStatus* customerStatus = new PrintCustomerStatus(stoi(input));
                (*customerStatus).act(*this);
                addAction(customerStatus);
            }
            continue;
        }

        if (word1 == "volunteerStatus") {
            // volunteerStatus <volunteer_id>
            if(ss >> input && !(ss>>nothing) && stringisNumber(input)){
                PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(stoi(input));
                (*volunteerStatus).act(*this);
                addAction(volunteerStatus);
            }
            continue;
        }
         if (word1 == "log") {
            if (!(ss >> input)){
                PrintActionsLog* log = new PrintActionsLog();
                (*log).act(*this);
                addAction(log);
            }
            continue;
        }

        if (word1 == "close") {
            if (!(ss >> input)){
                Close* close = new Close();
                (*close).act(*this);
                addAction(close);
            }
            continue;
        }

        if (word1 == "backup") {
            if (!(ss >> input)){
                BackupWareHouse* backup = new BackupWareHouse();
                (*backup).act(*this);
                addAction(backup);
            }
            continue;
        }
         if (word1 == "restore") {
            if (!(ss >> input)){
                RestoreWareHouse* restore = new RestoreWareHouse();
                (*restore).act(*this);
                addAction(restore);
            }
            continue;
         }
}  
}

bool WareHouse:: stringisNumber(const std::string& str) const {
    for (char ch : str) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}





void WareHouse :: parse (const string &configFilePath){  

    std:: ifstream file(configFilePath); // Open the file

    // Check if the file opened successfully
    if (file.is_open()) {
        string line;
        string firstWord;
        string name;
        string orderLimit;
        string type; // or role
        string dist;
        string distPerStep;
        string temp; // Becaue orderLimit is optional in volunteers
        int id;

        while (getline(file, line)) { 

            // Skip lines that beggin with '#'
            if ((line.size() == 0) | ( line.size() > 0 && line[0] == '#')) {
                continue; 
            }

            std:: istringstream iss(line);
            iss >> firstWord >> name >> type >> dist;
            // for costumers
            if (type == "civilian" && stringisNumber(dist))
            {
                iss >> orderLimit;
                id = customerCounter;
                if(stringisNumber(orderLimit)){
                    addCustomer(new CivilianCustomer(id, name, stoi(dist), stoi(orderLimit)));
                }
            }
            if (type == "soldier" && stringisNumber(dist) && stringisNumber(orderLimit)){
                iss >> orderLimit;
                id = customerCounter;
                if(stringisNumber(orderLimit)){
                    addCustomer(new SoldierCustomer(id, name, stoi(dist), stoi(orderLimit)));
                }
            }

            
            // for volunteers
            if (type.find("driver") != std::string::npos){
                iss >> distPerStep;
            }

            if(iss >> temp && temp != "#"){
                orderLimit = temp;
            } 

            if (type == "collector" && stringisNumber(dist)){
                id = volunteerCounter;
                addVolunteer(new CollectorVolunteer(id, name, stoi(dist)));
            }
            if (type == "limited_collector" && stringisNumber(dist) && stringisNumber(orderLimit)){
                id = volunteerCounter;
                addVolunteer(new LimitedCollectorVolunteer(id, name, stoi(dist), stoi(orderLimit)));
            }
            if (type == "driver" && stringisNumber(dist) && stringisNumber(distPerStep)){
                id = volunteerCounter;
                addVolunteer(new DriverVolunteer(id, name, stoi(dist), stoi(distPerStep)));
            }
            if (type == "limited_driver" && stringisNumber(dist) && stringisNumber(distPerStep) && stringisNumber(orderLimit)){
                id = volunteerCounter;
                addVolunteer(new LimitedDriverVolunteer(id, name, stoi(dist), stoi(distPerStep), stoi(orderLimit)));
            }             
        } 
    }
}


void WareHouse:: open(){
    isOpen=true;
    std::cout<<"Warehouse is Open!" << std::endl;
}

void WareHouse:: close(){
    PrintAllOrdersToClose();
    customerCounter = 0;
    volunteerCounter = 0;
    orderCounter= 0;
    isOpen= false;
}


void WareHouse:: addOrder(Order* order){
    pendingOrders.push_back(order);
    orderCounter++;
}

void WareHouse:: addAction(BaseAction* action){actionsLog.push_back(action);}

 Customer& WareHouse:: getCustomer(int customerId) const {
    for(Customer* customer: customers){
        if((*customer).getId()==customerId){
            return *customer;
        }
    }
    return *emptyCustomer;
 }

  Volunteer& WareHouse:: getVolunteer(int volunteerId) const {
    for(Volunteer* volunteer: volunteers){
        if((*volunteer).getId()==volunteerId){
            return *volunteer;
        }
    }
    return *emptyVolunteer;
 }

 Order& WareHouse:: getOrder(int orderId) const{
    for(Order* order: pendingOrders){
        if ((*order).getId() == orderId) {
            return *order;
        }
    }
    for (Order* order: inProcessOrders) {
        if ((*order).getId() == orderId) {
            return *order;
        }
    }
    for (Order* order: completedOrders) {
        if ((*order).getId() == orderId) {
            return *order;
        }
    }
    return *emptyOrder;
 }

const vector<BaseAction*>&  WareHouse:: getActions() const{return actionsLog;}

const int WareHouse:: getcustomerCounter() const{return customerCounter;}

const int WareHouse:: getvolunteerCounter() const{return volunteerCounter;}

const int WareHouse:: getorderCounter() const {return orderCounter;}

//destractor 
WareHouse::~WareHouse(){
    clearData();
}


// Copy constructor definition
WareHouse:: WareHouse(const WareHouse &other) : 
    isOpen(other.isOpen),
    actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(),
    customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter), 
    emptyCustomer(other.emptyCustomer->clone()),  
    emptyVolunteer(other.emptyVolunteer->clone()),  
    emptyOrder(other.emptyOrder->clone())
{
        for (Order* ord : other.pendingOrders){
            pendingOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.inProcessOrders){
            inProcessOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.completedOrders){
            completedOrders.push_back(ord -> clone());
        }
        for (Volunteer* vol : other.volunteers){
            volunteers.push_back(vol -> clone());
        }
        for (Customer* cust : other.customers){
            customers.push_back(cust -> clone());
        }
        for (BaseAction* act : other.actionsLog){
            actionsLog.push_back(act -> clone());
        }
}


void WareHouse::clearData() {
    for (BaseAction* action : actionsLog) {
        delete action;
        action= nullptr;
    }
    actionsLog.clear();

    for (Volunteer* volunteer : volunteers) {
        delete volunteer; 
        volunteer=nullptr;
    }
    volunteers.clear();

    for (Order* order : pendingOrders) {
        delete order; 
        order=nullptr;
    }
    pendingOrders.clear();

    for (Order* order : inProcessOrders) {
        delete order; 
        order=nullptr;
    }
    inProcessOrders.clear();

    for (Order* order : completedOrders) {
        delete order; 
        order=nullptr;
    }
    completedOrders.clear();

    for (Customer* customer : customers) {
        delete customer; 
        customer=nullptr;
    }
    customers.clear();
    
    delete emptyCustomer;
    delete emptyOrder;
    delete emptyVolunteer;
    emptyCustomer = nullptr;
    emptyOrder = nullptr;
    emptyVolunteer = nullptr;
}

// assignment operator
WareHouse& WareHouse::operator=(const WareHouse& other) {
    if (this != &other) {
        isOpen = other.isOpen;
        delete emptyCustomer;
        delete emptyVolunteer;
        delete emptyOrder;
        emptyCustomer=other.emptyCustomer->clone();
        emptyVolunteer=other.emptyVolunteer->clone();
        emptyOrder=other.emptyOrder->clone();

        clearData();

        for (Order* ord : other.pendingOrders){
            pendingOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.inProcessOrders){
            inProcessOrders.push_back(ord -> clone());
        }
        for (Order* ord : other.completedOrders){
            completedOrders.push_back(ord -> clone());
        }
        for (Volunteer* vol : other.volunteers){
            volunteers.push_back(vol -> clone());
        }
        for (Customer* cust : other.customers){
            customers.push_back(cust -> clone());
        }
        for (BaseAction* act : other.actionsLog){
            actionsLog.push_back(act -> clone());
        }

        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter=other.orderCounter;
    }
    return *this;
}

// Move Constructor
WareHouse::WareHouse(WareHouse&& other) noexcept
    : isOpen(other.isOpen),
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)),
      pendingOrders(std::move(other.pendingOrders)),
      inProcessOrders(std::move(other.inProcessOrders)),
      completedOrders(std::move(other.completedOrders)),
      customers(std::move(other.customers)),
      customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter),
      orderCounter(other.orderCounter),
      emptyCustomer(other.emptyCustomer),  
      emptyVolunteer(other.emptyVolunteer),  
      emptyOrder(other.emptyOrder)  
{
    other.emptyCustomer = nullptr;
    other.emptyOrder = nullptr;
    other.emptyVolunteer = nullptr;
}

// Move Assignment Operator
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept {
    if (this != &other) { 

        isOpen = other.isOpen;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        orderCounter = other.orderCounter;
        delete emptyCustomer;
        delete emptyVolunteer;
        delete emptyOrder;
        emptyCustomer=other.emptyCustomer; 
        emptyVolunteer=other.emptyVolunteer; 
        emptyOrder=other.emptyOrder;

        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.orderCounter = 0;
        other.emptyCustomer= nullptr;
        other.emptyVolunteer=nullptr;
        other.emptyOrder=nullptr;
    }
    return *this;
}

const std::vector<Volunteer*>& WareHouse:: getVolunteers()const {return volunteers;}


const std::vector<Order*>& WareHouse:: getpendingOrders() const {return pendingOrders;}


const std::vector<Order*>& WareHouse:: getinProcessOrders() const {return inProcessOrders;}


const std:: vector<Order*>& WareHouse:: getcompletedOrders()  const{return completedOrders;}


const vector<Customer*>& WareHouse:: getcustomers()const {return customers;}


void WareHouse::removeLimitedVolunteer(){
    auto it = volunteers.begin();
    while (it != volunteers.end()) {
        Volunteer* volunteer = *it;
        if (!(*volunteer).isBusy() && !(*volunteer).hasOrdersLeft()) {
            it = volunteers.erase(it);
            delete volunteer;
        } else {
            ++it;
        }
}
}



void WareHouse:: addCustomer(Customer* customer){
    customers.push_back(customer);
    customerCounter++;
}

int WareHouse::getCoustomerCounter(){return customers.size();}


void WareHouse:: addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
    volunteerCounter++;
}


bool WareHouse:: isOrderExist(int orderId){
    for(Order* order: pendingOrders){
        if ((*order).getId() == orderId) {
            return true;
        }
    }
    for (Order* order: inProcessOrders) {
        if ((*order).getId() == orderId) {
            return true;
        }
    }
    for (Order* order: completedOrders) {
        if ((*order).getId() == orderId) {
            return true;
        }
    }
    return false;
}


bool WareHouse:: isCustomerExist(int customerId){
    for(Customer* customer: customers){
        if((*customer).getId()==customerId){
            return true;
        }
    }  
    return false;
}


 bool WareHouse:: isVolunteerExist(int volunteerId){
    for(Volunteer* volunteer: volunteers){
        if((*volunteer).getId()==volunteerId){
            return true;
        }
    }  
    return false;
}

void WareHouse:: PrintAllOrdersToClose(){
    for (const auto& order : pendingOrders) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId()
                  << " , OrderStatus: " << orderStatusToString(order->getStatus()) << std::endl;
    }
    for (const auto& order : inProcessOrders) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId()
                  << " , OrderStatus: " << orderStatusToString(order->getStatus()) << std::endl;
    }
    for (const auto& order : completedOrders) {
        std::cout << "OrderID: " << order->getId() << " , CustomerID: " << order->getCustomerId()
                  << " , OrderStatus: " << orderStatusToString(order->getStatus()) << std::endl;
    }
}

std::string WareHouse:: orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING:
            return "Pending";
        case OrderStatus::COLLECTING:
            return "collecting";
        case OrderStatus::DELIVERING:
            return "dellivering";
        case OrderStatus::COMPLETED:
            return "Completed";
        default:
            return "Unknown";
    }
}

// simulate step
bool WareHouse:: findCollector(Order* order) { // check if the volunteer is a collector
    for (Volunteer* volunteer : volunteers){
        if ((*volunteer).getType() == "CollectorVolunteer" || (*volunteer).getType() == "LimitedCollectorVolunteer"){
            if ((*volunteer).canTakeOrder(*order)) {
                (*volunteer).acceptOrder(*order);
                (*order).setCollectorId(volunteer->getId());
                return true;
        }
    }
    }
    return false;
}

bool WareHouse::findDriver(Order* order) { // check if the volunteer is a driver
    for (Volunteer* volunteer : volunteers) {
        if ((*volunteer).getType() == "DriverVolunteer" || (*volunteer).getType() == "LimitedDriverVolunteer"){
            if ((*volunteer).canTakeOrder(*order)) {
                (*volunteer).acceptOrder(*order);
                (*order).setDriverId(volunteer->getId());

                return true;
        }
    }
    }
    return false;
}

void WareHouse::moveTOInProcess(Order* order, OrderStatus targetStatus){
        auto it = std::find(pendingOrders.begin(), pendingOrders.end(), order);
        if (it != pendingOrders.end()) {
        inProcessOrders.push_back(order);
        (*order).setStatus(targetStatus);   
        }
}

void WareHouse:: step() {
    //step 1
    std::vector<Order*> ordersToRemove;
    for (Order* order : pendingOrders) { // go over the pending orders
        if ((*order).getStatus() == OrderStatus::PENDING) {
            if (findCollector(order)){
                moveTOInProcess(order, OrderStatus::COLLECTING);
                ordersToRemove.push_back(order);
            }
        }
        else {
            if ((*order).getStatus() == OrderStatus::COLLECTING) {
            if (findDriver(order)){
                moveTOInProcess(order,OrderStatus::DELIVERING);
                ordersToRemove.push_back(order);
            }
        }
        }
    }
    for (Order* order : ordersToRemove) {
        pendingOrders.erase(std::remove(pendingOrders.begin(), pendingOrders.end(), order), pendingOrders.end());
    }
    //step 2
    for (Volunteer* volunteer : volunteers){ // one step forward in the process 
          (*volunteer).step();
    }
    //step 3
    vector<int> orderId_toRemove;
    for (auto &volunteer : volunteers)
    {
        int orderId = (*volunteer).isCompleted();
        if (orderId != -1)
        {

            Order &order = getOrder(orderId);

            if (order.getStatus() == OrderStatus::COLLECTING)
            {
                pendingOrders.push_back(&order);
                orderId_toRemove.push_back(order.getId());
            }
            else if (order.getStatus() == OrderStatus::DELIVERING)
            {
                order.setStatus(OrderStatus::COMPLETED);
                completedOrders.push_back(&order);
                orderId_toRemove.push_back(order.getId());
            }
        }
    }
    removeFromPendingOrders(orderId_toRemove);

    //step 4
    removeLimitedVolunteer();
}

void WareHouse::removeFromPendingOrders(vector<int> listOfOrderId)
{
    if (!listOfOrderId.empty())
    {
            for (auto id : listOfOrderId)
            {
                inProcessOrders.erase(inProcessOrders.begin() + indexOfOrder(id, inProcessOrders));
            }
        }
}

int WareHouse:: indexOfOrder(int orderId, vector<Order *> &orderList)
{
    int i = 0;
    for (auto &order : orderList)
    {
        if ((*order).getId() == orderId)
        {
            return i;
        }
        i++;
    }
    return -1;
}








