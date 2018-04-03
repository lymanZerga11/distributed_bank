#include "../include/client.h"

#include <iostream>

std::string get_name()
{
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    return name;
}

std::string get_password()
{
    std::string password;
	std::cout << "Enter your password: ";
	std::getline(std::cin, password);
	return password;
}

int get_currency_type()
{
    int currency_type;
     
	std::cout << "Enter your choice (1: SGD, 2: USD, 3: QAR) for the currency type: ";
	std::cin >> currency_type;
    std::cin.get();
	return currency_type;
}

int get_account_number()
{
    int account_number;
    std::cout << "Enter your account number: ";
    std::cin >> account_number;
    std::cin.get();
    return account_number;
}

float get_account_balance()
{
    float account_balance = 0.0;
    std::cout << "Enter the initial account balance: ";
    std::cin >> account_balance;
    std::cin.get();
    return account_balance;
}

float get_amount(std::string action)
{
    float amount = 0.0;
    std::cout << "Enter the amount to " + action + ": ";
    std::cin >> amount;
    std::cin.get();
    return amount;
}

int get_monitor_interval()
{
    int monitor_interval_in_seconds;
    std::cout << "Enter the monitor interval in seconds: ";
    std::cin >> monitor_interval_in_seconds;
    std::cin.get();
    return monitor_interval_in_seconds;
}

int main () {
    Client client ("127.0.0.1", 52222);
    std::string user_choice = "";
    while(user_choice != "8") {
        std::string name;
        std::string password;
        std::uint32_t account_number;
        std::uint32_t currency_type;
        std::uint32_t monitor_interval_in_seconds = 0;
        float account_balance = 0.0;
        float amount = 0.0;
      
    	std::cout << "\033[0m" << std::endl << "\033[1;36m" << "Banking Services:" << std::endl;
    	std::cout << "1: Open a new bank account" << std::endl;
    	std::cout << "2: Close an existing bank account" << std::endl;
    	std::cout << "3: Deposit money into account" << std::endl;
    	std::cout << "4: Withdraw money from an account" << std::endl;
    	std::cout << "5: Monitor updated made to all bank accounts" << std::endl;
    	std::cout << "6: View account information" << std::endl; // Idempotent operation
    	std::cout << "7: Take a loan" << std::endl; // Non-idempotent operation
    	std::cout << "8: Quit application\n" << std::endl << "\033[0m";	
    	std::cout << "Press the key for the required service and hit enter: ";
    	
    	std::getline(std::cin, user_choice);
    	
    	if (user_choice == "1")
    	{
            std::cout << "You have chosen to open a new bank account." << std::endl;
            name = get_name();
            password = get_password();
            currency_type = get_currency_type();
            account_balance = get_account_balance();
            uint32_t new_account_number = client.open_account(name, password, currency_type, account_balance);
            if (new_account_number != DEFAULT_ACCOUNT_NUMBER) log(INFO) << "Opened account - Account number: " << account_number <<std::endl; 
    	}
    	else if (user_choice == "2")
    	{
            std::cout << "You have chosen to close an existing account." << std::endl;
            
            name = get_name();
            account_number = get_account_number();
            password = get_password();      
            uint32_t success = client.close_account(name, account_number, password);
            if (success == 1) log(INFO) << "Closed account" <<std::endl; 
    	}
    	else if (user_choice == "3")
    	{
            std::cout << "You have chosen to deposit money into your account." << std::endl;
            
            name = get_name();
            account_number = get_account_number();
            password = get_password();
            currency_type = get_currency_type();
            amount = get_amount("deposit");        
            float new_account_balance = client.deposit_money(name, account_number, password, currency_type, amount);
            if (new_account_balance != DEFAULT_ACCOUNT_BALANCE) log(INFO) << "Deposited Money -  Balance: " << new_account_balance <<std::endl; 
    	}
    	else if (user_choice == "4")
    	{
            std::cout << "You have chosen to withdraw money from your account." << std::endl;
            
            name = get_name();
            account_number = get_account_number();
            password = get_password();
            currency_type = get_currency_type();
            amount = get_amount("withdraw");        
            float new_account_balance = client.withdraw_money(name, account_number, password, currency_type, amount);
            if (new_account_balance != DEFAULT_ACCOUNT_BALANCE) log(INFO) << "Withdrew Money -  Balance: " << new_account_balance <<std::endl;
    	}
    	else if (user_choice == "5")
    	{
            std::cout << "You have chosen to monitor updates made to all bank accounts." << std::endl;
    
            monitor_interval_in_seconds = get_monitor_interval();        
    	}
    	else if (user_choice == "6")
    	{
            std::cout << "You have chosen to view your account information." << std::endl;
            
            name = get_name();
            account_number = get_account_number();
            password = get_password();
            float new_account_balance = client.check_balance(name, account_number, password);
            if (new_account_balance != DEFAULT_ACCOUNT_BALANCE) log(INFO) << "Check Balance -  Balance: " << new_account_balance <<std::endl;
    	}
    	else if (user_choice == "7")
    	{
            std::cout << "You have chosen to request for a loan." << std::endl;
            
            name = get_name();
            account_number = get_account_number();
            password = get_password();
            uint32_t success = client.take_loan(name, account_number, password);
            if (success == 1) log(INFO) << "Loan Request Successful" <<std::endl; 
    	}
    	else if (user_choice == "8")
    	{
            std::cout << "Quitting the application." << std::endl;
    	}
    	else
    	{
    	    std::cout << "Invalid choice. Please enter a number between 1 and 8.\n" << std::endl;
    	}
    }
    return 0;
}