#ifndef ENTRYID
  #define ENTRYID 0

#include <iostream>
#include <fstream>
typedef unsigned int entry_id;

class EntryId
{
public:
  static EntryId& getInstance();
  entry_id getEntryId();

protected:
  EntryId();
  ~EntryId();
  EntryId(const EntryId&);
  EntryId& operator=(const EntryId&);

private:
	entry_id lastEntryId;
};

#endif
