#pragma once
#include <vector>
#include <string>
#include "Skeleton.h"
#include "Quaternion/Quaternion.h"
#include "Vector/Vector3.h"

// RT = Rotation Translation
struct RT
{
	RT() = default;
	RT(LibMath::Vector3 translation, LibMath::Quaternion rotation) : m_translation(translation), m_rotation(rotation){}

	RT operator*(const RT& rt);
	LibMath::Vector3 m_translation;
	LibMath::Quaternion m_rotation;
};

class AnimClip
{
public:
	AnimClip(const char* p_animationName, Skeleton& skeleton, size_t pkeyCount);
	const std::vector<RT>& GetTransformAtKeyFrame(int keyFrame) const;
	const size_t GetKeyCount() const;
	const int GetLastKey() const;
	int m_currentKeyFrame = 0;
	void UpdatePos(float deltaTime);
	float m_pos = 0.f;
private:
	int m_animStartKey = 0;
	int m_animEndKey = 0;
	std::vector<std::vector<RT>> m_keyFrames;
	size_t m_keyCount = 0;
	
	
};
