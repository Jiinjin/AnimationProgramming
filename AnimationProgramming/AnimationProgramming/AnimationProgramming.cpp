// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Engine.h"
#include "AnimClip.h"
#include "Simulation.h"
#include "Interpolation.h"
#include "Vector/Vector.h"

#include "Quaternion/Quaternion.h"
#include "AnimClip.h"
#include "Matrix/Matrix4.h"
#include "Animator.h"

#include <vector>
#include <string>
#include <cassert>

#define FRAMERATE 30.f

class CSimulation : public ISimulation
{

	Skeleton m_skeleton;
	Animator m_animator;
	float m_frameTimer = 0;
	inline static const float m_frameDuration = 1.f / FRAMERATE;
	ANIMATION m_animationToBePlayed = ANIMATION::WALK;
	ANIMATION m_nextAnimationToBePlayed = ANIMATION::RUN;
	float m_transitionTime = 5.f;
	float m_blend = 0.f;


	virtual void Init() override
	{
		int spine01 = GetSkeletonBoneIndex("spine_01");
		int spineParent = GetSkeletonBoneParentIndex(spine01);
		const char* spineParentName = GetSkeletonBoneName(spineParent);

		/*float posX, posY, posZ, quatW, quatX, quatY, quatZ;
		keyCount = GetAnimKeyCount("ThirdPersonWalk.anim");
		GetAnimLocalBoneTransform("ThirdPersonWalk.anim", spineParent, keyCount / 2, posX, posY, posZ, quatW, quatX, quatY, quatZ);

		printf("Spine parent bone : %s\n", spineParentName);
		printf("Anim key count : %ld\n", keyCount);
		printf("Anim key : pos(%.2f,%.2f,%.2f) rotation quat(%.10f,%.10f,%.10f,%.10f)\n", posX, posY, posZ, quatW, quatX, quatY, quatZ);*/

		m_skeleton.Initialize();

		m_animator.SetSkeleton(m_skeleton);
		m_animator.AddAnimationClip("ThirdPersonWalk.anim", GetAnimKeyCount("ThirdPersonWalk.anim"));
		m_animator.AddAnimationClip("ThirdPersonRun.anim", GetAnimKeyCount("ThirdPersonRun.anim"));

	}
	
	virtual void Update(float frameTime) override
	{

		m_animator.GetAnimClip(m_animationToBePlayed).UpdatePos(frameTime);
		m_animator.GetAnimClip(m_nextAnimationToBePlayed).UpdatePos(frameTime);

		m_blend += frameTime / m_transitionTime;
		
		float alpha = (sin(LibMath::Radian(m_blend * LibMath::PI)) / 2.f) + 0.5f;

	
		std::vector<LibMath::Matrix4> skinningPoseData;
		skinningPoseData.reserve(m_skeleton.GetBoneCount());
		
		//AnimateManequin(skinningPoseData, m_animationToBePlayed, m_animator.GetAnimClip(m_animationToBePlayed).m_currentKeyFrame);
		AnimateManequinTransition(skinningPoseData, m_animator.GetAnimClip(m_animationToBePlayed), m_animator.GetAnimClip(m_nextAnimationToBePlayed), alpha);
		SetSkinningPose((float*)skinningPoseData.data(), skinningPoseData.size());
		
		DrawAxis();
		//DrawDebugAnim(m_animator.GetAnimClip(m_animationToBePlayed).m_currentKeyFrame, m_animationToBePlayed);
		
		//DrawBindPose();
	}

	void AnimateManequin(std::vector<LibMath::Matrix4>& skinningPoseData, ANIMATION animation, int currentKeyFrame)
	{
		
		float alpha = m_frameTimer / m_frameDuration;
		

		for (int i = 0; i < m_skeleton.GetBoneCount(); i++)
		{
			const Bone* currentBone = m_skeleton.GetBoneByIndex((i));
			/* We ignore IK for now */
			if (currentBone->IsIK() == false)
			{
				//TODO: change keycound to handle both animation
				const RT& animRT = m_animator.GetAnimClip(animation).GetTransformAtKeyFrame(currentKeyFrame)[i];
				const RT& animRTNext = m_animator.GetAnimClip(animation).GetTransformAtKeyFrame((currentKeyFrame + 1) % m_animator.GetAnimClip(animation).GetKeyCount())[i];

				RT animRTFinal = RT(LibMath::Interpolation::Lerp(animRT.m_translation, animRTNext.m_translation, alpha),
					LibMath::Interpolation::Slerp(animRT.m_rotation, animRTNext.m_rotation, alpha));

				LibMath::Matrix4 boneDefaultGlobalTransform = currentBone->GetGlobalTransform();

				LibMath::Matrix4 boneCurrentGlobalTransform = LibMath::Matrix4::Identity();;

				boneCurrentGlobalTransform.Rotate(animRTFinal.m_rotation).Translate(animRTFinal.m_translation);;

				skinningPoseData.push_back(boneCurrentGlobalTransform * boneDefaultGlobalTransform.GetInverse());
			}
		}
	}

