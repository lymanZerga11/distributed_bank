#ifndef INCLUDED_client_h
#define INCLUDED_client_h

class Server{
    std::uint64_t  server_id;  // client's ip_address and port HI 32 bits
    std::uint16_t log_level;
    std::unordered_map <request_id_type, response_message_type> at_most_once_map;
    std::unordered_map <account_number_type, account> accounts;
    
    // add config file with at-least-once and at-most-once
    // warning and log level
    // server ip Address
    // timeout period

    UdpClient udp_client;  // initialise at startup
    std::error_stream;
};

void Server::Server(const std::string &host_address, const int port);

std::uint64_t Server::open_account    (std::string name, std::string password, std::uint16_t currency_type);
bool Server::close_account            (std::string name, std::uint64_t account_number, std::string password);
double Server::deposit_money          (std::string name, std::uint64_t account_number, std::string password, std::uint16_t currency_type, double amount);
double Server::withdraw_money         (std::string name, std::uint64_t account_number, std::string password, std::uint16_t currency_type, double amount);
bool Server::take_loan                (std::string name, std::uint64_t account_number, std::string password);
bool Server::change_password          (std::string name, std::string password);
// bool Client::monitor                  (std::string name, std::string password);

void Server::process_message          ();
Account Server::authenticate          (std::string name, std::uint64_t account_number, std::string password);
bool Server::validate_request         (const Message & request_message);

void Server::kill_server();

#endif

// give exchange rates
