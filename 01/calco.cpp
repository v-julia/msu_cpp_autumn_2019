
#include <iostream>
#include <sstream>

enum token_value
{
    NUMBER,
    END,
    PLUS='+',
    MINUS='-',
    MUL='*',
    DIV='/',
};

std::stringstream cin;           // входной поток символов в вычисляемом выражении из строки агрументов
token_value       curr_tok;      // текущий токен (число,знак операции или признак окончания)

void  check_input_args(int argc, char* argv[]);
int   term();
int   expr();
int   get_number();
void  get_oper();



int main(int argc, char* argv[])
{
    check_input_args(argc, argv); // получить входной поток символов cin из командной строки

    std::cout << expr();          // вычислить значение выражения
    return 0;
}

// проверка входных аргументов
void check_input_args(int argc, char* argv[])
{
    if ( argc != 2 ) {
        std::cout << "input error - no expression!";
        exit(1);
    }
    cin = std::stringstream(argv[1]);
}

// обработка всех операций + или -
int expr()
{
    int left = term(); // term выполнит серию подряд идущих операций * или /
    for ( ;;)
        switch ( curr_tok ) { // значение curr_tok уже было получено в term() и оно может быть только + или -
        case PLUS:
            left += term(); // случай '+'
            break;
        case MINUS:
            left -= term(); // случай '-'
            break;
        default:
            return left;
        }
}

// обработка блока из операций * или /
int term()
{
    int left = get_number();
    for ( ;;) {
        get_oper(); // запишет в глобальную переменную curr_tok вид прочитанной операции
        switch ( curr_tok ) {
        case MUL: // '*'
            left *= get_number();
            break;
        case DIV: //  '/'
        {
            int d = get_number();
            if ( d == 0 ) {
                std::cout << "divide by zero in term()!";
                exit(5);
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
int get_number()
{
    char ch;
    int num;

    do { // пропускает пробелы после знака операции
        if ( !cin.get(ch) ) {
            std::cout << "invalid arg in get_number()!";
            exit(2);
            // если после знака операции нет числа - то ошибка
        }
    } while ( isspace(ch) );

    if ( isdigit(ch) ) {
        cin.putback(ch);
        cin >> num;
        curr_tok=NUMBER; // в принципе обозначать curr_tok, когда он NUMBER для этого примера необязательно
        return num;
    }
    else if ( ch == '-' ) { // унарный минус
        return -get_number();
    }
    else {
        std::cout << "invalid token in get_number()!";
        exit(3);
        // если после знака операции не число, то тоже ошибка
    }
}

// считывает арифметические операции
void get_oper()
{
    char ch;
    do { // пропускает пробелы после числа
        if ( !cin.get(ch) ) {
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
        std::cout << "invalid operation in get_oper()!";
        exit(4);
        // здесь тоже - после числа должен быть знак операции, иначе - выход с ошибкой
    }
}
