#ifndef INCLUDED_server_h
#define INCLUDED_server_h

#define DEBUG 1

#include "../include/message.h"
#include "../include/udp_server.h"
#include "../utils/exceptions.cpp"
#include "../utils/log.cpp"

#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <random>
#include <chrono>

#define LOAN_AMOUNT 100.0
#define BASE_CURRENCY_TYPE SGD

struct Account {
  std::uint32_t account_number;
  std::uint32_t currency_type;
  float balance;
  std::string name;
  std::string password;

  Account () : account_number(DEFAULT_ACCOUNT_NUMBER), balance(DEFAULT_ACCOUNT_BALANCE), name(DEFAULT_NAME), password(DEFAULT_PASSWORD) {}
  Account (std::string _name, std::string _password,
           std::uint32_t _account_number, float _balance) {
    name = _name;
    password = _password;
    account_number = _account_number;
    balance = _balance;
  }
};

inline std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << "account_number: " << account.account_number << " | ";
    os << " balance: " << account.balance << " | ";
    os << " name: " << account.name << " | ";
    os << " password: " << account.password;
    return os;
}

class Server{
    std::uint64_t  server_id;
    std::uint16_t log_level;
    std::unordered_map <uint64_t, Message> at_most_once_map;
    std::unordered_map <uint32_t, Account> accounts;
    udp_server udp;
  
public:
    Server                   (const std::string &host_address, const int port);
    std::uint32_t open_account    (std::string name, std::string password, std::uint16_t currency_type, float amount);
    std::uint32_t close_account   (std::string name, std::uint32_t account_number, std::string password);
    float deposit_money           (std::string name, std::uint32_t account_number, std::string password, std::uint16_t currency_type, float amount);
    float withdraw_money          (std::string name, std::uint32_t account_number, std::string password, std::uint16_t currency_type, float amount);
    std::uint32_t take_loan       (std::string name, std::uint32_t account_number, std::string password);
    float check_balance           (std::string name, std::uint32_t account_number, std::string password);
    // bool Client::monitor                  (std::string name, std::string password);
    
    void process_messages         ();
    bool authenticate             (std::string name, std::uint32_t account_number, std::string password);
    bool validate_request         (const Message & request_message);
    
    void kill_server              ();
};

#endif

// give exchange rates
