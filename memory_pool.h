#pragma once

#include <stdlib.h>

#include "freelist.h"

template<size_t N>
class FixedMemoryPool
{
public:
    static_assert(N >= sizeof(void*), "N muse greater then sizeof(void *)");

    ~FixedMemoryPool()
    {
        release();
    }

    void* get()
    {
        auto ptr = _free_list.get();
        return ptr ? ptr : malloc(N);
    }

    void put(const void* ptr) noexcept
    {
        _free_list.put(const_cast<void*>(ptr));
    }

    void release()
    {
        void* p = nullptr;
        while (!!(p = _free_list.get()))
        {
            free(p);
        }
    }
private:
    FreeList _free_list;
};
