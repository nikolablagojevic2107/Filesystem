#pragma once
#include <string>
#include <vector>
#include <memory>
typedef std::string Text;
class FSVisitor;
class AccessDescriptor;
class Folder;
class FSObject {
	protected:
	AccessDescriptor* AccessDescriptor_;
	Text name;
public:
	Folder* parent_;
	virtual void accept(FSVisitor&)=0;
	Text getName();
	void setName(Text);
	//~FSObject();
	virtual FSObject* copy()=0;
	virtual long size() = 0;
	AccessDescriptor* getAccessDescriptor();
	FSObject(Text,Folder *parent=nullptr);
	friend int main();
	virtual ~FSObject();
};
class File:public FSObject {
	std::vector <bool> content_;
public:
	std::vector <bool> read();
	void write(std::vector<bool>content);
	void accept(FSVisitor&);
	File(Text,Folder *f=nullptr);
	FSObject* copy();
	long size();
	File(File&);
	friend int main();
	~File();
};
class Folder :public FSObject {
	std::vector<FSObject*> containedObjects_;
public:
	std::vector<FSObject*>& getObjects(); 
	void remove(FSObject*);
	void add(FSObject*);
	void accept(FSVisitor&);
	Folder(Text,Folder*f=nullptr);
	FSObject* copy();
	long size();
	friend int main();
	~Folder();
};