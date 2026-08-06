#include "Customer.h"
#include <Logger.h>

namespace Banking
{
    Customer::Customer(Banking::Types::CUSTOMER_ID id, const std::string& firstName, const std::string& lastName, const std::string& address, const std::string& phoneNumber, const std::string& email, const std::string& city, const std::string& state, const std::string& zipCode, const std::string& createdAt)
        : id(id), firstName(firstName), lastName(lastName), address(address), phoneNumber(phoneNumber), email(email), city(city), state(state), zipCode(zipCode), createdAt(createdAt)
    {
    }

    Customer::Customer(Banking::Types::CUSTOMER_ID id, const std::string& name, const std::string& address, const std::string& phoneNumber, const std::string& email)
        : id(id), firstName(name), lastName(""), address(address), phoneNumber(phoneNumber), email(email), city(""), state(""), zipCode(""), createdAt("")
    {
    }

    Banking::Types::CUSTOMER_ID Customer::getId() const
    {
        return id;
    }

    std::string Customer::getFirstName() const
    {
        return firstName;
    }

    std::string Customer::getLastName() const
    {
        return lastName;
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
    std::string Customer::getCity() const
    {
        return city;
    }
    std::string Customer::getState() const
    {
        return state;
    }
    std::string Customer::getZipCode() const
    {
        return zipCode;
    }
    std::string Customer::getCreatedAt() const
    {
        return createdAt;
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
    Customer& Customer::setFirstName(const std::string& firstName)
    {
        this->firstName = firstName;
        return *this;
    }

    Customer& Customer::setLastName(const std::string& lastName)
    {
        this->lastName = lastName;
        return *this;
    }
    Customer& Customer::setCity(const std::string& city)
    {
        this->city = city;
        return *this;
    }
    Customer& Customer::setState(const std::string& state)
    {
        this->state = state;
        return *this;
    }
    Customer& Customer::setZipCode(const std::string& zipCode)
    {
        this->zipCode = zipCode;
        return *this;
    }
    Customer& Customer::setCreatedAt(const std::string& createdAt)
    {
        this->createdAt = createdAt;
        return *this;
    }

    Customer::Customer(const Customer& other)
        : id(other.id), firstName(other.firstName), lastName(other.lastName), address(other.address), phoneNumber(other.phoneNumber), email(other.email)
    {
    }

    Customer& Customer::operator=(const Customer& other)
    {
        if (this != &other)
        {
            id = other.id;
            firstName = other.firstName;
            lastName = other.lastName;
            address = other.address;
            phoneNumber = other.phoneNumber;
            email = other.email;
            city = other.city;
            state = other.state;
            zipCode = other.zipCode;
            createdAt = other.createdAt;

        }
        return *this;
    }
    std::string Customer::toString() const
    {
        return "[Customer ID: " + std::to_string(id) + ", Name: " + firstName + " " + lastName + ", Address: " + address + ", Phone Number: " + phoneNumber + ", Email: " + email + " , City: " + city + ", State: " + state + ", Zip Code: " + zipCode + ", Created At: " + createdAt + "]";
    }
    /*
    
    Customer::Customer(Customer&& other) noexcept
        : id(other.id), name(std::move(other.name)), address(std::move(other.address)), phoneNumber(std::move(other.phoneNumber)), email(std::move(other.email))
    {
        other.id = 0;
    }   
    Customer& Customer::operator=(Customer&& other) noexcept
    {
        if (this != &other)
        {
            id = other.id;
            name = std::move(other.name);
            address = std::move(other.address);
            phoneNumber = std::move(other.phoneNumber);
            email = std::move(other.email);

            other.id = 0;
        }
        return *this;
    }   
        */
    //print customer info
    void Customer::printInfo() const
    {
        std::cout << "Customer ID: " << id << std::endl;
        std::cout << "Name: " << firstName << " " << lastName << std::endl;
        std::cout << "Address: " << address << std::endl;
        std::cout << "Phone Number: " << phoneNumber << std::endl;
        std::cout << "Email: " << email << std::endl;
    }   

    Customer::~Customer()
    {
    }
}