#pragma once

#include <boost/scoped_ptr.hpp>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace xml {
	class node;
	class document;
};

typedef boost::shared_ptr<xml::document> XmlDocumentPtr;

namespace gui
{

class Font;
typedef boost::shared_ptr<Font> FontPtr;
class Imageset;
typedef boost::shared_ptr<Imageset> ImagesetPtr;

class System;
class BaseWindow;
typedef boost::intrusive_ptr<BaseWindow> WindowPtr;

class WindowFactory;

class  WindowManager
{
public:
	WindowManager(System& sys, const std::string& resPath, const std::string& scheme);
	~WindowManager(void);

	WindowPtr createWindow(const std::string& type, const std::string& name);
	WindowPtr loadXml(const std::string& filename);

	// public resource creators:
	ImagesetPtr loadImageset(const std::string& name);
	FontPtr loadFont(const std::string& name);

	Imageset*	getImageset(std::string name);
	Font*		getFont(std::string name);

	void loadLeafWindow(WindowPtr wnd, const std::string& xml);

	void reset(bool complete);

protected:
	typedef std::vector<WindowPtr> WindowVector;
	void loadScheme(const std::string& scheme);

	WindowPtr createFromFile(const std::string& file, WindowVector& newWindows);
	WindowPtr createWindow(WindowPtr parent, xml::node& n, WindowVector& newWindows);
	
	void loadWindowProperties(WindowPtr wnd, xml::node& n);
	void loadWindowEvents(WindowPtr wnd, xml::node& n);
	void onLoaded(WindowPtr wnd);
	
	ImagesetPtr createImageset(const std::string& filename);
	FontPtr createFont(const std::string& filename);
	
	XmlDocumentPtr loadCachedXml(const std::string& name);
	bool isWindowNode(xml::node& node) const;
	void loadLuaFile(const std::string& xmlfile);
	
protected:
	WindowManager& operator=(const WindowManager&) { return *this; }
	boost::scoped_ptr<WindowFactory> m_factory;
	std::string m_resourcePath; // path to a GUI root directory
	std::string m_scheme;
	System&		m_system;
	
	ImagesetPtr m_defaultImageset;
	FontPtr		m_defaultFont;
	typedef std::list<std::string> TypesList;
	TypesList m_registredTypes;

	typedef std::map<std::string, ImagesetPtr> ImagesetMap;
	typedef ImagesetMap::iterator ImagesetIt;
	ImagesetMap m_imagesetRegistry;

	typedef std::map<std::string, FontPtr> FontMap;
	typedef FontMap::iterator FontIt;
	FontMap m_fontRegistry;

	typedef std::map<std::string, XmlDocumentPtr> DocumentCacheMap;
	typedef DocumentCacheMap::iterator DocumentCacheIt;
	DocumentCacheMap m_docCache;

	typedef std::vector<std::string> LuaFilesVector;
	LuaFilesVector m_loadedLuaFiles;	
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif