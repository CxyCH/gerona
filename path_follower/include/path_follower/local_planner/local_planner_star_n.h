#ifndef LOCAL_PLANNER_STAR_N_H
#define LOCAL_PLANNER_STAR_N_H

/// PROJECT
#include <path_follower/local_planner/local_planner_star.h>

class LocalPlannerStarN : virtual public LocalPlannerStar
{
public:
    LocalPlannerStarN(RobotController& controller, PoseTracker& pose_tracker,
                      const ros::Duration &update_interval);
private:
    virtual double f(double& g, double& score, double& heuristic) override;
};

#endif // LOCAL_PLANNER_STAR_N_H
