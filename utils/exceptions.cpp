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
  const char * what () const throw () {
      return "ServerSideError";
   }
};

class InvalidInputError: public std::exception
{
  const char * what () const throw () {
      return "InvalidInputError";
   }
};