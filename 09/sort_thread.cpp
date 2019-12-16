// задание
// Выполнить сортировку бинарного файла содержащего числа uint64_t в 2 потока.
// Доступно 8 Мб памяти, больше выделять нельзя ни явно, ни неявно(например, в виде контейнеров),
// при этом файл может быть размером превышающим доступную память.
// Пространство на диске можно считать бесконечным.
// Сортировку выполнять в новый файл.


#include "Uint64_order.h"


int main()
{
    // для проверки создать файл с исходными неупорядоченными числами
    std::string file_for_sort = "for_sort.dat";
    std::string sorted_file;


    //======
    // это создание тестовых данных
    size_t sz = sizeof(uint64_t);
    size_t cnt_block = 393216;
    bool create_data = true;
    if ( create_data ) {
        Timer tm; // посмотрим сколько времени занимает запись файла (11 секунд)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max() / 4);
        size_t sz_sorting_data = cnt_block * 10;
        std::ofstream for_sort;
        int czap = 0;
        for_sort.open(file_for_sort, std::ios::binary); // по условию файл должен быть бинарный
        for ( size_t i = 0; i < sz_sorting_data; ++i ) {
            uint64_t rnd = dis(gen);
            for_sort.write(reinterpret_cast<char*>( &rnd ), sz);
            //std::cout << "rnd= " << rnd << "\n";
            ++czap;
        }
        for_sort.close();
        std::cout << "for_sort file created, czap= " << czap << "\n";
    }
    //======





    //======
    // Сортировка чисел
    //======
    {
        Uint64_order order(cnt_block);
        sorted_file = order(file_for_sort);
    }
    //======
    // Окончание сортировки чисел
    //======




    //======
    // Проверка, правильно ли отсортировали файл
    std::ifstream check_sort;
    check_sort.open(sorted_file, std::ios::binary);
    uint64_t uprev;
    uint64_t unext;
    bool is_correct_sorting = true;

    bool stop_read = !check_sort.read(reinterpret_cast<char*>( &uprev ), sz);

    size_t count_of_numbers = 1;
    while ( !stop_read ) {
        stop_read = !check_sort.read(reinterpret_cast<char*>( &unext ), sz);
        if ( !stop_read ) {
            ++count_of_numbers;
            if ( uprev > unext ) {
                std::cout << "EROR SORTING Pos= " << count_of_numbers << "\n";
                is_correct_sorting = false;
            }
            unext = uprev;
        }
    }
    std::cout << count_of_numbers << " numbers were sorted " << "\n";
    std::cout << "Sorting is '" << ( is_correct_sorting ? "correct" : "uncorrect" ) << "'.\n";
}

