#ifndef TASKID
  #define TASKID 0
  
#include <iostream>
#include <fstream>
typedef unsigned int task_id;

class TaskId
{
public:
  static TaskId& getInstance();
  task_id getTaskId();
  
protected:
  TaskId();
  ~TaskId();
  TaskId(const TaskId&);
  TaskId& operator=(const TaskId&);

private:  
	task_id lastTaskId;
};

#endif
