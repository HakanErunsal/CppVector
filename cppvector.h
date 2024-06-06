#include <cstdint>
#include <utility>
#include <new>

template <typename T, int chunk_size = 1>
class vector
{
public:
    typedef T* pointer;
    typedef T* iterator;

    vector();

    vector(const vector& rhs);
    vector(vector&& rhs);

    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs);

    iterator begin();
    iterator end();

    void push_back(const T& x);
    //void push_back(T&& x);

    template <typename... A>
    void emplace_back(A&&... a);

    void insert(iterator where, const T& x);

    void swap(vector<T>& x, vector& y);

    void resize(std::size_t size);

    void reserve(std::size_t capacity);

    T& operator[](std::size_t index);

    const T& operator[](std::size_t index) const;

    std::size_t size() const;

    std::size_t capacity() const;

    bool operator!=(const iterator& b) const
    {
        return *buff != *b.buff;
    }

    void clear();

    ~vector();

private:
    
    int my_size;
    int my_capacity;
    pointer buff;

    vector()
        : buff(nullptr)
        , my_size(0)
        , my_capacity(0)
    {}

    vector(const vector& rhs)
        : my_size(rhs.my_size)
        , my_capacity(rhs.my_capacity)
    {
        if (my_capacity == 0)
        {
            buff = nullptr;
            return;
        }
        buff = new T[rhs.my_capacity];

        for (int i = 0; i < rhs.size(); i++)
        {
            buff[i] = T(rhs[i]);
        }
    }

    vector(vector&& rhs)
        : my_size(rhs.my_size)
        , my_capacity(rhs.my_capacity)
    {
        if (my_capacity == 0)
        {
            buff = nullptr;
            return;
        }
        
        for (int i = 0; i < rhs.size(); i++)
        {
            buff[i] = std::move(rhs[i]);
        }

        rhs.resize(0);
    }

    ~vector()
    {
        if (buff) delete buff;
    }

    iterator begin()
    {
        return buff;
    }

    iterator end()
    {
        return buff + my_size;
    }

    void push_back(const T& x)
    {
        if (my_capacity <= my_size)
        {
            //increase the buff size by chunk size
            resize(my_capacity + chunk_size);
        }

        buff[my_size] = x;
        my_size++;
    }

    template <typename... A>
    void emplace_back(A&&... a)
    {
        if (my_capacity > my_size)
        {
            buff[my_size] = T(a...);
            ++my_size;
        }
        else
        {
            resize(my_size + 1);
            buff[my_size] = T(a...);
        }
    }

    void resize(std::size_t size)
    {
        pointer newBuff = new T[size];

        if (size < my_size)
        {
            my_size = size;
        }

        for (size_t i = 0; i < my_size; i++)
        {
            newBuff[i] = buff[i];
        }

        delete[] buff;
        buff = newBuff;
        my_capacity = size;
    }

    void insert(iterator where, const T& x)
    {
        iterator prevEnd = end();

        //found where, if not enough capacity, increase
        if (my_capacity <= my_size) { resize(my_size + 1); }

        if (where == prevEnd) *prevEnd = x;
        else
        {
            //TODO fix?
            memmove(where + 1, where, (prevEnd - where) * sizeof(T));
        }

        *where = x;
    }
    
    std::size_t size() const
    {
        return my_size;
    }

    std::size_t capacity() const
    {
        return my_capacity;
    }

    T& operator[](std::size_t index)
    {
        return buff[index];
    }

    const T& operator[](std::size_t index) const
    {
        return buff[index];
    }

};
