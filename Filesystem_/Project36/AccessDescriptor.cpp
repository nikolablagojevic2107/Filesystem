#include "AccessDescriptor.h"
#include "FSObject.h"
#include "FSVisistor.h"
#include "Exception.h"
#include "Filesystem.h"

AccessDescriptor::AccessDescriptor(FSObject* object):FSObject_(object) 
{
	allowedOperations_.push_back("read");
	allowedOperations_.push_back("write");
}
void AccessDescriptor::add(Text operationName) 
{
	bool isAlreadyThere = false;
	for(unsigned i =0;i<allowedOperations_.size();i++)
	{
		if (allowedOperations_[i] == operationName)
			isAlreadyThere = true;
	}
	if(!isAlreadyThere)allowedOperations_.push_back(operationName);

};
void AccessDescriptor::remove(Text operationName) 
{
	for (unsigned i = 0;i < allowedOperations_.size();i++)
	{
		if (allowedOperations_[i] == operationName)
		{
			allowedOperations_.erase(allowedOperations_.begin() + i);
			break;
		}
		
	}
};
std::vector<Text> AccessDescriptor::getAllowedOperations() { return allowedOperations_; };
bool AccessDescriptor::checkAccess(Text operationName)
{
	for (unsigned i = 0;i < allowedOperations_.size();i++)
	{
		if (allowedOperations_[i] == operationName)
		{
			return 1;
		}
	}
	return 0;
};
Text FSOperation::getName() { return opName; };
void ReadFile::execute()
{
	data=file_->read();
};
ReadFile::ReadFile(File&f)
{
	opName = "ReadFile";
	file_ = static_cast<File*>(f.copy());
}
std::vector<bool>&ReadFile::getData() 
{
	return data;
};
CreateFile::CreateFile(Folder*f,Text fileName):parent(f)
{
	opName = "CreateFile";
	fileName_ = fileName;
};
void CreateFile::execute() 
{
	file = new File(fileName_,parent);
	parent->add(file);
};
File* CreateFile::getFile()
{
	return file;
}
CreateFolder::CreateFolder(Folder*f, Text folderName):parent(f)
{
	opName = "CreateFolder";
	folderName_ = folderName;
};
void CreateFolder::execute() 
{
	f = new Folder(folderName_,parent);
	parent->add(f);
};
Folder* CreateFolder::getFolder() { return f; }
ListDirectory::ListDirectory(Folder* f) :parent(f) { list = nullptr;opName = "ListDirectory"; };
void ListDirectory::execute() 
{
	list = &parent->getObjects();
};
std::vector<FSObject*>& ListDirectory::getObjects() { return *list; };

WriteFile::WriteFile(File* f, std::vector<bool>content) :file_(f),content_(content)
{
	opName = "WriteFile";
};
void WriteFile::execute() 
{
	file_->write(content_);
};
void DeleteObject::execute() 
{
	delete object_;
};
DeleteObject::DeleteObject(FSObject* object):object_(object) {};

