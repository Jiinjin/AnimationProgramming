#pragma once
#include <string>
#include <Matrix/Matrix4.h>
#include "Engine.h"
class Bone
{
public:
	Bone(unsigned int pIndexParent, const char* pName, LibMath::Matrix4 pLocalTransform, LibMath::Matrix4 pGlobalTransform)
		: indexParent(pIndexParent), name(pName), localTransform(pLocalTransform), globalTranform(pGlobalTransform)
	{}

	bool IsIK() const
	{
		return name.find("ik_") == 0;
	}

	const LibMath::Matrix4& GetGlobalTransform() const
	{
		return globalTranform;
	}
	int GetIndexParent() const
	{
		return indexParent;
	}

	LibMath::const_row GetGlobalTranslation() const
	{
		return globalTranform[3];
	}

	bool HasParent() const
	{
		return GetIndexParent() != -1;
	}
	
private:
	std::string name;
	int indexParent = 0;
	
	LibMath::Matrix4 localTransform;
	LibMath::Matrix4 globalTranform;


};

