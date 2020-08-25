#include "Exception.h"
#include <iostream>
FSException::FSException(const char * msg) 
{
	message = msg;
	cause_ = nullptr;
};
FSException* FSException::getCause() { return cause_; };
FSException::FSException(const char * msg, FSException& cause) 
{
	message = msg;
	cause_ = &cause;
};
AccessException::AccessException(const char * msg):FSException(msg) {};
AccessException::AccessException(const char * msg, FSException& cause):FSException(msg,cause) {};
const char* AccessException::AccessException::what() { return message; };

OperationFailedException::OperationFailedException(const char * msg):FSException(msg){};
OperationFailedException::OperationFailedException(const char * msg, FSException& cause):FSException(msg,cause){};
const char* OperationFailedException::what() { return message; };

NameCollisionException::NameCollisionException(const char * msg):OperationFailedException(msg){};
NameCollisionException::NameCollisionException(const char * msg, FSException& cause) :OperationFailedException(msg, cause) {};
const char* NameCollisionException::what() { return message; };

WriteFailedException::WriteFailedException(const char * msg):OperationFailedException(msg) {};
WriteFailedException::WriteFailedException(const char * msg, FSException& cause):OperationFailedException(msg,cause) {};
const char* WriteFailedException::what() { return message; };

ObjectNotFoundException::ObjectNotFoundException(const char * msg):OperationFailedException(msg){};
ObjectNotFoundException::ObjectNotFoundException(const char * msg, FSException& cause) :OperationFailedException(msg, cause) {};
const char* ObjectNotFoundException::what() { return message; };

FSException::~FSException() 
{
	std::cout << "Destruction: "<< message;
	delete [] message;
	if (cause_) delete cause_;
	message = nullptr;
};
void FSException::setCause(FSException* e) 
{
	cause_ = e;
};
NameCollisionException::~NameCollisionException() 
{

};