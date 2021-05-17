#include "Skeleton.h"

void Skeleton::Initialize()
{
	for (size_t nbBone = 0; nbBone < GetSkeletonBoneCount(); nbBone++)
	{
		InitializeBone(nbBone);
	}
}

void Skeleton::InitializeBone(const unsigned int boneIndex)
{
	float bonePosX, bonePosY, bonePosZ, boneQuatW, boneQuatX, boneQuatY, boneQuatZ;
	GetSkeletonBoneLocalBindTransform(boneIndex, bonePosX, bonePosY, bonePosZ, boneQuatW, boneQuatX, boneQuatY, boneQuatZ);

	printf("bone : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", bonePosX, bonePosY, bonePosZ, boneQuatW, boneQuatX, boneQuatY, boneQuatZ);

	LibMath::Matrix4 translate = LibMath::Matrix4::Identity();
	translate.Translate(bonePosX, bonePosY, bonePosZ);
	LibMath::Matrix4 rotate = LibMath::Matrix4::Identity();
	rotate.Rotate(LibMath::Quaternion(boneQuatX, boneQuatY, boneQuatZ, boneQuatW));
	LibMath::Matrix4 localTransform = translate * rotate;

	int parentBoneIndex = GetSkeletonBoneParentIndex(boneIndex);

	LibMath::Matrix4 globalTransform = localTransform;
	if (boneIndex > 0)
	{
		globalTransform = GetBoneByIndex(parentBoneIndex)->GetGlobalTransform() * localTransform;
	}

	AddBone(parentBoneIndex, localTransform, globalTransform);
}