	void AnimateManequinTransition(std::vector<LibMath::Matrix4>& skinningPoseData, AnimClip& firstAnim, AnimClip& secondAnim, float alpha)
	{
		for (int i = 0; i < m_skeleton.GetBoneCount(); i++)
		{
			const Bone* currentBone = m_skeleton.GetBoneByIndex((i));
			/* We ignore IK for now */
			if (currentBone->IsIK() == false)
			{
				//TODO: change keycound to handle both animation
				const RT& animRT = firstAnim.GetTransformAtKeyFrame(firstAnim.m_currentKeyFrame)[i];
				const RT& animRTNext = firstAnim.GetTransformAtKeyFrame((firstAnim.m_currentKeyFrame + 1) % firstAnim.GetKeyCount())[i];

				//TODO: change keycound to handle both animation
				const RT& secondAnimRT = secondAnim.GetTransformAtKeyFrame(secondAnim.m_currentKeyFrame)[i];
				const RT& secondAnimRTNext = secondAnim.GetTransformAtKeyFrame((secondAnim.m_currentKeyFrame + 1) % secondAnim.GetKeyCount())[i];

				RT animRTFinal = RT(LibMath::Interpolation::Lerp(animRT.m_translation, animRTNext.m_translation, firstAnim.m_pos),
					LibMath::Interpolation::Slerp(animRT.m_rotation, animRTNext.m_rotation, firstAnim.m_pos));

				RT secondAnimRTFinal = RT(LibMath::Interpolation::Lerp(secondAnimRT.m_translation, secondAnimRTNext.m_translation, secondAnim.m_pos),
					LibMath::Interpolation::Slerp(secondAnimRT.m_rotation, secondAnimRTNext.m_rotation, secondAnim.m_pos));

				RT finalRT = RT(LibMath::Interpolation::Lerp(animRTFinal.m_translation, secondAnimRTFinal.m_translation, alpha),
					LibMath::Interpolation::Slerp(animRTFinal.m_rotation, secondAnimRTFinal.m_rotation, alpha));

				LibMath::Matrix4 boneDefaultGlobalTransform = currentBone->GetGlobalTransform();

				LibMath::Matrix4 boneCurrentGlobalTransform = LibMath::Matrix4::Identity();;

				boneCurrentGlobalTransform.Rotate(finalRT.m_rotation).Translate(finalRT.m_translation);;

				skinningPoseData.push_back(boneCurrentGlobalTransform * boneDefaultGlobalTransform.GetInverse());
			}
		}
		
	}


	void DrawAxis()
	{
		// X axis
		DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

		// Y axis
		DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

		// Z axis
		DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);
	}

	void DrawBindPose()
	{
		for (size_t nbBone = 1; nbBone < m_skeleton.GetBoneCount(); nbBone++)
		{
			DrawOneBone(nbBone);
		}
	}

	void DrawDebugAnim(const int keyFrame, ANIMATION animation)
	{
		for (size_t nbBone = 1; nbBone <= m_skeleton.GetBoneCount(); nbBone++)
		{
			DrawAnimationAtFrame(keyFrame, nbBone, animation);
		}
	}

	void DrawOneBone(const int boneIndex)
	{
		const Bone* bone = m_skeleton.GetBoneByIndex(boneIndex);

		if (bone == nullptr || bone->IsIK())
		{
			return;
		}

		LibMath::const_row start = bone->GetGlobalTranslation();
		LibMath::const_row end = m_skeleton.GetBoneByIndex(bone->GetIndexParent())->GetGlobalTranslation();

		DrawLine(start[0], start[1] - 50, start[2], end[0], end[1] - 50, end[2], 0, 0, 0);
	}

	void DrawAnimationAtFrame(int frame, const int boneIndex, ANIMATION animation)
	{
		const Bone* bone = m_skeleton.GetBoneByIndex(boneIndex);
		
		if (bone == nullptr || bone->IsIK())
		{
			return;
		}

		
		LibMath::Vector3 translationAnim = m_animator.GetAnimClip(animation).GetTransformAtKeyFrame(frame)[boneIndex].m_translation;
		LibMath::Vector3 translationAnimParent = m_animator.GetAnimClip(animation).GetTransformAtKeyFrame(frame)[bone->GetIndexParent()].m_translation;
		
	
		DrawLine(translationAnim.x, translationAnim.y - 50, translationAnim.z, translationAnimParent.x, translationAnimParent.y - 50, translationAnimParent.z, 0, 0, 0);
	}
};




int main()
{
	CSimulation simulation;
	Run(&simulation, 1400, 800);

	return 0;
}