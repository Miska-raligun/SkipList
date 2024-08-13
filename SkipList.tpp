//header的forwards指向了第i层的第一个节点
template <typename K,typename V>
SkipList<K,V>::SkipList(int l):max_level(l),skip_list_level(0),element_count(0){
    K k;
    V v;
    // 创建头节点，并初始化键值为默认值
    this->header = new Node<K, V>(k, v, max_level);
};

template <typename K,typename V>
SkipList<K,V>::~SkipList(){
};

template <typename K,typename V>
int SkipList<K,V>::get_random_level(){
    int level=0;
    while(rand()%2){
        level++;
    }
    level=level>max_level?max_level:level;
    return level;
};

template <typename K,typename V>
Node<K,V>* SkipList<K,V>::create_node(K key,V value,int level){
    Node<K,V>* node=new Node(key,value,level);
    return node;
};

template <typename K, typename V>
int SkipList<K, V>::insert_element(K key, V value) {
    Node<K, V>* now = this->header;
    Node<K, V>* update[max_level+1];
    memset(update,0,sizeof(Node<K,V>*)*(max_level + 1));

    // 从最高层向下搜索插入位置
    for (int now_level = skip_list_level; now_level >= 0; --now_level) {
        while (now->forwards[now_level] != nullptr && now->forwards[now_level]->get_key() < key) {
            now = now->forwards[now_level];
        }
        update[now_level] = now;
    }

    // 插入节点
    now = now->forwards[0];
    if (now != nullptr && now->get_key() == key) {
        return 1; // key 已经存在
    }
    // 检查待插入的键是否已存在于跳表中
    if (now == nullptr || now->get_key() != key) {
        // 通过随机函数决定新节点的层级高度
        int random_level = get_random_level();
        // 如果新节点的层级超出了跳表的当前最高层级
        if (random_level > skip_list_level) {
            // 对所有新的更高层级，将头节点设置为它们的前驱节点
            for (int i = skip_list_level + 1; i <= random_level; i++) {
                update[i] = header;
            }
            // 更新跳表的当前最高层级为新节点的层级
            skip_list_level = random_level;
        }
        
        Node<K, V> *inserted_node = create_node(key, value, random_level);
        // 在各层插入新节点，同时更新前驱节点的 forward 指针
        for (int i = 0; i <= random_level; i++) {
            // 新节点指向当前节点的下一个节点
            inserted_node->forwards[i] = update[i]->forwards[i];
            // 当前节点的下一个节点更新为新节点
            update[i]->forwards[i] = inserted_node;
        }
        element_count++;
    }
    return 0;
}


template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    Node<K, V>* now = header;
    
    for (int now_level = skip_list_level; now_level >= 0; --now_level) {
        while (now->forwards[now_level] != nullptr && now->forwards[now_level]->get_key() < key) {
            now = now->forwards[now_level];
        }
    }
    now=now->forwards[0];
    if(now!=nullptr&&now->get_key()==key){
        return true;
    }
    return false;  // 如果遍历完所有层级仍未找到，返回 false
};

template <typename K, typename V>
void SkipList<K, V>::printList() {
    for(int i=skip_list_level;i>=0;i--){
        Node<K,V>* now=header->forwards[i];
        while(now!=nullptr){
            std::cout<<now->get_key()<<" ";
            now=now->forwards[i];
        }
        std::cout<<std::endl;
    }
    return;
};

