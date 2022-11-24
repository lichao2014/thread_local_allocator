#pragma once

#include <memory>

#include "singleton.h"
#include "object_manager.h"
#include "memory_pool.h"

class IObjectPool
{
public:
    virtual ~IObjectPool() = default;
    virtual void release() = 0;

    static void release_all()
    {
        ObjectManager<IObjectPool>::get_instance().visit(
            [](IObjectPool& i) { i.release(); }
        );
    }
};

template<typename T>
class ThreadLocalObjectPool : public ObjectVisitor<IObjectPool>
                            , public Singleton<ThreadLocalObjectPool<T>, true>
{
public:
    T* allocate(size_t n)
    {
        assert(n == 1);
        return static_cast<T*>(_pool.get());
    }

    void deallocate(const T* ptr, size_t n)
    {
        _pool.put(ptr);
    }

    void release()
    {
        _pool.release();
    }
private:
    FixedMemoryPool<(sizeof(T) < sizeof(void*)) ? sizeof(void*) : sizeof(T)> _pool;
};

template<typename T>
class ThreadLocalAllocator : public std::allocator<T>
{
public:
    using is_always_equal = std::false_type;

    template<typename U>
    struct rebind
    {
        using other = ThreadLocalAllocator<U>;
    };

    ThreadLocalAllocator() = default;

    template<typename U>
    ThreadLocalAllocator(const ThreadLocalAllocator<U>& other) {}

    T* allocate(size_t n)
    {
        return ThreadLocalObjectPool<T>::get_instance().allocate(n);
    }

    void deallocate(const T* ptr, size_t n)
    {
        ThreadLocalObjectPool<T>::get_instance().deallocate(ptr, n);
    }

    template<typename U>
    bool operator==(const ThreadLocalAllocator<U>& other) const noexcept
    {
        return this == std::addressof(other);
    }

    template<typename U>
    bool operator!=(const ThreadLocalAllocator<U>& other) const noexcept
    {
        return this != std::addressof(other);
    }
};
