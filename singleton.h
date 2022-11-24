#pragma once

template<typename T, bool IsThreadLocal>
class Singleton
{
public:
    static T& get_instance() noexcept
    {
        static T s_instance;
        return s_instance;
    }
};

template<typename T>
class Singleton<T, true>
{
public:
    static T& get_instance() noexcept
    {
        static thread_local T s_instance;
        return s_instance;
    }
};
