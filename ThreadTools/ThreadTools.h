#ifndef THREAD_TOOLS_H
#define THREAD_TOOLS_H

#include <atomic>
#include <cstddef>
#include <cstring>
#include <deque>
#include <thread>
#include <vector>
#include <xutility>

#include "DEBUG.h"
#include "../General/General.h"

//In development, all code will be in the one file
//Exceptions are deprecated
template <typename T>
class cqueue
{
private:
    T* array = nullptr;
    T* pass_end_ptr = nullptr;
    T* ptr_begin = nullptr;
    T* ptr_end = nullptr;

    signed int add_space();
public:
    cqueue(size_t preallocate_size = 1);
    ~cqueue();
    cqueue(const cqueue&);
    cqueue(cqueue&&);
    cqueue& operator=(const cqueue&);
    cqueue& operator=(cqueue&&);
    void push_back(T&&);
    template <typename... Args>
    void emplace_back(Args&&...);
    T& pop_back();

    T& front();
    T& back();
    // not support std::initializer_list
};

template <typename T>
signed int cqueue<T>::add_space()
{
    if (array == nullptr)
    {
        return 1; // uninitialized
    }
    if (ptr_begin == array - 1)
    {
        ptr_begin++;
    }
    ptr_end++;
    if (ptr_end == pass_end_ptr)
    {
        if (ptr_begin == array)
        {
            ptr_end--;
            return 2; // space not enough
        }
        else
        {
            ptr_end = array + 1;
        }
    }
    return 0;
}

template <typename T>
cqueue<T>::cqueue(size_t preallocate_size)
{
    if (preallocate_size != 0)
    {
        array = new T[preallocate_size];
        pass_end_ptr = array + preallocate_size;
        ptr_begin = array - 1;
        ptr_end = array;
    }
    
}

template <typename T>
cqueue<T>::cqueue(const cqueue& src)
{
    *this = src;
}

template <typename T>
cqueue<T>::cqueue(cqueue&& src)
{
    *this = ::std::move(src);
}

template <typename T>
cqueue<T>::~cqueue()
{
    delete array;
}

template <typename T>
cqueue<T>& cqueue<T>::operator=(const cqueue& src)
{
    delete array;
    if (src.array != nullptr)
    {
        array = new T[src.pass_end_ptr - src.array];
        pass_end_ptr = array - src.array + src.pass_end_ptr;
        memcpy(array, src.array, sizeof(T) * (src.pass_end_ptr - src.array));
        ptr_begin = src.ptr_begin - src.array + array;
        ptr_end = src.ptr_end - src.array + array;
    }
    else
    {
        ptr_end = ptr_begin = pass_end_ptr = array = nullptr;
    }
    return *this;
}

template <typename T>
cqueue<T>& cqueue<T>::operator=(cqueue&& src)
{
    array = src.array;
    pass_end_ptr = src.pass_end_ptr;
    ptr_begin = src.ptr_begin;
    ptr_end = src.ptr_end;
    src.array = nullptr;
    src.pass_end_ptr = nullptr;
    src.ptr_begin = nullptr;
    src.ptr_end = nullptr;
    return *this;
}

template <typename T>
void cqueue<T>::push_back(T&& src)
{
    signed int return_code = add_space();
    if (return_code == 0)
    {
        *(ptr_end - 1) = std::forward<T>(src);
    }
    else if (return_code == 1)
    {
        throw CarrotCppLibException(
            std::string("queue is uninitialized or no space allocated")
        );
    }
    else if (return_code == 2)
    {
        throw CarrotCppLibException(
            std::string("space is not enough")
            );
    }
}

template <typename T>
template <typename... Args>
void cqueue<T>::emplace_back(Args&&... argv)
{
    signed int return_code = add_space();
    if (return_code == 0)
    {
        *(ptr_end - 1) = std::move(T(std::forward<Args...>(argv)));
    }
    else if (return_code == 1)
    {
        throw CarrotCppLibException(
            std::string("queue is uninitialized or no space allocated")
        );
    }
    else if (return_code == 2)
    {
        throw CarrotCppLibException(
            std::string("space is not enough")
            );
    }
}

template <typename T>
T& cqueue<T>::pop_back()
{
    T* res = nullptr;
    if (ptr_end == array)
    {
        if (ptr_begin + 1 == array)
        {
            throw CarrotCppLibException("no element to pop");
        }
        else
        {
            res = array;
            ptr_end = pass_end_ptr;
        }
    }
    else
    {
        ptr_end--;
        res = pass_end_ptr;
    }
    return *res;
}

template <typename T>
T& cqueue<T>::front()
{
    if (array == nullptr)
    {
        throw CarrotCppLibException(
            std::string("no space allocated")
        );
    }
    if (ptr_begin == array - 1)
    {
        throw CarrotCppLibException(
            std::string("no element in queue")
        );
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
