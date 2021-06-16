# test_pybind

Notes:

- Building works well for me but target_include_directories might need be adjusted
if path is not /opt/ros/foxy/include (for adding rclcpp headers)

- by including std::shared_ptr<OurClass> as a second template argument inside py::class,
we can compile ros node classes with pybind successfully.

- After compiling the pybind object make sure to append to $PYTHONPATH the cpython object
located in build/test_pybind

- rclcpp::spin(subscriber_) inside the Interface() constructor seems to work but of course
blocks when called in python.

- spinning the node in an new thread (using std::thread) caused ipython to crash.

- spin_once() in the while loop prints Hello only two times.

- spinning in Python instead creates an error, as in following:

In [1]: import rclpy

In [2]: import test_pybind_module as pyb

In [3]: i = pyb.Interface()

In [4]: s = i.GetSubscriber()

In [5]: rclpy.spin(s)

/opt/ros/foxy/lib/python3.8/site-packages/rclpy/__init__.py in spin(node, executor)
    187     executor = get_global_executor() if executor is None else executor
    188     try:
--> 189         executor.add_node(node)
    190         while executor.context.ok():
    191             executor.spin_once()

/opt/ros/foxy/lib/python3.8/site-packages/rclpy/executors.py in add_node(self, node)
    250             if node not in self._nodes:
    251                 self._nodes.add(node)
--> 252                 node.executor = self
    253                 # Rebuild the wait set so it includes this new node
    254                 self._guard.trigger()

AttributeError: 'test_pybind_module.Subscriber' object has no attribute 'executor'


