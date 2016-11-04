/// HEADER
#include <path_follower/local_planner/local_planner_astar.h>

/// PROJECT


LocalPlannerAStar::LocalPlannerAStar(RobotController &controller, PoseTracker &pose_tracker,
                                     const ros::Duration &update_interval)
    : LocalPlannerClassic(controller,pose_tracker,update_interval),
      LocalPlannerStar(controller,pose_tracker,update_interval)
{

}

double LocalPlannerAStar::G(LNode*& current, LNode*& succ,
                            const std::vector<Constraint::Ptr>& constraints,
                            const std::vector<Scorer::Ptr>& scorer,
                            const std::vector<bool>& fconstraints,
                            const std::vector<double>& wscorer,
                            double& score){
    (void) constraints;
    (void) fconstraints;
    double tentative_gScore = current->gScore_ ;
    if(succ->twin_ != nullptr){
        tentative_gScore += Cost(*(succ->twin_), scorer, wscorer, score);
    }else{
        tentative_gScore += Cost(*(succ), scorer, wscorer, score);
    }
    return tentative_gScore;
}

void LocalPlannerAStar::updateSucc(LNode *&current, LNode *&f_current, LNode &succ){
    (void) current;
    (void) f_current;
    (void) succ;
}
