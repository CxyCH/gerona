#ifndef ROBOTCONTROLLER_OMNIDRIVE_PID_H
#define ROBOTCONTROLLER_OMNIDRIVE_PID_H

/// THIRD PARTY
#include <Eigen/Core>

/// PROJECT
#include "robotcontroller.h"
#include "multiplepidwrapper.h"
#include "visualizer.h"
#include "obstacledetectoromnidrive.h"

class RobotController_Omnidrive_Pid : public RobotController
{
public:
    RobotController_Omnidrive_Pid(ros::Publisher &cmd_publisher, BehaviouralPathDriver *path_driver);

    virtual void publishCommand();

    //! Immediatley stop any motion.
    virtual void stopMotion();

    virtual void initOnLine();

    virtual ObstacleDetector* getObstacleDetector()
    {
        return &obstacle_detector_;
    }


protected:
    virtual void behaveOnLine();

    virtual void behaveAvoidObstacle();

    /**
     * @return True, when turning point is reached, otherwise false.
     */
    virtual bool behaveApproachTurningPoint();


private:
    struct Command
    {
        // was wird hier gebraucht? Wahrscheinlich 2d-vektor für velocity und scalar für rotation?!
        // -> jupp, vektor aber besser in polarkoordinaten

        //! Speed of the movement.
        float speed;
        //! Direction of movement as angle to the current robot orientation.
        float direction_angle;
        //! rotational velocity.
        float rotation;


        // initialize all values to zero
        Command():
            speed(0.0f), direction_angle(0.0f), rotation(0.0f)
        {}

        operator geometry_msgs::Twist()
        {
            geometry_msgs::Twist msg;
            msg.linear.x  = speed * cos(direction_angle);
            msg.linear.y  = speed * sin(direction_angle);
            msg.angular.z = rotation;
            return msg;
        }

        bool isValid()
        {
            if ( isnan(speed) || isinf(speed)
                 || isnan(direction_angle) || isinf(direction_angle)
                 || isnan(rotation) || isinf(rotation) )
            {
                ROS_FATAL("Non-numerical values in command: %d,%d,%d,%d,%d,%d",
                          isnan(speed), isinf(speed),
                          isnan(direction_angle), isinf(direction_angle),
                          isnan(rotation), isinf(rotation));
                // fix this instantly, to avoid further problems.
                speed = 0.0;
                direction_angle = 0.0;
                rotation = 0.0;

                return false;
            } else {
                return true;
            }
        }
    };

    struct ControllerOptions
    {
        double dead_time_;
    };

    enum PID
    {
        DIRECTION = 0, ORIENTATION = 1
    };

    Visualizer *visualizer_;

    MultiplePidWrapper pids_;


    Command cmd_;
    ControllerOptions options_;
    ObstacleDetectorOmnidrive obstacle_detector_;

    void configure();

    bool setCommand(double e_direction, double e_rotation, float speed);

    //! Predict the position of the robot.
    Eigen::Vector2d predictPosition();

    //! Check if approaching turning point is done.
    bool checkIfTurningPointApproached();

    /**
     * @brief Calculate distance of the robot to the next path segment.
     *
     * Note: This method calculates the distance to the *next* path segment, not the current one!
     */
    double calculateLineError();

    double calculateDirectionError();

    //! Calculate the distance of the robot to the next waypoint.
    double calculateDistanceToWaypoint();
};

#endif // ROBOTCONTROLLER_OMNIDRIVE_PID_H
