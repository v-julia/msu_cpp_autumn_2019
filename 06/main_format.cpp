
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>



template <typename T>
void get_arg(std::vector<std::string>& vs, T&& arg)
{
    std::ostringstream oss;
    oss << arg;
    if ( !oss ) {
        throw std::runtime_error("input Arg Error");
    }
    vs.push_back(oss.str());
}


template <typename T, typename... Args>
void get_arg(std::vector<std::string>& vs, T&& arg, Args&&... args)
{
    get_arg(vs, arg);
    get_arg(vs, std::forward<Args>(args)...);
}


template <typename... Args>
void GetVectArgs(std::vector<std::string>& vs, Args&&... args)
{
    get_arg(vs, std::forward<Args>(args)...);
}


void InsertArgs(std::string& s, std::vector<std::string>& vs)
{
    char br_open = '{';
    char br_close = '}';
    size_t pos = 0;
    size_t pos_next = 0;

    while ( true ) {
        // найти открывающую скобку
        pos_next = s.find(br_open, pos);
        if ( pos_next == std::string::npos ) {
            break;
        }
        pos = pos_next;
        // найти закрывающую скобку
        pos_next = s.find(br_close, pos);
        if ( pos_next == std::string::npos ) {
            throw std::runtime_error("invalid formatted string syntax!");
        }
        // узнать номер аргумента
        int nom_arg;
        try {
            nom_arg = std::stoi(s.substr(pos + 1, pos_next - pos));
        }
        catch ( ... ) {
            throw std::runtime_error("invalid arg number in formatted string!");
        }
        if ( nom_arg < 0 || vs.size() <= nom_arg ) {
            throw std::runtime_error("invalid arg number in formatted string!");
        }
        // записать в строку вместо {123}
        s.replace(pos, pos_next - pos + 1, vs[nom_arg]);
        pos += vs[nom_arg].size();
    }
}


template <typename... Args>
std::string format(const std::string& inStr, Args&&... args)
{
    std::string s = inStr;
    std::vector<std::string> vs;

    GetVectArgs(vs, args...);

    InsertArgs(s, vs);

    return s;
}




int main()
{
    std::string ss;
    std::string s;

    int a = 3;



    std::cout << "\n" << "Test ONE. Correct" << "\n";
    s = "This is {0}. {0} bought {1} shares for {2} {3}. {0} shit {4} {3}. {0} is smart. Be like {0}!";
    std::cout << "Original string:" << "\n";
    std::cout << s << "\n";
    std::cout << "Format command : ss = format(s, 'Petya', 100, a, 'dollars', 100 * 3);" << "\n";
    
    ss = format(s, "Petya", 100, a, "dollars", 100 * a);
    
    std::cout << "Formated string:" << "\n";
    std::cout << ss << "\n";




    std::cout << "\n" << "\n" << "Test TWO. Exception" << "\n";
    try {
        s = "This is {0}. {0} bought {1} shares for {12} {3}. {0} shit {4} {3}. {0} is smart. Be like {0}!";
        std::cout << "Original string:" << "\n";
        std::cout << s << "\n";
        std::cout << "Format command : ss = format(s, 'Petya', 100, 3, 'dollars', 100 * 3);" << "\n";
        ss = format(s, "Petya", 100, a, "dollars", 100 * a);
        std::cout << "Formated string:" << "\n";
        std::cout << ss << "\n";
    }
    catch ( std::runtime_error & er ) {
        std::cout << "catched: " << er.what() << "\n";
    }




    std::cout << "\n" << "\n" << "Test THREE. Exception" << "\n";
    try {
        s = "This is {0}. {0} bought {1} shares for {2} {3}. {0} shit {4} {3}. {0} is smart. Be like {0!";
        std::cout << "Original string:" << "\n";
        std::cout << s << "\n";
        std::cout << "Format command : ss = format(s, 'Petya', 100, 3, 'dollars', 100 * 3);" << "\n";
        ss = format(s, "Petya", 100, a, "dollars", 100 * a);
        std::cout << "Formated string:" << "\n";
        std::cout << ss << "\n";
    }
    catch ( std::runtime_error & er ) {
        std::cout << "catched: " << er.what() << "\n";
    }


}


