#ifndef INCLUDED_client_h
#define INCLUDED_client_h

#include "../include/message.h"

struct Account {
  std::uint32_t account_number;
  std::uint32_t currency_type;
  float balance;
  std::string name;
  std::string password;

  Account (std::string name, std::string password, std::uint16_t currency_type
           std::uint32_t account_number, float balance) {
    name = name;
    password = password;
    currency_type = currency_type;
    account_number = account_number;
    balance = balance;
  }
};

class Server{
    std::uint64_t  server_id;
    std::uint16_t log_level;
    std::unordered_map <uint64_t, Message> at_most_once_map;
    std::unordered_map <uint32_t, Account> accounts;
    udp_server udp;
};

void Server::Server                   (const std::string &host_address, const int port);
std::uint32_t Server::open_account    (std::string name, std::string password, std::uint16_t currency_type, float amount);
std::uint32_t Server::close_account   (std::string name, std::uint32_t account_number, std::string password);
float Server::deposit_money           (std::string name, std::uint32_t account_number, std::string password, std::uint16_t currency_type, float amount);
float Server::withdraw_money          (std::string name, std::uint32_t account_number, std::string password, std::uint16_t currency_type, float amount);
std::uint32_t Server::take_loan       (std::string name, std::uint32_t account_number, std::string password);
float Server::check_balance           (std::string name, std::uint32_t account_number, std::string password);
// bool Client::monitor                  (std::string name, std::string password);

void Server::process_messages         ();
bool Server::authenticate             (std::string name, std::uint32_t account_number, std::string password);
bool Server::validate_request         (const Message & request_message);

void Server::kill_server              ();

#endif

// give exchange rates
