#include "../include/server.h"

float exchange (float amount, std::uint32_t currency_type) {
  float new_amount = amount;
  switch (currency_type) {
    case SGD:
      new_amount = amount;
      break;
    case USD:
      new_amount = 1.31 * amount;
      break;
    case QAR:
      new_amount = 0.36 * amount;
      break;
  }
  return new_amount;
}

Server::Server(const std::string &host_address, const int port) : udp (host_address, port){
  if (DEBUG) log(WARN) << "Server initialised on " + host_address + ":" + std::to_string(port) + "..." <<std::endl;
}

std::uint32_t Server::open_account (std::string name, std::string password, std::uint16_t currency_type, float amount)  {
  std::uint32_t account_number = 1000000001;
  while (accounts.find(account_number) != accounts.end()) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine engine(seed);
    std::uniform_int_distribution<uint32_t> distr(1000000000, 3000000000);
    account_number = distr(engine);
  }
  Account new_account (name, password, account_number, exchange(amount, currency_type));
  accounts[account_number] = new_account;
  log(INFO) << "Opened Account - " << new_account <<std::endl;
  std::stringstream update; update << "Opened Account - " << new_account;
  update_monitors (update.str());
  return account_number;
}

std::uint32_t Server::close_account (std::string name, std::uint32_t account_number, std::string password) {
  std::uint32_t success = 0;
  if (authenticate (name, account_number, password)) {
    log(INFO) << "Closing Account - " << accounts[account_number] << std::endl;
    std::stringstream update; update << "Closing Account - " << accounts[account_number];
    update_monitors (update.str());
    accounts.erase(account_number);
    success = 1;
  }
  else {
    throw AuthenticationFailed();
  }
  return success;
}

float Server::deposit_money (std::string name, std::uint32_t account_number, std::string password,
                              std::uint16_t currency_type, float amount) {
  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  if (authenticate (name, account_number, password)) {
    accounts[account_number].balance += exchange(amount, currency_type);
    account_balance = accounts[account_number].balance;
  }
  else {
    throw AuthenticationFailed();
  }
  log(INFO) << "Deposit Money - " << accounts[account_number] << std::endl;
  std::stringstream update; update << "Deposit Money - " << accounts[account_number];
  update_monitors (update.str());
  return account_balance;
}

float Server::withdraw_money (std::string name, std::uint32_t account_number, std::string password,
                              std::uint16_t currency_type, float amount) {
  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  if (authenticate (name, account_number, password)) {
    accounts[account_number].balance -= exchange(amount, currency_type);
    account_balance = accounts[account_number].balance;
  }
  else {
    throw AuthenticationFailed();
  }
  log(INFO) << "Withdraw Money - " << accounts[account_number] << std::endl;
  std::stringstream update; update << "Withdraw Money - " << accounts[account_number];
  update_monitors (update.str());
  return account_balance;
}

std::uint32_t Server::take_loan (std::string name, std::uint32_t account_number, std::string password) {
  std::uint32_t success = 0;
  if (authenticate (name, account_number, password)) {
    accounts[account_number].balance += LOAN_AMOUNT;
    success = 1;
  }
  else {
    throw AuthenticationFailed();
  }
  log(INFO) << "Take Loan - " << accounts[account_number] << std::endl;
  std::stringstream update; update << "Take Loan - " << accounts[account_number];
  update_monitors (update.str());
  return success;
}

float Server::check_balance (std::string name, std::uint32_t account_number, std::string password) {
  float account_balance = DEFAULT_ACCOUNT_BALANCE;
  if (authenticate (name, account_number, password)) {
    account_balance = accounts[account_number].balance;
  }
  else {
    throw AuthenticationFailed();
  }
  log(INFO) << "Check Balance - " << accounts[account_number] << std::endl;
  std::stringstream update; update << "Check Balance - " << accounts[account_number];
  update_monitors (update.str());
  return account_balance;
}

// bool Client::monitor                  (std::string name, std::string password);

