#include "Common.h"
#include "Customer.h"
#include <vector>


int main()
{
    Banking::Common common;

    common.printWelcome();

    // Create a new customer
    Banking::Types::CUSTOMER_ID customerId = 1;
    Banking::Customer customer(customerId, "John Doe", "123 Main St", "555-1234", "john.doe@example.com");
    std::vector<Banking::Customer> customers;
    customers.push_back(customer);
    //customers[0].printInfo();

    return 0;
}
