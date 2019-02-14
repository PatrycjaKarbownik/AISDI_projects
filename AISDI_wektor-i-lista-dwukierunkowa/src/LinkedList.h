#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t; //odleglosc miedzy woma elementami
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
 // class Node;

  using iterator = Iterator;
  using const_iterator = ConstIterator;


  struct Node
{
	Type *key;
	Node *prev, *next;

	/*
	Node()
	{
		this-> key = nullptr;
		this-> prev = nullptr;
		this-> next = nullptr;
	}
	Node(Type key): Node()
	{
	    this->key = new Type(key);
	}
	*/
	Node(): key(nullptr), prev(nullptr), next(nullptr){};

	Node(Type obj): Node()
	{
	    key = new Type(obj);
	}

	~Node()
	{
	    delete key;
	}
};

private:
    Node *head;
    Node *tail;
    size_type size;

public:

  LinkedList(): head(nullptr), tail(nullptr), size(0)
  {
      head = new Node();
      tail = new Node();
      head->next = tail;
      tail->prev = head;
      //size = 0;
  }

  LinkedList(std::initializer_list<Type> l): LinkedList()
  {
      for(auto it = l.begin(); it != l.end(); ++it)
        append(*it);
    //(void)l; // disables "unused argument" warning, can be removed when method is implemented.
    //throw std::runtime_error("TODO");
  }

  LinkedList(const LinkedList& other): LinkedList()
  {
      if(this == &other)
        return;

      for(auto it = other.begin(); it != other.end(); ++it)
        append(*it);

    //(void)other;
    //throw std::runtime_error("TODO");
  }

  LinkedList(LinkedList&& other) //konstruktor przenoszacy
  {
      if(this == &other)
        return;

      head = other.head;
      tail = other.tail;
      size = other.size;
      other.head = other.tail = nullptr;
      other.size = 0;

    //(void)other;
    //throw std::runtime_error("TODO");
  }

  ~LinkedList()
  {
    while(!isEmpty())
        erase(begin());
    delete head;
    delete tail;
  }

  LinkedList& operator=(const LinkedList& other)
  {
      if(this == &other)
        return *this;

      while(!isEmpty())
        erase(begin());

      for(auto it = other.begin(); it != other.end(); ++it)
        append(*it);

      return *this;
    //(void)other;
    //throw std::runtime_error("TODO");
  }

  LinkedList& operator=(LinkedList&& other)
  {

    if(this == &other)
        return *this;

    while(!isEmpty())
        erase(begin());

    head = other.head;
    tail = other.tail;
    size = other.size;
    other.head = other.tail = nullptr;
    other.size = 0;

    return *this;

    //(void)other;
    //throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
      //if (size == 0) return true;
      //return false;
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
    Node *temp = new Node(item);
    temp->prev = tail->prev;
    temp->next = tail;

    tail->prev->next = temp;
    tail->prev = temp;

    size++;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  void prepend(const Type& item)
  {
      Node *temp = new Node(item);
      temp->prev = head;
      temp->next = head->next;

      head->next->prev = temp;
      head->next = temp;

      size++;
      //(void)item;
      //throw std::runtime_error("TODO");
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      if(insertPosition == begin())
        prepend(item);
      else if(insertPosition == end())
        append(item);
      else
      {
          Node* temp = new Node(item);
          temp->prev = insertPosition.node->prev;
          temp->next = insertPosition.node;

          insertPosition.node->prev->next = temp;
          insertPosition.node->prev = temp;
          size++;
      }

    //(void)insertPosition;
    //(void)item;
    //throw std::runtime_error("TODO");
  }

  Type popFirst()
  {
      if(isEmpty())
        throw std::logic_error("Object cannot be popped");

      Type temp = *(begin());
      erase(begin());
      return temp;
    //throw std::runtime_error("TODO");
  }

  Type popLast()
  {
      if(isEmpty())
        throw std::logic_error("Object cannot be popped");

      Type temp = *(end()-1);
      erase(end()-1);
      return temp;
    //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& possition)
  {
      if(possition == end())
        throw std::out_of_range("Object cannot be erased");

    possition.node->next->prev = possition.node->prev;

    possition.node->prev->next = possition.node->next;


      delete possition.node;
      --size;
    //(void)possition;
    //throw std::runtime_error("TODO");
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      auto it = firstIncluded;
      while(it != lastExcluded)
      {
          it++;
          erase(it - 1);
      }
    //(void)firstIncluded;
    //(void)lastExcluded;
    //throw std::runtime_error("TODO");
  }

  iterator begin()
  {
      return iterator(head->next);
    //throw std::runtime_error("TODO)");
  }

  iterator end()
  {
      return iterator(tail);
    //throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
      return const_iterator(head->next);
    //throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
      return const_iterator(tail);
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
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;

private:
    Node *node;
    friend void LinkedList <Type>::insert(const const_iterator&, const Type&);
    friend void LinkedList <Type>::erase(const const_iterator&);

public:

  explicit ConstIterator(Node* pointer = nullptr): node(pointer)
  {}

  reference operator*() const
  {
      if(node->key == nullptr)
        throw std::out_of_range("Out of range");
      return *(node->key);
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator++() //pre-inkrementacja ++i
  {
    if(node->next == nullptr)
        throw std::out_of_range("Out of range");

    node=node->next;

    return *this;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int) //post-inkrementacja i++
  {
      auto result = *this;
      operator++();
    //++*temp;
      return result;
    //throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
      if(node->prev->prev == nullptr) //sprawdzam czy przypadkiem node->prev to nie jest nasz head
        throw std::out_of_range("Out of range");

      node=node->prev;

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
      auto result = *this;
      for(difference_type i=0; i<d; ++i)
        ++result;

      return result;
    //(void)d;
    //throw std::runtime_error("TODO");
  }

  ConstIterator operator-(difference_type d) const
  {
      auto result = *this;
      for(difference_type i=0; i<d; ++i)
        --result;

      return result;
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
   // (void)other;
   // throw std::runtime_error("TODO");
  }

};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node* pointer = nullptr): const_iterator(pointer)
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


/*
template <typename Type>
struct LinkedList<Type>::Node
{
	Type *key;
	Node *prev, *next;

	Node()
	{
		this-> key = nullptr;
		this-> prev = nullptr;
		this-> next = nullptr;
	}
	Node(Type key): Node()
	{
	    this->key = new Type(key);
	}

	~Node()
	{
	    delete key;
	}

	Node* CopyNodes(Node *x)
	{
	    if (!this) return nullptr;

	    Node *temp = new Node(this->key, this->prev, this->next);

	    temp->prev=x;
	    temp->next = this->next->CopyNodes(temp);

	    return temp;
	}

	bool CompareNodes(Node *x)
	{
	    if(!this && !x)
            return true;
        else if(!this || !x)
            return false;

        return (this->key == x-> key && this->prev->CompareNodes(x->prev) && this->next->CompareNodes(x->next));
	}
};
*/

}

#endif // AISDI_LINEAR_LINKEDLIST_H
