#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>


#define CAP 10

namespace aisdi
{

template <typename Type>
class Vector
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;


private:
  Type *head;
  Type *tail;
  size_type size;
  size_type capacity;

  Type* resize()
  {
      if(size + 1 <= capacity)
        return nullptr;

      capacity = 2 * size;

      return new Type[capacity];
  }

  void move_left(const const_iterator& position_from, const const_iterator& position_to)
  {
      iterator to = iterator(position_to.node, *this);
      const_iterator from = position_from;

      for(; from != end(); ++from, ++to)
            *to = *from;

      tail = to.node;
  }

public:
  Vector(): size(0), capacity(CAP)
  {
      tail = head = new Type[CAP];
  }

  Vector(std::initializer_list<Type> l): Vector()
  {
    //  size = 0;
    //  capacity = l.size();
    //  tail = head = new Type[capacity];

      for(auto it = l.begin(); it != l.end(); ++it)
        append(*it);
      //(void)l; // disables "unused argument" warning, can be removed when method is implemented.
      //throw std::runtime_error("TODO");
  }

  Vector(const Vector& other)//: Vector()
  {
      if(this == &other)
        return;

   //   size = other.size;
        size = 0;
      capacity = other.size;
      tail = head = new Type[capacity];

      for(auto it = other.begin(); it != other.end(); ++it)
        append(*it);
      //(void)other;
      //throw std::runtime_error("TODO");
  }

  Vector(Vector&& other)
  {
      if(this == &other)
        return;

    //Type *del = head;

      size = other.size;
      capacity = other.capacity;

      head = other.head;
      tail = other.tail;

      other.tail = other.head = nullptr;

   // delete[] del;

      //(void)other;
      //throw std::runtime_error("TODO");
  }

  ~Vector()
  {
     // tail = nullptr;

      size = 0;
      capacity = 0;

      delete[] head;
   //   head = nullptr;
  }

  Vector& operator=(const Vector& other)
  {
      if(this == &other)
        return *this;

    //  while(!isEmpty())
      //  erase(begin());

      delete[] head;
      head = tail = nullptr;
      size = 0;
      capacity = other.size;

      head = tail = new Type[capacity];

      for(auto it = other.begin(); it != other.end(); ++it)
        append(*it);

      return *this;
      //(void)other;
      //throw std::runtime_error("TODO");
  }

  Vector& operator=(Vector&& other)
  {
      if(this == &other)
        return *this;

   //   while(!isEmpty())
     //   erase(begin());
     delete[] head;

 //     Type *del = head;

      size = other.size;
      capacity = other.capacity;
      head = other.head;
      tail = other.tail;

      other.size = 0;
      other.capacity = CAP;
      other.tail = other.head = nullptr;

 //     delete[] del;

      return *this;
      //(void)other;
      //throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
      return !size;
      //throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
      return size;
      //throw std::runtime_error("TODO");
  }

  void append(const Type& item)
  {
      Type *temp;

      if((temp = resize()))
      {
          Type *del = head;

          head = temp;

          Type *i = temp;

          const_iterator it = begin();
          for(; it != end(); ++it, ++i)
            *i = *it;

          tail = i;

          delete[] del;
      }
      *(tail++) = item;

      size++;
      //(void)item;
      //throw std::runtime_error("TODO");
  }

