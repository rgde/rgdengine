#pragma once


namespace gui
{


//!< 32 bit ARGB representation of a Color.
typedef unsigned int argb_t;    

/*!
\brief
	Class representing Color values within the system.
*/
class GUILIB_API Color
{
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	Color(void);
	Color(const Color& val);
	Color(float red, float green, float blue, float alpha = 1.0f);
	Color(argb_t argb);

	/*************************************************************************
		Accessors
	*************************************************************************/
	argb_t	getARGB(void) const
	{
		if (!m_argbValid)
		{
			m_argb = calculateARGB();
			m_argbValid = true;
		}

		return m_argb;
	}
	
	float	getAlpha(void) const	{return m_alpha;}
	float	getRed(void) const		{return m_red;}
	float	getGreen(void) const	{return m_green;}
	float	getBlue(void) const		{return m_blue;}

	float	getHue(void) const;
	float	getSaturation(void) const;
	float	getLumination(void) const;


	/*************************************************************************
		Manipulators
	*************************************************************************/
	void	setARGB(argb_t argb);
	inline void setAlpha(float alpha)
    {
        m_argbValid = false;
        m_alpha = alpha;
    }

	inline void setRed(float red)
    {   
        m_argbValid = false;
        m_red = red;
    }

	inline void setGreen(float green)
    {
        m_argbValid = false;
        m_green = green;
    }

	inline void setBlue(float blue)
    {
        m_argbValid = false;
        m_blue = blue;
    }

	inline void set(float red, float green, float blue, float alpha = 1.0f)
    {
        m_argbValid = false;
        m_alpha = alpha;
        m_red = red;
        m_green = green;
        m_blue = blue;
    }

	inline void setRGB(float red, float green, float blue)
    {
        m_argbValid = false;
        m_red = red;
        m_green = green;
        m_blue = blue;
    }

	inline void setRGB(const Color& val)
    {
        m_red = val.m_red;
        m_green = val.m_green;
        m_blue = val.m_blue;
        if (m_argbValid)
        {
            m_argbValid = val.m_argbValid;
            if (m_argbValid)
                m_argb = (m_argb & 0xFF000000) | (val.m_argb & 0x00FFFFFF);
        }
    }

	void	setHSL(float hue, float saturation, float luminance, float alpha = 1.0f);

	void	invertColour(void);
	void	invertColourWithAlpha(void);

	/*************************************************************************
		Operators
	*************************************************************************/
	inline Color& operator=(argb_t val)
    {
        setARGB(val);
        return *this;
    }

	inline Color& operator=(const Color& val)
    {
        m_alpha = val.m_alpha;
        m_red   = val.m_red;
        m_green = val.m_green;
        m_blue  = val.m_blue;
        m_argb  = val.m_argb;
        m_argbValid = val.m_argbValid;

        return *this;
    }

	inline Color& operator&=(argb_t val)
    {
        setARGB(getARGB() & val);
        return *this;
    }

	inline Color& operator&=(const Color& val)
    {
        setARGB(getARGB() & val.getARGB());
        return *this;
    }

	inline Color& operator|=(argb_t val)
    {
        setARGB(getARGB() | val);
        return *this;
    }

	inline Color& operator|=(const Color& val)
    {
        setARGB(getARGB() | val.getARGB());
        return *this;
    }

	inline Color& operator<<=(int val)
    {
        setARGB(getARGB() << val);
        return *this;
    }

	inline Color& operator>>=(int val)
    {
        setARGB(getARGB() >> val);
        return *this;
    }

	inline Color operator+(const Color& val) const
    {
        return Color(
            m_red   + val.m_red, 
            m_green + val.m_green, 
            m_blue  + val.m_blue,
            m_alpha + val.m_alpha
        );
    }

	inline Color operator-(const Color& val) const
    {
        return Color(
            m_red   - val.m_red,
            m_green - val.m_green,
            m_blue  - val.m_blue,
            m_alpha - val.m_alpha
        );
    }

	inline Color operator*(const float val) const
    {       
        return Color(
            m_red   * val, 
            m_green * val, 
            m_blue  * val,
            m_alpha * val 
        );  
    }

    inline Color& operator*=(const Color& val)
    {
        m_red *= val.m_red;
        m_blue *= val.m_blue;
        m_green *= val.m_green;
        m_alpha *= val.m_alpha;

		m_argbValid = false;

        return *this;
    }

	/*************************************************************************
		Compare operators
	*************************************************************************/
	inline bool operator==(const Color& rhs) const
    {
        return m_red   == rhs.m_red   &&
               m_green == rhs.m_green &&
               m_blue  == rhs.m_blue  &&
               m_alpha == rhs.m_alpha;
    }

	inline bool operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }

	//
	// Conversion operators
	//
	operator argb_t() const		{return getARGB();}

private:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		calculate and return the ARGB value based on the current Color component values.
	*/
	argb_t	calculateARGB(void) const;

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	float m_alpha, m_red, m_green, m_blue;		//!< Colour components.
	mutable argb_t m_argb;						//!< Colour as ARGB value.
	mutable bool m_argbValid;					//!< True if argb value is valid.
};

Color StringToColor(const std::string& str);
std::string ColorToString(const Color& val);
Color HexStringToColor(const std::string& str);
std::string ColorToHexString(const Color& val);

}