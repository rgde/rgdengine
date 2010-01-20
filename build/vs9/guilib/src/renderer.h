#pragma once

#include "rect.h"
#include "colorRect.h"
#include "texture.h"
#include "texmanager.h"
#include "image.h"
#include "log.h"
#include "renderCallback.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace gui
{

class Font;
typedef boost::shared_ptr<Font> FontPtr;

enum OrientationFlags
{
	FlipHorizontal		= 1,	//!< Horizontal flip the image
	FlipVertical		= 2,	//!< Vertical flip the image
	RotateRightAngle	= 4		//!< Rotate the image anticlockwise 90 degree
};

enum QuadSplitMode
{
	TopLeftToBottomRight,	//!< Diagonal goes from top-left to bottom-right
	BottomLeftToTopRight	//!< Diagonal goes from bottom-left to top-right
};

class  Renderer
{
public:
	struct QuadInfo
	{
		Texture*			texture;
		Rect				position;
		float				z;
		Rect				texPosition;
		unsigned long		topLeftCol;
		unsigned long		topRightCol;
		unsigned long		bottomLeftCol;
		unsigned long		bottomRightCol;

        QuadSplitMode       splitMode;

		bool operator<(const QuadInfo& other) const
		{
			// this is intentionally reversed.
			return z > other.z;
		}
	};

	struct RenderCallbackInfo
	{
		// для коллбеков
		AfterRenderCallbackFunc afterRenderCallback;
		BaseWindow* window;
		Rect dest;
		Rect clip;
	};

	struct BatchInfo
	{
		Texture* texture;
		std::size_t startQuad;
		std::size_t numQuads;

		RenderCallbackInfo callbackInfo;
	};

	Renderer(void);
	virtual ~Renderer(void);

	virtual void	addCallback( AfterRenderCallbackFunc callback,
								 BaseWindow* window, const Rect& dest, const Rect& clip) = 0;
	virtual void	draw(const Image& img, const Rect& dest_rect, float z, const Rect& clip_rect, const ColorRect& colors, QuadSplitMode quad_split_mode);
	virtual void	draw(const Image& img, const Rect& dest_rect, float z, const Rect& clip_rect, const ColorRect& colors, QuadSplitMode quad_split_mode, Image::ImageOps horz, Image::ImageOps vert);
	virtual void	immediateDraw(const Image& img, const Rect& dest_rect, float z, const Rect& clip_rect, const ColorRect& colors, QuadSplitMode quad_split_mode);

	virtual	void	doRender(void) = 0;
	virtual	void	clearRenderList(void);	

	virtual void	beginBatching();
	virtual void	endBatching();
	void			clearCache();
	
	virtual void	setQueueingEnabled(bool setting)  { m_isQueueing = setting; }
	bool	isQueueingEnabled(void) const { return m_isQueueing; }

	virtual	TexturePtr	createTexture(const std::string& filename) {throw std::exception("Unsupported operation!");}
	virtual	void		destroyTexture(TexturePtr tex) {};
	virtual	TexturePtr	loadFromMemory(const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat) = 0;
	
	//temporary
	virtual	TexturePtr	createEmptyTexture(unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat) = 0;
	virtual TexturePtr	reloadTextureFromBuffer(TexturePtr p, const void* buffPtr, unsigned int buffWidth, unsigned int buffHeight, Texture::PixelFormat pixFormat) = 0;

	virtual FontPtr		createFont(const std::string& name, const std::string& fontname, unsigned int size) = 0;


	virtual void startCaptureForCache(BaseWindow* window) ;
	virtual void endCaptureForCache(BaseWindow* window) ;
	virtual void drawFromCache(BaseWindow* window) {}


	const Size&	getOriginalSize(void) const	{ return m_originalsize; }
	const Size	getSize(void);
	unsigned int getMaxTextureSize(void) const { return m_maxTextureSize; }
	unsigned int getHorzScreenDPI(void) const { return m_hdpi; }
	unsigned int getVertScreenDPI(void) const { return m_vdpi; }

	void	setOriginalSize(const Size& sz) { m_originalsize = sz; }

	void	resetZValue(void)				{m_current_z = GuiZInitialValue;}
	void	advanceZValue(void)				{m_current_z -= GuiZElementStep;}
	float	getCurrentZ(void) const			{return m_current_z;}
	float	getZLayer(unsigned int layer) const		{return m_current_z - ((float)layer * GuiZLayerStep);}

	virtual void	OnResetDevice();
	virtual void	OnLostDevice();

	void setAutoScale(bool status) { m_autoScale = status; }
	bool isAutoScale() { return m_autoScale; }
	virtual Size getViewportSize(void) const = 0;

	Rect virtualToRealCoord( const Rect& virtualRect ) const;
	Rect realToVirtualCoord( const Rect& realRect ) const;

	void setLogCallback(LoggerCallback log_cb) {m_log_cb = log_cb;}

	void cleanup(bool complete);

protected:
	virtual	void addQuad(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode) {}
	
	void sortQuads(void);
	virtual void renderQuadDirect(const Rect& dest_rect, const Rect& tex_rect, float z, const Image& img, const ColorRect& colours, QuadSplitMode quad_split_mode) = 0;
	
	friend class TextureManager;
	virtual	TexturePtr	createTextureInstance(const std::string& filename) { throw std::exception("Unsupported operation!"); }

protected:
	Renderer& operator=(const Renderer&) { return *this; }
	void computeVirtualDivRealFactor(Size& coefOut) const;
	
	TextureManager m_texmanager;

	
	typedef std::vector<QuadInfo> Quads;
	typedef std::vector<BatchInfo> Batches;

	Quads m_quads;
	Batches m_batches;

	size_t m_num_quads;
	size_t m_num_batches;
	
	Size m_originalsize;
	unsigned int m_hdpi;
	unsigned int m_vdpi;
	unsigned int m_maxTextureSize;

	bool m_isQueueing;
	bool m_autoScale;

	const float	GuiZInitialValue;
	const float	GuiZElementStep;
	const float	GuiZLayerStep;
	float	m_current_z;

	LoggerCallback m_log_cb;


	typedef std::vector <QuadInfo> CachedQuadList;
	struct QuadCacheRecord
	{	
		CachedQuadList m_vec;
		std::size_t num;
	};
	typedef std::map <BaseWindow*, QuadCacheRecord> QuadCacheMap;
	QuadCacheMap m_mapQuadList;
	QuadCacheRecord* m_currentCapturing;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif