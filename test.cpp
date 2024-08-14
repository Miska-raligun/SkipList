#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "SkipList.h"

int main(){
    std::shared_ptr<SkipList<int,std::string>> sl=std::make_shared<SkipList<int,std::string>>(10);
    sl->load_file();
    sl->insert_element(4,"skiplist");
    sl->dump_file();
    return 0;
}