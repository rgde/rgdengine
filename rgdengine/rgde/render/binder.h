//////////////////////////////////////////////////////////////////////////
// @author Sidorenko "PC" Alexander
// @date 11.06.2006
// email: sidorenko.alexander@gmail.com
// project: RGDE
// description: Binder class. See class description.
// example of use:
//
// texture_ptr getTexture(int n)
// {
//    texture_ptr textures[3];
//    return textures[n];
// }
// 
// struct SSome
// {
//    SSome() {}
//    const math::Color& getColor() const { return m_color; }
//
//    math::Color m_color;
// }
//
// void testBinder()
// {
//    typedef Binder<int> CBinder;
//    CBinder* binder = new CBinder(pEffect);//pEffect is some non-null
//                                           //PEffect pointer
//
//    //Functor parameter. Binder calls taken GetFunction with taken int
//    //parameter and setups result to effect.
//    binder->addParameter<texture_ptr>(CBinder::Types<texture_ptr>::GetFunction(
//                                           boost::bind(&getTexture, _1)),
//                                           "TEXTURE_PARAMETER");
//
//    SSome some;
//
//    //Another functor parameter. Binder calls taken ParamTypeGetFunction 
//    //without parameters (in this example only, becose SSome::getColor
//    //doesn't need any parameters) and setups result to effect. Difference
//    //is that ParamTypeGetFunction sometimes generates warning "Returning
//    //address of local variable" so in this case use GetFunction returns
//    //copy for all types.
//    CBinder::Types<math::Color>::ParamTypeGetFunction getFunc = 
//                             boost::bind(&SSome::getColor, &some);
//    binder->addParameter<math::Color>(getFunc, "COLOR_PARAMETER");
//
//
//    binder->addParameter<int>(1, "NUM_LIGHTS");
//
//    binder->setupParameter(2);//i.e.:
//    //pEffect->getParams()["TEXTURE_PARAMETER"]->set(getTexture(2));
//    //pEffect->getParams()["COLOR_PARAMETER"]->set(some.getColor());
//    //pEffect->getParams()["NUM_LIGHTS"]->set(1);
//    //Biner has a number of advantages. It's more stable.
//    //If there's no parameter with given name, it doesn't call
//    //set method. It's faster becose pEffect->getParams()["SOME_NAME"]
//    //does std::map lookup by std::string (parameter name).
// }
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <boost/call_traits.hpp>
#include <rgde/render/effect.h>
#include <rgde/math/transform.h>

namespace render
{
	/** Binder template class.
	  * Binder is used to bind some parameters to Effect. See example of use
	  * in the beginning of the file. FType is parameter type for GetFunction
	  * and ParamTypeGetFunction. Binder calls GetFunction (or 
	  * ParamTypeGetFunction) with Types<FType>::ParamType parameter...
	  */
	template <class FType>
	class Binder
	{
	public:

		template <class PType>
		struct Types
		{
			typedef typename boost::call_traits<PType>::param_type ParamType;
			typedef typename Types<FType>::ParamType FTypeParamType;
			//GetFunction is used to get value to be setup to Effect. Returns
			//PType
			typedef boost::function<PType(FTypeParamType)>GetFunction;
			//ParamTypeGetFunction is used to get value to be setup to Effect
			//too. Defference is that ParamTypeGetFunction returns ParamType
			//instead of PType. It allows to setup parameter by more effective
			//way when it's possible.
			typedef boost::function<ParamType(FTypeParamType)>
													ParamTypeGetFunction;
			//Effect set function.
			typedef bool (Effect::IParameter::*EffectSetFunction)(ParamType);
		};

		typedef boost::function<void(typename Types<FType>::ParamType)> Functor;

		typedef boost::shared_ptr<Binder> PBinder;

		static PBinder create(const PEffect& pEffect)
		{
			if(pEffect)
				return PBinder(new Binder(pEffect));
			else
				return PBinder();
		}

		virtual ~Binder() { }

		const PEffect& getEffect() const { return m_effect; }

		void setupParameters(typename Types<FType>::ParamType param) const
		{
			for(size_t i = 0; i < m_functors.size(); i++)
				m_functors[i](param);
		}

		//PType is type of parameter to be set to shader
		template <class PType>
		bool addParameter(typename Types<PType>::ParamType val,
						  const std::string& name)
		{
			Effect::IParameter* param = getParameter(name);

			if(NULL == param)
				return false;

			addFunctor(Binder::createFunctor<PType>(val, param));

			return true;
		}

		template <class PType>
		bool addParameter(const typename Types<PType>::GetFunction& f,
						  const std::string& name)
		{
			Effect::IParameter* param = getParameter(name);

			if(NULL == param)
				return false;

			addFunctor(Binder::createFunctor<PType>(f, param));

			return true;
		}

		template <class PType>
		bool addParameter(const typename Types<PType>::ParamTypeGetFunction& f,
						  const std::string& name)
		{
			Effect::IParameter* param = getParameter(name);

			if(NULL == param)
				return false;

			addFunctor(Binder::createFunctor<PType>(f, param));

			return true;
		}

	private:
		Binder(const PEffect& pEffect)
			: m_effect(pEffect)
		{
		}

		template <class PType>
		static Functor createFunctor(typename Types<PType>::ParamType val,
									 Effect::IParameter* param)
		{
			return boost::bind(getEffectSetFunction<PType>(), param, val);
		}

		template <class PType>
		static Functor createFunctor(const typename Types<PType>::GetFunction& f,
									 Effect::IParameter* param)
		{
			return boost::bind(getEffectSetFunction<PType>(), param,
							   boost::bind(f, _1));
		}

		template <class PType>
		static Functor createFunctor(const typename Types<PType>::ParamTypeGetFunction& f,
									 Effect::IParameter* param)
		{
			return boost::bind(getEffectSetFunction<PType>(), param,
							   boost::bind(f, _1));
		}

		template <class PType>
		static inline typename Types<PType>::EffectSetFunction getEffectSetFunction()
		{
			return (typename Types<PType>::EffectSetFunction) &Effect::IParameter::set;
		}

		Effect::IParameter* getParameter(const std::string& name) const
		{
			const Effect::Parameters& params = m_effect->getParams();
			Effect::Parameters::const_iterator it = params.find(name);

			Effect::IParameter* result = NULL;

			if(it != params.end())
				result = it->second;

			return result;
		}

		void addFunctor(const Functor& f)
		{
			m_functors.push_back(f);
		}

		typedef std::vector<Functor> Functors;

		PEffect  m_effect;
		Functors m_functors;

	};
	//Dynamic binder is used to setup 'dynamic' parameters,
	//i.e. parameters which change from object to object.
	typedef Binder<math::frame_ptr> DynamicBinder;
	typedef DynamicBinder::PBinder PDynamicBinder;

	//Static binder is used to setup 'static' parameters,
	//i.e. parameters which are the same for all objects.
	//We don't need any parameters for this binder so let's
	//use int.
	typedef Binder<int> StaticBinder;
	typedef StaticBinder::PBinder PStaticBinder;
}