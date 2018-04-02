#ifndef INCLUDED_client_h
#define INCLUDED_client_h

#include "../include/server.h"

#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <random>
#include <chrono>

void Server::Server(const std::string &host_address, const int port) : udp (host_address, port){
}

std::uint32_t Server::open_account (std::string name, std::string password, std::uint16_t currency_type, float amount)  {
  std::uint32_t account_number = 1000000001;
  while (accounts.find(account_number) != accounts.end()) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<uint32_t> distr(1000000000, 3000000000);
    std::uint32_t account_number = distr(engine);
  }
  Account new_account (name, password, currency_type, account_number, exchange(amount, currency_type);
  return account_number;
}

std::uint32_t Server::close_account (std::string name, std::uint32_t account_number, std::string password) {
  std::uint32_t success = 0;
  if (authenticate (account_number, password, name)) {
    accounts.erase(account_number);
    success = 1;
  }
  else {
    throw AuthenticationFailed;
  }
  return success;
}

float Server::deposit_money (std::string name, std::uint32_t account_number, std::string password,
                              std::uint16_t currency_type, float amount) {
  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  if (authenticate (name, account_number, password)) {
    accounts[account_number].balance += exchange(amount, currency_type);
    account_balance = accounts[account_number].balance
  }
  else {
    throw AuthenticationFailed;
  }
  return account_balance;
}

float Server::withdraw_money (std::string name, std::uint32_t account_number, std::string password,
                              std::uint16_t currency_type, float amount) {
  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  if (authenticate (name, account_number, password)) {
    accounts[account_number].balance -= exchange(amount, currency_type);
    account_balance = accounts[account_number].balance
  }
  else {
    throw AuthenticationFailed;
  }
  return account_balance;
}

std::uint32_t Server::take_loan (std::string name, std::uint32_t account_number, std::string password) {
  std::uint32_t success = 0;
  if (authenticate (name, account_number, password)) {
    accounts[account_number].balance += exchange(loan_amount, currency_type);
    success = 1;
  }
  else {
    throw AuthenticationFailed;
  }
  return success;
}

float Server::check_balance (std::string name, std::uint32_t account_number, std::string password) {
  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  if (authenticate (name, account_number, password)) {
    account_balance = accounts[account_number].balance;
  }
  else {
    throw AuthenticationFailed;
  }
  return account_balance;
}

// bool Client::monitor                  (std::string name, std::string password);

void Server::process_messages () {
  while (true) {
    char request_packet[PACKET_SIZE];
    char response_packet[PACKET_SIZE];
    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);

    if (udp.recvfrom(udp.get_socket(), request_packet, PACKET_SIZE, 0,
        (struct sockaddr *) &client_address, (socklen_t *) &client_address_length) > 1) {
      Message request_message (DEFAULT_REQUEST_ID);
      Message response_message (DEFAULT_REQUEST_ID);
      request_message.deserialize(request_packet);
      if (validate_request (request_message)) {
        if (at_most_once_map.find(request_message.request_id) != at_most_once_map.end()) {
          if (request_semantics == AT_MOST_ONCE)
            response_message = at_most_once_map[request_message.request_id];
          else ;
          // warn
        }
        else {
          switch (request_message.request_type) {
            case OPEN_ACC:
              response_message.account_number = open_account(request_message.name, request_message.password, request_message.currency_type);
              break;
            case CLOSE_ACC:
              response_message.success = close_account(request_message.name, request_message.account_number, request_message.password);
              break;
            case DEPOSIT:
              response_message.account_balance = deposit_money(request_message.name, request_message.account_number, request_message.password,
                            request_message.currency_type, request_message.amount);
              break;
            case WITHDRAW:
              response_message.account_balance = withdraw_money(request_message.name, request_message.account_number, request_message.password,
                            request_message.currency_type, request_message.amount);
              break;
            case TAKE_LOAN:
              response_message.success = take_loan(request_message.name, request_message.account_number, request_message.password);
              break;
            case CHECK_BALANCE:
              response_message.account_balance = check_balance(request_message.name, request_message.account_number, request_message.password);
              break;
          }
        }
      }
      else throw InvalidRequestError;
      at_most_once_map[request_message.request_id] = response_message;
      response_message.serialize(response_packet);
      udp.sendto(udp.get_socket(), request_packet, PACKET_SIZE, 0,
                 (struct sockaddr *) &client_address, (socklen_t *) &client_address_length);
    }
  }
}

bool Server::authenticate (std::string name, std::uint32_t account_number, std::string password) {
  return accounts.find(account_number) != accounts.end() &&
         accounts[account_number].password == password &&
         accounts[account_number].name == name;
}

bool Server::validate_request (const Message & request_message) {
  return true;
  // same as in client
}

void Server::kill_server() {
  ;
}

#endif

// give exchange rates
