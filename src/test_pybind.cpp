#include <pybind11/pybind11.h>

#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Subscriber : public rclcpp::Node {
 public:
  Subscriber() : rclcpp::Node("minimal_subscriber", rclcpp::NodeOptions()) {
    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "topic", 10, std::bind(&Subscriber::TopicCallback, this, std::placeholders::_1));
  }

 private:
  void TopicCallback(const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    data_ = msg->data;
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  std::string data_;
};

class Interface {
 public:
  Interface() {
    rclcpp::init(0, nullptr);
    subscriber_ = std::make_shared<Subscriber>();

    // rclcpp::executors::MultiThreadedExecutor executor;
    // executor.add_node(subscriber_);
    // auto spin_executor = [&executor]() { executor.spin_once(); };
    // std::thread executor_thread(spin_executor);

    // auto spin_executor = [&]() { rclcpp::spin(subscriber_); };
    // std::thread executor_thread(spin_executor);

    // rclcpp::spin(subscriber_);

    // rclcpp::executors::SingleThreadedExecutor executor;
    // executor.add_node(subscriber_);
    // while (rclcpp::ok()) {
    //   executor.spin_once();
    //   std::cout << "Hello\n";
    // }
  }

  std::shared_ptr<Subscriber> GetSubscriber() { return subscriber_; }

  ~Interface() { rclcpp::shutdown(); }

 private:
  std::shared_ptr<Subscriber> subscriber_;
};

namespace py = pybind11;

PYBIND11_MODULE(test_pybind_module, m) {
  m.doc() = "pybind11 subscriber plugin";

  py::class_<rclcpp::Node, std::shared_ptr<rclcpp::Node>>(m, "Node").def(
      py::init<const std::string &, const rclcpp::NodeOptions &>());
  py::class_<Subscriber, std::shared_ptr<Subscriber>, rclcpp::Node>(m, "Subscriber").def(py::init<>());
  py::class_<Interface>(m, "Interface").def(py::init<>()).def("GetSubscriber", &Interface::GetSubscriber);
}
