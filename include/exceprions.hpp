#include <exception>
#include <string>
class FilterOverflowException : public std::exception
{
private:
    std::string _message;

public:
    FilterOverflowException(const std::string &message) : _message(message) {}

    const char *what() const noexcept override
    {
        return _message.c_str();
    }
};