/*

@author PC
*/
#pragma once

class CSkeleton
{
public:

	typedef std::vector<math::PFrame> Bones;
	typedef std::vector<math::Matrix44f> Matrices;

public:

	CSkeleton(const Bones& bones);

	const Bones& getBones() const;

	const Matrices& getTransitionMatrices() const;
	const Matrices& getInverseZeroMatrices() const;

	void makeTransitionMatrices();

private:

	Bones    m_bones;
	Matrices m_transitionMatrices;
	Matrices m_inverseZeroMatrices;

};

typedef boost::intrusive_ptr<CSkeleton> PSkeleton;