#pragma once

#include <cassert>
#include <stdexcept>
#include <boost/shared_array.hpp>

template <typename T>
class shared_array_range {
private:
    boost::shared_array<T> arr_;
    T *first_;
    T *last_;

public:
    shared_array_range()
    : first_(), last_()
    { }

    shared_array_range(const shared_array_range &other, std::size_t first,
                       std::size_t last)
    : arr_(other.arr_),
      first_(other.first_ + first),
      last_(other.first_ + last)
    {
        assert(first_ <= last_);
    }

    shared_array_range(boost::shared_array<T> arr, std::size_t last)
    : arr_(arr),
      first_(arr.get()),
      last_(arr.get() + last)
    { }

    shared_array_range(boost::shared_array<T> arr, std::size_t first,
                       std::size_t last)
    : arr_(arr),
      first_(arr.get() + first),
      last_(arr.get() + last)
    { }

    T& operator[](std::size_t index) const
    {
        assert(index < size());
        return first_[index];
    }
    
    T& at(std::size_t index) const
    {
        if (index >= size()) {
            throw std::out_of_range("index out of shared array range");
        }
        return first_[index];
    }

    T* begin() const
    {
        return first_;
    }

    T* end() const
    {
        return last_;
    }

    std::size_t size() const
    {
        assert(first_ <= last_);
        return last_ - first_;
    }
    
    bool empty() const
    {
        assert(first_ <= last_);
        return first_ == last_;
    }

    boost::shared_array<T> arr() const
    {
        return arr_;
    }

    T* ptr() const
    {
        return first_;
    }

};
