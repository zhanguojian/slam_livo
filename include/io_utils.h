/* read and write utils */

#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp/serialization.hpp>
#include <rclcpp/serialized_message.hpp>
#include <rosbag2_cpp/reader.hpp>
#include <rosbag2_storage/storage_options.hpp>
#include <rosbag2_cpp/converter_options.hpp>
#include <rosbag2_storage/serialized_bag_message.hpp>

#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>

/**
 * ROSBAG IO
 * 指定一个包名，添加一些回调函数，就可以顺序遍历这个包
 */
class RosbagIO {
public:
    // 传入 rosbag2 的目录路径（包含 metadata.yaml 的文件夹），或完整 uri
    explicit RosbagIO(std::string bag_filename)
    {
        assert(bag_filename != "");
        // rosbag2 录制的是“目录”，open 时传目录即可
        bag_path_ = std::string(ROOT_DIR) + "data/bag/" + bag_filename;
    }

    // 通用回调
    using MessageProcessFunction =
        std::function<bool(const rosbag2_storage::SerializedBagMessageSharedPtr m)>;

    // 点云回调
    using PointCloudHandle =
        std::function<bool(const sensor_msgs::msg::PointCloud2::ConstSharedPtr &)>;

    // 图像回调
    using ImageHandle =
        std::function<bool(const sensor_msgs::msg::Image::ConstSharedPtr &)>;

    // imu回调
    using ImuHandle =
        std::function<bool(const sensor_msgs::msg::Imu::ConstSharedPtr &)>;

    // 运行主函数
    void go();

    void printfBagMetaInfo()
    {
        const auto &bag_meta_info = reader_.get_metadata();
        total_message_count_ = bag_meta_info.message_count;

        std::cout << "消息总数为： " << total_message_count_ << std::endl;
        std::cout << "bag路径为:" << bag_path_ << std::endl;
        std::cout << "话题\t 话题类型\t 消息数量" << std::endl;
        for (const auto &topic : bag_meta_info.topics_with_message_count) {
            std::cout << topic.topic_metadata.name << "\t"
                      << topic.topic_metadata.type << "\t"
                      << topic.message_count << std::endl;
        }
        std::cout << "--------------------------------" << std::endl;
    }

    // 通用处理函数
    RosbagIO &AddHandle(const std::string &topic_name, MessageProcessFunction handle)
    {
        process_func_.emplace(topic_name, std::move(handle));
        return *this;
    }

    // 点云 handle
    RosbagIO &AddPointCloudHandle(const std::string &topic_name, PointCloudHandle f)
    {
        return AddHandle(topic_name,
            [this, f](const rosbag2_storage::SerializedBagMessageSharedPtr m) -> bool {
                rclcpp::SerializedMessage serialized_msg(*m->serialized_data);
                auto msg = std::make_shared<sensor_msgs::msg::PointCloud2>();
                point_cloud_serializer_.deserialize_message(&serialized_msg, msg.get());
                if (msg == nullptr) return false;
                return f(msg);
            });
    }

    // imu handle
    RosbagIO &AddImuHandle(const std::string &topic_name, ImuHandle f)
    {
        return AddHandle(topic_name,
            [this, f](const rosbag2_storage::SerializedBagMessageSharedPtr m) -> bool {
                rclcpp::SerializedMessage serialized_msg(*m->serialized_data);
                auto msg = std::make_shared<sensor_msgs::msg::Imu>();
                imu_serializer_.deserialize_message(&serialized_msg, msg.get());
                if (msg == nullptr) return false;
                return f(msg);
            });
    }

    // image handle
    RosbagIO &AddImageHandle(const std::string &topic_name, ImageHandle f)
    {
        return AddHandle(topic_name,
            [this, f](const rosbag2_storage::SerializedBagMessageSharedPtr m) -> bool {
                rclcpp::SerializedMessage serialized_msg(*m->serialized_data);
                auto msg = std::make_shared<sensor_msgs::msg::Image>();
                image_serializer_.deserialize_message(&serialized_msg, msg.get());
                if (msg == nullptr) return false;
                return f(msg);
            });
    }

    void cleanProcessFunc() { process_func_.clear(); }

private:
    std::string bag_path_;
    std::string storage_id_;
    std::map<std::string, MessageProcessFunction> process_func_;

    rosbag2_cpp::Reader reader_;

    rclcpp::Serialization<sensor_msgs::msg::PointCloud2> point_cloud_serializer_;
    rclcpp::Serialization<sensor_msgs::msg::Imu> imu_serializer_;
    rclcpp::Serialization<sensor_msgs::msg::Image> image_serializer_;

    uint64_t total_message_count_ = 0llu;
    uint64_t read_message_count_ = 0llu;
};
// /* read and write utils */

// #pragma once

// #include <rosbag2_cpp/reader.hpp>
// #include <rosbag2_cpp/view.hpp>
// #include <rosbag2_storage/serialized_bag_message.hpp>
// #include <sensor_msgs/msg/imu.hpp>
// #include <sensor_msgs/msg/point_cloud2.hpp>
// #include <sensor_msgs/msg/image.hpp>
// #include <sensor_msgs/msg/compressed_image.hpp>

