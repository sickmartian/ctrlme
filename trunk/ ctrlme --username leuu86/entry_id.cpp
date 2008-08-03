#include "entry_id.h"

using namespace std;

EntryId &EntryId::getInstance() {
  static EntryId inst;
  return inst;
}

EntryId::EntryId() {
  ifstream saved( "lastentry.dat", ios::in );
  if (!saved) {
    lastEntryId = 0;
  } else {
    saved.read( reinterpret_cast< char * >( &lastEntryId ), sizeof(entry_id) );
  }
  saved.close();
}

EntryId::~EntryId() {
  ofstream save( "lastentry.dat", ios::out );
  if (save) {
    save.write( reinterpret_cast< char * >( &lastEntryId), sizeof(entry_id) );
  }
  save.close();
}

entry_id EntryId::getEntryId() {
	return ++lastEntryId;
}
