//header的forwards指向了第i层的第一个节点
template <typename K,typename V>
SkipList<K,V>::SkipList(int l):max_level(l),skip_list_level(0),element_count(0){
    K k;
    V v;
    // 创建头节点，并初始化键值为默认值
    header = std::make_shared<Node<K, V>>(k, v, max_level);
};

template <typename K,typename V>
SkipList<K,V>::~SkipList(){
    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
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
std::shared_ptr<Node<K,V>> SkipList<K,V>::create_node(K key,V value,int level){
    return std::make_shared<Node<K,V>>(key,value,level);
};

template <typename K, typename V>
int SkipList<K, V>::insert_element(K key, V value) {
    std::lock_guard<std::mutex> lock(mtx);
    std::shared_ptr<Node<K, V>> now = header;
    std::vector<std::shared_ptr<Node<K, V>>> update(max_level+1,nullptr);

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
        
        std::shared_ptr<Node<K,V>> inserted_node = create_node(key, value, random_level);
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
    std::shared_ptr<Node<K, V>> now = header;
    
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
void SkipList<K, V>::delete_element(K key) {
    std::lock_guard<std::mutex> lock(mtx);
    std::shared_ptr<Node<K, V>> now = header;
    std::vector<std::shared_ptr<Node<K, V>>> update(skip_list_level + 1,nullptr);

    // 从最高层向下搜索更新位置
    for (int now_level = skip_list_level; now_level >= 0; --now_level) {
        while (now->forwards[now_level] != nullptr && now->forwards[now_level]->get_key() < key) {
            now = now->forwards[now_level];
        }
        update[now_level] = now;
    }

    // 获取要删除的节点
    std::shared_ptr<Node<K, V>> target = now->forwards[0];

    // 检查是否确实找到了要删除的节点
    if (target == nullptr || target->get_key() != key) {
        return;  // 如果目标节点不存在，直接返回
    }

    // 删除节点并更新指针
    for (int i = 0; i <= skip_list_level; ++i) {
        if (update[i]->forwards[i] != target) {
            break;  // 跳出循环，更新指针完成
        }
        update[i]->forwards[i] = target->forwards[i];
    }

    // 检查是否需要更新 skip_list_level
    while (skip_list_level > 0 && header->forwards[skip_list_level] == nullptr) {
        skip_list_level--;
    }

    element_count--;
    return;
}

template <typename K, typename V>
void SkipList<K, V>::printList() {
    for(int i=skip_list_level;i>=0;i--){
        std::shared_ptr<Node<K,V>> now=header->forwards[i];
        std::cout<<"LeveL "<<i+1<<": ";
        while(now!=nullptr){
            std::cout<<now->get_key()<<"-"<<now->get_value()<<" ";
            now=now->forwards[i];
        }
        std::cout<<std::endl;
    }
    return;
};

template <typename K, typename V>
void SkipList<K, V>::dump_file() {
    _file_writer.open(WRITE_FILE); // 打开文件
    std::shared_ptr<Node<K, V>> node = header->forwards[0]; // 从头节点开始遍历

    while (node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << ";\n"; // 写入键值对
        node = node->forwards[0]; // 移动到下一个节点
    }

    _file_writer.flush(); // 刷新缓冲区，确保数据完全写入
    _file_writer.close(); // 关闭文件
}

// Load data from disk
template <typename K, typename V>
void SkipList<K, V>::load_file() {
    _file_reader.open(READ_FILE);
    if (!_file_reader.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    std::string line;
    std::string key;
    std::string value;

    auto is_valid_string = [&](const std::string& str) -> bool {
        return !str.empty() && str.find(delimiter) != std::string::npos;
    };

    auto get_key_value_from_string = [&](const std::string& str, std::string& key, std::string& value) {
        if (!is_valid_string(str)) {
            return;
        }
        size_t pos = str.find(delimiter);
        if (pos != std::string::npos) {
            key = str.substr(0, pos);
            value = str.substr(pos + 1);
        }
    };

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key.empty() || value.empty()) {
            continue;
        }
        // Define key as int type
        insert_element(stoi(key), value);
        std::cout << "key: " << key << " value: " << value << std::endl;
    }

    _file_reader.close();
}

template <typename K,typename V>
int SkipList<K,V>::size(){
    return element_count;
};
