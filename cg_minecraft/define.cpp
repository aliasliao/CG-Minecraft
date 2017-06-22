#include "stdafx.h"
#include "define.h"

bool isExtModel(cub type)
{
	int iType = static_cast<int>(type);
	int iTea = static_cast<int>(cub::teapot);
	int iLAST = static_cast<int>(cub::LAST);

	return iType >= iTea && iType < iLAST;
}
