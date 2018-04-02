#ifndef INCLUDED_client_h
#define INCLUDED_client_h

class Server{
    std::uint64_t  server_id;  // client's ip_address and port HI 32 bits
    std::uint16_t log_level;
    std::unordered_map <request_id_type, response_message_type> at_most_once_map;
    std::unordered_map <account_number_type, account> accounts;
    UdpClient udp_client;  // initialise at startup
    std::error_stream;
};

void Server::Server(const std::string &host_address, const int port) {

}

std::uint64_t Server::open_account (std::string name, std::string password, std::uint16_t currency_type)  {
  //  generate random number 10 digit and while not present
  Account new_account (name, password, currency_type, account_number, start_amount);
  return account_number;
}

bool Server::close_account (std::string name, std::uint64_t account_number, std::string password) {
  if (authenticate (account_number, password, name)) {
    accounts.erase(account_number);
  }
  else {
    raise AuthenticationFailed;
  }
}

double Server::deposit_money (std::string name, std::uint64_t account_number, std::string password,
                              std::uint16_t currency_type, double amount) {
  if (authenticate (account_number, name, password)) {
    accounts[account_number].balance += exchange(amount, currency_type);
  }
  else {
    raise AuthenticationFailed;
  }
}
double Server::withdraw_money (std::string name, std::uint64_t account_number, std::string password,
                              std::uint16_t currency_type, double amount) {
  if (authenticate (account_number, name, password)) {
    accounts[account_number].balance -= exchange(amount, currency_type);
  }
  else {
    raise AuthenticationFailed;
  }
}

bool Server::take_loan (std::string name, std::uint64_t account_number, std::string password) {
  if (authenticate (account_number, name, password)) {
    accounts[account_number].balance += exchange(loan_amount, currency_type);
  }
  else {
    raise AuthenticationFailed;
  }
}

bool Server::change_password (std::string name, std::uint64_t account_number, std::string password, std::string new_password) {
  if (authenticate (account_number, name, password)) {
    accounts[account_number].password = new_password;
  }
  else {
    raise AuthenticationFailed;
  }
}

// bool Client::monitor                  (std::string name, std::string password);

void Server::process_messages () {
  while (true) {
    char request_packet[max_size];

    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);

    if (udp_server.recvfrom(request_packet, max_size, (struct sockaddr *) &client_address, (socklen_t *) &client_address_length) == packet_size) {
      Message response_message;
      Message request_message.deserialize(request_packet);
      if (validate_request (request_message)) {
        if (request_semantics == AT_MOST_ONCE &&
            at_most_once_map.find(request_message.request_id) != at_most_once_map.end()) {
          // respond with response from map
        }
        else {
          switch (request_message.request_type) {
            case OPEN_ACC:
              open_account(request_message.name, request_message.password, request_message.currency_type);
              break;
            case CLOSE_ACC:
              close_account(request_message.name, request_message.account_number, request_message.password);
              break;
            case DEPOSIT:
              deposit_money(request_message.name, request_message.account_number, request_message.password,
                            request_message.currency_type, request_message.amount);
              break;
            case WITHDRAW:
              withdraw_money(request_message.name, request_message.account_number, request_message.password,
                            request_message.currency_type, request_message.amount);
              break;
            case TAKE_LOAN:
              take_loan(request_message.name, request_message.account_number, request_message.password);
              break;
            case CHANGE_PASSWORD:
              change_password(request_message.name, request_message.account_number, request_message.password,
                              request_message.new_password);
              break;
            // default:
            //   break;
          }
        }
      }
      at_most_once_map[request_message.request_id] = response_message;
      udp_server.sendto(response_message.serialize(), max_size, (struct sockaddr *) &client_address, (socklen_t *) &client_address_length);
    }
    else raise InvalidRequestError;
  }
}

bool Server::authenticate (std::string name, std::uint64_t account_number, std::string password) {
  return accounts.find(account_number) != accounts.end() &&
         accounts[account_number].password == password &&
         accounts.name == name;
}

bool Server::validate_request (const Message & request_message) {
  return true;
  // same as in client
}

void Server::kill_server() {
  die;
}

#endif

// give exchange rates
