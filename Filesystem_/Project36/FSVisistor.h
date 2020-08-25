#pragma once
#include <string>
#include <vector>
class File;
class Folder;
typedef std::string Text;
class FSObject;
class FSVisitor {
public:
	virtual void visitFile(File*) = 0;
	virtual void visitFolder(Folder*) =0;
};
class SearchVisitor:public FSVisitor
{
	Text filename;
	std::vector<FSObject*>foundObjects;
public:
	void visitFile(File*);
	void visitFolder(Folder*);
	SearchVisitor(Text);
	std::vector<FSObject*>& getObjects();
};
class CopyVisitor :public FSVisitor 
{
	std::vector<Folder*>folders;
	std::vector<File*>files;
public:
	void visitFile(File*);
	void visitFolder(Folder*);
	std::vector<Folder*>&getFolders();
	std::vector<File*>&getFiles();
};