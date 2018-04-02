#include "../include/message.h"
#include <arpa/inet.h>
#include <string.h>

float htonf(float val) {
    uint32_t rep;
    memcpy(&rep, &val, sizeof(rep));
    rep = htonl(rep);
    memcpy(&val, &rep, sizeof(rep));
    return val;
}

float ntohf(float val) {
    uint32_t rep;
    memcpy(&rep, &val, sizeof(rep));
    rep = ntohl(rep);
    memcpy(&val, &rep, sizeof(rep));
    return val;
}

Message::Message () {
  magic_number = DEFAULT_MAGIC_NUMBER;
  request_id = DEFAULT_REQUEST_ID;
  request_type = DEFAULT_REQUEST_TYPE;
  account_number = DEFAULT_ACCOUNT_NUMBER;
  currency_type = DEFAULT_CURRENCY_TYPE;
  is_reply = DEFAULT_IS_REPLY;
  success = DEFAULT_SUCCESS;
  amount = DEFAULT_AMOUNT;
  account_balance = DEFAULT_ACCOUNT_BALANCE;
  name = DEFAULT_NAME;
  password = DEFAULT_PASSWORD;
}

void Message::serialize (char* buffer) {
  std::uint32_t _magic_number = htonl(magic_number);
  std::uint32_t _request_id = htonl(request_id);
  std::uint32_t _request_type = htonl(request_type);
  std::uint32_t _account_number = htonl(account_number);
  std::uint32_t _currency_type = htonl(currency_type);
  std::uint32_t _is_reply = htonl(is_reply);
  std::uint32_t _success = htonl(success);
  float _amount = htonf(amount);
  float _account_balance = htonf(account_balance);

  std::uint32_t name_size = name.size() + 1;
  std::uint32_t password_size = PASSWORD_SIZE;
  std::uint32_t _name_size = htonl(name_size);
  std::uint32_t _password_size = htonl(password_size);
  std::string _name = name; // max_length = 63 characters + null
  std::string _password = password; // // max_length = 7 characters + null

  memset(buffer, 0, sizeof(std::uint32_t) * 9 + sizeof(float) * 2 + MAX_NAME_SIZE + PASSWORD_SIZE);
  std::uint32_t buffer_offset = 0;
  memcpy(buffer + buffer_offset, (void*)&_magic_number, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_request_id, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_request_type, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_account_number, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_currency_type, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_is_reply, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_success, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_amount, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(buffer + buffer_offset, (void*)&_account_balance, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(buffer + buffer_offset, (void*)&_name_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, _name.c_str(), name_size);
  buffer_offset += name_size;
  memcpy(buffer + buffer_offset, (void*)&_password_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, _password.c_str(), password_size);
  buffer_offset += password_size;
}

void Message::deserialize (char* buffer) {
  std::uint32_t _magic_number;
  std::uint32_t _request_id;
  std::uint32_t _request_type;
  std::uint32_t _account_number;
  std::uint32_t _currency_type;
  std::uint32_t _is_reply;
  std::uint32_t _success;
  float _amount;
  float _account_balance;

  std::uint32_t _name_size;
  std::uint32_t _password_size;
  std::string _name; // max_length = 63 characters + null
  std::string _password; // // max_length = 7 characters + null

  _name.resize(MAX_NAME_SIZE);
  _password.resize(PASSWORD_SIZE);
  // assert(buffer_size == sizeof(std::uint32_t) * 9 + sizeof(float) * 2 + 64 + 8);
  std::uint32_t buffer_offset = 0;
  memcpy(&_magic_number, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_request_id, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_request_type, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_account_number, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_currency_type, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_is_reply, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_success, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_amount, buffer + buffer_offset, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(&_account_balance, buffer + buffer_offset, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(&_name_size, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy((char*)_name.data(), buffer + buffer_offset, ntohl(_name_size));
  buffer_offset += ntohl(_name_size);
  memcpy(&_password_size, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy((char*)_password.data(), buffer + buffer_offset, ntohl(_password_size));
  buffer_offset += ntohl(_password_size);

  magic_number = ntohl(_magic_number);
  request_id = ntohl(_request_id);
  request_type = ntohl(_request_type);
  account_number = ntohl(_account_number);
  currency_type = ntohl(_currency_type);
  is_reply = ntohl(_is_reply);
  success = ntohl(_success);
  amount = ntohf(_amount);
  account_balance = ntohf(_account_balance);
  name = _name;
  password = _password;
}
