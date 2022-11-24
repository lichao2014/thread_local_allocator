#pragma once

#include <cassert>
#include <algorithm>

class FreeList
{
public:
    FreeList() noexcept = default;
    ~FreeList() { assert(nullptr == _head); }

    FreeList(FreeList&& other) noexcept { swap(other); }

    FreeList& operator=(FreeList&& other) noexcept
    {
        swap(other);
        return *this;
    }

    FreeList(const FreeList&) = delete;
    void operator=(const FreeList&) = delete;

    void* get() noexcept
    {
        void* p = _head;
        if (p)
        {
            _head = next(_head);
        }

        return p;
    }

    void put(void* ptr) noexcept
    {
        next(ptr) = _head;
        _head = ptr;
    }

    void swap(FreeList& other) noexcept
    {
        std::swap(_head, other._head);
    }
private:
    static void*& next(void* ptr)
    {
        return *static_cast<void**>(ptr);
    }

    void* _head = nullptr;
};
