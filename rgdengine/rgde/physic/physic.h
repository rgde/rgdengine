/******************************************************************************
@file    physic.h
@author  Neonic
@mail    gaba@male.ru
@project RGDE
@date    23.12.2005
@desc  
 ******************************************************************************/

//Neonic: octree

#ifndef PHYSIC_H
#define PHYSIC_H
#ifdef _WIN32
#pragma once
#endif

#pragma warning(disable : 4244) // type conversion warning.
#pragma warning(disable : 4305)	// truncation from 'const double' to 'float'

#include <rgde/base/singelton.h>

// this define allow us to use customized physic
#define PHYSIC_CUSTOM

#ifndef PHYSIC_CUSTOM
#define PHYSIC_REALM physic
#else
#define PHYSIC_REALM physic2
#endif

#ifndef PHYSIC_CUSTOM

//bullet includes
#include "CcdPhysicsEnvironment.h"
#include "CcdPhysicsController.h"

#include "PHY_IMotionState.h"
#include "SimdTransform.h"

#include "CollisionShapes/BoxShape.h"
#include "CollisionShapes/Simplex1to4Shape.h"
#include "CollisionShapes/EmptyShape.h"

#include "Dynamics/RigidBody.h"
#include "ConstraintSolver/SimpleConstraintSolver.h"
#include "ConstraintSolver/OdeConstraintSolver.h"
#include "CollisionDispatch/ToiContactDispatcher.h"
#include "BroadphaseCollision/SimpleBroadphase.h"

#include "PHY_Pro.h"

//интерфейс для отладочной графики bullet
#include "IDebugDraw.h"


namespace physic
{
	using namespace math;

	/* \brief Вспомогательный класс определения положения объекта
	*/
	class CPhysicObject :
		public PHY_IMotionState
	{
	private:
		//TODO: add model ptr

		//TODO: add pos/rot/scale modificator to control renderable object

		// motion state
		SimdTransform		m_worldTransform;
		Matrix44f			m_transform;

		CcdPhysicsController* m_pcController;

		bool				m_IsChanged;

	public:
		CPhysicObject( void );
		~CPhysicObject( void );

		CcdPhysicsController* getController( void ) { return m_pcController; };
		CcdPhysicsController* operator()( void ) { return m_pcController; };
		void operator() ( CcdPhysicsController* p ) { m_pcController = p; };

		void	registerController( void );

		Matrix44f getTransform( void ) const;
		void	getWorldPosition( Point3f& pos );
		void	getWorldScaling( Vec3f& scale );
		void	getWorldOrientation( Quatf& rot );

		void	setWorldPosition( const Point3f& pos );
		void	setWorldOrientation( const Quatf& rot );

		// motion state function implementations
		void	getWorldPosition( float& posX, float& posY, float& posZ );
		void	getWorldScaling( float& scaleX, float& scaleY, float& scaleZ );
		void	getWorldOrientation( float& quatIma0, float& quatIma1, float& quatIma2, float& quatReal );
		
		void	setWorldPosition( float posX, float posY, float posZ );
		void	setWorldOrientation( float quatIma0, float quatIma1, float quatIma2, float quatReal );

		void	calculateWorldTransformations( void );
	};

	typedef std::auto_ptr<CPhysicObject> PPhysicObject;
	typedef std::vector<CPhysicObject*> TVecPhysObjPtr;

	/* \brief Основной класс физической системы
	*/
	class CPhysic
		: public CcdPhysicsEnvironment
	{
	public:
		CPhysic( void );
		~CPhysic( void );

		//инициализация
		void init( void );              

		//Установить время обновления
		void setUpdateTime( float dt ) { m_fDeltaTime = dt; };

		//обновить физическую систему
		void update( void );

		//отрисовать геометрию физики
		void drawDebug( void );

		//отладка
		int	getDebug( void ) { return m_iDebugPhysic; };
		void setDebug( int dbg ) { m_iDebugPhysic = dbg; };
		void switchDebug( void );

		//включение/выключение паузы.
		//TODO: На самом деле достаточно затормозить ITask, но мне пока неизвестно, как его выделить из IApplication
		void switchIdle( void ) { m_bIdle = !m_bIdle; };
		bool getIdle( void ) { return m_bIdle; };

		//создать физический объект
		CPhysicObject* createObject( void );

		//получить указатель на хранилище физических объектов
		TVecPhysObjPtr* getStorage( void ) { return &m_vecObjects; };

		//подготовиться к созданию указанного количества объектов. это просто маленький хинт
		void prepareStorage( int num ) { m_vecObjects.reserve(num); };

	private:

		float						m_fDeltaTime;
		// отладка физики. отрисовка AABB или контактных точек
		int							m_iDebugPhysic;
		// состояние процесса
		bool						m_bIdle;
		
		// Все физические объекты мы храним здесь
		TVecPhysObjPtr				m_vecObjects;

		//TODO: modify vector storage to auto-deletion

		//TODO: add sync between physic & renderable object in update cycle
	};
	typedef base::TSingelton<CPhysic>	Physic;
}

#endif // PHYSIC_CUSTOM

namespace physic2
{
	using namespace math;
//	using namespace octree;

