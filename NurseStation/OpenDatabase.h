#pragma once

#include "ado2.h"

class OpenDatabase
{
public:
	OpenDatabase(void);
	~OpenDatabase(void);
public:
	BOOL OpenDB(CADODatabase &db);
};
