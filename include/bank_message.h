#ifndef INCLUDED_message_h
#define INCLUDED_message_h

#include <string>

#define MAGIC_NUMBER 12590
#define VERSION 1
#define PACKET_SIZE 192

#define INVALID_NAME "0"
#define INVALID_REQUEST_TYPE 0
#define INVALID_REQUEST_ID 0
#define INVALID_ACCOUNT_NUMBER 0
#define INVALID_PASSWORD "0"
#define INVALID_CURRENCY_TYPE 0
#define INVALID_AMOUNT 0.00
#define INVALID_ACCOUNT_BALANCE 0.00

enum client_requests {INVALID=0, OPEN_ACC, CLOSE_ACC, DEPOSIT, WITHDRAW, TAKE_LOAN, CHECK_BALANCE};

class Message {
public:
    std::uint32_t magic_number;
    std::uint32_t request_id;
    std::uint32_t request_type;
    std::uint32_t account_number;
    std::uint32_t currency_type;
    std::uint32_t is_reply;
    std::uint32_t success;
    float amount;
    float account_balance;

    std::string name; // max_length = 64 characters
    std::string password; // // max_length = 64 characters
};

const char* Message::serialize   ();
void Message::deserialize        (const char*);

#endif

// each message will have a request or reply id
