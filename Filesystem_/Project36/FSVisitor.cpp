#include "FSVisistor.h"
#include "FSObject.h"
#include "AccessDescriptor.h"
void CopyVisitor::visitFile(File*file) 
{
	files.push_back(file);
};
void CopyVisitor::visitFolder(Folder*folder) 
{
	folders.push_back(folder);
};
std::vector<Folder*>& CopyVisitor::getFolders() 
{
	return folders;
};
std::vector<File*>& CopyVisitor::getFiles() 
{
	return files;
};

void SearchVisitor::visitFile(File*file) 
{
	if (file->getName() == filename) foundObjects.push_back(file);
};
void SearchVisitor::visitFolder(Folder*folder) 
{
	if (folder->getName() == filename) foundObjects.push_back(folder);
	for (unsigned i = 0;i < folder->getObjects().size();i++)
	{
		if(folder->getObjects()[i]->getAccessDescriptor()->checkAccess("read")) 
		{
			folder->getObjects()[i]->accept(*this);
		};
     }
};
SearchVisitor::SearchVisitor(Text t) 
{
	filename = t;
};
std::vector<FSObject*>& SearchVisitor::getObjects() 
{
	return foundObjects;
};