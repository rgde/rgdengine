/*

@author PC
*/
#pragma once

class IUpdateController
{
public:

	virtual ~IUpdateController() { }

	virtual void update(float dt) = 0;

};

typedef boost::shared_ptr<IUpdateController> PUpdateController;

class IStateController
{
public:

	virtual ~IStateController() { }

	virtual void setStateNorm(float normTime, float weight) = 0;
	virtual void addStateNorm(float normTime, float weight) = 0;

};

typedef boost::shared_ptr<IStateController> PStateController;

class ITimeController
{
public:

			ITimeController(float fullTime) { setFullTime(1.0f); setFullTime(fullTime); }
	virtual ~ITimeController() { }

			float getFullTime() const { return m_fullTime; }

protected:

			void setFullTime(float fullTime)
			{
				if(fullTime > 0.0f)
					m_fullTime = fullTime;
			}

private:

	float m_fullTime;

};

typedef boost::shared_ptr<ITimeController> PTimeController;

class ITimeStateController : public ITimeController,
							 public IStateController
{
public:

			ITimeStateController(float fullTime) : ITimeController(fullTime) { }
	virtual ~ITimeStateController() { }

			void setState(float time, float weight) { setStateNorm(time/getFullTime(), weight); }
			void addState(float time, float weight) { addStateNorm(time/getFullTime(), weight); }

protected:

	virtual void setStateNorm(float normTime, float weight) = 0;
	virtual void addStateNorm(float normTime, float weight) = 0;

};

typedef boost::shared_ptr<ITimeStateController> PTimeStateController;

class ITimeCounterController : public ITimeController,
							   public IUpdateController
{
public:

		    ITimeCounterController(float fullTime);
	virtual ~ITimeCounterController();

	        bool  getEnabled() const;
			void  setEnabled(bool bEnabled);

			bool  getLooped() const;
			void  setLooped(bool bLooped);

			float getNormTime() const;
			void  setNormTime(float normTime);

			float getTime() const;
			void  setTime(float time);

			float getRate() const;
			void  setRate(float fRate);

			void  update(float dt);
			void  updateState();

			void  play();
			void  start();
			void  pause();
			void  stop();

			void  reset();

protected:

	virtual void setStateNorm(float normTime) = 0;

private:

	bool  m_bEnabled;
	bool  m_bLooped;
	float m_fNormTime;
	float m_fRate;

};

typedef boost::shared_ptr<ITimeCounterController> PTimeCounterController;