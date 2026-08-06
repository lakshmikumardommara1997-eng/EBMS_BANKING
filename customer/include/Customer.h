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
            std::string firstName;
            std::string lastName;
            std::string address;
            std::string phoneNumber;
            std::string email;
            std::string city;
            std::string state;
            std::string zipCode;
            std::string createdAt;
        public:
            Customer(Banking::Types::CUSTOMER_ID id, const std::string& firstName, const std::string& lastName, const std::string& address, const std::string& phoneNumber, const std::string& email, const std::string& city = "", const std::string& state = "", const std::string& zipCode = "", const std::string& createdAt = "");
            Customer(Banking::Types::CUSTOMER_ID id, const std::string& name, const std::string& address, const std::string& phoneNumber, const std::string& email);
            Banking::Types::CUSTOMER_ID getId() const;
            std::string getFirstName() const;
            std::string getLastName() const;
            std::string getAddress() const;
            std::string getPhoneNumber() const;
            std::string getEmail() const;
            std::string getCity() const;
            std::string getState() const;
            std::string getZipCode() const;
            std::string getCreatedAt() const;
            Customer& setFirstName(const std::string& firstName);
            Customer& setLastName(const std::string& lastName);
            Customer& setAddress(const std::string& address);
            Customer& setPhoneNumber(const std::string& phoneNumber);
            Customer& setEmail(const std::string& email);
            Customer& setCity(const std::string& city);
            Customer& setState(const std::string& state);
            Customer& setZipCode(const std::string& zipCode);
            Customer& setCreatedAt(const std::string& createdAt);
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
            std::string toString() const;
            Customer(){};

    };
}

#endif