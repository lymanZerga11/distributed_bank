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

uint64_t htonll(uint64_t value) {
    static const int num = 42;
    if (*reinterpret_cast<const char*>(&num) == num) {
        const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
        const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));
        return (static_cast<uint64_t>(low_part) << 32) | high_part;
    }
    else {
        return value;
    }
}

uint64_t ntohll(uint64_t value) {
    static const int num = 42;
    if (*reinterpret_cast<const char*>(&num) == num) {
        const uint32_t high_part = ntohl(static_cast<uint32_t>(value >> 32));
        const uint32_t low_part = ntohl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));
        return (static_cast<uint64_t>(low_part) << 32) | high_part;
    }
    else {
        return value;
    }
}

Message::Message () {
  request_id = DEFAULT_REQUEST_ID;
  magic_number = DEFAULT_MAGIC_NUMBER;
  request_type = DEFAULT_REQUEST_TYPE;
  account_number = DEFAULT_ACCOUNT_NUMBER;
  currency_type = DEFAULT_CURRENCY_TYPE;
  is_reply = DEFAULT_IS_REPLY;
  success = DEFAULT_SUCCESS;
  amount = DEFAULT_AMOUNT;
  account_balance = DEFAULT_ACCOUNT_BALANCE;
  name = DEFAULT_NAME;
  password = DEFAULT_PASSWORD;
  request_semantic = DEFAULT_REQUEST_SEMANTIC;
  error_data = DEFAULT_ERROR_DATA;
  monitor_data = DEFAULT_MONITOR_DATA;
  monitor_interval_in_seconds = DEFAULT_MONITOR_INTERVAL_IN_SECONDS;
}

Message::Message (uint64_t request_id) : request_id(request_id) {
  magic_number = DEFAULT_MAGIC_NUMBER;
  request_type = DEFAULT_REQUEST_TYPE;
  account_number = DEFAULT_ACCOUNT_NUMBER;
  currency_type = DEFAULT_CURRENCY_TYPE;
  is_reply = DEFAULT_IS_REPLY;
  success = DEFAULT_SUCCESS;
  amount = DEFAULT_AMOUNT;
  account_balance = DEFAULT_ACCOUNT_BALANCE;
  name = DEFAULT_NAME;
  password = DEFAULT_PASSWORD;
  request_semantic = DEFAULT_REQUEST_SEMANTIC;
  error_data = DEFAULT_ERROR_DATA;
  monitor_data = DEFAULT_MONITOR_DATA;
  monitor_interval_in_seconds = DEFAULT_MONITOR_INTERVAL_IN_SECONDS;
}

void Message::serialize (char* buffer) {
  std::uint32_t _magic_number = htonl(magic_number);
  std::uint64_t _request_id = htonll(request_id);
  std::uint32_t _request_type = htonl(request_type);
  std::uint32_t _account_number = htonl(account_number);
  std::uint32_t _currency_type = htonl(currency_type);
  std::uint32_t _is_reply = htonl(is_reply);
  std::uint32_t _success = htonl(success);
  std::uint32_t _request_semantic = htonl(request_semantic);
  std::uint32_t _monitor_interval_in_seconds = htonl(monitor_interval_in_seconds);

  float _amount = htonf(amount);
  float _account_balance = htonf(account_balance);

  std::uint32_t name_size = name.size() + 1;
  std::uint32_t error_data_size = error_data.size() + 1;
  std::uint32_t monitor_data_size = monitor_data.size() + 1;
  std::uint32_t password_size = PASSWORD_SIZE;

  std::uint32_t _name_size = htonl(name_size);
  std::uint32_t _error_data_size = htonl(error_data_size);
  std::uint32_t _monitor_data_size = htonl(monitor_data_size);
  std::uint32_t _password_size = htonl(password_size);

  std::string _name = name; // max_length = 63 characters + null
  std::string _error_data = error_data; // // max_length = 63 characters + null
  std::string _monitor_data = monitor_data; // // max_length = 63 characters + null
  std::string _password = password; // // max_length = 7 characters + null

  memset(buffer, 0, sizeof(std::uint32_t) * 9 + sizeof(std::uint64_t) * 1 + sizeof(float) * 2 + MAX_NAME_SIZE
                    + PASSWORD_SIZE + MAX_ERROR_DATA_SIZE + MAX_MONITOR_DATA_SIZE);
  std::uint32_t buffer_offset = 0;
  memcpy(buffer + buffer_offset, (void*)&_magic_number, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_request_id, sizeof(std::uint64_t));
  buffer_offset += sizeof(std::uint64_t);
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
  memcpy(buffer + buffer_offset, (void*)&_request_semantic, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)&_monitor_interval_in_seconds, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);

  memcpy(buffer + buffer_offset, (void*)&_amount, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(buffer + buffer_offset, (void*)&_account_balance, sizeof(float));
  buffer_offset += sizeof(float);

  memcpy(buffer + buffer_offset, (void*)&_name_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, _name.c_str(), name_size);
  buffer_offset += name_size;

  memcpy(buffer + buffer_offset, (void*)&_error_data_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, _error_data.c_str(), error_data_size);
  buffer_offset += error_data_size;

  memcpy(buffer + buffer_offset, (void*)&_monitor_data_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, _monitor_data.c_str(), monitor_data_size);
  buffer_offset += monitor_data_size;

  memcpy(buffer + buffer_offset, (void*)&_password_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, _password.c_str(), password_size);
  buffer_offset += password_size;
}

