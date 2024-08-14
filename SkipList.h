#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include "Node.h"
#include <fstream>
#include <mutex>

const std::string delimiter = ":"; // 分隔符
static constexpr const char* WRITE_FILE = "data/write.txt";
static constexpr const char* READ_FILE = "data/read.txt";
std::mutex mtx;

template <typename K, typename V>
class SkipList {
public:
    SkipList(int);                      // 构造函数
    ~SkipList();                        // 析构函数
    int get_random_level();             // 获取节点的随机层级
    std::shared_ptr<Node<K, V>> create_node(K, V, int); // 节点创建
    int insert_element(K, V);           // 插入节点
    void display_list();                // 展示节点
    bool search_element(K);             // 搜索节点
    void delete_element(K);             // 删除节点
    void dump_file();                   // 持久化数据到文件
    void load_file();                   // 从文件加载数据
    //void clear(std::shared_ptr<Node<K,V>>);           // 递归删除节点
    int size();                         // 跳表中的节点个数
    void printList();
private:
    int max_level;  // 最大层级
    int skip_list_level;  // 当前层级
    std::shared_ptr<Node<K, V>> header;  // 头节点
    // 其他私有成员（省略）
    int element_count;  // 节点数量
    
    std::ofstream _file_writer;
    std::ifstream _file_reader;

};

#include "SkipList.tpp"