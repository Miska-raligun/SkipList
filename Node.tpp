template <typename K,typename V>
Node<K,V>::Node(const K k,const V v,int level):key(k),value(v),node_level(level){
    this->forwards=new Node<K,V>* [level+1];
    memset(this->forwards,0,sizeof(Node*)*(level+1));
};

template <typename K,typename V>
Node<K,V>::~Node(){
    delete[] forwards;
};

template <typename K,typename V>
K Node<K,V>::get_key() const{
    return key;
};

template <typename K,typename V>
V Node<K,V>::get_value() const{
    return value;
};

template <typename K,typename V>
void Node<K,V>::set_value(V value){
    this->value=value;
};