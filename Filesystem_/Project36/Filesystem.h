#pragma once
#include <string>
#include <vector>
typedef std::string Text;
class FSObject;
class File;
class Folder;
class ProtectedOperation;
class NameCollisionException;
class AccessException;
class ObjectNotFoundException;
class WriteFailedException;

class Filesystem {
public:
	Folder* rootFolder;
	int diskSize;
public:
	    File* createFile(Text fName,Folder *destFolder) throw(NameCollisionException, AccessException);
		Folder* createFolder(Text fName,Folder *parentFolder)  throw(NameCollisionException, AccessException);
		std::vector<FSObject*>& listFolder(Folder*) throw(AccessException);
		std::vector<bool>readFile(File*) throw(AccessException);
		void writeFile(std::vector<bool>,File *) throw(AccessException, WriteFailedException);
		void grantAccess(FSObject*, Text opName);
		void revokeAccess(FSObject*, Text opName);
		Folder* openFolder(Text folderPath) throw(AccessException, ObjectNotFoundException);
		std::vector<FSObject*>search(Text objName);
		void copyPaste(FSObject* objToCopy, Folder* destFolder, Text newName)
		throw(AccessException, WriteFailedException, NameCollisionException);
		void move(FSObject* objToMove, Folder* destFolder) throw(AccessException, NameCollisionException);
		void Delete(FSObject* objToDelete) throw(AccessException);
		long freeSpace();
		static Filesystem* getInstance();
		~Filesystem();
private:
	Filesystem();
	Filesystem(const Filesystem &) {}
	Filesystem(Filesystem &&){}
	Filesystem operator=(const Filesystem&) {}
	Filesystem operator=(Filesystem&&) {}
	static Filesystem* Instance;
	static ProtectedOperation* operation;
};