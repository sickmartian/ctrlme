#include "taskid.h"

using namespace std;

TaskId &TaskId::getInstance() {
  static TaskId inst;
  return inst;
}

TaskId::TaskId() {
  ifstream saved( "lastid.dat", ios::in );
  if (!saved) {
    lastTaskId = 1;
  } else {
    saved.read( reinterpret_cast< char * >( &lastTaskId ), sizeof(task_id) );
  }
  saved.close();
}

TaskId::~TaskId() {
  ofstream save( "lastid.dat", ios::out );
  if (save) {
    save.write( reinterpret_cast< char * >( &lastTaskId), sizeof(task_id) );
  }
  save.close();
}

task_id TaskId::getTaskId() {
	return ++lastTaskId;
}
