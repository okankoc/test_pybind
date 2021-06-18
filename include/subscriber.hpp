#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

struct Data {
  std::string data = "";
  bool modified = false;
};

class Subscriber : public rclcpp::Node {
 public:
  Subscriber() : rclcpp::Node("minimal_subscriber", rclcpp::NodeOptions()) {
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "topic", 10, std::bind(&Subscriber::TopicCallback, this, std::placeholders::_1));
    data_ = std::make_shared<Data>();
  }

  std::shared_ptr<Data> GetData() {
    data_->modified = false;
    return data_;
  };

  std::shared_ptr<Data> data_;

private:
  void TopicCallback(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    data_->data = msg->data;
    data_->modified = true;
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

class Interface {
 public:
  Interface() {
    rclcpp::init(0, nullptr);
    subscriber_ = std::make_shared<Subscriber>();

    // rclcpp::executors::MultiThreadedExecutor executor;
    // executor.add_node(subscriber_);
    // executor.spin();
    // auto spin_executor = [&executor]() { executor.spin_once(); };
    // std::thread executor_thread(spin_executor);

    // auto spin_executor = [&]() { rclcpp::spin(subscriber_); };
    // std::thread executor_thread(spin_executor);

    // rclcpp::spin(subscriber_);
  }

  std::shared_ptr<Data> FetchData() {
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(subscriber_);
    while (rclcpp::ok() && !subscriber_->data_->modified) {
      executor.spin_once();
    }
    return subscriber_->GetData();
  }

  std::shared_ptr<Subscriber> GetSubscriber() { return subscriber_; }

  ~Interface() { rclcpp::shutdown(); }

 private:
  std::shared_ptr<Subscriber> subscriber_;
};

