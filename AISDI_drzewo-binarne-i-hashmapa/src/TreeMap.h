#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
    struct Node
    {
        value_type data;
        Node *left, *right, *parent;

        Node(key_type key, mapped_type mapped)
            : data(std::make_pair(key, mapped)), left(nullptr), right(nullptr), parent(nullptr) {}

        Node(value_type it) : Node(it.first, it.second) {}

        ~Node()
        {
            if(left)
                delete left;
            if(right)
                delete right;
        }
    };

    Node *root;
    size_type count;

public:

  TreeMap() : root(nullptr), count(0)
  {}

  ~TreeMap()
  {
      erase();
  }

  TreeMap(std::initializer_list<value_type> list) : TreeMap()
  {
    for(auto it = list.begin(); it != list.end(); ++it)
        insert(new Node(*it));
  }

  TreeMap(const TreeMap& other) : TreeMap()
  {
    if(this == &other)
        return;

    for(auto it = other.begin(); it != other.end(); ++it)
        insert(new Node(*it));
  }

  TreeMap(TreeMap&& other) : TreeMap()
  {
      if(this == &other)
        return;

      root = other.root;
      count = other.count;

      other.count = 0;
      other.root = nullptr;
  }

  TreeMap& operator=(const TreeMap& other)
  {
    if(this == &other)
        return *this;

    erase();

    for(auto it = other.begin(); it != other.end(); ++it)
        insert(new Node(*it));

    return *this;
  }


  TreeMap& operator=(TreeMap&& other)
  {
      if(this == &other)
        return *this;

      erase();

      root = other.root;
      count = other.count;

      other.count = 0;
      other.root = nullptr;

      return *this;
  }

  bool isEmpty() const
  {
    return (count == 0);
  }

  mapped_type& operator[](const key_type& key)
  {
    Node *temp = findNode(key);
    if(!temp)
    {
        temp = new Node(key, mapped_type{});
        insert(temp);
    }

    return temp->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    Node *temp = findNode(key);
    if(!temp)
        throw std::out_of_range("valueOfC");
    return temp->data.second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    Node *temp = findNode(key);
    if(!temp)
        throw std::out_of_range("valueOf");
    return temp->data.second;
  }

  const_iterator find(const key_type& key) const
  {
    return const_iterator(this, findNode(key));
  }

  iterator find(const key_type& key)
  {
    return iterator(this, findNode(key));
  }

  void remove(const key_type& key)
  {
      auto temp = findNode(key);
      if(temp == nullptr)
        throw std::out_of_range("removeK");

      removeNode(temp);
  }

  void remove(const const_iterator& it)
  {
      if(this != it.tree || it == cend())
        throw std::out_of_range("remove");

      removeNode(it.node);
  }

  size_type getSize() const
  {
    return count;
  }

  bool operator==(const TreeMap& other) const
  {
    if(this->count != other.count)
        return false;

    for(auto it = begin(), itOther = other.begin(); it != end(); ++it, ++itOther)
        if(*it != *itOther)
            return false;

    return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    if(count == 0)
        return end();

    Node *tmp = root;
    while(tmp->left)
        tmp = tmp->left;

    return iterator(this, tmp);
  }

  iterator end()
  {
    return iterator(this, nullptr);
  }

  const_iterator cbegin() const
  {
    if(count == 0)
        return cend();

    Node *tmp = root;
    while(tmp->left)
        tmp = tmp->left;

    return const_iterator(this, tmp);

  }

  const_iterator cend() const
  {
    return const_iterator(this, nullptr);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

private:

    void erase()
    {
        delete root;
        root = nullptr;
        count = 0;
    }

    Node* findNode(const key_type& key) const
    {
        Node *tmp = root;

        while(tmp && tmp->data.first != key)
        {
           if(key < tmp->data.first)
                tmp = tmp->left;
           else
                tmp = tmp->right;
        }

        return tmp;
    }

    void insert(Node *newNode)
    {
        if (root == nullptr)
            root = newNode;
        else
        {
            Node *current = root;

            while(true)
            {
                if(newNode->data.first < current->data.first)
                {
                    if(current->left)
                        current = current->left;
                    else
                        {
                            current->left = newNode;
                            newNode->parent = current;
                            break;
                        }
                }
                else if(newNode->data.first > current->data.first)
                {
                    if(current->right)
                        current = current->right;
                    else
                    {
                        current->right = newNode;
                        newNode->parent = current;
                        break;
                    }
                }
                else
                {
                    delete newNode;
                    return;
                }
            }
        }

        ++count;
        return;
    }

    void removeNode(Node *node)
    {
        if(node == nullptr)
                throw std::out_of_range("removeNode");

        //wezel nie ma dzieci lub ma jedno dziecko
        if(!node->left)
            replace(node, node->right);
        else if(!node->right)
            replace(node, node->left);
        //ma dwoje dzieci
        else
        {
          Node *successor = node->right;
          while(successor->left)
            successor = successor->left;

            //w miejsce nastepnika wstawiamy jego prawe poddrzewo
            replace(successor, successor->right);
            //w miejsce usuwanego wezla wstawiamy nastepnik
            replace(node, successor);

        }

        --count;
        delete node;
    }

    void replace(Node* delNode, Node* suc)
    {
        if(delNode->parent == nullptr)
            root = suc;
        else if(delNode->parent->left == delNode)
            delNode->parent->left = suc;
        else
            delNode->parent->right = suc;


        if(suc)
        {
            suc->parent = delNode->parent;

            if(delNode->right && delNode->right != suc)
                suc->right = delNode->right;

            if(delNode->left && delNode->left != suc)
                suc->left = delNode->left;
        }

        delNode->parent = delNode->left = delNode->right = nullptr;
    }

};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

private:
    const TreeMap *tree;
    Node *node;
    friend void TreeMap<KeyType, ValueType>::remove(const const_iterator&);

public:

  explicit ConstIterator(const TreeMap *tree = nullptr, Node *node = nullptr) : tree(tree), node(node)
  {}

  ConstIterator(const ConstIterator& other) : tree(other.tree), node(other.node)
  {}

  ConstIterator& operator++()
  {
    if(node == nullptr || tree == nullptr)
        throw std::out_of_range("operator++");
    else if(node->right)
    {
        node = node->right;

        while(node->left)
            node = node->left;
    }
    else
    {
        Node *temp = node->parent;
        while(temp && node == temp->right)
            {
                node = temp;
                temp = temp->parent;
            }

        node = temp;
    }

    return *this;
  }

  ConstIterator operator++(int)
  {
    auto result = *this;
    operator++();
    return result;
  }

  ConstIterator& operator--()
  {
      if(tree == nullptr || tree->root == nullptr || tree->begin() == (*this))
        throw std::out_of_range("operator--");
      else if(node == nullptr)
      {
          node = tree->root;
          while(node->right)
            node = node->right;
      }
      else if(node->left)
      {
          node = node->left;
          while(node->right)
            node = node->right;
      }
      else
      {
          Node *temp = node->parent;
          while(temp && node == temp->left)
          {
              node = temp;
              temp = temp->parent;
          }
          node = temp;
      }

      return *this;
  }

  ConstIterator operator--(int)
  {
    auto result = *this;
    operator--();
    return result;
  }

  reference operator*() const
  {
    if(node == nullptr || tree == nullptr)
      throw std::out_of_range("operator*");
    return node->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return (this->node == other.node && this->tree == other.tree);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator(TreeMap *tree = nullptr, Node *node = nullptr) : ConstIterator(tree, node)
  {}

  Iterator(const ConstIterator& other) : ConstIterator(other)
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

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_MAP_H */