CopyPaste::CopyPaste(FSObject* object, Folder* Parent,Text t) :parent(Parent),object_(object),newName(t)
{

	opName = "CopyPaste";
};
void CopyPaste::execute()
{
	FSObject* object_2 = object_->copy();
	object_2->setName(newName);
	object_2->parent_ = parent;
	parent->add(object_2);
};
void Search::execute() 
{
	SearchVisitor v(filename_);
	parent->accept(v);
	objects = v.getObjects();
};
Search::Search(Folder*f,Text filename) 
{
	parent = f;
	filename_ = filename;
};
std::vector<FSObject*> Search::getObjects() { return objects; };
bool IsThereAFolderWithSameName(FSObject *folder,Text name)
{
	Folder* destFolder = static_cast<Folder*>(folder);
	for(int i = 0;i < destFolder->getObjects().size();i++)
	if(destFolder->getObjects()[i]->getName() == name)
	return true;	
	return false;
}
char* ConvertToChar(std::string s)
{
	char* c = new char[s.size() + 1];
	std::copy(s.begin(), s.end(), c);
	c[s.size()] = '\0';
	return c;
}
bool ProtectedOperation::checkPrecondition() 
{
	if (wrappedOperation->getName() == "ReadFile")
	{
		if (!object1->getAccessDescriptor()->checkAccess("read")) 
		{
			std::string s = ("cannot Read "+object1->getName()+"\n");
			throw *new AccessException(ConvertToChar(s));
		}
	}
	if (wrappedOperation->getName() == "WriteFile")
	{
		if (!object1->getAccessDescriptor()->checkAccess("write"))
		{
			std::string s = ("cannot Write " + object1->getName() + "\n");
			throw *new AccessException(ConvertToChar(s));
		}
	}
	if (wrappedOperation->getName() == "CreateFile")
	{
		if(!object1->getAccessDescriptor()->checkAccess("write"))
		{
			std::string s("cannot Write " + object1->getName() + "\n");
			char* c = ConvertToChar(s);
			if (IsThereAFolderWithSameName(object1, sameName))
			{
	AccessException* a=new AccessException(c,*new NameCollisionException(ConvertToChar("File already exists\n")));
	throw *a;
			}
			else
			{
				AccessException* a=new AccessException(c);
				throw *a;
			}
		}
		if (IsThereAFolderWithSameName(object1, sameName))
		throw* new NameCollisionException(ConvertToChar("File already exists\n"));
	}
	if (wrappedOperation->getName() == "CreateFolder")
	{
		if (!object1->getAccessDescriptor()->checkAccess("write"))
		{
			std::string s("cannot Write " + object1->getName() + "\n");
			char* c = ConvertToChar(s);
			if (IsThereAFolderWithSameName(object1, sameName))
			{
	AccessException* a=new AccessException(c, *new NameCollisionException(ConvertToChar("Folder already exists\n")));
	throw *a;
			}
			else
			{
				AccessException *a=new AccessException(c);
				throw *a;
			}
		}
		if (IsThereAFolderWithSameName(object1, sameName))
		throw * new NameCollisionException(ConvertToChar("Folder already exists\n"));
		}

	if (wrappedOperation->getName() == "ListDirectory")
	{
		if (!object1->getAccessDescriptor()->checkAccess("read"))
		{
			std::string s("cannot Read " + object1->getName() + "\n");
			throw *new AccessException(ConvertToChar(s));
		}
	}
	if (wrappedOperation->getName() == "DeleteObject")
	{
		if (!object1->getAccessDescriptor()->checkAccess("write"))
		{
			std::string s("cannot Write " + object1->getName() + "\n");
			throw *new AccessException(ConvertToChar(s));
		}
	}
	if (wrappedOperation->getName() == "CopyPaste")
	{
		AccessException* a1 = nullptr;
		AccessException* a2 = nullptr;
		NameCollisionException* nc = nullptr;
		WriteFailedException* wf = nullptr;
		if (Filesystem::getInstance()->freeSpace() < object1->size())
			wf = new WriteFailedException(ConvertToChar("Not enough memory\n"));

		if (IsThereAFolderWithSameName(object2, sameName))
			nc = new NameCollisionException(ConvertToChar("file with that name already exists \n"));

		if (!object1->getAccessDescriptor()->checkAccess("read"))
		{
			std::string s("cannot Read " + object1->getName() + "\n");
			a1 = new AccessException(ConvertToChar(s));
		}
		if (!object2->getAccessDescriptor()->checkAccess("write"))
		{
			std::string s("cannot Write " + object2->getName() + "\n");
			a2 = new AccessException(ConvertToChar(s), *a1);
		}
		if (nc) nc->setCause(wf);
		if (a2) { if (nc) { a2->setCause(nc); } else { a2->setCause(wf); } }
		if (a1) { if (a2) { a1->setCause(a2); } else { if(nc) a1->setCause(nc);else { a1->setCause(wf); } } }
		if (a1) throw* a1;
		if (a2) throw* a2;
		if (nc) throw* nc;
		if (wf) throw* wf;

	}
	return true;
};

void ProtectedOperation::execute() 
{
	if (checkPrecondition()) wrappedOperation->execute();
};
void ProtectedOperation::setObject1(FSObject* obj1) { object1 = obj1; };
void ProtectedOperation::setObject2(FSObject* obj2) { object2 = obj2; };
void ProtectedOperation::setText(Text t) { sameName = t; };