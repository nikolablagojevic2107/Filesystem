#include "FSObject.h"
#include "Filesystem.h"
#include "Exception.h"
#include <iostream>
void printData(std::vector<bool>data)
{
	for (int i =0;i<data.size()-1;i++)
	{
		std::cout << data[i] << "|";
	}
	std::cout << data[data.size()-1]<<"\n";
}
void printObjects(std::vector<FSObject*>objects) 
{
	for (unsigned i = 0;i < objects.size();i++)
	{
		std::cout << objects[i]->getName() << "(";
		if(objects[i]->parent_) std::cout << objects[i]->parent_->getName() << ") ";
	}
	std::cout << "\n";
}

int main() 
{
	/*Folder Tests 
/*	Folder f("root");
	Folder f1("f1");
	Folder f2("f2");
	Folder f3("f3");
	f.add(&f1);
	f.add(&f2);
	f.add(&f3);
	File file_("a file");
	file_.write({ 1,1,1,0 });
	File file_2("a file");
	file_2.write({ 1,1,1,0 });
	f2.add(&file_);
	Folder* fi = static_cast<Folder*>(f.copy());
	std::cout << fi->getObjects()[1]->getName();
	f.add(&file_2);
	std::cout << "\n" << f.size();
	std::cout << "\n" << fi->size() << "\n";
	*/
	/* Test1 in Filesystem*/
	/*
	Folder* f = Filesystem::getInstance()->rootFolder;
	Filesystem::getInstance()->grantAccess(f,"read");
	File* file;
	Filesystem::getInstance()->grantAccess(Filesystem::getInstance()->createFolder("Osylion", f), "read");
	file = Filesystem::getInstance()->createFile("f11", f);
	Filesystem::getInstance()->writeFile({ 1,0,1,1 },file);
	f = Filesystem::getInstance()->openFolder("/rootFolder/Osylion");
	file = Filesystem::getInstance()->createFile("f5", f);
	Filesystem::getInstance()->writeFile({ 1,0,1 }, file);
	std::cout << Filesystem::getInstance()->openFolder("/rootFolder/Osylion")->size()<<"\n";
	std::cout << Filesystem::getInstance()->rootFolder->size();
	*/
	/*Filesystem* system = Filesystem::getInstance();
	Folder* f1 = system->createFolder("Osylion", system->rootFolder);
	Folder* f2 = system->createFolder("Subutai", system->rootFolder);
	Folder* f3 = system->createFolder("f3",f1);
	Folder* f4 = system->createFolder("f4",f1);
	Folder* f5 = system->createFolder("f5",f1);
	File* file1 = system->createFile("file1", system->rootFolder);
	File* file2 = system->createFile("file2",f1);
	File* file3 = system->createFile("file3",f3);
	File* file4 = system->createFile("file4", f2);
	File* file5 = system->createFile("file5",f2);
	File* file6 = system->createFile("file6", f2);
	std::cout << system->diskSize << "\n";
	system->writeFile({1},file1);
	system->writeFile({1,0},file2);
	system->writeFile({1,1},file3);
	system->writeFile({1,0,0},file4);
	system->writeFile({1,0,1},file5);
	system->writeFile({1,1,0},file6);
	std::cout << system->diskSize << "\n";*/
	/*Folder *ff = system->openFolder("/rootFolder/Osylion/f3");
	std::cout << ff->getName() << "\n";
	printData(system->readFile(file1));
	printData(system->readFile(file2));
	printData(system->readFile(file3));
	printData(system->readFile(file4));
	printData(system->readFile(file5));
	printData(system->readFile(file6));*/
	/*system->createFolder("Sephhiroth", system->rootFolder);
	system->createFolder("Sephhiroth", f5);
	system->createFolder("Sephhiroth", f1);*/
	//printObjects(system->search("Sephhiroth"));
	//printObjects(f5->getObjects());
	//std::cout << f5->getObjects().size();
	//Folder* ff = system->openFolder("/rootFolder/Osylion/f5");
	//system->move(f2, f1);
	//system->Delete(f1);
	/*system->copyPaste(f1, system->rootFolder, "Ossir");
	std::cout << system->diskSize << "\n";
	system->Delete(system->rootFolder);
	std::cout << system->diskSize << "\n";*/
	/*printObjects(system->rootFolder->getObjects());
	printObjects(f1->getObjects());
	printObjects(f2->getObjects());*/
	/*system->Delete(f1);*/
	/*Folder* f = system->openFolder("/rootFolder/Ossir");
	printObjects(f->getObjects());*/
	/*printObjects(f3->getObjects());
	printObjects(f4->getObjects());
	printObjects(f5->getObjects());*/

	//Final TEST
	Filesystem* filesystem = Filesystem::getInstance();
	try
	{
		//Creating Folder
		Folder* root=filesystem->rootFolder;
		Folder *folder1 = filesystem->createFolder("Folder1", root);
		//filesystem->revokeAccess(root, "write"); //AccessException
		//filesystem->createFolder("Folder1", root); //NameCollision
		Folder *folder2 = filesystem->createFolder("Folder2", root);
		
		//Creating File 
		File * f1=filesystem->createFile("File1", root);
		//filesystem->revokeAccess(root,"write");//AccessException
		//filesystem->createFile("File1",root );//Name Collision
		File * F1 =filesystem->createFile("File1", folder1);

		//Listing Folders
		//filesystem->revokeAccess(folder1, "read"); //AccessException
		printObjects(filesystem->listFolder(root));
		printObjects(filesystem->listFolder(folder1));

		//Writing&reading Data in File
		std::cout << "Current system space: " << filesystem->freeSpace()<<"\n";
		filesystem->writeFile({0,1,1},f1);
		// filesystem->revokeAccess(f1,"read"); //AccessException
		printData(filesystem->readFile(f1));
		std::cout << "Current system space: " << filesystem->freeSpace() << "\n";
		//filesystem->writeFile({ 1,0,1 }, f1); //WriteFailedException
		//filesystem->revokeAccess(f1,"write"); //AccessException
		//filesystem->grantAccess(f1, "write"); //Remvoves AccessException
		filesystem->writeFile({ 1 }, f1);
		std::cout << "Current system space: " << filesystem->freeSpace() << "\n";

		//Open Folder based on path
		filesystem->createFolder("Folder3", folder1);
		//Folder* folder3 = filesystem->openFolder("/rootFolder/Folder1/Folder"); //ObjectNotFound
		//filesystem->revokeAccess(root, "read"); //AccessException during lookup for folder path
		Folder* folder3 = filesystem->openFolder("/rootFolder/Folder1/Folder3");
		std::cout << folder3->getName()<<"\n";
		filesystem->createFolder("Folder3", folder2);

		//Searching folder by name
		printObjects(filesystem->search("Folder3"));
		filesystem->revokeAccess(folder2,"read"); // if search can't read folder/file it ignores it
		printObjects(filesystem->search("Folder3"));
		filesystem->grantAccess(folder2, "read");
		printObjects(filesystem->search("Folder3"));

		//CopyPaste&delete
		/*filesystem->createFolder("rootFolder", folder2);         //after this set diskSpace to 10                                     
		filesystem->revokeAccess(root, "read");                    // We can't read the content of a folder to copy
		filesystem->revokeAccess(folder2,"write");                 // We can't write in the destination folder
		filesystem->copyPaste(root, folder2, "rootFolder");*/      // Name collision + not enough memory
	
		std::cout << "Current system space: " << filesystem->freeSpace() << "\n";
		filesystem->copyPaste(root, folder2, "rootFolder2"); 
		printObjects(filesystem->listFolder(root));
		printObjects(filesystem->listFolder(folder1));
		printObjects(filesystem->listFolder(folder2));
		printObjects(filesystem->listFolder(filesystem->openFolder("/rootFolder/Folder2/rootFolder2")));

		//delete 
		std::cout << "Current system space: " << filesystem->freeSpace() << "\n";
		filesystem->Delete(root);
		std::cout << "Current system space: " << filesystem->freeSpace() << "\n";

	}
	catch (AccessException& e_)
	{
		FSException* e = &e_;
		std::cout << e->what();
		while (e->getCause())
		{
			e = e->getCause();
			std::cout << e->what();
		}
	}
	catch (WriteFailedException& e_)
	{
		FSException* e = &e_;
		std::cout << e->what();
		while (e->getCause())
		{
			e = e->getCause();
			std::cout << e->what();
		}
	}
	catch (NameCollisionException &e_)
	{
		FSException* e = &e_;
		std::cout << e->what();
		while (e->getCause())
		{
			e = e->getCause();
			std::cout << e->what();
		}
	}
	catch (ObjectNotFoundException& e_)
	{
		FSException* e = &e_;
		std::cout << e->what();
		while (e->getCause())
		{
			e = e->getCause();
			std::cout << e->what();
		}
	}
	catch (...)
	{
		std::cout << "Uknown Error";
	}
}