	/** \brief Базовый контейнер физической системы
		class Ptr должен быть boost::shared_ptr<T>
	*/
	template<class T, class Ptr>
	class IPhContainer :
		public std::vector<Ptr>
	{
		//TODO: perform tests within container
	public:
		typedef T					elem_type;
		typedef T*					pointer;
		typedef Ptr					spointer;
		typedef std::vector<Ptr>	base;

		/// создать объект
		virtual spointer			create( void )
		{
			spointer p = spointer(new T());
			assert(p);
			push_back(p);
			return p;
		}

		/// удалить объект
		virtual void				erase( pointer p )
		{
			assert(p);
			Ptr ptr(p);
			for(iterator it = begin(); it!=end(); it++)
				if(*it==ptr)
				{
					base::erase(it);
					break;
				}
		};
	};

	// classes
	class	CPhSystem;
	struct	CPhWorld;
	struct	CPhController;
	struct	CPhContact;
	struct	CPhJoint;

	// pointers
	typedef boost::shared_ptr<CPhWorld>			PPhWorld;
	typedef boost::shared_ptr<CPhController>	PPhController;
	typedef boost::shared_ptr<CPhContact>		PPhContact;
	typedef boost::shared_ptr<CPhJoint>			PPhJoint;

	// containers
	typedef IPhContainer<CPhController,PPhController>	TPhControllers;
	typedef IPhContainer<CPhWorld,PPhWorld>				TPhWorlds;
	typedef IPhContainer<CPhContact,PPhContact>			TPhContacts;
	typedef IPhContainer<CPhJoint,PPhJoint>				TPhJoints;

	enum ePhGeom {
		PGNone		= 0,			// nothing
		PGSphere,					// simple sphere geometry
        PGCube,						// simple cube geometry
		PGTriangles,				// advanced geometry
	};

	/** \brief Контроллер физического объекта
	*/
	struct CPhController
	{
		/// физический мир, к которому принадлежит объект
		PPhWorld		m_pWorld;
		/// Соединения
		TPhJoints		m_Joints;
		/// масса
		float			m_Mass;
		/// положение, центр массы
		Point3f			m_position;
		/// ориентация
		Quatf			m_rotation;
		/// Размер
		Vec3f			m_scale;
		/// флаги
		unsigned		m_Flags;
		/// указатель на данные пользователя
		void*			m_pData;
		/// матрица трансформации
		Matrix44f		m_Transform;
		/// необходимо ли пересобрать матрицу трансформации
		bool			m_bIsNeedRecompute;

		/// линейное и угловое ускорения
		Vec3f			m_lvel, m_avel;
		/// аккумуляторы силы и вращения
		Vec3f			m_fass, m_tass;

	// Геометрия
		//typedef TIndexedGeometry<vertex::Position, false>	Geometry;
		//typedef boost::shared_ptr<Geometry>					PGeometry;
		/// Тип геометрии
		ePhGeom			m_Type;
		/// Данные геометрии - по аналогии с CMesh 
		//PGeometry		m_geometry;

		/// конструктор
		CPhController( void );
		/// Сменить физический мир
		void			changeWorld( PPhWorld p );
	};

	/** \brief Контакт между физическими объектами
	*/
	struct CPhContact
	{
		//
	};

	/** \brief Соединение между физическими объектами
	*/
	struct CPhJoint
	{
		//
	};
	
	/** \brief Физический мир
	*/
	struct CPhWorld
	{
		/// гравитация
		Point3f			m_vGravity;
		/// физические контроллеры
		TPhControllers	m_Controllers;
		/// физические соединения
		TPhJoints		m_Joints;
		/// контакты. заполнение контактов зависит от используемой функции обновления
		TPhContacts		m_Contacts;

		/// конструктор
		CPhWorld( void );
		/// обновление
		void	update( float dt );
		/// обновление с заполнением собственного контакт-листа
		void	update( float dt, TPhContacts* p );
	};

	/** \brief Физическая система
	*/
	class CPhSystem
	{
	private:
		/// время прошедшее от последнего цикла
		float			m_fDeltaTime;
		/// время деактивации
		float			m_fDeactivationTime;
		/// Список миров
		TPhWorlds		m_Worlds;
		/// отладка физики. отрисовка AABB или контактных точек
		int				m_iDebugPhysic;
		/// состояние процесса
		bool			m_bIdle;

	public:
		/// конструктор
		CPhSystem( void );
		/// инициализация
		void				init( void );              
		/// Установить время обновления
		void				setUpdateTime( float dt ) { m_fDeltaTime = dt; };
		/// обновить физическую систему
		void				update( void );
		/// отрисовать геометрию физики
		void				drawDebug( void );
		/// установка времени деактивации
		void				setDeactivationTime( float f );
		/// Установить гравитацию для выбранного мира
		void				setGravity( PPhWorld w, Point3f &pt);
		/// Получить гравитацию выбранного мира
		Point3f				getGravity( PPhWorld w );
		/// Создать физический мир
		PPhWorld			createWorld( void );
		/// Уничтожить физический мир
		void				eraseWorld( PPhWorld pWorld );
		/// Уничтожить все существующие физические миры
		void				cleanWorlds( void );
		/// Создать физический контроллер
		PPhController		createController( PPhWorld p );
		/// подготовиться к созданию указанного количества объектов. это просто маленький хинт
		void				reserve( unsigned num );
		/// получение хранилищ
		TPhWorlds*			getWorlds( void );
		/// отладка
		int					getDebug( void );
		void				setDebug( int dbg );
		void				switchDebug( void );
		//включение/выключение паузы.
		//TODO: На самом деле достаточно затормозить ITask, но мне пока неизвестно, как его выделить из IApplication
		void				switchIdle( void ) { m_bIdle = !m_bIdle; };
		bool				getIdle( void ) { return m_bIdle; };
	};

	typedef base::TSingelton<CPhSystem> Physic;
}

#endif // PHYSIC_H
