#include "Bone.h"
#include "Engine.h"
#include <vector>

class Skeleton
{

public:
	const Bone* GetBoneByIndex(const unsigned int index) const
	{
		if (index < m_bones.size())
		{
			return &m_bones[index];
		}

		return nullptr;
	}

	void AddBone(int indexParent, LibMath::Matrix4 localTransform, LibMath::Matrix4 globalTransform)
	{
		m_bones.emplace_back(indexParent, GetSkeletonBoneName(m_bones.size()), localTransform, globalTransform);
	}

	size_t GetBoneCount() const
	{
		return m_bones.size();
	}

	void Initialize();

private:
	void InitializeBone(const unsigned int boneIndex);
	std::vector<Bone> m_bones;
};