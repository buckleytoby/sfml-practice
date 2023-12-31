#include "taskmanager.hpp"


const std::string to_string(ActionTypes action_type_){
    switch (action_type_){
        case ActionTypes::Default: return "Default";
        case ActionTypes::SelectClosestBuilding: return "Select Closest Building";
        case ActionTypes::SetGoalToSelectedBuilding: return "Set Goal To Selected Building";
        case ActionTypes::MoveTowardsGoal: return "Move Towards Goal";
        case ActionTypes::TransferInventory: return "Transfer Inventory";
        case ActionTypes::SetGoalToBuilding: return "Set Goal To Building";
        case ActionTypes::TakeInventory: return "Take Inventory";
        case ActionTypes::end: return "end";
        default: return "UNKNOWN";
    }
}
////////////////////////////// Action //////////////////////////////
void Action::update(bool is_complete){
    completed_ = is_complete;
}
SetGoalToBuilding::SetGoalToBuilding(int building_id){
    universal_ids_.push_back(building_id);
}
void SetGoalToBuilding::update(bool is_complete){
    // get building ptr from id
    // auto building_ptr = GetBuildingPtrFromId(universal_ids_[0]);
    // // set goal to building center
    // SetGoal(building_ptr->GetCenter().x_, building_ptr->GetCenter().y_);
}
////////////////////////////// End Action //////////////////////////////
////////////////////////////// Task //////////////////////////////
void Task::update(bool is_complete){
    // update the active action, recursion if action is a task
    actions_[active_action_nb_]->update(is_complete);
    // test success criteria
    if(actions_[active_action_nb_]->IsComplete()){
        IncrementActiveAction();
    }
}
bool Task::IsComplete(){
    // no actions -> complete
    // all actions complete -> complete
    // else, not complete
    bool complete = true;
    for (auto action: actions_){
        complete = complete && action->IsComplete();
    }
    return complete;
}
ActionTypes Task::GetActiveActionType(){
    // recurse
    return actions_[active_action_nb_]->GetActiveActionType();
}
void Task::IncrementActiveAction(){
    prior_action_ = active_action_nb_;
    active_action_nb_ = (active_action_nb_ + 1) % actions_.size();
}
std::vector<std::string> Task::GetActionNames(){
    std::vector<std::string> action_names;
    for (auto action: actions_){
        action_names.push_back(action->GetName());
    }
    return action_names;
}
void Task::NewAndAddAction(ActionTypes type){
    auto action = std::make_shared<Action>(type);
    AddAction(action);
}
TaskPtr Task::copy(){
    TaskPtr task_ptr = std::make_shared<Task>(name_);
    for (auto action: actions_){
        task_ptr->AddAction(action);
    }
    return task_ptr;
}
////////////////////////////// End Task //////////////////////////////
////////////////////////////// TaskManager //////////////////////////////
std::vector<std::string> TaskManager::GetTaskNames(){
    std::vector<std::string> task_names;
    for (auto task: tasks_){
        task_names.push_back(task->GetName());
    }
    return task_names;
}
std::vector<std::string> TaskManager::GetActiveTaskActionNames(){
    if (active_task_ == nullptr){
        return {};
    }
    return active_task_->GetActionNames();
}
void TaskManager::NewTask(){
    int idx = tasks_.size();
    std::string name = "New Task " + std::to_string(idx);
    tasks_.push_back(std::make_shared<Task>(name));
    active_task_ = tasks_.back();
}
void TaskManager::RemoveTask(int idx){
    // value protection
    if (idx < 0 || idx >= tasks_.size()){
        return;
    }
    tasks_.erase(tasks_.begin() + idx);
}
void TaskManager::EditTask(int idx){
    // value protection
    if (idx < 0 || idx >= tasks_.size()){
        return;
    }
    active_task_ = tasks_[idx];
}
std::vector<std::string> TaskManager::GetActionsAndTasks(){
    std::vector<std::string> out;

    // iterate through all ActionTypes enum
    for (int i=0; i<static_cast<int>(ActionTypes::end); i++){
        out.push_back(to_string(static_cast<ActionTypes>(i)));
    }

    // iterate through all custom tasks
    for (auto& taskptr: tasks_){
        out.push_back(taskptr->GetName());
    }
    return out;
}
TaskPtr TaskManager::GetTask(int idx){
    // value protection
    if (idx < 0 || idx >= tasks_.size()){
        return nullptr;
    }
    return tasks_[idx]->copy();
}
////////////////////////////// End TaskManager //////////////////////////////