void Message::deserialize (char* buffer) {
  std::uint32_t _magic_number;
  std::uint64_t _request_id;
  std::uint32_t _request_type;
  std::uint32_t _account_number;
  std::uint32_t _currency_type;
  std::uint32_t _is_reply;
  std::uint32_t _success;
  std::uint32_t _request_semantic;
  std::uint32_t _monitor_interval_in_seconds;

  float _amount;
  float _account_balance;

  std::uint32_t _name_size;
  std::uint32_t _error_data_size;
  std::uint32_t _monitor_data_size;
  std::uint32_t _password_size;

  std::string _name; // max_length = 63 characters + null
  std::string _error_data; // max_length = 63 characters + null
  std::string _monitor_data; // max_length = 63 characters + null
  std::string _password; // max_length = 7 characters + null

  _name.resize(MAX_NAME_SIZE);
  _error_data.resize(MAX_ERROR_DATA_SIZE);
  _monitor_data.resize(MAX_MONITOR_DATA_SIZE);
  _password.resize(PASSWORD_SIZE);
  // assert(buffer_size == sizeof(std::uint32_t) * 9 + sizeof(float) * 2 + 64 + 8);
  std::uint32_t buffer_offset = 0;
  memcpy(&_magic_number, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_request_id, buffer + buffer_offset, sizeof(std::uint64_t));
  buffer_offset += sizeof(std::uint64_t);
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
  memcpy(&_request_semantic, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(&_monitor_interval_in_seconds, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);

  memcpy(&_amount, buffer + buffer_offset, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(&_account_balance, buffer + buffer_offset, sizeof(float));
  buffer_offset += sizeof(float);

  memcpy(&_name_size, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy((char*)_name.data(), buffer + buffer_offset, ntohl(_name_size));
  buffer_offset += ntohl(_name_size);

  memcpy(&_error_data_size, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy((char*)_error_data.data(), buffer + buffer_offset, ntohl(_error_data_size));
  buffer_offset += ntohl(_error_data_size);

  memcpy(&_monitor_data_size, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy((char*)_monitor_data.data(), buffer + buffer_offset, ntohl(_monitor_data_size));
  buffer_offset += ntohl(_monitor_data_size);

  memcpy(&_password_size, buffer + buffer_offset, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy((char*)_password.data(), buffer + buffer_offset, ntohl(_password_size));
  buffer_offset += ntohl(_password_size);

  magic_number = ntohl(_magic_number);
  request_id = ntohll(_request_id);
  request_type = ntohl(_request_type);
  account_number = ntohl(_account_number);
  currency_type = ntohl(_currency_type);
  is_reply = ntohl(_is_reply);
  success = ntohl(_success);
  request_semantic = ntohl(_request_semantic);
  monitor_interval_in_seconds = ntohl(_monitor_interval_in_seconds);

  amount = ntohf(_amount);
  account_balance = ntohf(_account_balance);

  name = _name;
  error_data = _error_data;
  password = _password;
  monitor_data = _monitor_data;
}