  void prepend(const Type& item)
  {
      Type* temp;

      if((temp = resize()))
      {
          Type *del = head;

          *temp = item;
          head = temp;

          Type *i = temp + 1;
          const_iterator it = begin();
          for(; it != end(); ++it, ++i)
            *i = *it;

          tail = i;

        delete[] del;
      }
      else
      {
          if(isEmpty())
            {
                tail++;
            }
          else
            {
                iterator it = end() - 1;
                  tail++;

                  for(; it!= begin(); --it)
                    *(it + 1) = *it;

                  *(it + 1) = *it;
            }

          *head = item;
      }
      size++;
      //(void)item;
      //throw std::runtime_error("TODO");
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      if(insertPosition == begin())
      {
          prepend(item);
          return;
      }

      if(insertPosition == end())
      {
          append(item);
          return;
      }

      Type *temp;

      if((temp = resize()))
      {
          Type *del = head;

          head = temp;

          Type *i = temp;
          const_iterator it = begin();
          for(; it != insertPosition; ++it, ++i)
            *i = *it;

          *i = item;

          ++i;
          for(; it != end(); ++it, ++i)
            *i = *it;

          tail = i;

          delete[] del;
      }
      else
      {
          if(isEmpty())
            {
                tail++;
            }
          else
            {
                iterator it = end() - 1;
                  tail++;

                  for(; it!= insertPosition; --it)
                    *(it + 1) = *it;

                  *(it + 1) = *it;
            }

          *((iterator)insertPosition) = item;
      }

      size++;
      //(void)insertPosition;
      //(void)item;
      //throw std::runtime_error("TODO");
  }

  Type popFirst()
  {
      if(isEmpty())
        throw std::logic_error("Object cannot be popped");

      Type temp = *head;

      move_left(begin() + 1, begin());

      size--;
      return temp;
      //throw std::runtime_error("TODO");
  }

  Type popLast()
  {
      if(isEmpty())
        throw std::logic_error("Object cannot be popped");

      Type temp = *(tail - 1);
      --tail;
      size--;
      return temp;
      //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& possition)
  {
      if(isEmpty())
        throw std::out_of_range("Erasing in an empty vector");

      if(possition == end())
        throw std::out_of_range("Erasing at end iterator");

      if(possition == end() - 1)
      {
          popLast();
          return;
      }

      move_left(possition + 1, possition);
      size--;
      //(void)possition;
      //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      if(isEmpty())
        throw std::out_of_range("Erasing in an empty vector");

      if(firstIncluded == lastExcluded)
        return;

      size -= lastExcluded.node - firstIncluded.node;

      iterator i = firstIncluded;

      move_left(lastExcluded, firstIncluded);

      //(void)firstIncluded;
      //(void)lastExcluded;
      //throw std::runtime_error("TODO");
  }

  iterator begin()
  {
      return iterator(head, *this);
      //throw std::runtime_error("TODO");
  }

  iterator end()
  {
      return iterator(tail, *this);
      //throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
      return const_iterator(head, *this);
      //throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
      return const_iterator(tail, *this);
      //throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename Type>
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;

private:
    Type *node;
    const Vector<Type>& vec;

    friend void aisdi::Vector<Type>::erase(const ConstIterator&, const ConstIterator&);
    friend void aisdi::Vector<Type>::erase(const ConstIterator&);
    friend void aisdi::Vector<Type>::move_left(const ConstIterator&, const ConstIterator&);

public:

  explicit ConstIterator(Type *ptr, const Vector<Type>& vr): node(ptr), vec(vr)
  {}

  reference operator*() const
  {
      if(node == vec.tail)
        throw std::out_of_range("Out of range");

      return *node;
      //throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
      if(node == vec.tail)
        throw std::out_of_range("Out of range");

      ++node;
      return *this;
      //throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
      auto result = *this;
      operator++();
      return result;
      //throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
      if(node == vec.head)
        throw std::out_of_range("Out of range");

      --node;
      return *this;
      //throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
      auto result = *this;
      operator--();
      return result;
      //throw std::runtime_error("TODO");
  }

  ConstIterator operator+(difference_type d) const
  {
      if(node + d > vec.tail)
        throw std::out_of_range("Out of range");

      return ConstIterator(node + d, vec);
      //(void)d;
      //throw std::runtime_error("TODO");
  }

  ConstIterator operator-(difference_type d) const
  {
      if(node - d < vec.head)
        throw std::out_of_range("Out of range");

      return ConstIterator(node - d, vec);
      //(void)d;
      //throw std::runtime_error("TODO");
  }

  bool operator==(const ConstIterator& other) const
  {
      return (this->node == other.node);
      //(void)other;
      //throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
      return (this->node != other.node);
      //(void)other;
      //throw std::runtime_error("TODO");
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(Type *ptr, const Vector<Type>& vr): ConstIterator(ptr, vr)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_VECTOR_H
