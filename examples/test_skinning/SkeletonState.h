/*

@author PC
*/

#pragma once

#include "BoneState.h"

/* Skeleton state class. Contains state of a skeleton i.e.
	a number of indexed bone states. An animation can animate
	only a part of skeleton so this class contains bone
	index for each bone state (pair of 'bone state' and 'index'
	is called 'indexed bone state'). */
class CSkeletonState
{
public:

	CSkeletonState() {}

	void addBone(uint index)
	{
		m_states.push_back(IndexedBoneState(index, CBoneState()));
	}

	//TODO: Extend a set of functions to manage bones

	inline size_t getNumBones() const { return m_states.size(); }

	/* 'n' is index of bone in this class' bones array. */
	inline const CBoneState& getBoneState(uint n) const
	{
		return m_states[n].second;
	}
	inline void setBoneState(uint n, const CBoneState& state)
	{
		m_states[n].second = state;
	}
	inline uint getBoneIndex(uint n) const
	{
		return m_states[n].first;
	}

	inline void operator *=(float w)
	{
		size_t num = m_states.size();
		for(size_t i = 0; i < num; i++)
			m_states[i].second *= w;
	}

private:
	typedef std::pair<uint, CBoneState> IndexedBoneState;
	typedef std::vector<IndexedBoneState> BoneStates;

private:

	BoneStates m_states;
};