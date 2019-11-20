
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>



template <typename T>
void GetVectArgs(std::vector<std::string>& vs, T&& arg)
{
    std::ostringstream oss;
    oss << arg;
    if ( !oss ) {
        throw std::runtime_error("input Arg Error");
    }
    vs.push_back(oss.str());
}

template <typename T, typename... Args>
void GetVectArgs(std::vector<std::string>& vs, T&& arg, Args&&... args)
{
    GetVectArgs(vs, std::forward<T>(arg));
    GetVectArgs(vs, std::forward<Args>(args)...);
}

enum class FindBr
{
    find_open_br,
    find_close_br
};

std::string InsertArgs(const std::string& s, std::vector<std::string>& vs)
{
    std::string r;
    char br_open = '{';
    char br_close = '}';
    size_t pos_open_br = 0;
    size_t pos_close_br = 0;
    FindBr find_br = FindBr::find_open_br;

    for ( size_t i = 0; i < s.size(); ++i ) {
        const char& c = s[i];
        if ( find_br == FindBr::find_open_br ) {
            // найти открывающую скобку
            if ( c == br_open ) {
                pos_open_br = i;
                r += s.substr(pos_close_br, pos_open_br - pos_close_br);
                find_br = FindBr::find_close_br;
            }
            else if ( c == br_close ) {
                throw std::runtime_error("invalid syntax - }order}!");
            }
        }
        else {
            // найти закрывающую скобку
            if ( c == br_close ) {
                pos_close_br = i + 1;
                std::string tmp = s.substr(pos_open_br + 1, pos_close_br - pos_open_br);
                // узнать номер аргумента
                int nom_arg = std::stoi(tmp);
                if ( nom_arg < 0 || vs.size() <= nom_arg ) {
                    throw std::runtime_error("invalid arg number in formatted string!");
                }
                r += vs[nom_arg]; // подставить arg
                find_br = FindBr::find_open_br;
            }
            else if ( c == br_open ) {
                throw std::runtime_error("invalid syntax -  {order{!");
            }
        }

    }
    if ( find_br == FindBr::find_close_br ) {
        throw std::runtime_error("invalid syntax - }!");
    }

    return r;
}


template <typename... Args>
std::string format(const std::string& inStr, Args&&... args)
{
    std::vector<std::string> vs;

    GetVectArgs(vs, args...);

    return InsertArgs(inStr, vs);

}




int main()
{
    std::string ss;
    std::string s;

    int a = 3;



    std::cout << "\n" << "Test ONE. Correctly" << "\n";

    s = "This is {0}. {0} bought {1} shares for {2} {3}. {0} lost {4} {3}. {0} is smart. Be like {0}!";
    std::cout << "Original string:" << "\n";
    std::cout << s << "\n";
    std::cout << "Format command : ss = format(s, 'Petya', 100, a, 'dollars', 100 * 3);" << "\n";

    ss = format(s, "Petya", 100, a, "dollars", 100 * a);

    std::cout << "Formated string:" << "\n";
    std::cout << ss << "\n";




    std::cout << "\n" << "\n" << "Test TWO. Exception" << "\n";
    try {
        s = "This is {0}. {0} bought {1} shares for {12} {3}. {0} lost {4} {3}. {0} is smart. Be like {0}!";
        ss = format(s, "Petya", 100, a, "dollars", 100 * a);
    }
    catch ( std::runtime_error & er ) {
        std::cout << "catched: " << er.what() << "\n";
    }




    std::cout << "\n" << "\n" << "Test THREE. Exception" << "\n";
    try {
        s = "This is {0}. {0} bought {1} shares for {2} {3}. {0} lost {4} {3}. {0} is smart. Be like {0!";
        ss = format(s, "Petya", 100, a, "dollars", 100 * a);
    }
    catch ( std::runtime_error & er ) {
        std::cout << "catched: " << er.what() << "\n";
    }




    std::cout << "\n" << "\n" << "Test FOUR. Exception" << "\n";
    try {
        s = "This is {0}. {{0} bought {1} shares for {2} {3}. {0} lost {4} {3}. {0} is smart. Be like {0!";
        ss = format(s, "Petya", 100, a, "dollars", 100 * a);
    }
    catch ( std::runtime_error & er ) {
        std::cout << "catched: " << er.what() << "\n";
    }


}


