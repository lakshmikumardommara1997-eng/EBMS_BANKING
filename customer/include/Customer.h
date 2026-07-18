#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "Types.h"
#include <iostream>

namespace Banking
{
    class Customer{
        private:
            Banking::Types::CUSTOMER_ID id;
            std::string name;
            std::string address;
            std::string phoneNumber;
            std::string email;
        public:
            Customer(Banking::Types::CUSTOMER_ID id, const std::string& name, const std::string& address, const std::string& phoneNumber, const std::string& email);
            Banking::Types::CUSTOMER_ID getId() const;
            std::string getName() const;
            std::string getAddress() const;
            std::string getPhoneNumber() const;
            std::string getEmail() const;
            Customer& setName(const std::string& name);
            Customer& setAddress(const std::string& address);   
            Customer& setPhoneNumber(const std::string& phoneNumber);
            Customer& setEmail(const std::string& email);
            Customer(const Customer& other);
            Customer& operator=(const Customer& other);
            ~Customer();
            /*
                as per Rule of 0, we dont need to implement move constructor and move assignment operator since we are not 
                managing any resources manually. The compiler will generate them for us.
                when we need to implement them,
                we can implement them as follows:
                1.  When we have a class that manages resources (like dynamic memory, file handles, etc.), we need to implement move constructor and move assignment operator to efficiently transfer ownership of those resources from one object to another.
                2.  When we have a class that has a base class with a move constructor or move assignment operator, we need to implement move constructor and move assignment operator in the derived class to ensure that the base class's resources

            */
            //Customer(Customer&& other) noexcept;
           // Customer& operator=(Customer&& other) noexcept;
            void printInfo() const;
            Customer(){};

    };
}

#endif