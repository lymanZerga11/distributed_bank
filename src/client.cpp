#include "../include/client.h"

#include <iostream>
#include <arpa/inet.h>

Client::Client(const std::string &host_address, const int port) : udp(host_address, port) {
  client_id = uint64_t(ntohl(inet_addr("127.0.0.1"))) << 32 | uint64_t(udp.get_bound_port()) << 24;
  // initialise timeout, max_attempts, request_semantic from config
  timeout = 10000;
  request_semantic = AT_MOST_ONCE;
  max_request_attempts = 3;
  request_count = 0;
}

std::uint32_t Client::open_account (std::string name, std::string password, std::uint32_t currency_type, float amount) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = OPEN_ACC;
  request_message.name = name;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  std::uint32_t account_number = DEFAULT_ACCOUNT_NUMBER;
  try {
    account_number = get_response (request_message).account_number; }
  catch (InvalidInputError& e) {
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl; }
  catch (ServerSideError& e) {
    log(ERROR) << "Server returned: " << get_response (request_message).error_data  << " on request_id " << request_message.request_id << std::endl; }
  catch (MaxAttemptsError& e) {
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl; }
  return account_number;
  // if DEFAULT, dont print in console
}

std::uint32_t Client::close_account (std::string name, std::uint32_t account_number, std::string password) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = CLOSE_ACC;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;

  std::uint32_t success = DEFAULT_SUCCESS;
  try {
    success = get_response (request_message).success; }
  catch (InvalidInputError& e) {
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl; }
  catch (ServerSideError& e) {
    log(ERROR) << "Server returned: " << get_response (request_message).error_data  << " on request_id " << request_message.request_id << std::endl; }
  catch (MaxAttemptsError& e) {
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl; }
  return success;
}

float Client::deposit_money (std::string name, std::uint32_t account_number, std::string password,
                                     std::uint32_t currency_type, float amount) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = DEPOSIT;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  try {
    account_balance = get_response (request_message).account_balance; }
  catch (InvalidInputError& e) {
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl; }
  catch (ServerSideError& e) {
    log(ERROR) << "Server returned: " << get_response (request_message).error_data  << " on request_id " << request_message.request_id << std::endl; }
  catch (MaxAttemptsError& e) {
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl; }
  return account_balance;
}

float Client::withdraw_money (std::string name, std::uint32_t account_number, std::string password,
                                     std::uint32_t currency_type, float amount) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = WITHDRAW;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  try {
    account_balance = get_response (request_message).account_balance; }
  catch (InvalidInputError& e) {
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl; }
  catch (ServerSideError& e) {
    log(ERROR) << "Server returned: " << get_response (request_message).error_data  << " on request_id " << request_message.request_id << std::endl; }
  catch (MaxAttemptsError& e) {
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl; }
  return account_balance;
}

uint32_t Client::take_loan (std::string name, std::uint32_t account_number, std::string password) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = TAKE_LOAN;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;

  uint32_t success = DEFAULT_SUCCESS;
  try {
   success = get_response (request_message).success; }
  catch (InvalidInputError& e) {
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl; }
  catch (ServerSideError& e) {
    log(ERROR) << "Server returned: " << get_response (request_message).error_data  << " on request_id " << request_message.request_id << std::endl; }
  catch (MaxAttemptsError& e) {
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl; }
  return success;
}

float Client::check_balance (std::string name, std::uint32_t account_number, std::string password) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = CHECK_BALANCE;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;

  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  try {
   account_balance = get_response (request_message).account_balance; }
  catch (InvalidInputError& e) {
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl; }
  catch (ServerSideError& e) {
    log(ERROR) << "Server returned: " << get_response (request_message).error_data  << " on request_id " << request_message.request_id << std::endl; }
  catch (MaxAttemptsError& e) {
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl; }
  return account_balance;
}

bool Client::validate_request (const Message & request_message) {
  // check all valid range but can be default
  return true;
}

bool Client::validate_response (const Message & response_message) {
  // check all valid range but can be default
  return true;
}

Message Client::get_response (Message & request_message) {
  if (validate_request (request_message)) {
    char request_packet[2 * PACKET_SIZE];
    request_message.serialize(request_packet);
    char response_packet[2 * PACKET_SIZE];

    int attempts = 0;
    for (; attempts < max_request_attempts; attempts++) {
      udp.send(request_packet, PACKET_SIZE);
      if (udp.timed_recv(response_packet, PACKET_SIZE, timeout) > 0) {
        Message response_message (DEFAULT_REQUEST_ID);
        response_message.deserialize(response_packet);
        if (DEBUG) log(WARN) << response_message <<std::endl;
        if (validate_response(response_message) &&
            request_message.request_id == response_message.request_id) return response_message;
        else log(WARN) << "Invalid response on request_id " << request_message.request_id << ". Retrying..." << std::endl;
      }
      else log(WARN) << "Timeout on request_id " << request_message.request_id << ". Retrying..." << std::endl;
    }
    if (attempts >= max_request_attempts) throw MaxAttemptsError();
  }
  else
    throw InvalidInputError();
}

void Client::kill_client() {

}
