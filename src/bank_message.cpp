#include "bank_message.h"
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

const char* Message::serialize () {
  std::uint32_t _magic_number = htonl(magic_number);
  std::uint32_t _request_id = htonl(request_id);
  std::uint32_t _request_type = htonl(request_type);
  std::uint32_t _account_number = htonl(account_number);
  std::uint32_t _currency_type = htonl(currency_type);
  std::uint32_t _is_reply = htonl(is_reply);
  std::uint32_t _success = htonl(success);
  float _amount;
  float _account_balance;

  std::uint32_t name_size = name.size() + 1;
  std::uint32_t password_size = 8;
  std::uint32_t _name_size = htonl(name_size);
  std::uint32_t _password_size = htonl(password_size);
  std::string name; // max_length = 63 characters + null
  std::string password; // // max_length = 7 characters + null

  char *buffer;
  memset(buffer, 0, sizeof(std::uint32_t) * 9 + sizeof(float) * 2 + 64 + 8);
  std::uint32_t buffer_offset = 0;

  memcpy(buffer + buffer_offset, (void*)_magic_number, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_request_id, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_request_type, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_account_number, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_currency_type, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_is_reply, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_success, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_amount, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(buffer + buffer_offset, (void*)_account_balance, sizeof(float));
  buffer_offset += sizeof(float);
  memcpy(buffer + buffer_offset, (void*)_name_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_name, _name_size);
  buffer_offset += _name_size;
  memcpy(buffer + buffer_offset, (void*)_password_size, sizeof(std::uint32_t));
  buffer_offset += sizeof(std::uint32_t);
  memcpy(buffer + buffer_offset, (void*)_password, _password_size);
  buffer_offset += _password_size;
  return buffer;
}

void Message::deserialize (const char*) {

}

// each message will have a request or reply id
