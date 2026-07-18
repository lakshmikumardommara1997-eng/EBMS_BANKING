#include "Customer.h"

namespace Banking
{
    Customer::Customer(Banking::Types::CUSTOMER_ID id, const std::string& name, const std::string& address, const std::string& phoneNumber, const std::string& email)
        : id(id), name(name), address(address), phoneNumber(phoneNumber), email(email)
    {
    }

    Banking::Types::CUSTOMER_ID Customer::getId() const
    {
        return id;
    }

    std::string Customer::getName() const
    {
        return name;
    }

    std::string Customer::getAddress() const
    {
        return address;
    }

    std::string Customer::getPhoneNumber() const
    {
        return phoneNumber;
    }

    std::string Customer::getEmail() const
    {
        return email;
    }

    Customer& Customer::setName(const std::string& name)
    {
        this->name = name;
        return *this;
    }

    Customer& Customer::setAddress(const std::string& address)
    {
        this->address = address;
        return *this;
    }

    Customer& Customer::setPhoneNumber(const std::string& phoneNumber)
    {
        this->phoneNumber = phoneNumber;
        return *this;
    }

    Customer& Customer::setEmail(const std::string& email)
    {
        this->email = email;
        return *this;
    }

    Customer::Customer(const Customer& other)
        : id(other.id), name(other.name), address(other.address), phoneNumber(other.phoneNumber), email(other.email)
    {
    }

    Customer& Customer::operator=(const Customer& other)
    {
        if (this != &other)
        {
            id = other.id;
            name = other.name;
            address = other.address;
            phoneNumber = other.phoneNumber;
            email = other.email;
        }
        return *this;
    }
    //print customer info
    void Customer::printInfo() const
    {
        std::cout << "Customer ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Address: " << address << std::endl;
        std::cout << "Phone Number: " << phoneNumber << std::endl;
        std::cout << "Email: " << email << std::endl;
    }   

    Customer::~Customer()
    {
    }
}