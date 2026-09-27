#include <cmath>

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>

#include <geometry_msgs/msg/point_stamped.hpp>
#include <nav2_msgs/action/navigate_to_pose.hpp>

class NavToPoseClient : public rclcpp::Node {
public:
  using NavToPose = nav2_msgs::action::NavigateToPose;
  using GoalHandleNavToPose = rclcpp_action::ClientGoalHandle<NavToPose>;

  NavToPoseClient() : Node("nav_to_pose_client") {
    action_client_ =
        rclcpp_action::create_client<NavToPose>(this, "/navigate_to_pose");

    subscriber_point_ =
        this->create_subscription<geometry_msgs::msg::PointStamped>(
            "/clicked_point", 10,
            std::bind(&NavToPoseClient::point_callback, this,
                      std::placeholders::_1));
  }

  void send_goal(double x, double y, double yaw) {

    auto goal_msg = NavToPose::Goal();
    goal_msg.pose.header.frame_id = "map";
    goal_msg.pose.header.stamp = this->now();
    goal_msg.pose.pose.position.x = x;
    goal_msg.pose.pose.position.y = y;
    goal_msg.pose.pose.orientation.z = std::sin(yaw / 2.0);
    goal_msg.pose.pose.orientation.w = std::cos(yaw / 2.0);

    RCLCPP_INFO(this->get_logger(), "Sending goal: x=%.2f, y=%.2f, yaw=%.2f", x,
                y, yaw);

    if (!this->action_client_->wait_for_action_server()) {
      RCLCPP_ERROR(this->get_logger(),
                   "Action server not available after waiting");
      rclcpp::shutdown();
      return;
    }

    auto send_goal_options =
        rclcpp_action::Client<NavToPose>::SendGoalOptions();
    send_goal_options.goal_response_callback = std::bind(
        &NavToPoseClient::goal_response_callback, this, std::placeholders::_1);
    send_goal_options.feedback_callback =
        std::bind(&NavToPoseClient::feedback_callback, this,
                  std::placeholders::_1, std::placeholders::_2);
    send_goal_options.result_callback = std::bind(
        &NavToPoseClient::result_callback, this, std::placeholders::_1);

    this->action_client_->async_send_goal(goal_msg, send_goal_options);
  }

  void
  goal_response_callback(const GoalHandleNavToPose::SharedPtr &goal_handle) {
    if (!goal_handle) {
      RCLCPP_ERROR(this->get_logger(),
                   "Goal was rejected by the action server.");
    } else {
      RCLCPP_INFO(this->get_logger(), "Goal accepted by the action server.");
    }
  }

  void
  feedback_callback(GoalHandleNavToPose::SharedPtr,
                    const std::shared_ptr<const NavToPose::Feedback> feedback) {
    RCLCPP_INFO(this->get_logger(), "Distance remaining: %.2f",
                feedback->distance_remaining);
  }

  void result_callback(const GoalHandleNavToPose::WrappedResult &result) {
    switch (result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(), "Action completed with success: true");
      break;
    case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_INFO(this->get_logger(),
                  "Action completed with success: false (aborted)");
      break;
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_INFO(this->get_logger(),
                  "Action completed with success: false (canceled)");
      break;
    default:
      RCLCPP_ERROR(this->get_logger(), "Unknown result code");
      break;
    }
  }

  void point_callback(const geometry_msgs::msg::PointStamped::SharedPtr msg) {

    RCLCPP_INFO(this->get_logger(), "Received point: frame=%s x=%.2f y=%.2f",
                msg->header.frame_id.c_str(), msg->point.x, msg->point.y);

    send_goal(msg->point.x, msg->point.y, 0);
  }

private:
  rclcpp_action::Client<NavToPose>::SharedPtr action_client_;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr
      subscriber_point_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto action_client = std::make_shared<NavToPoseClient>();

  // Keep the node alive to receive callbacks
  rclcpp::spin(action_client);

  rclcpp::shutdown();
  return 0;
}