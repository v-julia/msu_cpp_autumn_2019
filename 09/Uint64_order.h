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





// Что бы убрать то, что было раньше глобальными переменными, делаем класс,
// например, class Order, и объявляем их внутри класса
// все функции, которые здесь были тоже делаем методами класса,
// в main создаем объект класса Order и вызываем метод этого класса


class Uint64_order
{
private:
    size_t cnt_block = 393216;            // размер блока в количестве чисел типа uint64_t
    std::queue<std::string> queue_parts;  // очередь частичных отсортированных файлов

    int nom_part = 1;                     // номер следующего частично отсортированного файла
    size_t sz = sizeof(uint64_t);

    std::mutex guard_nom_part;            // для блокировки nom_part
    std::mutex guard_queue_parts;         // для блокировки queue_parts
    std::mutex guard_input;               // для блокировки входного файла

private:

    // сортировка чисел из потока ввода
    void sort_uint64(std::ifstream& for_sort);

    // слияние двух файлов с упорядоченными числами
    void merge_parts(std::string& p1fn, std::string& p2fn, std::string& outfn);


    // слияние массива отсортированных чисел и файла с упорядоченными числами
    void merge_array_with_part(std::unique_ptr<uint64_t[]>& dat, size_t cnt, std::string& pfn, std::string& outfn);


public:
    Uint64_order(size_t block_size = 393216) : cnt_block(block_size) {}
    ~Uint64_order() {}
    void init(size_t block_size = 393216)
    {
        cnt_block = block_size;
        nom_part = 1;
        while ( !queue_parts.empty() )
            queue_parts.pop();
    }
    //======
    // Сортировка чисел
    //======
    std::string operator()(std::string file_for_sort = "for_sort.dat")
    {
        std::ifstream for_sort;
        for_sort.open(file_for_sort, std::ios::binary);
        if ( !for_sort ) {
            std::cout << "no file_for_sort!\n";
            return std::string("");
        }

        std::string file_sorted = "sorted.dat";
        std::cout << "Start sorting '" << file_for_sort << "'\n";

        {
            Timer ttt;
            auto fun = [this](std::ifstream& for_sort) { this->sort_uint64(for_sort); };
            std::thread t1(std::bind(fun,std::ref(for_sort)));
            std::thread t2(std::bind(fun, std::ref(for_sort)));
            t1.join();
            t2.join();
        }

        for_sort.close();
        remove(file_sorted.c_str());
        int rc = rename(queue_parts.front().c_str(), file_sorted.c_str());
        std::cout << "File sorted = '" << file_sorted << "'\n";
        return file_sorted;
    }

};

