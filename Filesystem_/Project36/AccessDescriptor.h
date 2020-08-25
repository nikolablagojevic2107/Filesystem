#pragma once
#include <string>
#include <vector>
#include <memory>
typedef std::string Text;
class OperationFailedException;
class AccessException;
class FSObject;
class File;
class Folder;
class AccessDescriptor {
	std::vector<Text> allowedOperations_;
	FSObject* FSObject_;
public:
	void add(Text operationName);
	void remove(Text operationName);
	std::vector<Text> getAllowedOperations();
	bool checkAccess(Text operationName);
	AccessDescriptor(FSObject*);
	
};

class FSOperation 
{
	protected:
	Text opName;
public:
	virtual void execute() = 0;
	Text getName();
};
class ReadFile : public FSOperation 
{ 
File* file_;
std::vector<bool> data;
public:
	ReadFile(File&);
	std::vector<bool>&getData();
	void execute(); 
};
class CreateFile : public FSOperation   
{
	Folder* parent;
	File* file;
	Text fileName_;
public:
	CreateFile(Folder*,Text fileName);
	void execute(); 
	File* getFile();
};
class CreateFolder:  public FSOperation 
{ 
	Folder*parent;
	Text folderName_;
	Folder* f;
public:
	void execute(); 
	CreateFolder(Folder*, Text folderName);
	Folder* getFolder();
};
class ListDirectory: public FSOperation 
{ 
	Folder* parent;
	std::vector<FSObject*>* list;
public:
	ListDirectory(Folder*);
	void execute(); 
	std::vector<FSObject*>& getObjects();
};
class WriteFile:  public FSOperation 
{
	File* file_;
	std::vector<bool> content_;
public:
	WriteFile(File *f,std::vector<bool>content);
	void execute(); 
};
class DeleteObject:  public FSOperation 
{ 
	FSObject* object_;
public:
	void execute(); 
	DeleteObject(FSObject*);
};
class CopyPaste:public FSOperation
{ 
	FSObject* object_;
	Folder *parent;
	Text newName;
public:
	CopyPaste(FSObject *,Folder *,Text);
	void execute(); 
};
class Search:public FSOperation 
{ 
	Folder* parent;
	std::vector<FSObject*> objects;
	Text filename_;
public:
	void execute();
	Search(Folder*,Text filename);
	std::vector<FSObject*> getObjects();
};
class ProtectedOperation : public FSOperation {
protected:
	bool checkPrecondition()throw(AccessException);
	FSObject* object1=nullptr;
	FSObject* object2=nullptr;
	Text sameName;
public:
	FSOperation* wrappedOperation=nullptr;
	void execute();
	void setObject1(FSObject*);
	void setObject2(FSObject*);
	void setText(Text t);
};


