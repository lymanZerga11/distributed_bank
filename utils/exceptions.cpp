#include <iostream>
#include <exception>

class MaxAttemptsError: public std::exception
{
  const char * what () const throw () {
      return "MaxAttemptsError";
   }
};

class ServerSideError: public std::exception
{ 
  std::string error_data;
  public:
  ServerSideError (std::string _error_data) : error_data(_error_data) {}
  const char * what () const throw () {
      return error_data.c_str();
   }
};

class InvalidInputError: public std::exception
{
  const char * what () const throw () {
      return "InvalidInputError";
   }
};

class AuthenticationFailed: public std::exception
{
  const char * what () const throw () {
      return "AuthenticationFailed";
   }
};

