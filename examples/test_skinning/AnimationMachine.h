/*

@author PC
*/

#pragma once

#include "Skeleton.h"
#include "SkeletonAnimation.h"

class CAnimationMachine
{
public:

	CAnimationMachine(const CSkeleton::Bones& bones)
		: m_state(bones)
	{
	}

	void addState(const PSkeletonAnimation& animation, float normTime, float weight)
	{
		CSkeletonState& state = animation->getState(normTime);
		state *= weight;
		m_state += state;
	}

	void setState(const PSkeletonAnimation& animation, float normTime, float weight)
	{
		CSkeletonState& state = animation->getState(normTime);
		state *= weight;
		m_state = state;
	}

	void apply()
	{
		size_t num = m_state.getNumBones();
		for(size_t i = 0; i < num; i++)
		{
			const math::PFrame& frame = m_state.getBoneFrame(uint(i));
			const CBoneState& state = m_state.getBoneState(uint(i));
			frame->setPosition(state.getPos());
			frame->setRotation(state.getRot());
			frame->setScale(state.getScale());
		}
	}

private:

	/* Skeleton full state class. Contains state of a skeleton.
	   This class is like to 'CSkeletonState' class, but with one
	   difference. This class contains state for all bones (full
	   state) without indices but with PFrames. Pair of 'bone state'
	   and 'frame' is called 'framed bone state' */
	class CSkeletonFullState
	{
	public:

		CSkeletonFullState() {}
		CSkeletonFullState(const CSkeleton::Bones& bones)
		{
			size_t num = bones.size();
			m_states.reserve(num);
			for(size_t i = 0; i < num; i++)
				addBone(bones[i]);
		}

		void addBone(const math::PFrame& frame)
		{
			m_states.push_back(FramedBoneState(frame, CBoneState()));
		}

		//TODO: Extend a set of functions to manage bones

		inline size_t getNumBones() const { return m_states.size(); }

		/* 'n' is index of bone in this class's bones array. */
		inline const math::PFrame& getBoneFrame(uint n) const
		{
			return m_states[n].first;
		}
		inline void setBoneState(uint n, const CBoneState& state)
		{
			m_states[n].second = state;
		}

		inline CBoneState getBoneState(uint n) const
		{
			return m_states[n].second;
		}

		inline void operator +=(const CSkeletonState& state)
		{
			size_t num = state.getNumBones();
			for(size_t i = 0; i < num; i++)
				m_states[state.getBoneIndex(uint(i))].second += state.getBoneState(uint(i));
		}

		inline void operator =(const CSkeletonState& state)
		{
			size_t num = state.getNumBones();
			for(size_t i = 0; i < num; i++)
				m_states[state.getBoneIndex(uint(i))].second = state.getBoneState(uint(i));
		}

	private:
		typedef std::pair<math::PFrame, CBoneState> FramedBoneState;
		typedef std::vector<FramedBoneState> BoneStates;

	private:

		BoneStates m_states;
	};

private:

	CSkeletonFullState m_state;

};