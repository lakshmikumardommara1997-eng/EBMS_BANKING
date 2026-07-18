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
            Customer(Customer&& other) noexcept;
            Customer& operator=(Customer&& other) noexcept;
            void printInfo() const;
            Customer(){};

    };
}

#endif