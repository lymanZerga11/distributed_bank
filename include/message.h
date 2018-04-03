#ifndef INCLUDED_message_h
#define INCLUDED_message_h

#include <string>
#include <iostream>

#define DEFAULT_MAGIC_NUMBER 12590
#define VERSION 1.0
#define PACKET_SIZE 512

#define DEFAULT_NAME "0"
#define DEFAULT_REQUEST_TYPE 0
#define DEFAULT_REQUEST_ID 0
#define DEFAULT_ACCOUNT_NUMBER 0
#define DEFAULT_PASSWORD "0000000"
#define DEFAULT_CURRENCY_TYPE 0
#define DEFAULT_AMOUNT 0.00
#define DEFAULT_ACCOUNT_BALANCE 0.00
#define DEFAULT_IS_REPLY 2
#define DEFAULT_SUCCESS 2
#define DEFAULT_REQUEST_SEMANTIC 0
#define DEFAULT_ERROR_DATA "0"
#define DEFAULT_MONITOR_DATA "0"

#define DEBUG 1

#define MAX_NAME_SIZE 64
#define MAX_ERROR_DATA_SIZE 64
#define MAX_MONITOR_DATA_SIZE 64
#define PASSWORD_SIZE 8

enum client_requests {INVALID_REQUEST=0, OPEN_ACC, CLOSE_ACC, DEPOSIT, WITHDRAW, TAKE_LOAN, CHECK_BALANCE};
enum currency_types {INVALID_CURRENCY=0, SGD, USD, QAR};
enum request_semantics {AT_MOST_ONCE=0, AT_LEAST_ONCE};

class Message {
public:
    std::uint32_t magic_number;
    std::uint64_t request_id;
    std::uint32_t request_type;
    std::uint32_t account_number;
    std::uint32_t currency_type;
    std::uint32_t is_reply;
    std::uint32_t success;
    std::uint32_t request_semantic;
    float amount;
    float account_balance;

    std::string name;  // max_length = 63 characters + null
    std::string password;  // max_length = 7 characters + null
    std::string error_data;  // max_length = 63 characters + null
    std::string monitor_data; // max_length = 63 characters + null

    Message                 (uint64_t request_id);
    Message                 ();
    void serialize          (char*);
    void deserialize        (char*);
};

inline std::ostream& operator<<(std::ostream& os, const Message& message) {
    os << "magic_number: " << message.magic_number << " | ";
    os << "request_id: " << message.request_id << " | ";
    os << "request_type: " << message.request_type << " | ";
    os << "account_number: " << message.account_number << " | ";
    os << "currency_type: " << message.currency_type << " | ";
    os << "is_reply: " << message.is_reply << " | ";
    os << "success: " << message.success << " | ";
    os << "request_semantic: " << message.request_semantic << " | ";
    os << "amount: " << message.amount << " | ";
    os << "account_balance: " << message.account_balance << " | ";
    os << "name: " << message.name << " | ";
    os << "password: " << message.password << " | ";
    os << "error_data: " << message.error_data << " | ";
    os << "monitor_data: " << message.monitor_data;
    return os;
}

// fix defaults

#endif
