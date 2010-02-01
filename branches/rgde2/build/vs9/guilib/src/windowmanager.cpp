#include "StdAfx.h"
#include "imageset.h"
#include "font.h"
#include "Renderer.h"
#include "factory.h"
#include "window.h"
#include "windowmanager.h"
#include "system.h"

#include "panel.h"
#include "cstatictext.h"
#include "cstaticimage.h"
#include "ceditbox.h"
#include "cbutton.h"
#include "ccheckbox.h"
#include "cframewindow.h"
#include "cprogress.h"
#include "cslider.h"
#include "cscrollpane.h"
#include "clist.h"
#include "ccombobox.h"
#include "cmarkuptext.h"
#include "cchatwindow.h"

namespace gui
{

WindowManager::WindowManager(System& sys, const std::string& resPath , const std::string& scheme) :
	m_system(sys),
	m_resourcePath(resPath),
	m_scheme(scheme)
{
	sys.logEvent(LogSystem, "The WindowManager was created");
	m_factory.reset(new WindowFactory(sys));

	sys.logEvent(LogSystem, "Registering window factory creators");

	m_factory->RegisterCreator("root", CreatorPtr(new RootCreator(sys, "root")));
	m_factory->RegisterCreator<BaseWindow>();	
	m_factory->RegisterCreator<StaticText>();
	m_factory->RegisterCreator<StaticImage>();
	m_factory->RegisterCreator<Button>();
	m_factory->RegisterCreator<ImageButton>();
	m_factory->RegisterCreator<Thumb>();
	m_factory->RegisterCreator<ScrollThumb>();
	m_factory->RegisterCreator<Slider>();
	m_factory->RegisterCreator<ScrollBar>();
	m_factory->RegisterCreator<Checkbox>();
	m_factory->RegisterCreator<Editbox>();
	m_factory->RegisterCreator<KeyBinder>();
	m_factory->RegisterCreator<FrameWindow>();
	m_factory->RegisterCreator<Panel>();
	m_factory->RegisterCreator<Progress>();
	m_factory->RegisterCreator<ProgressBar>();
	m_factory->RegisterCreator<ImageBar>();
	m_factory->RegisterCreator<ScrollPane>();
	m_factory->RegisterCreator<BaseList>();
	m_factory->RegisterCreator<ListBox>();
	m_factory->RegisterCreator<CategorizedList>();
	m_factory->RegisterCreator<Combobox>();
	m_factory->RegisterCreator<MarkupBase>();
	m_factory->RegisterCreator<MarkupText>();
	m_factory->RegisterCreator<ChatWindow>();
	m_registredTypes = m_factory->GetTypesList();	
}

WindowManager::~WindowManager(void)
{
}

void WindowManager::reset(bool complete)
{
	if(complete)
	{
		m_defaultImageset.reset();
		m_defaultFont.reset();
		m_imagesetRegistry.clear();
		m_fontRegistry.clear();
	}
	m_docCache.clear();
	m_loadedLuaFiles.clear();

	m_system.logEvent(LogSystem, "Loading system defaults");
	loadScheme(m_scheme);
}

void WindowManager::loadScheme(const std::string& scheme)
{
	if(scheme.empty())
		return;
	std::string filename(m_resourcePath);
	filename += "scheme\\";
	filename += scheme;
	filename += ".scheme";

	XmlDocumentPtr pdoc = loadCachedXml(filename);
	if(pdoc)
	{
		xml::node schemenode = pdoc->child("Scheme");
		if(!schemenode.empty())
		{
			xml::node setting = schemenode("DefaultImageset");
			if(!setting.empty())
			{
				std::string file(setting["file"].value());
				if(!file.empty())
				{
					if (m_defaultImageset = createImageset(file))
						m_imagesetRegistry[m_defaultImageset->getName()] = m_defaultImageset;
				}
			}
			setting = schemenode("DefaultFont");
			if(!setting.empty())
			{
				std::string file(setting["file"].value());
				if(!file.empty())
				{
					m_defaultFont = createFont(file);
					m_fontRegistry[m_defaultFont->getName()] = m_defaultFont;
				}
			}
		}
	}
}

ImagesetPtr WindowManager::loadImageset(const std::string& name)
{
	if(name.empty())
		return m_defaultImageset;

	ImagesetIt it = m_imagesetRegistry.find(name);
	if(it != m_imagesetRegistry.end())
		return it->second;

	ImagesetPtr imgset = createImageset(name + ".imageset");
	if(imgset)
	{
		m_system.logEvent(LogSystem, std::string("The imageset ") + name + " succefuly loaded.");
		m_imagesetRegistry[name] = imgset;
		return imgset;
	}
	m_system.logEvent(LogWarning, std::string("The imageset ") + name + " cannot be loaded.");
	return ImagesetPtr();
}

ImagesetPtr WindowManager::createImageset(const std::string& filename)
{
	ImagesetPtr retval;
	std::string path(m_resourcePath);
	path += "imageset\\";
	
	std::string file(path);
	file += filename;

	XmlDocumentPtr pdoc = loadCachedXml(file);
	if(!pdoc)
		return retval;
	
	xml::node imgsetnode = pdoc->child("Imageset");
	if(!imgsetnode.empty())
	{
		std::string setname = imgsetnode["Name"].value();
		std::string texfile = imgsetnode["Imagefile"].value();
		
		TexturePtr p = m_system.getRenderer().createTexture(path + texfile);
		if(p)
		{
			Imageset* imageset = new Imageset(setname, p);

			xml::node imgnode = imgsetnode.first_child();
			while(!imgnode.empty())
			{
				std::string nodename(imgnode.name());
				if(nodename == "Image")
				{
					std::string imgname = imgnode["Name"].value();
					float y = imgnode["YPos"].as_float();
					float x = imgnode["XPos"].as_float();
					float width = imgnode["Width"].as_float();
					float height = imgnode["Height"].as_float();

					imageset->defineImage(imgname, Rect(x, y, x+width, y+height));
				}
				imgnode = imgnode.next_sibling();
			}
			retval.reset(imageset);
		}
	}
	
	return retval;
}

Imageset* WindowManager::getImageset(std::string name)
{
	return loadImageset(name).get();
}

FontPtr WindowManager::loadFont(const std::string& name)
{
	if(name.empty())
		return m_defaultFont;

	FontIt it = m_fontRegistry.find(name);
	if(it != m_fontRegistry.end())
		return it->second;

	FontPtr font = createFont(name + ".font");
	if(font)
	{
		m_system.logEvent(LogSystem, std::string("The font ") + name + " succefuly loaded.");
		m_fontRegistry[name] = font;
		return font;
	}
	m_system.logEvent(LogError, std::string("The font ") + name + " cannot be loaded.");
	return FontPtr();
}

FontPtr WindowManager::createFont(const std::string& filename)
{
	FontPtr retval;

	std::string path(m_resourcePath);
	path += "font\\";
	
	std::string file(path);
	file += filename;

	XmlDocumentPtr pdoc = loadCachedXml(file);
	if(!pdoc)
		return retval;
		
	xml::node fontnode = pdoc->child("Font");
	if(!fontnode.empty())
	{
		std::string fontname = fontnode["Name"].value();
		std::string fontfile = fontnode["Filename"].value();
		std::string fonttype = fontnode["Type"].value();
		int fontsize = fontnode["Size"].as_int();
		
		try
		{
			retval = m_system.getRenderer().createFont(fontname, path + fontfile, fontsize);
		}
		catch(std::exception& e)
		{
			retval.reset();

			m_system.logEvent(LogError, e.what());
		}
	}
	
	return retval;
}

Font* WindowManager::getFont(std::string name)
{
	return loadFont(name).get();
}

WindowPtr WindowManager::createWindow(const std::string& type, const std::string& name)
{
	return m_factory->Create(type, name);
}

WindowPtr WindowManager::loadXml(const std::string& filename)
{
	WindowVector newWindows;
	WindowPtr w = createFromFile(filename, newWindows);
	
	if(!newWindows.empty())
	{
		std::for_each(newWindows.rbegin(), newWindows.rend(), boost::bind(&BaseWindow::onLoad, _1));
	}
	return w;
}

void WindowManager::loadLeafWindow(WindowPtr wnd, const std::string& xml)
{
	std::string file(m_resourcePath + xml);
	XmlDocumentPtr pdoc = loadCachedXml(file);
	if(pdoc)
	{
		loadLuaFile(xml);
		xml::node n = pdoc->first_child();
		loadWindowProperties(wnd, n);
		loadWindowEvents(wnd, n);
	}
}

WindowPtr WindowManager::createFromFile(const std::string& filename, WindowVector& newWindows)
{
	WindowPtr root;
	std::string file(m_resourcePath + filename);
	XmlDocumentPtr pdoc = loadCachedXml(file);
	if(pdoc)
	{
		loadLuaFile(filename);
		xml::node window = pdoc->first_child(); //find root
		if(!window.empty() && isWindowNode(window))
		{
			root = createWindow(WindowPtr(), window, newWindows);
		}
	}
	else
	{
		m_system.logEvent(LogWarning, std::string("The file ") + filename + " wasn't found or corrupted");
	}

	return root;
}

WindowPtr WindowManager::createWindow(WindowPtr parent, xml::node& n, WindowVector& newWindows)
{
	WindowPtr wnd;
	std::string type(n.name());
	std::string name(n["name"].value());
	std::string link(n["file"].value());

	if(parent)
	{
		wnd = parent->findChild(name);
		if(!wnd)
		{
			if(!link.empty())
			{
				wnd = createFromFile(link, newWindows);
			}
			else
			{
				wnd = createWindow(type, name);
				if(wnd)
					newWindows.push_back(wnd);
			}
			if(wnd)
			{
				std::string t(wnd->getType());
				if(type != t)
				{
					m_system.logEvent(LogError, std::string("The specified child type is mismatch existing one. Skipping child ") + name);
					return WindowPtr();
				}
			}
			parent->addChild(wnd);
		}
		else
		{
			if(!link.empty())
			{
				m_system.logEvent(LogError, std::string("An overriding the child type information is unacceptable. Skipping child ") + name);
				return WindowPtr();
			}
			std::string t(wnd->getType());
			if(type != t)
			{
				m_system.logEvent(LogError, std::string("The specified child type is mismatch existing one. Skipping child ") + name);
				return WindowPtr();
			}
		}
	}
	else
	{
		if(type != "root")
		{
			if(!link.empty())
			{
				wnd = createFromFile(link, newWindows);
			}
			else
			{
				wnd = createWindow(type, name);
				if(wnd)
					newWindows.push_back(wnd);
			}
		}
		else
		{
			wnd = createWindow(type, name);
		}
	}
	if(wnd)
	{
		if(type != "root")
		{
			wnd->setName(name);
			loadWindowProperties(wnd, n);
		}
		loadWindowEvents(wnd, n);

		xml::node children = n("Children");
		if(wnd->isCanHaveChildren() && !children.empty())
		{
			xml::node child = children.first_child();
			while(!child.empty())
			{
				if(isWindowNode(child))
				{
					createWindow(wnd, child, newWindows);
				}
				child = child.next_sibling();
			}
		}
	}

	return wnd;
}
void WindowManager::loadWindowProperties(WindowPtr wnd, xml::node& n)
{
	xml::node properties = n("Properties");
	if(!properties.empty())
		wnd->init(properties);
}
void WindowManager::loadWindowEvents(WindowPtr wnd, xml::node& n)
{
	xml::node events = n("Events");
	if(!events.empty())
		wnd->parseEventHandlers(events);
}

XmlDocumentPtr WindowManager::loadCachedXml(const std::string& file)
{
	XmlDocumentPtr retval;
	
	DocumentCacheIt cache = m_docCache.find(file);
	if(cache != m_docCache.end())
	{
		retval = cache->second;
	}
	else
	{	
		retval.reset(new xml::document());
		if(!retval->load_file(file.c_str()))
			retval.reset();
		else
			m_docCache[file] = retval;
	}
	return retval;
}

bool WindowManager::isWindowNode(xml::node& node) const
{
	std::string type(node.name());
	if(!type.empty())
	{
		TypesList::const_iterator it = std::find(m_registredTypes.begin(), m_registredTypes.end(), type);
		if(it != m_registredTypes.end())
			return true;
	}
	return false;
}

void WindowManager::loadLuaFile(const std::string& xmlfile)
{
	if(xmlfile.empty())
		return;
	size_t delim = xmlfile.find_last_of(".");
	std::string name;
	if(delim != std::string::npos)
		name = xmlfile.substr(0, delim);
	else
		name = xmlfile;

	if(!name.empty())
	{		
		name += ".lua";
		LuaFilesVector::iterator it = std::find(m_loadedLuaFiles.begin(), m_loadedLuaFiles.end(), name);
		if(it == m_loadedLuaFiles.end())
		{
			m_loadedLuaFiles.push_back(name);
			std::string file(m_resourcePath + name);
			m_system.executeScript(file);
		}
	}
}

void WindowManager::onLoaded(WindowPtr wnd)
{
	if(wnd)
	{
		wnd->onLoad();
		
		BaseWindow::ChildrenList& children = wnd->getChildren();
		BaseWindow::ChildrenIter i = children.begin();
		BaseWindow::ChildrenIter end = children.end();
		while(i != end)
		{
			onLoaded((*i));
			++i;
		}
	}
}

}
