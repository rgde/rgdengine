/*

@author PC
*/

#include "engine.h"

#include "skeleton.h"

CSkeleton::CSkeleton(const Bones& bones) : m_bones(bones)
{
	size_t nNumBones = m_bones.size();

	m_transitionMatrices.resize(nNumBones);
	m_inverseZeroMatrices.resize(nNumBones);

	for(size_t i = 0; i < nNumBones; i++)
	{
		m_inverseZeroMatrices[i] = m_bones[i]->getFullTransform();
		math::invert(m_inverseZeroMatrices[i]);
	}
}

const CSkeleton::Bones& CSkeleton::getBones() const
{
	return m_bones;
}

const CSkeleton::Matrices& CSkeleton::getTransitionMatrices() const
{
	return m_transitionMatrices;
}

const CSkeleton::Matrices& CSkeleton::getInverseZeroMatrices() const
{
	return m_inverseZeroMatrices;
}

void CSkeleton::makeTransitionMatrices()
{
	size_t nNumBones = m_bones.size();

	for(size_t i = 0; i < nNumBones; i++)
		m_transitionMatrices[i] = m_bones[i]->getFullTransform()*m_inverseZeroMatrices[i];
}