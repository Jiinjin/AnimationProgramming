#pragma once
#include <vector>
#include "AnimClip.h"

enum class ANIMATION
{
	WALK = 0,
	RUN
};

class Animator
{
public:
	void SetSkeleton(Skeleton& skeleton);
	void AddAnimationClip(const char* animName, size_t pkeyCount);
	std::vector<AnimClip>& GetAnimClipVector();
	AnimClip& GetAnimClip(ANIMATION animation);
private:
	std::vector<AnimClip> m_animationsVector;
	int m_indexAnimation = 0;
	Skeleton* m_skeleton = nullptr;
	

};