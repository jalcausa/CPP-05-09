#ifndef ARRAY_TPP
# define ARRAY_TPP

// ---------- Exception ----------

template <typename T>
const char *Array<T>::IndexOutOfBoundsException::what() const throw()
{
    return "Array: index out of bounds";
}

// ---------- Orthodox Canonical Form ----------

template <typename T>
Array<T>::Array() : _data(NULL), _size(0)
{
}

template <typename T>
Array<T>::Array(unsigned int n) : _data(new T[n]()), _size(n)
{
}

template <typename T>
Array<T>::Array(const Array<T> &other) : _data(NULL), _size(0)
{
    *this = other;
}

template <typename T>
Array<T>::~Array()
{
    delete[] _data;
}

template <typename T>
Array<T> &Array<T>::operator=(const Array<T> &other)
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

// ---------- Element access ----------

template <typename T>
T &Array<T>::operator[](unsigned int idx)
{
    if (idx >= _size)
        throw IndexOutOfBoundsException();
    return _data[idx];
}

template <typename T>
const T &Array<T>::operator[](unsigned int idx) const
{
    if (idx >= _size)
        throw IndexOutOfBoundsException();
    return _data[idx];
}

// ---------- Size ----------

template <typename T>
unsigned int Array<T>::size() const
{
    return _size;
}

#endif
