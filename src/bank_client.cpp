#include "bank_client.h"

void Client::Client(const std::string &host_address, const int port);

std::uint64_t Client::open_account (std::string name, std::string password, std::uint16_t currency_type) {
  Message request_message (request_type=1);
  request_message.name = name;
  request_message.password = password;
  request_message.currency_type = currency_type;

  std::uint64_t account_number =  0;
  try:
    account_number = get_response (request_message). account_number;
  catch InvalidInputError:
    std::error << "";
  return account_number;
  // udp_client will return result not error. catch. implement at-least and at-most in get_response()
}

bool Client::close_account (std::string name, std::uint64_t account_number, std::string password) {
  Message request_message (request_type=2);
  request_message.name = name;
  request_message.account_number = password;
  request_message.password = password;

  bool success = false;
  try:
    success = get_response (request_message).success;
  catch InvalidInputError:
    std::error << "";
  return success;
}

std::uint64_t Client::deposit_money (std::string name, std::uint64_t account_number, std::string password,
                                     std::uint16_t currency_type, std::uint64_t amount) {
  Message request_message (request_type=3);
  request_message.name = name;
  request_message.account_number = password;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  std::uint64_t account_balance = 0;
  try:
   account_balance = get_response (request_message).account_balance;
  catch InvalidInputError:
   std::error << "";
  return account_balance;
}

std::uint64_t Client::deposit_money (std::string name, std::uint64_t account_number, std::string password,
                                     std::uint16_t currency_type, std::uint64_t amount) {
  Message request_message (request_type=4);
  request_message.name = name;
  request_message.account_number = password;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;

  std::uint64_t account_balance = 0;
  try:
   account_balance = get_response (request_message).account_balance;
  catch InvalidInputError:
   std::error << "";
  return account_balance;
}

std::uint64_t Client::deposit_money (std::string name, std::uint64_t account_number, std::string password,
                                     std::uint16_t currency_type, std::uint64_t amount, std::uint64_t recipient_account_number) {
  Message request_message (request_type=4);
  request_message.name = name;
  request_message.account_number = password;
  request_message.password = password;
  request_message.currency_type = currency_type;
  request_message.amount = amount;
  request_message.recipient_account_number = recipient_account_number;

  std::uint64_t account_balance = 0;
  try:
   account_balance = get_response (request_message).account_balance;
  catch InvalidInputError:
   std::error << "";
  return account_balance;
}

bool Client::change_password (std::string name, std::uint64_t account_number, std::string password, std::string new_password) {
  Message request_message (request_type=2);
  request_message.name = name;
  request_message.account_number = password;
  request_message.password = password;
  request_message.new_password = new_password;

  bool success = false;
  try:
    success = get_response (request_message).success;
  catch InvalidInputError:
    std::error << "";
  return success;
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
    char response_packet[max_size];

    int attempts = 0;
    for (; attempts < max_attempts; attempts++) {
      udp_client.send(request_packet, max_size);

      if (udp_client.timed_recv(response_packet, max_size, timeout) > 0) {
        Message response_message.deserialize(response_packet);
        if (validate_response(response_message)) return response_message;
        else warn ValidateResponseError;
      }
      else warn << "Timeout" << std::endl;
    }

    if (attempts == max_attempts) raise "Failed Max Attempts";
  }
  else
    raise ValidateRequestError;
}

void Client::kill_client();
