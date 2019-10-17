
#include <iostream>
#include <sstream>

enum token_value
{
    END,
    PLUS='+',
    MINUS='-',
    MUL='*',
    DIV='/',
};


void  check_input_args(int argc, char* argv[], std::stringstream& sexpr, int& numerr);
int   term(std::stringstream& sexpr, int& numerr, token_value& curr_tok);
int   expr(std::stringstream& sexpr, int& numerr);
int   get_number(std::stringstream& sexpr, int& numerr, token_value& curr_tok);
void  get_oper(std::stringstream& sexpr, int& numerr, token_value& curr_tok);



int main(int argc, char* argv[])
{
    int numerr=0;
    std::stringstream sexpr;  // входной поток символов в вычисляемом выражении из строки агрументов

    check_input_args(argc, argv, sexpr, numerr);      // получить входной поток символов из командной строки

    if ( numerr == 0 ) {
        int res = expr(sexpr, numerr);             // вычислить значение выражения
        if ( numerr == 0 ) {
            std::cout << res;
            }
    }

    return numerr;
}

// проверка входных аргументов
void check_input_args(int argc, char* argv[], std::stringstream& sexpr, int& numerr)
{
    if ( argc != 2 ) {
        std::cerr << "input error - arg count != 1!";
        numerr=1;
        return;
    }
    sexpr = std::stringstream(argv[1]);
}

// обработка всех операций + или -
int expr(std::stringstream& sexpr, int& numerr)
{
    token_value curr_tok=END; // текущий токен (знак операции или признак окончания)

    int left = term(sexpr, numerr, curr_tok); // term выполнит серию подряд идущих операций * или /
    if ( numerr != 0 ) return 0;

    for ( ;;) {
        switch ( curr_tok ) { // значение curr_tok уже было получено в term() и оно может быть только + или -
        case PLUS:
            left += term(sexpr, numerr, curr_tok); // случай '+'
            break;
        case MINUS:
            left -= term(sexpr, numerr, curr_tok); // случай '-'
            break;
        default:
            return left;
        }
        if ( numerr != 0 ) return 0;
    }
}

// обработка блока из операций * или /
int term(std::stringstream& sexpr, int& numerr, token_value& curr_tok)
{
    int left = get_number(sexpr, numerr, curr_tok);
    if ( numerr != 0 ) return 0;

    for ( ;;) {

        get_oper(sexpr, numerr, curr_tok); // запишет в переменную curr_tok вид прочитанной операции
        if ( numerr != 0 ) return 0;

        switch ( curr_tok ) {
        case MUL: // '*'
            left *= get_number(sexpr, numerr, curr_tok);
            if ( numerr != 0 ) return 0;
            break;
        case DIV: //  '/'
        {
            int d = get_number(sexpr, numerr, curr_tok);
            if ( numerr != 0 ) return 0;
            if ( d == 0 ) {
                std::cerr << "divide by zero in term()!";
                numerr=5;
                curr_tok=END;
                return 0;
            }
            left /= d;
            break;
        }
        default:
            return left;
        }
    }
}

// считывает числа
int get_number(std::stringstream& sexpr, int& numerr, token_value& curr_tok)
{
    char ch;
    int num;

    do { // пропускает пробелы после знака операции
        if ( !sexpr.get(ch) ) {
            std::cerr << "invalid arg in get_number()!";
            numerr=2;
            curr_tok=END;
            return 0;
            // если после знака операции нет числа - то ошибка
        }
    } while ( isspace(ch) );

    if ( isdigit(ch) ) {
        sexpr.putback(ch);
        sexpr >> num;
        return num;
    }
    else if ( ch == '-' ) { // унарный минус
        return -get_number(sexpr, numerr, curr_tok);
    }
    else {
        std::cerr << "invalid token in get_number()!";
        numerr=3;
        curr_tok=END;
        return 0;
        // если после знака операции не число, то тоже ошибка
    }
}

// считывает арифметические операции
void get_oper(std::stringstream& sexpr, int& numerr, token_value& curr_tok)
{
    char ch;
    do { // пропускает пробелы после числа
        if ( !sexpr.get(ch) ) {
            curr_tok = END;
            return;
            // здесь после числа может не быть операции, поэтому - признак окончания расчетов
        }
    } while ( isspace(ch) );

    switch ( ch ) {
    case '*':
    case '/':
    case '+':
    case '-':
        curr_tok=token_value(ch);
        break;
    default:
        std::cerr << "invalid operation in get_oper()!";
        numerr=4;
        curr_tok=END;
        return;
        // здесь тоже - после числа должен быть знак операции, иначе - выход с ошибкой
    }
}


// некоторые примеры
// 1) ./calco "    25/5*6+7 +-   3   /1+20*3*4  /  3   /   4      -1"  =  53 
// 2) ./calco "  35 / 5 * 3-1 +-10*  9/3/   15      -66/11/   2/3   "  =  17 
// с ошибками
// 1)  ./calco = input error - arg count != 1!, retcode=1
// 2)  ./calco "1+  2+" = invalid arg in get_number()!, retcode=2
// 3)  ./calco "1+  2+a" = invalid token in get_number()!, retcode=3
// 4)  ./calco "2*  2 2 + 2"  = invalid operation in get_oper()!, retcode=4
// 5)  ./calco "2*  2/ 2 + 2/0+5" = invalid token in get_number()!, retcode=5


