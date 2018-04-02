#ifndef INCLUDED_client_h
#define INCLUDED_client_h

class Client{
    std::uint64_t  client_id;  // client's ip_address and port HI 32 bits
    std::uint16_t log_level;
    std::uint16_t timeout;
    std::uint16_t request_semantic;
    // add config file with at-least-once and at-most-once
    // warning and log level
    // server ip Address
    // timeout period

    UdpClient udp_client;  // initialise at startup
    std::error_stream;
};

void Client::Client(const std::string &host_address, const int port);

std::uint32_t Client::open_account    (std::string name, std::string password, std::uint32_t currency_type);
bool Client::close_account            (std::string name, std::uint32_t account_number, std::string password);
double Client::deposit_money          (std::string name, std::uint32_t account_number, std::string password, std::uint32_t currency_type, double amount);
double Client::withdraw_money         (std::string name, std::uint32_t account_number, std::string password, std::uint32_t currency_type, double amount);
bool Client::take_loan                (std::string name, std::uint32_t account_number, std::string password);
double Client::check_balance          (std::string name, std::uint32_t account_number, std::string password);
// bool Client::monitor                  (std::string name, std::string password);

bool Client::validate_request         (const Message & request_message);
bool Client::validate_response        (const Message & response_message);
Message Client::get_response          (const Message & request_message);

void Client::kill_client();

#endif

// give exchange rates
