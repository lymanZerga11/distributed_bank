#ifndef INCLUDED_message_h
#define INCLUDED_message_h

#include <string>

#define MAGIC_NUMBER 12590
#define VERSION 1
#define PACKET_SIZE 192

#define INVALID_CLIENT_ID "0"
#define INVALID_NAME "0"
#define INVALID_ACCOUNT_NUMBER "0"
#define INVALID_PASSWORD "0"
#define INVALID_CURRENCY_TYPE "0"
#define INVALID_AMOUNT "0"
#define INVALID_RECIPIENT_ACCOUNT_NUMBER "0"

enum client_requests {OPEN_ACC, CLOSE_ACC, DEPOSIT, WITHDRAW, TRANSFER, CHANGE_PASSWORD};

class Message {
public:
    std::uint64_t magic_number = MAGIC_NUMBER;
    std::string client_id = INVALID_CLIENT_ID;  // all default to the invalid
    std::uint16_t version = VERSION;

    std::uint64_t request_id;
    std::uint16_t request_type;

    std::string name;
    std::uint64_t account_number;
    std::string password;
    std::uint16_t currency_type;
    double amount;
    std::uint64_t recipient_account_number;

    // bool is_reply;
    // double account_balance;
    // bool success;

    void packet_to_message (const char*, Message&);
    void message_to_packet (const Message&, char*);
};

#endif

// each message will have a request or reply id
