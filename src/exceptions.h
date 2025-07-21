#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class TradingException : public std::exception {
private:
    std::string message;
public:
    explicit TradingException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class APIException : public TradingException {
public:
    explicit APIException(const std::string& msg) : TradingException("API Error: " + msg) {}
};

class DataException : public TradingException {
public:
    explicit DataException(const std::string& msg) : TradingException("Data Error: " + msg) {}
};

class CalculationException : public TradingException {
public:
    explicit CalculationException(const std::string& msg) : TradingException("Calculation Error: " + msg) {}
};

#endif // EXCEPTIONS_H