//----------------------------------------------------------------------------------------
//Blending modes.
//----------------------------------------------------------------------------------------

#define NO_BLENDING       \
AlphaBlendEnable = FALSE; \
SrcBlend         = ZERO;  \
DestBlend        = ZERO;

#define ALPHA_BLENDING         \
AlphaBlendEnable = TRUE;       \
SrcBlend         = SRCALPHA;   \
DestBlend        = INVSRCALPHA;

#define ADDITIVE_BLENDING \
AlphaBlendEnable = TRUE;  \
SrcBlend         = ONE;	  \
DestBlend        = ONE;

#define ADDITIVE_BLENDING2   \
AlphaBlendEnable = TRUE;     \
SrcBlend         = SRCALPHA; \
DestBlend        = ONE;