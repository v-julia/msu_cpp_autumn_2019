
#include <iostream>
#include <sstream>
#include <chrono>
#include <cmath>
#include <queue>
#include <memory>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <future>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <stdexcept>



class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize);

     // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func&& func, Args&&... args)->std::future<decltype( func(args...) )>;
    ~ThreadPool();
private:
    // набор потоков
    std::vector<std::thread> threads_;

    // очередь задач
    std::queue< std::function<void()> > tasks;

    // mutexes
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

ThreadPool::ThreadPool(size_t poolSize) : stop(false)
{
    // инициализация заданного количества потоков
    for ( size_t i = 0; i < poolSize; ++i )
        threads_.emplace_back(
        [this] {
        for ( ;;) {
            std::function<void()> task;

            {
                std::unique_lock<std::mutex> lock(this->queue_mutex);

                this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                if ( this->stop && this->tasks.empty() )
                    return;

                task = std::move(this->tasks.front());
                this->tasks.pop();
            }

            task();
        }
    }
    );
}

// Добавить задачу
template <class Func, class... Args>
auto ThreadPool::exec(Func&& func, Args&&... args)->std::future<decltype( func(args...) )>
{
    using return_type = decltype( func(args...) );

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if ( stop ) {
            throw std::runtime_error("ThreadPool is stopped");
        }

        tasks.emplace([task]() { ( *task )( ); });
    }
    condition.notify_one();
    return res;
}



ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    for ( std::thread& thread_ : threads_ ) {
        thread_.join();
    }
}







struct A
{

};

void foo(const A& x)
{
    std::cout << "done fooA\n";
};

int main()
{

    ThreadPool pool(4);
    std::vector< std::future<std::string> > results;

    for ( int i = 0; i < 27; ++i ) {
        results.emplace_back(
            pool.exec([i] {
            std::stringstream ss;
            ss << "start task N " << i << ", thread id=" << std::this_thread::get_id() << +"\n";
            std::cout << ss.rdbuf();

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(100, 1000);
            std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));

            std::stringstream se;
            se << "end task N " << i << ", thread id=" << std::this_thread::get_id() << +"\n";
            std::cout << se.rdbuf();
            return std::string("\n");
        })
        );
    }

    for ( auto&& result : results )
        std::cout << result.get();
    std::cout << std::endl;

    return 0;
}




//int main()
//{
//    int rc = 0;
//    std::cout << "START\n================\n\n";
//    // ===
//
//
//
//    ThreadPool pool(2);
//
//    auto task1 = pool.exec(foo, A());
//    task1.get();
//
//    auto task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//
//    task2.get();
//
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//    task2 = pool.exec([]() {     std::cout << "done []()\n"; return 1; });
//
//
//
//
//
//    // ===
//    std::cout << "\n\n\n\n================\nEND\nRET=" << rc << "\n\n\n";
//    return rc;
//}
