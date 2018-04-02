#ifndef INCLUDED_client_h
#define INCLUDED_client_h

#include <string>

#include "../include/message.h"

class Client{
    std::uint64_t client_id;  // client's ip_address and port HI 32 bits
    std::uint16_t log_level;
    std::uint16_t timeout;
    std::uint16_t request_semantic;
    std::uint16_t request_count;
    // add config file with at-least-once and at-most-once
    // server ip Address
    // timeout period
    udp_client udp;
};

void Client::Client                   (const std::string &host_address, const int port);

std::uint32_t Client::open_account    (std::string name, std::string password, std::uint32_t currency_type, float amount);
std::uint32_t Client::close_account   (std::string name, std::uint32_t account_number, std::string password);
float Client::deposit_money           (std::string name, std::uint32_t account_number, std::string password, std::uint32_t currency_type, float amount);
float Client::withdraw_money          (std::string name, std::uint32_t account_number, std::string password, std::uint32_t currency_type, float amount);
std::uint32_t Client::take_loan       (std::string name, std::uint32_t account_number, std::string password);
float Client::check_balance           (std::string name, std::uint32_t account_number, std::string password);
// bool Client::monitor                  (std::string name, std::string password);

bool Client::validate_request         (const Message & request_message);
bool Client::validate_response        (const Message & response_message);
Message Client::get_response          (const Message & request_message);

void Client::kill_client();

#endif

// give exchange rates add deposit amount for first and monitor
