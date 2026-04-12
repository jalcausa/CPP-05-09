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
        const char *what() const throw();
    };

    Array();
    Array(unsigned int n);
    Array(const Array<T> &other);
    ~Array();

    Array<T>     &operator=(const Array<T> &other);
    T            &operator[](unsigned int idx);
    const T      &operator[](unsigned int idx) const;

    unsigned int  size() const;

  private:
    T            *_data;
    unsigned int  _size;
};

# include "Array.tpp"

#endif
