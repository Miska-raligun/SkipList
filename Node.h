#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>

template <typename K,typename V>
class Node{
public:
    //层级
    int node_level;
    //当前层的下一个节点指针
    Node<K,V>** forwards;
private:
    K key;
    V value;
public:
    Node(K k,V v,int level);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V value);
};

#include "Node.tpp"