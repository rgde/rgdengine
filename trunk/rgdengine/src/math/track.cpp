#include "precompiled.h"

#include <rgde/math/track.h>

#include <boost/lexical_cast.hpp> //возможно стоит переделать на base::Lexical_cast [Dub]

namespace math
{
    //принять изменения во внешнем масиве
    void CTrack::apply()
    {
        //вспомогательный сплайн для вычисления расстояния на сплайне
        Vec3fCatmulRomDistSpline spline;

        //чистим
        m_splineUp.m_values.swap(Vec3fCatmulRomSpline::List());
        m_splineEyePt.m_values.swap(Vec3fCatmulRomSpline::List());
        m_splineLookatPt.m_values.swap(Vec3fCatmulRomSpline::List());
        m_splineSpeed.m_values.swap(FloatCatmulRomSpline::List());
        m_time2parameter.m_values.swap(FloatLinearInterpolatorf::Map());

        //заполняем
        List::iterator i = m_values.begin();
        while (i != m_values.end())
        {
            m_splineUp.m_values.push_back(i->m_vUp);
            m_splineEyePt.m_values.push_back(i->m_vEyePt);
            m_splineLookatPt.m_values.push_back(i->m_vLookatPt);
            m_splineSpeed.m_values.push_back(i->m_fSpeed);

            spline.m_values.push_back(i->m_vEyePt);

            ++i;
        }

        m_splineUp.apply();
        m_splineEyePt.apply();
        m_splineLookatPt.apply();
        m_splineSpeed.apply();

        spline.apply();

        //ключевой момент - заполнение массива соотвествия "время" <-> "параметр по умолчанию"
        float t = 0;
        float u = 0;
        m_time2parameter.addValue(t,u);

        const float step = spline.length() / (m_values.size()*20.f);
        for (float position=0; position<spline.length(); position+=step)
        {
            u = spline.getParameter(position);
            float speed = m_splineSpeed(u);

            t += step/speed; 

            m_time2parameter.addValue(t,u);
        }
    }

    //отменить изменения во внешнем масиве
    void CTrack::undo()
    {
        m_values.swap(List());

        Vec3fCatmulRomSpline::iterator i_up     = m_splineUp.m_values.begin();
        Vec3fCatmulRomSpline::iterator i_eyept  = m_splineEyePt.m_values.begin();
        Vec3fCatmulRomSpline::iterator i_lookat = m_splineLookatPt.m_values.begin();
        FloatCatmulRomSpline::iterator i_speed  = m_splineSpeed.m_values.begin();

        //все сплайны должны иметь одинаковый размер,
        //поэтому можно отлавливать выход из цикла
        //только по окончанию одного из сплайнов
        while (i_up != m_splineUp.m_values.end())
        {
            Key key;

            key.m_vUp       = *i_up;
            key.m_vEyePt    = *i_eyept;
            key.m_vLookatPt = *i_lookat;
            key.m_fSpeed    = *i_speed;

            m_values.push_back(key);

            ++i_up;
            ++i_eyept;
            ++i_lookat;
            ++i_speed;
        }
    }

