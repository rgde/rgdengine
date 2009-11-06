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
            m_splineUp.m_values.push_back(i->m_up);
            m_splineEyePt.m_values.push_back(i->m_eye_pos);
            m_splineLookatPt.m_values.push_back(i->m_lookat_pt);
            m_splineSpeed.m_values.push_back(i->m_speed);

            spline.m_values.push_back(i->m_eye_pos);

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

            key.m_up       = *i_up;
            key.m_eye_pos    = *i_eyept;
            key.m_lookat_pt = *i_lookat;
            key.m_speed    = *i_speed;

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
		io::readstream_ptr in = fs.find(strTrackFileName);

		if (!in)
		{
			io::path_add_scoped p("Tracks/");
			in = fs.find(strTrackFileName);
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

				TiXmlElement *up_el = tx->FirstChild("up")->ToElement();
				TiXmlElement *eye_el = tx->FirstChild("eye")->ToElement();
				TiXmlElement *lookat_el = tx->FirstChild("lookat")->ToElement();

				double t;
				float speed;
				math::vec3f up;
				math::vec3f eye;
				math::vec3f look_at;

				tx->Attribute("speed", &t); speed = (float)t;

				up_el->Attribute("x", &t); up.mData[0] = (float)t;
				up_el->Attribute("y", &t); up.mData[1] = (float)t;
				up_el->Attribute("z", &t); up.mData[2] = (float)t;

				eye_el->Attribute("x", &t); eye.mData[0] = (float)t;
				eye_el->Attribute("y", &t); eye.mData[1] = (float)t;
				eye_el->Attribute("z", &t); eye.mData[2] = (float)t;

				lookat_el->Attribute("x", &t); look_at.mData[0] = (float)t;
				lookat_el->Attribute("y", &t); look_at.mData[1] = (float)t;
				lookat_el->Attribute("z", &t); look_at.mData[2] = (float)t;

				Key key;
				key.m_up = up;
				key.m_eye_pos = eye;
				key.m_lookat_pt = look_at;
				key.m_speed = speed;
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
			key->SetAttribute("speed", boost::lexical_cast<std::string>(i->m_speed));

			TiXmlElement *up = (TiXmlElement*)(key->InsertEndChild(TiXmlElement("up")));
			up->SetAttribute("x",boost::lexical_cast<std::string>(i->m_up[0]));
			up->SetAttribute("y",boost::lexical_cast<std::string>(i->m_up[1]));
			up->SetAttribute("z",boost::lexical_cast<std::string>(i->m_up[2]));

			TiXmlElement *eye = (TiXmlElement*)(key->InsertEndChild(TiXmlElement("eye")));
			eye->SetAttribute("x",boost::lexical_cast<std::string>(i->m_eye_pos[0]));
			eye->SetAttribute("y",boost::lexical_cast<std::string>(i->m_eye_pos[1]));
			eye->SetAttribute("z",boost::lexical_cast<std::string>(i->m_eye_pos[2]));

			TiXmlElement *lookat = (TiXmlElement*)(key->InsertEndChild(TiXmlElement("lookat")));
			lookat->SetAttribute("x",boost::lexical_cast<std::string>(i->m_lookat_pt[0]));
			lookat->SetAttribute("y",boost::lexical_cast<std::string>(i->m_lookat_pt[1]));
			lookat->SetAttribute("z",boost::lexical_cast<std::string>(i->m_lookat_pt[2]));

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

        key.m_up       = m_splineUp(u);
        key.m_eye_pos    = m_splineEyePt(u);
        key.m_lookat_pt = m_splineLookatPt(u);
        key.m_speed    = m_splineSpeed(u);

        return key;
    }
}