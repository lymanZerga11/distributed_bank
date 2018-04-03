#ifndef INCLUDED_client_h
#define INCLUDED_client_h

#define DEBUG 1

#include <string>

#include "../include/message.h"
#include "../include/udp_client.h"
#include "../utils/log.cpp"
#include "../utils/exceptions.cpp"

class Client{
    std::uint64_t client_id;  // client's ip_address and port HI 32 bits
    std::uint16_t log_level;
    std::uint16_t timeout;
    std::uint16_t request_semantic;
    std::uint16_t request_count;
    std::uint16_t max_request_attempts;
    // add config file with at-least-once and at-most-once
    // server ip Address
    // timeout period
    udp_client udp;
public:
    Client                        (const std::string &host_address, const int port);

    std::uint32_t open_account    (std::string name, std::string password, std::uint32_t currency_type, float amount);
    std::uint32_t close_account   (std::string name, std::uint32_t account_number, std::string password);
    float deposit_money           (std::string name, std::uint32_t account_number, std::string password, std::uint32_t currency_type, float amount);
    float withdraw_money          (std::string name, std::uint32_t account_number, std::string password, std::uint32_t currency_type, float amount);
    std::uint32_t take_loan       (std::string name, std::uint32_t account_number, std::string password);
    float check_balance           (std::string name, std::uint32_t account_number, std::string password);
    void monitor                  (std::uint32_t );

    bool validate_request         (const Message & request_message);
    bool validate_response        (const Message & response_message);
    Message get_response          (Message & request_message);
    void kill_client              ();
};

#endif

// give exchange rates add deposit amount for first and monitor
