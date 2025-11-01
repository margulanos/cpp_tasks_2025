#include <string>
#include <exception>

class MathException : public std::exception {
private:
    std::string message;

public:
    MathException() : message("Math error") {}
    
    MathException(const std::string& msg) : message(msg) {}
    
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
    
    virtual ~MathException() = default;
};
