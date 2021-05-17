#include "AnimClip.h"
#include "Engine.h"
#include "Interpolation.h"

AnimClip::AnimClip(const char* p_animationName, Skeleton& skeleton, size_t pkeyCount) : m_animStartKey(0),
m_animEndKey(GetAnimKeyCount(p_animationName) - 1), m_keyCount(pkeyCount)
{
	m_keyFrames.reserve(m_animEndKey + 1);
	
	for (int indexKey = 0; indexKey <= m_animEndKey; indexKey++)
	{
		std::vector<RT> newKeyFrame;
		for (int indexBone = 0; indexBone < skeleton.GetBoneCount(); indexBone++)
		{
			if (skeleton.GetBoneByIndex(indexBone)->IsIK() == true)
			{
				continue;
			}
			//boneRotationTranslation
			RT BRT;
			GetAnimLocalBoneTransform(p_animationName, indexBone, indexKey, BRT.m_translation.x, BRT.m_translation.y, BRT.m_translation.z, BRT.m_rotation.W, BRT.m_rotation.X, BRT.m_rotation.Y, BRT.m_rotation.Z);
			//boneRotationTranslationBindPose
			RT BRTBindPose;
			GetSkeletonBoneLocalBindTransform(indexBone, BRTBindPose.m_translation.x, BRTBindPose.m_translation.y, BRTBindPose.m_translation.z, BRTBindPose.m_rotation.W, BRTBindPose.m_rotation.X, BRTBindPose.m_rotation.Y, BRTBindPose.m_rotation.Z);
			BRT = BRT * BRTBindPose;

			if (skeleton.GetBoneByIndex(indexBone)->HasParent())
			{
				//Bone Rotation Translation Parent
				const RT& BRTP = newKeyFrame[skeleton.GetBoneByIndex(indexBone)->GetIndexParent()];
				BRT = BRT * BRTP;
			}

			newKeyFrame.emplace_back(BRT);
			
			
		}
		if (newKeyFrame.empty() == false)
		{
			m_keyFrames.emplace_back(newKeyFrame);
		}
		
	}
}

const std::vector<RT>& AnimClip::GetTransformAtKeyFrame(int keyFrame) const
{
	return m_keyFrames[keyFrame];
}

const size_t AnimClip::GetKeyCount() const
{
	return m_keyCount;
}

const int AnimClip::GetLastKey() const
{
	return m_animEndKey;
}

void AnimClip::UpdatePos(float deltaTime)
{
	m_pos += deltaTime;

	while(m_pos >= 1.f)
	{
		m_pos -= 1.f;
	}

	m_currentKeyFrame = LibMath::Interpolation::MapInRange(
		m_pos,
		0.f,
		1.f,
		0.f,
		static_cast<float>(m_keyCount));
}

RT RT::operator*(const RT& rtBindPose)
{
	RT TempRT;
	TempRT.m_rotation = rtBindPose.m_rotation* m_rotation;
	TempRT.m_rotation.Normalize();

	TempRT.m_translation = rtBindPose.m_translation + (rtBindPose.m_rotation * m_translation);

	return TempRT;
}
