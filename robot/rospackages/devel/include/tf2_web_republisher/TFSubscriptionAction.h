// Generated by gencpp from file tf2_web_republisher/TFSubscriptionAction.msg
// DO NOT EDIT!


#ifndef TF2_WEB_REPUBLISHER_MESSAGE_TFSUBSCRIPTIONACTION_H
#define TF2_WEB_REPUBLISHER_MESSAGE_TFSUBSCRIPTIONACTION_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <tf2_web_republisher/TFSubscriptionActionGoal.h>
#include <tf2_web_republisher/TFSubscriptionActionResult.h>
#include <tf2_web_republisher/TFSubscriptionActionFeedback.h>

namespace tf2_web_republisher
{
template <class ContainerAllocator>
struct TFSubscriptionAction_
{
  typedef TFSubscriptionAction_<ContainerAllocator> Type;

  TFSubscriptionAction_()
    : action_goal()
    , action_result()
    , action_feedback()  {
    }
  TFSubscriptionAction_(const ContainerAllocator& _alloc)
    : action_goal(_alloc)
    , action_result(_alloc)
    , action_feedback(_alloc)  {
  (void)_alloc;
    }



   typedef  ::tf2_web_republisher::TFSubscriptionActionGoal_<ContainerAllocator>  _action_goal_type;
  _action_goal_type action_goal;

   typedef  ::tf2_web_republisher::TFSubscriptionActionResult_<ContainerAllocator>  _action_result_type;
  _action_result_type action_result;

   typedef  ::tf2_web_republisher::TFSubscriptionActionFeedback_<ContainerAllocator>  _action_feedback_type;
  _action_feedback_type action_feedback;





