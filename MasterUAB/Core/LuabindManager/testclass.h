#include <string>
#include <iostream>

class testclass
{
public:
    testclass(const std::string& s): m_string(s) {}
    void print_string() { std::cout << m_string << "\n"; }

private:
    std::string m_string;
};