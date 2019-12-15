// задание
// Выполнить сортировку бинарного файла содержащего числа uint64_t в 2 потока.
// Доступно 8 Мб памяти, больше выделять нельзя ни явно, ни неявно(например, в виде контейнеров),
// при этом файл может быть размером превышающим доступную память.
// Пространство на диске можно считать бесконечным.
// Сортировку выполнять в новый файл.


#include <chrono>
#include <iostream>
#include <random>
#include <limits>
#include <fstream>
#include <thread>
#include <mutex>
#include <functional>
#include <string>
#include <queue>
#include <iterator>
#include <cstdio>

class Timer
{
public:
    Timer()
        : start_(std::chrono::high_resolution_clock::now())
    {}

    ~Timer()
    {
        const auto finish = std::chrono::high_resolution_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>( finish - start_ ).count() << " us" << std::endl;
    }

private:
    const std::chrono::high_resolution_clock::time_point start_;
};


// sizeof(uint64_t)=8 байт, (8 Мб=8*1024*1024)/8 = 1024*1024 = 1048576 
// 1048576 элементов массива vector<uint64_t> можно считать из файла
// (1048576 / 2) = 524288  в 2 потока обрабатывать массивы размера не более 524288 элементов
// для qsort потребуется память ориентировочно в полтора раза больше
// поэтому выделять надо массив по 393216 чисел в каждом потоке
// алгоритм такой: 
// 1) прочитать из файла for_sort.dat числа uint64_t частями по 393216 чисел
//    читать можно одним потоком, потому что один файл с последовательным доступом
// 2) каждую часть отсортировать и записать в файл part_no.dat
//    это можно делать параллельно
// 3) собрать все части в один файл sorted.dat
//    это тоже можно параллельно



// сортировка чисел из потока ввода
void sort_uint64(std::ifstream& for_sort);

// слияние двух файлов с упорядоченными числами
void merge_parts(std::string& p1fn, std::string& p2fn, std::string& outfn);


// слияние массива отсортированных чисел и файла с упорядоченными числами
void merge_array_with_part(std::unique_ptr<uint64_t[]>& dat, size_t cnt, std::string& pfn, std::string& outfn);


const size_t cnt_block = 393216; // размер блока в количестве чисел типа uint64_t
//const size_t cnt_block = 2;

std::queue<std::string> queue_parts; // очередь частичных отсортированных файлов

int nom_part = 1;   // номер следующего частично отсортированного файла


std::mutex guard_nom_part;    // для блокировки nom_part
std::mutex guard_queue_parts;   // для блокировки queue_parts
std::mutex guard_input;  // для блокировки входного файла



int main()
{
    // для проверки создать файл с исходными неупорядоченными числами
    std::string file_for_sort = "for_sort.dat";
    std::string file_sorted = "sorted.dat";
    
    size_t sz = sizeof(uint64_t);


    //======
    // это создание тестовых данных
    bool create_data = true;
    if ( create_data ) {
        Timer tm; // посмотрим сколько времени занимает запись файла (11 секунд)
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max() / 4);
        size_t sz_sorting_data = cnt_block * 20;
        //size_t sz_sorting_data = 4;
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
    std::ifstream for_sort;
    for_sort.open(file_for_sort, std::ios::binary);
    
    std::cout << "Start sorting '" << file_for_sort << "'\n";

    {
        Timer ttt;

        std::thread t1(std::bind(sort_uint64, std::ref(for_sort)));
        std::thread t2(std::bind(sort_uint64, std::ref(for_sort)));
        t1.join();
        t2.join();
    }

    for_sort.close();
    remove(file_sorted.c_str());
    int rc = rename(queue_parts.front().c_str(), file_sorted.c_str());

    std::cout << "Sorted numbers in file '" << file_sorted << "'\n";
    //======
    // Окончание сортировки чисел
    //======




    //======
    // Проверка, правильно ли отсортировали файл
    std::ifstream check_sort;
    check_sort.open(file_sorted, std::ios::binary);
    uint64_t uprev;
    uint64_t unext;
    bool is_correct_sorting=true;

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
    std::cout <<  count_of_numbers << " numbers were sorted "<< "\n";
    std::cout << "Sorting is '" << (is_correct_sorting?"correct":"uncorrect") << "'.\n";
}


