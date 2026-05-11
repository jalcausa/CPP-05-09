#ifndef ARRAY_HPP
# define ARRAY_HPP

# include <exception>

template <typename T>
class Array
{
  public:
    class IndexOutOfBoundsException : public std::exception
    {
      public:
        const char *what() const throw()
        {
            return "Array: index out of bounds";
        }
    };

    Array() : _data(NULL), _size(0) {}

    Array(unsigned int n) : _data(new T[n]()), _size(n) {}

    Array(const Array<T> &other) : _data(NULL), _size(0)
    {
        *this = other;
    }

    ~Array() { delete[] _data; }

    Array<T> &operator=(const Array<T> &other)
    {
        if (this != &other)
        {
            delete[] _data;
            _size = other._size;
            _data = new T[_size];
            for (unsigned int i = 0; i < _size; ++i)
                _data[i] = other._data[i];
        }
        return *this;
    }

    T &operator[](unsigned int idx)
    {
        if (idx >= _size)
            throw IndexOutOfBoundsException();
        return _data[idx];
    }

    const T &operator[](unsigned int idx) const
    {
        if (idx >= _size)
            throw IndexOutOfBoundsException();
        return _data[idx];
    }

    unsigned int size() const { return _size; }

  private:
    T            *_data;
    unsigned int  _size;
};

#endif
