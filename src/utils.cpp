// utils.cpp

#include "include/io_utils.h"
#include <vector>
#include <cstdint> // for int64_t
#include <limits>  // for std::numeric_limits
#include <stdexcept> // for std::out_of_range

std::vector<int> convertToIntVectorSafe(const std::vector<int64_t>& int64_vector) {
    std::vector<int> int_vector;
    int_vector.reserve(int64_vector.size()); // 预留空间以提高效率

    for (int64_t value : int64_vector) {
        if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
            throw std::out_of_range("Value is out of range for int");
        }
        int_vector.push_back(static_cast<int>(value));
    }

    return int_vector;
}

void RosbagIO::go(){

    std::cout << "开始读取bag文件: " << bag_path_ << std::endl;

    bag_reader_.open(bag_path_);
    
    printfBagMetaInfo();

    if(!bag_reader_.is_open()){
        std::cerr << "无法打开bag文件;文件路径错误: " << bag_path_ << std::endl;
        return;
    }

    while(rclcpp::ok()){
        auto message = bag_reader_.read_next();
        if(message == nullptr){
            break;
        }
        auto it = process_func_.find(message->topic_name);
        if(it != process_func_.end()){
            it->second(message);
        }
    }

    bag_reader_.close();
    std::cout << "读取bag文件完成" << std::endl;
    std::cout << "总消息数: " << total_message_count_ << std::endl;
    std::cout << "已读消息数: " << read_message_count_ << std::endl;
    std::cout << "--------------------------------" << std::endl;
    }