void sort_uint64(std::ifstream& for_sort)
{
    
    std::unique_ptr<uint64_t[]> dat(new uint64_t[cnt_block]);
    //uint64_t* dat = new uint64_t[cnt_block];
    size_t sz = sizeof(uint64_t);
    while ( !for_sort.eof() ) {
        size_t cnt = 0;
        {
            std::lock_guard<std::mutex> lock(guard_input);
            while ( cnt < cnt_block )
                if ( !for_sort.read(reinterpret_cast<char*>( &dat[cnt] ), sz) )
                    break;
                else
                    ++cnt;
        }

        // упорядочить dat
        if ( cnt > 0 ) {
            qsort(static_cast<void*>(dat.get()), cnt, sz, [](const void* a, const void* b) {
                uint64_t arg1 = *static_cast<const uint64_t*>( a );
                uint64_t arg2 = *static_cast<const uint64_t*>( b );
                if ( arg1 < arg2 ) return -1;
                if ( arg1 > arg2 ) return 1;
                return 0;

            });

            // проверить очередь, узнать какую часть взять для обработки
            bool is_empty = true;
            std::string file_name_from_queue;
            {
                std::lock_guard<std::mutex> lock(guard_queue_parts);
                is_empty = queue_parts.empty();
                if ( !is_empty ) {
                    file_name_from_queue = queue_parts.front();
                    queue_parts.pop();
                }
            }

            // получить номер частичного файла 
            std::string file_name_merged;
            {
                std::lock_guard<std::mutex> lock(guard_nom_part);
                file_name_merged = "part_" + std::to_string(nom_part++);
            }

            // записать в файл
            if ( is_empty ) {
                std::ofstream sorted_part;
                sorted_part.open(file_name_merged, std::ios::binary);
                for ( size_t i = 0; i < cnt; ++i ) {
                    sorted_part.write(reinterpret_cast<char*>( &dat[i] ), sz);
                }
                sorted_part.close();
            }
            else { // merge dat и файл из очереди
                merge_array_with_part(dat, cnt, file_name_from_queue, file_name_merged);
            }

            // поместить в очередь
            {
                std::lock_guard<std::mutex> lock(guard_queue_parts);
                queue_parts.emplace(file_name_merged);
            }
        }
    }
    while ( true ) {
        size_t sz_que;
        std::string file_part1_name;
        std::string file_part2_name;
        {
            std::lock_guard<std::mutex> lock(guard_queue_parts);
            sz_que = queue_parts.size();
            if ( sz_que > 1 ) {
                file_part1_name = queue_parts.front();
                queue_parts.pop();
                file_part2_name = queue_parts.front();
                queue_parts.pop();
            }
        }
        if ( sz_que > 1 ) {

            std::string file_name_merged;
            {
                std::lock_guard<std::mutex> lock(guard_nom_part);
                file_name_merged = "part_" + std::to_string(nom_part++);
            }

            // слить два файла в один
            merge_parts(file_part1_name, file_part2_name, file_name_merged);

            // поместить результат в очередь
            {
                std::lock_guard<std::mutex> lock(guard_queue_parts);
                queue_parts.emplace(file_name_merged);
            }

        }
        else {
            break;
        }
    }

    //delete[] dat;
}


void merge_parts(std::string& p1fn, std::string& p2fn, std::string& outfn)
{
    std::ifstream part1file;
    std::ifstream part2file;
    std::ofstream outfile;
    part1file.open(p1fn, std::ios::binary);
    part2file.open(p2fn, std::ios::binary);
    outfile.open(outfn, std::ios::binary);

    uint64_t value1;
    uint64_t value2;
    size_t sz = sizeof(uint64_t);

    bool stop_part1 = !part1file.read(reinterpret_cast<char*>( &value1 ), sz);;
    bool stop_part2 = !part2file.read(reinterpret_cast<char*>( &value2 ), sz);;

    while ( !( stop_part1 && stop_part2 ) ) {
        if ( !stop_part1 && !stop_part2 ) {
            if ( value1 < value2 ) {
                outfile.write(reinterpret_cast<char*>( &value1 ), sz);
                stop_part1 = !part1file.read(reinterpret_cast<char*>( &value1 ), sz);;
            }
            else {
                outfile.write(reinterpret_cast<char*>( &value2 ), sz);
                stop_part2 = !part2file.read(reinterpret_cast<char*>( &value2 ), sz);;
            }
        }
        if ( !stop_part1 && stop_part2 ) {
            outfile.write(reinterpret_cast<char*>( &value1 ), sz);
            stop_part1 = !part1file.read(reinterpret_cast<char*>( &value1 ), sz);;
        }
        if ( stop_part1 && !stop_part2 ) {
            outfile.write(reinterpret_cast<char*>( &value2 ), sz);
            stop_part2 = !part2file.read(reinterpret_cast<char*>( &value2 ), sz);;
        }
    }
    part1file.close();
    part2file.close();
    outfile.close();
    remove(p1fn.c_str()); // delete file
    remove(p2fn.c_str());
}

void merge_array_with_part(std::unique_ptr<uint64_t[]>& dat, size_t cnt, std::string& pfn, std::string& outfn)
{
    std::ifstream partfile;
    std::ofstream outfile;
    partfile.open(pfn, std::ios::binary);
    outfile.open(outfn, std::ios::binary);

    uint64_t value;
    size_t sz = sizeof(uint64_t);

    bool stop_dat = ( cnt == 0 );
    size_t idx = 0;
    bool stop_part = !partfile.read(reinterpret_cast<char*>( &value ), sz);

    while ( !( stop_dat && stop_part ) ) {
        if ( !stop_dat && !stop_part ) {
            if ( dat[idx] < value ) {
                outfile.write(reinterpret_cast<char*>( &dat[idx++] ), sz);
                stop_dat = ( idx >= cnt );
            }
            else {
                outfile.write(reinterpret_cast<char*>( &value ), sz);
                stop_part = !partfile.read(reinterpret_cast<char*>( &value ), sz);;
            }
        }
        if ( !stop_dat && stop_part ) {
            outfile.write(reinterpret_cast<char*>( &dat[idx++] ), sz);
            stop_dat = ( idx >= cnt );
        }
        if ( stop_dat && !stop_part ) {
            outfile.write(reinterpret_cast<char*>( &value ), sz);
            stop_part = !partfile.read(reinterpret_cast<char*>( &value ), sz);;
        }
    }
    partfile.close();
    outfile.close();
    remove(pfn.c_str()); // delete file
}
