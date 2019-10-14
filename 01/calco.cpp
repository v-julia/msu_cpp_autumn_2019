
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main(int argc, char* argv[])
{

    if (argc != 2) {
        std::cout << "invalid expression!";
        return 1;
    }
    
    std::string inp_expr(argv[1]); // входная строка содержит вычисляемое выражение
   
    
    inp_expr.erase(std::remove(inp_expr.begin(), inp_expr.end(), ' '),inp_expr.end());
    

    
    int sz_inp = inp_expr.size(); // кол-во символов в выражении без пробелов
    std::string operations = "+-*/"; // допустимые операции
    int rank_stack = 0; // сколько ожидающих операций в стэке
    int prev_arg = 0; // предыдущий аргумент
    int curr_arg = 0; // текущий аргумент
    char prev_op = '0'; // предыдущая операция
    char curr_op = '0';

    
    // проверка на правильность начала строки
    if (!((isdigit(inp_expr[0]) != 0) || (inp_expr[0] == '-'))) {
        std::cout << "invalid expression!";
        return 1;
    }
    // первый агрумент пишем в result
    size_t pos = 1;
    int result;
    try {
        result = std::stoi(inp_expr, &pos, 10);// запись в result
    }
    catch (std::exception) {
        std::cout << "invalid expression!";
        return 1;
    }
    // дальше цикл считывания знака операции и очередного аргумента
    while (pos < sz_inp) {
        curr_op = inp_expr[pos++]; // очередная операция
         // если ее нет в списке допустимых, то выдать ошибку
        if (operations.find(curr_op) < 0) {
            std::cout << "invalid expression!";
            return 1;
        }
        // считать очередной аргумент
       // при этом - проверить правильность первого символа
        if (!((('0' <= inp_expr[pos]) && (inp_expr[pos] <= '9')) || (inp_expr[pos] == '-'))) {
            std::cout << "invalid expression!";
            return 1;
        }
        size_t dpos = 0;
        try {
            curr_arg = std::stoi(inp_expr.substr(pos), &dpos, 10);// запись в result
            pos += dpos;
        }
        catch (std::exception) {
            std::cout << "invalid expression!";
            return 1;
        }
        //если очередная операция = * или /, то выполнить расчет
        if (curr_op == '*' || curr_op == '/') {
            if (rank_stack == 0) {
                result = (curr_op == '*') ? result*curr_arg : result / curr_arg;
            }
            else {
                prev_arg = (curr_op == '*') ? prev_arg*curr_arg : prev_arg / curr_arg;
            }
        }
        else {
            //если очередная операция = + или -
            if (rank_stack == 0) {
                // если предыдущих операций не было, то положить в стек
                prev_arg = curr_arg;
                prev_op = curr_op;
                rank_stack = 1;
            }
            else {
                // иначе - выполнить предыдущую операцию
                result = (prev_op == '+') ? result + prev_arg : result - prev_arg;
                // и запомнить текущий результат
                prev_op = curr_op;
                prev_arg = curr_arg;
            }
        }
    }
    // заполнить result
    if (rank_stack == 1) {
        result = (prev_op == '+') ? result + prev_arg : result - prev_arg;
    }
    std::cout << result;
}