    //загрузка трека
	void CTrack::load(const std::string& strTrackFileName)
    {
		//base::lmsg << "loading track: " << "\"" << strTrackFileName << "\"";
		io::CFileSystem& fs = io::TheFileSystem::get();
		io::readstream_ptr in = fs.findFile(strTrackFileName);

		if (!in)
		{
			io::ScopePathAdd p("Tracks/");
			in = fs.findFile(strTrackFileName);
		}

		if (!in)
		{
			std::string error = "CTrack::load: can't load file " + strTrackFileName;
			//base::lerr << "CTrack::load: can't load file: " << strTrackFileName;
			//throw std::exception(error.c_str());
            throw std::exception("CTrack::load: can't load file");
		}

		std::vector<byte> data;
		io::stream_to_vector(data, in);

		TiXmlDocument xml;
		xml.Parse((const char*)&(data[0]));

        m_values.swap(List());

		TiXmlElement *mod_elem = xml.RootElement();
		TiXmlElement *elem = mod_elem->FirstChild("keys")->ToElement();

		if (elem)
		{
            TiXmlNode *node;
			TiXmlElement *tx = 0;
            while (node = elem->IterateChildren("key",tx))
			{
                tx = node->ToElement();
                if (!tx)
                    break;

				TiXmlElement *up = tx->FirstChild("up")->ToElement();
				TiXmlElement *eye = tx->FirstChild("eye")->ToElement();
				TiXmlElement *lookat = tx->FirstChild("lookat")->ToElement();

				double t;
				float fSpeed;
				math::Vec3f vUp;
				math::Vec3f vEye;
				math::Vec3f vLookAt;

				tx->Attribute("speed", &t); fSpeed = (float)t;

				up->Attribute("x", &t); vUp.mData[0] = (float)t;
				up->Attribute("y", &t); vUp.mData[1] = (float)t;
				up->Attribute("z", &t); vUp.mData[2] = (float)t;

				eye->Attribute("x", &t); vEye.mData[0] = (float)t;
				eye->Attribute("y", &t); vEye.mData[1] = (float)t;
				eye->Attribute("z", &t); vEye.mData[2] = (float)t;

				lookat->Attribute("x", &t); vLookAt.mData[0] = (float)t;
				lookat->Attribute("y", &t); vLookAt.mData[1] = (float)t;
				lookat->Attribute("z", &t); vLookAt.mData[2] = (float)t;

				Key key;
				key.m_vUp = vUp;
				key.m_vEyePt = vEye;
				key.m_vLookatPt = vLookAt;
				key.m_fSpeed = fSpeed;
				m_values.push_back(key);
			}	                
        }

        apply();
    }

    //сохранение трека
	void CTrack::save(const std::string& strTrackFileName)
    {
        //undo();

		TiXmlDocument doc;

		TiXmlElement *track = (TiXmlElement*)(doc.InsertEndChild(TiXmlElement("track")));
		TiXmlElement *keys = (TiXmlElement*)(track->InsertEndChild(TiXmlElement("keys")));

		List::iterator i = this->m_values.begin();
		while (i != this->m_values.end())
		{
			TiXmlElement *key = (TiXmlElement*)(keys->InsertEndChild(TiXmlElement("key")));
			key->SetAttribute("speed", boost::lexical_cast<std::string>(i->m_fSpeed));

			TiXmlElement *up = (TiXmlElement*)(key->InsertEndChild(TiXmlElement("up")));
			up->SetAttribute("x",boost::lexical_cast<std::string>(i->m_vUp[0]));
			up->SetAttribute("y",boost::lexical_cast<std::string>(i->m_vUp[1]));
			up->SetAttribute("z",boost::lexical_cast<std::string>(i->m_vUp[2]));

			TiXmlElement *eye = (TiXmlElement*)(key->InsertEndChild(TiXmlElement("eye")));
			eye->SetAttribute("x",boost::lexical_cast<std::string>(i->m_vEyePt[0]));
			eye->SetAttribute("y",boost::lexical_cast<std::string>(i->m_vEyePt[1]));
			eye->SetAttribute("z",boost::lexical_cast<std::string>(i->m_vEyePt[2]));

			TiXmlElement *lookat = (TiXmlElement*)(key->InsertEndChild(TiXmlElement("lookat")));
			lookat->SetAttribute("x",boost::lexical_cast<std::string>(i->m_vLookatPt[0]));
			lookat->SetAttribute("y",boost::lexical_cast<std::string>(i->m_vLookatPt[1]));
			lookat->SetAttribute("z",boost::lexical_cast<std::string>(i->m_vLookatPt[2]));

			++i;
		}

		//std::string s(strTrackFileName.begin(), strTrackFileName.end());
		//s = "./Media/Tracks/"+s;
        std::string s = "./Media/Tracks/"+strTrackFileName;

		doc.SaveFile(s);
    }

    //узнать общую длительность трека
    float CTrack::getTotalTime()
    {
        if (m_time2parameter.m_values.size() <= 1)
            return 0;
        return m_time2parameter.m_values.rbegin()->first -
               m_time2parameter.m_values.begin()->first;
    }

    //получение точки трека
    CTrack::Key CTrack::getKey(float t)
    {
        Key key;

        float u = m_time2parameter(t);

        key.m_vUp       = m_splineUp(u);
        key.m_vEyePt    = m_splineEyePt(u);
        key.m_vLookatPt = m_splineLookatPt(u);
        key.m_fSpeed    = m_splineSpeed(u);

        return key;
    }
}