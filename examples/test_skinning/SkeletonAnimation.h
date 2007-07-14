/*

@author PC
*/
#pragma once

#include "Interpolators.h"
#include "SkeletonState.h"
#include "BaseAnimationClasses.h"

/* Skeleton animation class. Can return special
   class called 'CSkeletonState' which contains
   state of the skeleton at some time. */
class CSkeletonAnimation : public ITimeController
{
public:

	DECLARE_INTERPOLATOR_TYPE(BoneStateLinearInterpolatorf, CBoneState,
							  float, LinearInterpolationMethod)

	typedef BoneStateLinearInterpolatorf BoneAnimation;

public:

	CSkeletonAnimation(float fullTime) : ITimeController(fullTime) {}

	void addBone(uint index)
	{
		m_state.addBone(index);
		m_animation.push_back(IndexedBoneAnimation(index, BoneAnimation()));
	}

	//TODO: Extend a set of functions to manage bones

	/* 'n' is index of bone in this class' bones array. */
	inline uint getBoneIndex(uint n)
	{
		return m_animation[n].first;
	}

	inline BoneAnimation& getBoneAnimation(uint n)
	{
		return m_animation[n].second;
	}

	inline CSkeletonState& getState(float normTime)
	{
		size_t num = m_animation.size();

        for(size_t i = 0; i < num; i++)
			m_state.setBoneState(uint(i), m_animation[i].second.getValue(normTime));

		return m_state;
	}

private:
	typedef std::pair<uint, BoneAnimation> IndexedBoneAnimation;
	typedef std::vector<IndexedBoneAnimation> Animation;

private:

	CSkeletonState m_state;
	Animation m_animation;

};

typedef boost::shared_ptr<CSkeletonAnimation> PSkeletonAnimation;