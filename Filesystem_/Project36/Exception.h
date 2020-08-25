#pragma once
#include <exception>
#include <string>
class FSException :public std::exception 
{
protected:
	FSException* cause_;
	const char* message;
public:
	    FSException(const char *);
		FSException(const char *, FSException& cause);
	    virtual const char* what() =0;
		FSException* getCause();
		void setCause(FSException *);
		virtual ~FSException();
};
class AccessException :public FSException 
{
public:
	AccessException(const char *);
	AccessException(const char *, FSException& cause);
	virtual const char* what();
};
class OperationFailedException :public FSException 
{
public:
	OperationFailedException(const char *);
	OperationFailedException(const char *, FSException& cause);
	virtual const char* what();

};
class NameCollisionException :public OperationFailedException 
{
public:
	NameCollisionException(const char *);
	NameCollisionException(const char *, FSException& cause);
	virtual const char* what();
	~NameCollisionException();
};
class WriteFailedException :public OperationFailedException 
{
public:
	WriteFailedException(const char *);
	WriteFailedException(const char *, FSException& cause);
	virtual const char* what();
};
class ObjectNotFoundException :public OperationFailedException 
{
public:
	ObjectNotFoundException(const char *);
	ObjectNotFoundException(const char *, FSException& cause);
	virtual const char* what();
};