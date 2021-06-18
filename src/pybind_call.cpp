#include <pybind11/pybind11.h>

#include "subscriber.hpp"

namespace py = pybind11;

PYBIND11_MODULE(test_pybind_module, m) {
  m.doc() = "pybind11 subscriber plugin";

  // Define Data struct
  py::class_<Data, std::shared_ptr<Data>>(m, "Data").def(py::init<>()).def_readonly("data", &Data::data);

  // Define Node
  py::class_<rclcpp::Node, std::shared_ptr<rclcpp::Node>>(m, "Node").def(
                                                                         py::init<const std::string &, const rclcpp::NodeOptions &>());

  // Define Subscriber
  py::class_<Subscriber, std::shared_ptr<Subscriber>, rclcpp::Node>(m, "Subscriber").def(py::init<>());

  // Define Interface
  py::class_<Interface>(m, "Interface").def(py::init<>()).def("FetchData", &Interface::FetchData).def("GetSubscriber", &Interface::GetSubscriber);
}
