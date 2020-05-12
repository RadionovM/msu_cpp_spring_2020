#include <stdexcept>

template <class T>
class Allocator
{
public:
    T* allocate(const size_t size)
    {
        T* ptr = (T*)::operator new(sizeof(T) * size);
        return ptr;
    }

    void deallocate(T* ptr) 
    {
        operator delete(ptr);
    }

    template <class... Args>
    void construct(T* ptr, Args&&... arg)
    {
      ptr = new(ptr) T(std::forward<Args>(arg)...);
    }

    void destroy(T* ptr)
    {
        ptr->~T();
    }
};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T> 
{
    T *ptr;
public:

    Iterator(T* ptr):
                ptr(ptr)
                {}

    Iterator(const Iterator<T>& it):
                ptr(it.ptr)
                {}

    bool operator==(const Iterator<T>& other) 
    {
        return ptr == other.ptr;
    }

    bool operator!=(const Iterator<T>& other) 
    {
        return !(*this == other);
    }

    bool operator<(const Iterator<T>& other) 
    {
        return ptr < other.ptr;
    }

    bool operator>(const Iterator<T>& other) 
    {
        return ptr > other.ptr;
    }

    T& operator*() 
    {
        return *ptr;
    }

    Iterator<T>& operator++() 
    {
        ++ptr;
        return *this;
    }

    Iterator<T>& operator--() 
    {
        --ptr;
        return *this;
    }

    Iterator<T>& operator+=(size_t count) 
    {
        ptr += count;
        return *this;
    }

    Iterator<T>& operator-=(size_t count) 
    {
        ptr -= count;
        return *this;
    }

    Iterator<T> operator+(size_t count) 
    {
        Iterator<T> tmp(ptr + count);
        return tmp;
    }

    Iterator<T> operator-(size_t count) 
    {
        Iterator<T> tmp(ptr - count);
        return tmp;
    }

    Iterator<T> operator+(const Iterator<T>& other) 
    {
        Iterator<T> tmp(ptr + other.ptr);
        return tmp;
    }

    Iterator<T> operator-(const Iterator<T>& other) 
    {
        Iterator<T> tmp(ptr - other.ptr);
        return tmp;
    }

    friend Iterator<T> operator+(size_t count, const Iterator<T>& other) 
    {
        Iterator<T> tmp(other.ptr + count);
        return tmp;
    }

    friend Iterator<T> operator-(size_t count, const Iterator<T>& other) 
    {
        Iterator<T> tmp(count - other.ptr);
        return tmp;
    }

    T& operator[](size_t count) 
    {
        return ptr[count];
    }
};

template <class T>
class Vector
{
private:
    size_t _size;
    const size_t coef {2};
    size_t _capacity;
    T *_data;
    Allocator<T> allocator;

public:

    Vector(const size_t count=0):_size(count),
                                 _capacity((count > 0 ? count : 1) * coef),
                                 _data(allocator.allocate(_capacity))
    {}
    ~Vector()
    {
        clear();
        allocator.deallocate(_data);
    }

    size_t size() const 
    {
        return _size;
    }

    bool empty() const 
    {
        return !_size;
    }

    size_t capacity() const 
    {
        return _capacity;
    }

    void push_back(const T& value)
    {
        if(_capacity == _size)
            reserve(_size * coef + 1);
        allocator.construct(_data + _size, value);
        ++_size;
    }

    void pop_back()
    {
        if(_size > 0)
            allocator.destroy(_data + _size - 1);
            --_size;
    }

    void reserve(const size_t count) 
    {
        if(count > _capacity) 
        {
            T* new_data = allocator.allocate(count);
            for(size_t i = 0; i < _size; ++i) 
                allocator.construct(new_data + i, _data[i]);
            for(size_t i = 0; i< _size; ++i) 
                allocator.destroy(_data + i);
            allocator.deallocate(_data);
            _data = new_data;
            _capacity = count;
        }
    }

    void resize(const size_t count) 
    {
        if(count == _size)
            return;
        if(count > _size) 
        {
            reserve(count * coef + 1);
            for(size_t i = _size; i < count; ++i)
                allocator.construct(_data + i);
        }
        else
            for(size_t i = count; i < _size; ++i) 
                allocator.destroy(_data + i);
        _size = count;
    }

    void clear()
    {
        resize(0);
    }

    T& operator[](size_t pos) 
    {
        if(pos >= _size)
            throw std::out_of_range("");
        return _data[pos];
    }
    const T& operator[](size_t pos) const 
    {
        if(pos >= _size)
            throw std::out_of_range("");
        return _data[pos];
    }

    Iterator<T> begin()
    {
        return Iterator<T>(_data);
    }

    Iterator<T> end()
    {
        return Iterator<T>(_data + _size);
    }

    std::reverse_iterator<Iterator<T>> rbegin() 
    {
        return std::reverse_iterator<Iterator<T>>(_data + _size);
    }

    std::reverse_iterator<Iterator<T>> rend() 
    {
        return std::reverse_iterator<Iterator<T>>(_data);
    }
};
