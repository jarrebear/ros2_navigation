#include <functional>

#include <rclcpp/rclcpp.hpp>

#include <geometry_msgs/msg/point_stamped.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>

class InitialPosePublisher : public rclcpp::Node {
public:
  InitialPosePublisher() : Node("pose_publisher_node") {

    subscriber_point_ =
        this->create_subscription<geometry_msgs::msg::PointStamped>(
            "/clicked_point", 10,
            std::bind(&InitialPosePublisher::point_callback, this,
                      std::placeholders::_1));

    publisher_pose_ =
        this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>(
            "/initialpose", 10);

    RCLCPP_INFO(this->get_logger(), "Initial pose publisher ready");
  }

private:
  void point_callback(const geometry_msgs::msg::PointStamped::SharedPtr msg) {
    geometry_msgs::msg::PoseWithCovarianceStamped pose_msg;

    pose_msg.header.frame_id = "/map";
    pose_msg.pose.pose.position = msg->point;
    publisher_pose_->publish(pose_msg);
    RCLCPP_INFO(this->get_logger(), "Initial pose published!");
  }

private:
  rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr
      publisher_pose_;

  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr
      subscriber_point_;
};

int main(int argc, char **argv) {
  // initialize the ROS2 communication
  rclcpp::init(argc, argv);
  // declare the node constructor
  auto node = std::make_shared<InitialPosePublisher>();
  // keeps the node alive, waits for a request to kill the node (ctrl+c)
  rclcpp::spin(node);
  // shutdown the ROS2 communication
  rclcpp::shutdown();
  return 0;
}