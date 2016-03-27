#ifndef OBJECT_H
#define OBJECT_H

class Object
{
	char* identifier;
	char* type;
public:
	Object(char* _id);
	~Object();
};

#endif