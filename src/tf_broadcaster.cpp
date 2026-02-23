#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"

#include <cmath>
#include <memory>

using namespace std::chrono_literals;

class TFBroadcaster : public rclcpp::Node
{
public:
  TFBroadcaster()
  : Node("tf_broadcaster")
  {
    // Create Transform Broadcaster
    tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);

    // Create timer (100 ms)
    timer_ = this->create_wall_timer(
      100ms,
      std::bind(&TFBroadcaster::broadcast_transform, this)
    );
  }

private:
  void broadcast_transform()
  {
    geometry_msgs::msg::TransformStamped transform;

    // Time
    transform.header.stamp = this->get_clock()->now();
    transform.header.frame_id = "world";
    transform.child_frame_id = "robot";

    // Circular motion
    double t = this->get_clock()->now().seconds();
    transform.transform.translation.x = 2.0 * std::cos(t);
    transform.transform.translation.y = 2.0 * std::sin(t);
    transform.transform.translation.z = 0.0;

    // Identity rotation
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = 0.0;
    transform.transform.rotation.w = 1.0;

    // Broadcast transform
    tf_broadcaster_->sendTransform(transform);
  }

  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TFBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
