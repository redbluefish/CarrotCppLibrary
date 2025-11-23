#ifndef THREAD_TOOLS_H
#define THREAD_TOOLS_H

#include <atomic>
#include <cstddef>
#include <cstring>
#include <thread>
#include <vector>

//In development, all code will be in the one file
template <typename T>
class cqueue
{
private:
    T* array = nullptr;
    T* pass_end_ptr = nullptr;
    T* ptr_begin = nullptr;
    T* ptr_end = nullptr;
public:
    cqueue(size_t preallocate_size = 0);
    ~cqueue();
    cqueue(const cqueue&);
    cqueue(cqueue&&);
    cqueue& operator=(const cqueue&);
    cqueue& operator=(cqueue&&);
    // not support std::initializer_list
};

template <typename T>
cqueue<T>::cqueue(size_t preallocate_size)
{
    if (preallocate_size != 0)
    {
        array = new T[preallocate_size];
        pass_end_ptr = array + preallocate_size;
        ptr_begin = ptr_end = array;
    }
}

template <typename T>
cqueue<T>::cqueue(const cqueue<T>& src)
{
    if (src.array != nullptr && src.pass_end_ptr != nullptr)
    {
        fuck you
    }
}

class task_pool
{
private:
    void thread_function();

    size_t work_thread_number = 32;
    ::std::vector<::std::thread> work_threads;

    ::std::atomic<int64_t> task_number = 1;

    template<typename CallableT, typename... ArgsT>
    struct task
    {
        CallableT func;
        ::std::tuple<ArgsT...> args;
    };
public:
    class task_handle
    {
    private:
        ::std::atomic<bool> started = false;
        ::std::atomic<bool> finished = false;
    };

    task_pool();
    task_pool(size_t set_work_thread_number);
    task_pool(task_pool&) = delete;
    task_pool(task_pool&&);
    task_pool operator=(task_pool&) = delete;
    task_pool operator=(task_pool&&);
    ~task_pool();

    void set_work_thread_number(int32_t);

    template <typename CallableT, typename... ArgsT>
    task_handle& upload_task(CallableT callable, ArgsT... args);

};

//void task_pool::thread_function()
//{
//
//}

template class cqueue<int>;

#endif