  typedef boost::shared_ptr< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> const> ConstPtr;

}; // struct TFSubscriptionAction_

typedef ::tf2_web_republisher::TFSubscriptionAction_<std::allocator<void> > TFSubscriptionAction;

typedef boost::shared_ptr< ::tf2_web_republisher::TFSubscriptionAction > TFSubscriptionActionPtr;
typedef boost::shared_ptr< ::tf2_web_republisher::TFSubscriptionAction const> TFSubscriptionActionConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace tf2_web_republisher

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'actionlib_msgs': ['/opt/ros/kinetic/share/actionlib_msgs/cmake/../msg'], 'tf2_web_republisher': ['/home/ali/Programming/robotics-prototype/robot/rospackages/devel/share/tf2_web_republisher/msg', '/home/ali/Programming/robotics-prototype/robot/rospackages/src/tf2_web_republisher/msg'], 'geometry_msgs': ['/opt/ros/kinetic/share/geometry_msgs/cmake/../msg'], 'roscpp': ['/opt/ros/kinetic/share/roscpp/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
{
  static const char* value()
  {
    return "15787ffd6a2492c0022abe990c898794";
  }

  static const char* value(const ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x15787ffd6a2492c0ULL;
  static const uint64_t static_value2 = 0x022abe990c898794ULL;
};

template<class ContainerAllocator>
struct DataType< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
{
  static const char* value()
  {
    return "tf2_web_republisher/TFSubscriptionAction";
  }

  static const char* value(const ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
{
  static const char* value()
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
\n\
TFSubscriptionActionGoal action_goal\n\
TFSubscriptionActionResult action_result\n\
TFSubscriptionActionFeedback action_feedback\n\
\n\
================================================================================\n\
MSG: tf2_web_republisher/TFSubscriptionActionGoal\n\
# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
\n\
Header header\n\
actionlib_msgs/GoalID goal_id\n\
TFSubscriptionGoal goal\n\
\n\
================================================================================\n\
MSG: std_msgs/Header\n\
# Standard metadata for higher-level stamped data types.\n\
# This is generally used to communicate timestamped data \n\
# in a particular coordinate frame.\n\
# \n\
# sequence ID: consecutively increasing ID \n\
uint32 seq\n\
#Two-integer timestamp that is expressed as:\n\
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n\
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n\
# time-handling sugar is provided by the client library\n\
time stamp\n\
#Frame this data is associated with\n\
# 0: no frame\n\
# 1: global frame\n\
string frame_id\n\
\n\
================================================================================\n\
MSG: actionlib_msgs/GoalID\n\
# The stamp should store the time at which this goal was requested.\n\
# It is used by an action server when it tries to preempt all\n\
# goals that were requested before a certain time\n\
time stamp\n\
\n\
# The id provides a way to associate feedback and\n\
# result message with specific goal requests. The id\n\
# specified must be unique.\n\
string id\n\
\n\
\n\
================================================================================\n\
MSG: tf2_web_republisher/TFSubscriptionGoal\n\
# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
# goal\n\
string[] source_frames\n\
string target_frame\n\
float32 angular_thres\n\
float32 trans_thres\n\
float32 rate\n\
\n\
================================================================================\n\
MSG: tf2_web_republisher/TFSubscriptionActionResult\n\
# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
\n\
Header header\n\
actionlib_msgs/GoalStatus status\n\
TFSubscriptionResult result\n\
\n\
================================================================================\n\
MSG: actionlib_msgs/GoalStatus\n\
GoalID goal_id\n\
uint8 status\n\
uint8 PENDING         = 0   # The goal has yet to be processed by the action server\n\
uint8 ACTIVE          = 1   # The goal is currently being processed by the action server\n\
uint8 PREEMPTED       = 2   # The goal received a cancel request after it started executing\n\
                            #   and has since completed its execution (Terminal State)\n\
uint8 SUCCEEDED       = 3   # The goal was achieved successfully by the action server (Terminal State)\n\
uint8 ABORTED         = 4   # The goal was aborted during execution by the action server due\n\
                            #    to some failure (Terminal State)\n\
uint8 REJECTED        = 5   # The goal was rejected by the action server without being processed,\n\
                            #    because the goal was unattainable or invalid (Terminal State)\n\
uint8 PREEMPTING      = 6   # The goal received a cancel request after it started executing\n\
                            #    and has not yet completed execution\n\
uint8 RECALLING       = 7   # The goal received a cancel request before it started executing,\n\
                            #    but the action server has not yet confirmed that the goal is canceled\n\
uint8 RECALLED        = 8   # The goal received a cancel request before it started executing\n\
                            #    and was successfully cancelled (Terminal State)\n\
uint8 LOST            = 9   # An action client can determine that a goal is LOST. This should not be\n\
                            #    sent over the wire by an action server\n\
\n\
#Allow for the user to associate a string with GoalStatus for debugging\n\
string text\n\
\n\
\n\
================================================================================\n\
MSG: tf2_web_republisher/TFSubscriptionResult\n\
# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
# result\n\
\n\
================================================================================\n\
MSG: tf2_web_republisher/TFSubscriptionActionFeedback\n\
# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
\n\
Header header\n\
actionlib_msgs/GoalStatus status\n\
TFSubscriptionFeedback feedback\n\
\n\
================================================================================\n\
MSG: tf2_web_republisher/TFSubscriptionFeedback\n\
# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
# feedback\n\
geometry_msgs/TransformStamped[] transforms\n\
\n\
\n\
================================================================================\n\
MSG: geometry_msgs/TransformStamped\n\
# This expresses a transform from coordinate frame header.frame_id\n\
# to the coordinate frame child_frame_id\n\
#\n\
# This message is mostly used by the \n\
# <a href=\"http://wiki.ros.org/tf\">tf</a> package. \n\
# See its documentation for more information.\n\
\n\
Header header\n\
string child_frame_id # the frame id of the child frame\n\
Transform transform\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Transform\n\
# This represents the transform between two coordinate frames in free space.\n\
\n\
Vector3 translation\n\
Quaternion rotation\n\
\n\
================================================================================\n\
MSG: geometry_msgs/Vector3\n\
# This represents a vector in free space. \n\
# It is only meant to represent a direction. Therefore, it does not\n\
# make sense to apply a translation to it (e.g., when applying a \n\
# generic rigid transformation to a Vector3, tf2 will only apply the\n\
# rotation). If you want your data to be translatable too, use the\n\
# geometry_msgs/Point message instead.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
================================================================================\n\
MSG: geometry_msgs/Quaternion\n\
# This represents an orientation in free space in quaternion form.\n\
\n\
float64 x\n\
float64 y\n\
float64 z\n\
float64 w\n\
";
  }

  static const char* value(const ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.action_goal);
      stream.next(m.action_result);
      stream.next(m.action_feedback);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct TFSubscriptionAction_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::tf2_web_republisher::TFSubscriptionAction_<ContainerAllocator>& v)
  {
    s << indent << "action_goal: ";
    s << std::endl;
    Printer< ::tf2_web_republisher::TFSubscriptionActionGoal_<ContainerAllocator> >::stream(s, indent + "  ", v.action_goal);
    s << indent << "action_result: ";
    s << std::endl;
    Printer< ::tf2_web_republisher::TFSubscriptionActionResult_<ContainerAllocator> >::stream(s, indent + "  ", v.action_result);
    s << indent << "action_feedback: ";
    s << std::endl;
    Printer< ::tf2_web_republisher::TFSubscriptionActionFeedback_<ContainerAllocator> >::stream(s, indent + "  ", v.action_feedback);
  }
};

} // namespace message_operations
} // namespace ros

#endif // TF2_WEB_REPUBLISHER_MESSAGE_TFSUBSCRIPTIONACTION_H
