#include "FSObject.h"
#include "AccessDescriptor.h"
#include "FSVisistor.h"

//FSObject
Text FSObject::getName() { return name; }
AccessDescriptor* FSObject::getAccessDescriptor() { return AccessDescriptor_; }
FSObject::FSObject(Text t, Folder* parent):name(t),parent_(parent),AccessDescriptor_(new AccessDescriptor(this)) {}
FSObject::~FSObject() { delete AccessDescriptor_;if (parent_) parent_->remove(this); };
void FSObject::setName(Text t) { name = t; };
//File
std::vector <bool> File::read() { return content_; };
void File::write(std::vector<bool>content) { for (unsigned i = 0;i < content.size();i++) content_.push_back(content[i]); }
void File::accept(FSVisitor& v) { v.visitFile(this); }
File::File(Text t,Folder *f) : FSObject(t,f) {}
FSObject*File::copy() { return (new File(*this)); };
long File::size() { return content_.size(); };
File::File(File& f):FSObject(f.getName())
{
	parent_ = f.parent_;
	for(unsigned i = 0;i < f.content_.size();i++)
	content_.push_back(f.content_[i]);
};
File::~File(){}
//Folder
void Folder::remove(FSObject* object) 
{
	for (unsigned i = 0;i < containedObjects_.size();i++)
	{
		if (containedObjects_[i] == object)
		containedObjects_.erase(containedObjects_.begin()+i);
    }
};
void Folder::add(FSObject* object) 
{
	containedObjects_.push_back(object);
};
void Folder::accept(FSVisitor&v) 
{
	v.visitFolder(this);
};
Folder::Folder(Text t,Folder *f) : FSObject(t,f) 
{
}
std::vector<FSObject*>& Folder::getObjects() { return containedObjects_; };
FSObject* Folder::copy() 
{
	Folder *f = new Folder(name);
	f->parent_ = parent_;
	CopyVisitor v;
	for(unsigned int i = 0;i < containedObjects_.size();i++)
		containedObjects_[i]->accept(v);
	for (unsigned int i = 0;i < v.getFiles().size();i++)
	{
		FSObject* copiedFile = v.getFiles()[i]->copy();
		f->containedObjects_.push_back(copiedFile);
		copiedFile->parent_ = f;
	}
	for (unsigned int i = 0;i < v.getFolders().size();i++)
	{
		FSObject* copiedFolder = v.getFolders()[i]->copy();
		f->containedObjects_.push_back(copiedFolder);
		copiedFolder->parent_ = f;
	}
	return f;
};
long Folder::size() 
{
	long size = 0;
	for (unsigned int i = 0;i < containedObjects_.size();i++)
	{
		size += containedObjects_[i]->size();
	}
	return size;
};
Folder::~Folder()
{
	for (unsigned int i = 0;i < containedObjects_.size();i++)
	{
		delete containedObjects_[i];
	}
}

