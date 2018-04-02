#include <iostream>
#include "../include/message.h"

int main () {
    Message message, de_message;
    message.magic_number = 1235;
    message.request_id = 3;
    message.request_type = 0;
    message.account_number = 12344;
    message.currency_type = 1;
    message.is_reply = 0;
    message.success = 1;
    message.amount = 54.3;
    message.account_balance = 54.34;

    char buf[1000];
    message.serialize(buf);
    de_message.deserialize(buf);
    std::cout << de_message.name << std::endl;
    std::cout << de_message.password << std::endl;
    std::cout << de_message.magic_number << std::endl;
    std::cout << de_message.request_id << std::endl;
    std::cout << de_message.request_type << std::endl;
    std::cout << de_message.account_number << std::endl;
    std::cout << de_message.currency_type << std::endl;
    std::cout << de_message.is_reply << std::endl;
    std::cout << de_message.success << std::endl;
    std::cout << de_message.amount << std::endl;
    std::cout << de_message.account_balance << std::endl;

    return 0;
}