// /**
//  * ROSBAG IO
//  * 指定一个包名，添加一些回调函数，就可以顺序遍历这个包
//  */
// class RosbagIO {
//     public:
//        explicit RosbagIO(std::string bag_filename) 
//        {
//         assert(bag_filename != "");
//         bag_path_ = std::string(ROOT_DIR) + "/data/" + "bag/" + bag_filename + ".db3";
  
//        }
       

//        //通用回调
//        using MessageProcessFunction = std::function<bool(const rosbag2_storage::SerializedBagMessageSharedPtr m)>;

//        //点云回调
//        using PointCloudHandle = std::function<bool(const sensor_msgs::msg::PointCloud2::ConstSharedPtr &)>;
     
//        //图像回调
//        using ImageHandle = std::function<bool(const sensor_msgs::msg::Image::ConstSharedPtr &)>;
     
//        //imu回调
//        using ImuHandle = std::function<bool(const sensor_msgs::msg::Imu::ConstSharedPtr &)>;
    
//        //运行主函数 
//        void go();

//        void printfBagMetaInfo(){
//         auto bag_meta_info = reader_.get_metadata();

//         total_message_count_ = bag_meta_info->message_count;

//         std::cout << "消息总数为： " << total_message_count_ << std::endl;
//         std::cout << "bag文件为:" << bag_path_ << std::endl;

//         std::cout << "话题\t 话题类型\t 消息数量" << std::endl;
//         for(const auto &topic : bag_meta_info->topics){
//             std::cout << topic.name << "\t" << topic.type << "\t" << topic.message_count << std::endl;
//         }

//         std::cout << "--------------------------------" << std::endl;
//         std::cout << "总消息数: " << total_message_count_ << std::endl;
//         std::cout << "已读消息数: " << read_message_count_ << std::endl;
//         std::cout << "--------------------------------" << std::endl;
//        }


//        //通用处理函数
//        RosbagIO &AddHandle(const std::string &topic_name, MessageProcessFunction handle){
//         process_func_.emplace(topic_name, handle);
//         return *this;
//        }


//        //handle封装点云handle，可以链式调用，点云实际调用函数
//        RosbagIO &AddPointCloudHandle(const std::string &topic_name, PointCloudHandle f){
//         return AddHandle(topic_name, [f](const rosbag2_storage::SerializedBagMessageSharedPtr m) -> bool{
//             //rosbag2_storage::SerializedBagMessageSharedPtr 是rosbag2的序列化消息指针
//             rclcpp::SerializedMessage serialized_msg(m->serialized_data);
//             auto msg = std::make_shared<sensor_msgs::msg::PointCloud2>();
//             //serializer是rosbag2的序列化器
//             point_cloud_serializer_.deserialize_message(&serialized_msg, msg);
//             if(msg == nullptr) return false;
//             return f(msg);
//         } );
//     }
       
//     //handle封装imuhandle，可以链式调用，imu实际调用函数
//        RosbagIO &AddImuHandle(const std::string &topic_name, ImuHandle f){
//         return AddHandle(topic_name, [f](const rosbag2_storage::SerializedBagMessageSharedPtr m) -> bool{
//             //rosbag2_storage::SerializedBagMessageSharedPtr 是rosbag2的序列化消息指针
//             rclcpp::SerializedMessage serialized_msg(m->serialized_data);
//             auto msg = std::make_shared<sensor_msgs::msg::Imu>();
//             //serializer是rosbag2的序列化器
//             imu_serializer_.deserialize_message(&serialized_msg, msg);
//             if(msg == nullptr) return false;
//             return f(msg);
//         } );
//     }
       

//        //handle封装imagehandle，可以链式调用，image实际调用函数
//         RosbagIO &AddImageHandle(const std::string &topic_name, ImageHandle f){
//         return AddHandle(topic_name, [f](const rosbag2_storage::SerializedBagMessageSharedPtr m) -> bool{
//             //rosbag2_storage::SerializedBagMessageSharedPtr 是rosbag2的序列化消息指针
//             auto msg = std::make_shared<sensor_msgs::msg::Image>();
//             //serializer是rosbag2的序列化器
//             image_serializer_.deserialize_message(&serialized_msg, msg);
//             if(msg == nullptr) return false;
//             return f(msg);
//         } );
//     }

//     //清空处理函数
//     void cleanProcessFunc(){
//         process_func_.clear();
//     }

//     private:
//       std::string bag_path_;
//       std::map<std::string, MessageProcessFunction> process_func_;

//       rosbag2_cpp::Reader reader_;
      
//       rclcpp::Serialization<sensor_msgs::msg::PointCloud2> point_cloud_serializer_;

//       rclcpp::Serialization<sensor_msgs::msg::Imu> imu_serializer_;

//       rclcpp::Serialization<sensor_msgs::msg::Image> image_serializer_;

//       uint64_t total_message_count_ = 0llu;

//       uint64_t read_message_count_ = 0llu;
// };