void Server::process_messages () {
  while (true) {
    char request_packet[PACKET_SIZE];
    char response_packet[PACKET_SIZE];
    struct sockaddr_in client_address;
    int client_address_length = sizeof(client_address);

    if (udp.recv(udp.get_socket(), request_packet, PACKET_SIZE, 0,
        client_address, client_address_length) > 1) {
      Message request_message (DEFAULT_REQUEST_ID);
      Message response_message (DEFAULT_REQUEST_ID);
      response_message.is_reply = 1;

      request_message.deserialize(request_packet);
      response_message.request_id = request_message.request_id;

      if (DEBUG) log(WARN) << request_message << std::endl;
      try {
        if (validate_request (request_message)) {
          if (at_most_once_map.find(request_message.request_id) != at_most_once_map.end()) {
            if (request_message.request_semantic == AT_MOST_ONCE) {
              log(WARN) << "Invoked at-most-once on command already run on request_id " + std::to_string(request_message.request_id) << std::endl;
              response_message = at_most_once_map[request_message.request_id];
            }
            else
              log(WARN) << "Invoked at-least-once on command already run on request_id " + std::to_string(request_message.request_id) << std::endl;
          }
          else {
            switch (request_message.request_type) {
              case OPEN_ACC:
                response_message.account_number = open_account(request_message.name, request_message.password, request_message.currency_type, request_message.amount);
                break;
              case CLOSE_ACC:
                response_message.success = close_account(request_message.name, request_message.account_number, request_message.password);
                break;
              case DEPOSIT:
                response_message.account_balance = deposit_money(request_message.name, request_message.account_number, request_message.password,
                              request_message.currency_type, request_message.amount);
                break;
              case WITHDRAW:
                response_message.account_balance = withdraw_money(request_message.name, request_message.account_number, request_message.password,
                              request_message.currency_type, request_message.amount);
                break;
              case TAKE_LOAN:
                response_message.success = take_loan(request_message.name, request_message.account_number, request_message.password);
                break;
              case CHECK_BALANCE:
                response_message.account_balance = check_balance(request_message.name, request_message.account_number, request_message.password);
                break;
              case MONITOR:
                monitor_clients.push_back(monitor_client(client_address, client_address_length, request_message.request_id, request_message.monitor_interval_in_seconds));
                log(INFO) << "Monitor Added - " << inet_ntoa(client_address.sin_addr) << ":" << ntohl(client_address.sin_port) << std::endl;
                std::stringstream update; update << "Monitor Added - " << inet_ntoa(client_address.sin_addr) << ":" << ntohl(client_address.sin_port);
                update_monitors (update.str());
                break;
            }
          }
          at_most_once_map[request_message.request_id] = response_message;
        }
        else throw InvalidInputError();
      }
      catch (InvalidInputError & e) {
        log(ERROR) << "Invalid request received on request_id " + std::to_string(request_message.request_id) << std::endl;;
        response_message.error_data = "Invalid request received on request_id " + std::to_string(request_message.request_id);
      }
      catch (AuthenticationFailed & e) {
        log(ERROR) << "Authentication Failed on request_id " + std::to_string(request_message.request_id) << std::endl;;
        response_message.error_data = "Authentication Failed on request_id " + std::to_string(request_message.request_id);
      }
      response_message.serialize(response_packet);
      udp.send(udp.get_socket(), response_packet, PACKET_SIZE, 0,
                 client_address, client_address_length);
    }
  }
}

bool Server::authenticate (std::string name, std::uint32_t account_number, std::string password) {
  return accounts.find(account_number) != accounts.end() &&
         accounts[account_number].password == password &&
         accounts[account_number].name == name;
}

bool Server::validate_request (const Message & request_message) {
  return true;
  // same as in client
}

void Server::kill_server() {
  ;
}

void Server::update_monitors (std::string update) {
    for (auto monitor : monitor_clients) {
      if (monitor.monitor_endpoint > std::chrono::system_clock::now()) {
        Message response_message (DEFAULT_REQUEST_ID);

        response_message.request_id = monitor.request_id;
        response_message.monitor_data = update;
        response_message.is_reply = 1;
        char response_packet[PACKET_SIZE];
        response_message.serialize(response_packet);
        udp.send(udp.get_socket(), response_packet, PACKET_SIZE, 0,
                 monitor.client_address, monitor.client_address_length);

        // add code on client side to monitor loop
      }
      else {
        // remove
      }
    }
}
