#pragma once

#include <type_traits>

template<typename T>
struct IntrusiveNode
{
    T *prev = nullptr;
    T *next = nullptr;

    T *data() noexcept { return static_cast<T *>(this); }
    const T * data() const noexcept { return static_cast<const T *>(this); }

    void remove() noexcept
    {
        this->next->prev = this->prev;
        this->prev->next = this->next;
        this->next = nullptr;
        this->prev = nullptr;
    }
};

template<typename T>
class IntrusiveList
{
public:
    static_assert(std::is_base_of<IntrusiveNode<T>, T>::value, "T must drived from IntrusiveNode<T>");

    IntrusiveList() noexcept = default;

    void push_front(T *ptr) noexcept
    {
        push_back(_head, ptr);
    }

    void push_back(T* ptr) noexcept
    {
        push_front(_head, ptr);
    }

    void push_front(IntrusiveNode<T>& hint, T* ptr) noexcept
    {
        ptr->prev = hint.prev;
        ptr->next = hint.data();
        hint.prev->next = ptr;
        hint.prev = ptr;
    }

    void push_back(IntrusiveNode<T>& hint, T* ptr) noexcept
    {
        ptr->prev = hint.data();
        ptr->next = hint.next;
        hint.next->prev = ptr;
        hint.next = ptr;
    }

    bool is_empty() const noexcept { return &_head == _head.next; }

    T* front() noexcept { return _head.next->data(); }
    const T* front() const noexcept { return _head.next->data(); }

    T* back() noexcept { return _head.prev->data(); }
    const T* back() const noexcept { return _head.prev->data(); }

    T* begin() noexcept { return _head.next->data(); }
    const T* begin() const noexcept { return _head.next->data(); }

    T* end() noexcept { return _head.data(); }
    const T* end() const noexcept { return _head.data(); }
private:
    IntrusiveNode<T> _head{ _head.data(), _head.data() };
};
