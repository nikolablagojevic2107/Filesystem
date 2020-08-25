#include "FSObject.h"
#include "AccessDescriptor.h"
#include "Exception.h"
#include "Filesystem.h"
char* ConvertToChar(std::string s);
File* Filesystem::createFile(Text fName, Folder* destFolder)  
{
	CreateFile * f = new CreateFile(destFolder, fName);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation =f;
	operation->setObject1(destFolder);
	operation->setText(fName);
	operation->execute();
	return f->getFile();
};
Folder* Filesystem::createFolder(Text fName, Folder* parentFolder)
{
	CreateFolder* f = new CreateFolder(parentFolder, fName);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation = f;
	operation->setObject1(parentFolder);
	operation->setText(fName);
	operation->execute();
	return f->getFolder();
};
std::vector<FSObject*>& Filesystem::listFolder(Folder*f) 
{
	ListDirectory* l = new ListDirectory(f);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation = l;
	operation->setObject1(f);
	operation->execute();
	return l->getObjects();
};
std::vector<bool>Filesystem::readFile(File*f) 
{
	ReadFile* r = new ReadFile(*f);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation = r;
	operation->setObject1(f);
	operation->execute();
	return r->getData();
};
void Filesystem::writeFile(std::vector<bool>content,File *file) 
{
	if (diskSize < content.size()) throw *new WriteFailedException(ConvertToChar("not enough space on disk \n"));
	WriteFile* r = new WriteFile(file,content);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation = r;
	operation->setObject1(file);
	operation->execute();
	diskSize -= content.size();
};
void Filesystem::grantAccess(FSObject* object, Text opName) { object->getAccessDescriptor()->add(opName); };
void Filesystem::revokeAccess(FSObject*object, Text opName){ object->getAccessDescriptor()->remove(opName); };
Folder* Filesystem::openFolder(Text folderPath)
{
	std::vector<Text>folders;
	Text word;
	for (unsigned int i = 1;i < folderPath.size();i++)
	{
		if (folderPath[i] == '/') { folders.push_back(word); word.clear();}else word.push_back(folderPath[i]);
	}
	folders.push_back(word);
	int j = 1;
	Folder* currentFolder = rootFolder;
	while (j<folders.size())
	{
		if (!currentFolder->getAccessDescriptor()->checkAccess("read"))
			throw *new AccessException(ConvertToChar("cannot read a certain folder in open folder operation\n"));
		Search s(currentFolder, folders[j]);
		s.execute();
		currentFolder = static_cast<Folder*>(s.getObjects()[0]);
		if (currentFolder == nullptr) 
		throw * new ObjectNotFoundException(ConvertToChar("Sorry we couldnt find the given folder"));
		j++;
	}
	return currentFolder;
};
std::vector<FSObject*>Filesystem::search(Text objName)
{
	Search s(rootFolder,objName);
	s.execute();
	return s.getObjects();
};
void Filesystem::copyPaste(FSObject* objToCopy, Folder* destFolder, Text newName) 
{
	CopyPaste*cp = new CopyPaste(objToCopy, destFolder,newName);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation = cp;
	operation->setObject1(objToCopy);
	operation->setObject2(destFolder);
	operation->setText(newName);
    int preSize = objToCopy->size();
	operation->execute();
	diskSize -= preSize;
};

void Filesystem::move(FSObject* objToMove, Folder* destFolder) 
{
	if (!destFolder->getAccessDescriptor()->checkAccess("write")) 
	{ throw *new AccessException(ConvertToChar("cannot move folder")); }
	objToMove->parent_->remove(objToMove);
	objToMove->parent_ = destFolder;
	destFolder->add(objToMove);
};
void Filesystem::Delete(FSObject* objToDelete) 
{
	diskSize += objToDelete->size();
	DeleteObject* d = new DeleteObject(objToDelete);
	if (operation->wrappedOperation) delete operation->wrappedOperation;
	operation->wrappedOperation = d;
	operation->setObject1(objToDelete);
	operation->execute();
};
long Filesystem::freeSpace() { return diskSize; };
Filesystem* Filesystem::getInstance() { if (Instance == nullptr) Instance = new Filesystem; return Instance; };
Filesystem::~Filesystem() { delete Instance;delete operation; };

Filesystem::Filesystem() { rootFolder = new Folder("rootFolder");diskSize = 10; };

ProtectedOperation* Filesystem::operation = new ProtectedOperation;
Filesystem* Filesystem::Instance = nullptr;
