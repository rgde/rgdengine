class CSkeleton//Хранит указатель на Root кость скелета в дереве, а также косточки (матрицы, обратные их начальным)
			   //в линейном массиве, и, конечно же, матрицы перехода
{
public:
	typedef math::PFrame Bone;
	typedef std::vector<Bone> Bones;
	typedef std::vector<math::Matrix34f> Matrices;

	const Bone& getRootFrame() const;
	const Bones& getBones() const;

	const Matrices& getInverseZeroMatrices();
	const Matrices& getTransitionMatrices();

	void update();//Пересчитывает TransitionMatrices для текущего момента времени (в соответствии с положением фреймов)
	//...
};

class CFrameAnimation//Хранит анимацию по кейфреймам для одного фрейма
{
public:
	struct Key
	{
		math::Quatf		rotation;
		math::Point3f	postition;
		math::Vec3f		scale;
		float			time;	
	};

	typedef .... Keys;

	CFrameAnimation(const math::PFrame& frame) // кого анимировать
		m_frame(frame)
	{
	}

	Keys& getKeys() {return m_keys;}

	void update(float fNormalizedTime) const;

private:
	math::PFrame m_frame;
	Keys	m_keys;
};

/*
class CSkeletonAnimation//Хранит анимации для костей скелета
{
public:
	CSkeletonAnimation(const PSkeleton& pSkeleton) 
		: m_skeleton(pSkeleton)
	{
	}

	void update(float fNormalizedTime) const
	{
		//pSkeleton->getBones(), а потом для всех этих косточек из множества CFrameAnimationов (см ниже) обновляем
		//положение
	}

private:
	//Здесь дата как множество CFrameAnimationов (для каждого бона)
};
*/
/*
class CHierarchyDescriptor
{
public:

	struct SHierarchyNode
	{
		SHierarchyNode(SHierarchyNode* pParentNode, int nType);
		SHierarchyNode(int nType);

		int                          nType;
		SHierarchyNode*              pParentNode;
		std::vector<SHierarchyNode*> vChildNodes;
	};

	SHierarchyNode* 

};
*/
class CAnimation
{
public:

	typedef std::vector<CFrameAnimation> FrameAnimationsVector;
//	typedef std::vector<CSkeletonAnimation> SkeletonAnimationsVector;

	const FrameAnimationsVector& getFrameAnimations() const;
//	const SkeletonAnimationsVector& getSkeletonAnimations() const;

	FrameAnimationsVector& getFrameAnimations();
//	SkeletonAnimationsVector& getSkeletonAnimations();

};

class CFrameAnimator//Фактически это то, что раньше было контроллером. Оно привызявается к фрейму и анимирует его,
					//получая команды извне.
{
public:

	CFrameAnimator(math::PFrame& pFrame);

	void setAnimation(PFrameAnimation pAnimation);

	void updateState();//апдейт без приращения dt
	void update(float dtNorm);

	void getCurrentNormalizedTime()
    
	void pause();
	void play();

	void setLooped(bool bLooped);
};

class CAnimationController
{
public:
/*
	template <class T>
	struct SClient
	{
		SClient(bool bLooped, float fRate, T& rClient);

		//Возможно еще что-то
		bool  bLooped;
		float fRate;
		T&    rClient;
	};

	typedef SClient<math::PFrame> FrameClient;
	typedef SClient<PSkeleton> SkeletonClient;

	void addFrame(const FrameClient&, PFrameAnimation);
	void addSkeleton(const SkeletonClient&, PSkeletonAnimation);
*/
/*
	struct SClient
	{
		SClient(bool bLooped, float fRate, T& rClient);

		//Возможно еще что-то
		bool  bLooped;
		float fRate;
		T&    rClient;
	};
*/
	void addAnimator(PFrameAnimator pAnimator);
	//еще всякая фигня на тему

	void addSkeleton(PSkeleton);
	//еще всякая фигня на тему

	void update(float dt);//Обновляет все экземпляры CFrameAnimator и говорит всем вверенным скелетам обновить матрицы
	void play();
	void pause();

	void setCurrentAnimation(PAnimation);//Пробегаем по всем CFrameAnimatorам и ставит им анимацию из это вот самой
										 //анимации
};

class CMesh
{
public:

//...

	CAnimationController& getAnimationController();

//...

};