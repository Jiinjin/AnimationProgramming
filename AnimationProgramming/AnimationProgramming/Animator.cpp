#include "Animator.h"

#include "Engine.h"

void Animator::SetSkeleton(Skeleton& skeleton)
{
	m_skeleton = &skeleton;
}

void Animator::AddAnimationClip(const char* animName, size_t pkeyCount)
{
	if (m_skeleton != nullptr)
	{
		m_animationsVector.emplace_back(animName, *m_skeleton, pkeyCount);
	}
	
}

std::vector<AnimClip>& Animator::GetAnimClipVector()
{
	return m_animationsVector;
}

AnimClip& Animator::GetAnimClip(ANIMATION animation)
{
	switch (animation)
	{
	case ANIMATION::WALK:
		return m_animationsVector[0];
		break;
	case ANIMATION::RUN:
		return m_animationsVector[1];
		break;
	default:
		return m_animationsVector[0];
		break;
	}
	
}

