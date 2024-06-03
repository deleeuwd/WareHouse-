#pragma once
#include <string>
#include <vector>

#include "Order.h"
#include "Customer.h"
 

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        void clearData();
        void parse(const string &configFilePath);
        bool stringisNumber(const std::string& str) const;
        // rule of 5
        ~WareHouse(); 
        WareHouse& operator=(const WareHouse& other);
        WareHouse(const WareHouse& other);
        WareHouse(WareHouse&& other) noexcept;
        WareHouse& operator=(WareHouse&& other) noexcept;
        

        const int getcustomerCounter() const;
        const int getvolunteerCounter() const;
        const int getorderCounter() const;
        
        const std::vector<Volunteer*>& getVolunteers()const ;
        const std::vector<Order*>& getpendingOrders() const;
        const std::vector<Order*>& getinProcessOrders() const;
        const std::vector<Order*>& getcompletedOrders() const;
        const vector<Customer*>& getcustomers() const;
        void moveTOInProcess(Order* order, OrderStatus targetStatus);
        void removeLimitedVolunteer();
        void addCustomer(Customer* customer);
        int getCoustomerCounter();
        void addVolunteer(Volunteer* volunteer) ;
        bool isOrderExist(int orderId);
        bool isCustomerExist(int customerId);
        bool isVolunteerExist(int volunteerId);
        void PrintAllOrdersToClose();
        std::string orderStatusToString(OrderStatus status);

        bool findCollector(Order* order);
        bool findDriver(Order* order);
        void step();
        void removeFromPendingOrders(vector<int> listOfOrderId);
        int indexOfOrder(int orderId, vector<Order *> &orderList);



        

    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;
        Customer* emptyCustomer;
        Volunteer* emptyVolunteer;
        Order* emptyOrder;
};