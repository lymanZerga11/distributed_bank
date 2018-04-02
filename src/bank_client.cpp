#include "bank_client.h"
#include "message.h"

void Client::Client(const std::string &host_address, const int port) : udp_client(host_address, port) {
  client_id = uint64_t(ntohl(inet_addr("127.0.0.1"))) << 32 | uint64_t(udp_client.get_bound_port()) << 24;
  // initialise log_level, timeout, max_attempts, request_semantic from config
  request_count = 0;
}

std::uint32_t Client::open_account (std::string name, std::string password, std::uint32_t currency_type) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = OPEN_ACC;
  request_message.name = name;
  request_message.password = password;
  request_message.currency_type = currency_type;

  std::uint32_t account_number = DEFAULT_ACCOUNT_NUMBER;
  try:
    account_number = get_response (request_message).account_number;
  catch InvalidInputError:
   log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl;
  catch ServerSideError:
   log(ERROR) << "Server returned: " << get_response (request_message).error_string  << " on request_id " << request_message.request_id << std::endl;
  catch MaxAttemptsError:
   log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl;
  return account_number;
  // udp_client will return result not error. catch. implement at-least and at-most in get_response()
  // if DEFAULT, dont print in console
  // serversideerror implement error string to print in case and default error string to "0"
}

std::uint32_t Client::close_account (std::string name, std::uint32_t account_number, std::string password) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = CLOSE_ACC;
  request_message.account_number = account_number;
  request_message.password = password;

  std::uint32_t success = DEFAULT_SUCCESS;
  try:
    success = get_response (request_message).success;
  catch InvalidInputError:
   log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl;
  catch ServerSideError:
   log(ERROR) << "Server returned: " << get_response (request_message).error_string  << " on request_id " << request_message.request_id << std::endl;
  catch MaxAttemptsError:
   log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl;
  return success;
}

double Client::deposit_money (std::string name, std::uint32_t account_number, std::string password,
                                     std::uint32_t currency_type, float amount) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = DEPOSIT;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  double account_balance = DEFAULT_ACCOUNT_BALANCE;
  try:
    account_balance = get_response (request_message).account_balance;
  catch InvalidInputError:
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl;
  catch ServerSideError:
    log(ERROR) << "Server returned: " << get_response (request_message).error_string  << " on request_id " << request_message.request_id << std::endl;
  catch MaxAttemptsError:
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl;
  return account_balance;
}

double Client::withdraw_money (std::string name, std::uint32_t account_number, std::string password,
                                     std::uint32_t currency_type, float amount) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = WITHDRAW;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  double account_balance = DEFAULT_ACCOUNT_BALANCE;
  try:
    account_balance = get_response (request_message).account_balance;
  catch InvalidInputError:
    log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl;
  catch ServerSideError:
    log(ERROR) << "Server returned: " << get_response (request_message).error_string  << " on request_id " << request_message.request_id << std::endl;
  catch MaxAttemptsError:
    log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl;
  return account_balance;
}

uint32_t Client::take_loan (std::string name, std::uint32_t account_number, std::string password) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = TAKE_LOAN;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;

  uint32_t success = DEFAULT_SUCCESS;
  try:
   success = get_response (request_message).success;
  catch InvalidInputError:
   log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl;
  catch ServerSideError:
   log(ERROR) << "Server returned: " << get_response (request_message).error_string  << " on request_id " << request_message.request_id << std::endl;
  catch MaxAttemptsError:
   log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl;
  return success;
}

double Client::check_balance (std::string name, std::uint32_t account_number, std::string password) {
  Message request_message (client_id | (request_count++));
  request_message.request_type = CHECK_BALANCE;
  request_message.name = name;
  request_message.account_number = account_number;
  request_message.password = password;

  double account_balance = DEFAULT_ACCOUNT_BALANCE;
  try:
   account_balance = get_response (request_message).account_balance;
  catch InvalidInputError:
   log(ERROR) << "Invalid input entered on request_id " << request_message.request_id << std::endl;
  catch ServerSideError:
   log(ERROR) << "Server returned: " << get_response (request_message).error_string  << " on request_id " << request_message.request_id << std::endl;
  catch MaxAttemptsError:
   log(ERROR) << "Maximum failed on request_id " << request_message.request_id << std::endl;
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

Message Client::get_response (const Message & request_message) {
  if (validate_request (request_message)) {
    const char* request_packet = request_message.serialize();
    char response_packet[PACKET_SIZE];

    int attempts = 0;
    for (; attempts < max_request_attempts; attempts++) {
      udp_client.send(request_packet, PACKET_SIZE);

      if (udp_client.timed_recv(response_packet, PACKET_SIZE, timeout) > 0) {
        Message response_message.deserialize(response_packet);
        if (validate_response(response_message)) return response_message;
        else log(WARN) << "Invalid response on request_id " << request_message.request_id << ". Retrying..." << std::endl;
      }
      else log(WARN) << "Timeout on request_id " << request_message.request_id << ". Retrying..." << std::endl;
    }
    if (attempts == max_attempts) throw MaxAttemptsError;
  }
  else
    throw InvalidInputError;
}

void Client::kill_client() {

}
