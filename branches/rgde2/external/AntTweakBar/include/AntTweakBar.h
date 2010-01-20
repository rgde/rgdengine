// ----------------------------------------------------------------------------
//
//  @file       AntTweakBar.h
//
//  @brief      AntTweakBar is a light and intuitive graphical user interface 
//              that can be readily integrated into OpenGL and DirectX 
//              applications in order to interactively tweak them.
//
//  @author     Philippe Decaudin - http://www.antisphere.com
//  @date       2005/09/20
//
//  @doc        http://www.antisphere.com/Wiki/tools:anttweakbar
//
//  @license    This file is part of the AntTweakBar library.
//              AntTweakBar is a free software released under the zlib license.
//              For conditions of distribution and use, see License.txt
//
// ----------------------------------------------------------------------------


#if !defined TW_INCLUDED
#define TW_INCLUDED

#include <stddef.h>

#define TW_VERSION  113 // Version Mmm : M=Major mm=minor (e.g., 102 is version 1.02)


#ifdef  __cplusplus
#   if defined(_MSC_VER)
#       pragma warning(push)
#       pragma warning(disable: 4995 4530)
#       include <string>
#       pragma warning(pop)
#   else
#       include <string>
#   endif
    extern "C" {
#endif  // __cplusplus


// ----------------------------------------------------------------------------
//  OS specific definitions
// ----------------------------------------------------------------------------
#   define TW_CALL
#   define TW_EXPORT_API
#   define TW_IMPORT_API

#define TW_STATIC


// ----------------------------------------------------------------------------
//  Bar functions and definitions
// ----------------------------------------------------------------------------

typedef struct CTwBar TwBar; // structure CTwBar is not exposed.

TwBar *      TwNewBar(const char *barName);
int          TwDeleteBar(TwBar *bar);
int          TwDeleteAllBars();
int          TwSetTopBar(const TwBar *bar);
TwBar *      TwGetTopBar();
int          TwSetBottomBar(const TwBar *bar);
TwBar *      TwGetBottomBar();
const char * TwGetBarName(TwBar *bar);
int          TwGetBarCount();
TwBar *      TwGetBarByIndex(int barIndex);
TwBar *      TwGetBarByName(const char *barName);
int          TwRefreshBar(TwBar *bar);


// ----------------------------------------------------------------------------
//  Var functions and definitions
// ----------------------------------------------------------------------------

typedef enum ETwType
{
    TW_TYPE_UNDEF   = 0,
#ifdef __cplusplus
    TW_TYPE_BOOLCPP = 1,
#endif // __cplusplus
    TW_TYPE_BOOL8   = 2,
    TW_TYPE_BOOL16,
    TW_TYPE_BOOL32,
    TW_TYPE_CHAR,
    TW_TYPE_INT8,
    TW_TYPE_UINT8,
    TW_TYPE_INT16,
    TW_TYPE_UINT16,
    TW_TYPE_INT32,
    TW_TYPE_UINT32,
    TW_TYPE_FLOAT,
    TW_TYPE_DOUBLE,
    TW_TYPE_COLOR32,    // 32 bits color. Order is RGBA if API is OpenGL or Direct3D10, and inversed if API is Direct3D9 (can be modified by defining 'colorOrder=...', see doc)
    TW_TYPE_COLOR3F,    // 3 floats color. Order is RGB.
    TW_TYPE_COLOR4F,    // 4 floats color. Order is RGBA.
    TW_TYPE_CDSTRING,   // Null-terminated C Dynamic String (pointer to an array of char dynamically allocated with malloc/realloc/strdup)
#ifdef __cplusplus
    TW_TYPE_STDSTRING = (0x2fff0000+sizeof(std::string)),  // C++ STL string (std::string)
#endif // __cplusplus
    TW_TYPE_QUAT4F = TW_TYPE_CDSTRING+2, // 4 floats encoding a quaternion {qx,qy,qz,qs}
    TW_TYPE_QUAT4D,     // 4 doubles encoding a quaternion {qx,qy,qz,qs}
    TW_TYPE_DIR3F,      // direction vector represented by 3 floats
    TW_TYPE_DIR3D       // direction vector represented by 3 doubles
} TwType;
#define TW_TYPE_CSSTRING(n) ((TwType)(0x30000000+((n)&0xfffffff))) // Null-terminated C Static String of size n (defined as char[n], with n<2^28)

typedef void (* TwSetVarCallback)(const void *value, void *clientData);
typedef void (* TwGetVarCallback)(void *value, void *clientData);
typedef void (* TwButtonCallback)(void *clientData);

int      TwAddVarRW(TwBar *bar, const char *name, TwType type, void *var, const char *def);
int      TwAddVarRO(TwBar *bar, const char *name, TwType type, const void *var, const char *def);
int      TwAddVarCB(TwBar *bar, const char *name, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void *clientData, const char *def);
int      TwAddButton(TwBar *bar, const char *name, TwButtonCallback callback, void *clientData, const char *def);
int      TwAddSeparator(TwBar *bar, const char *name, const char *def);
int      TwRemoveVar(TwBar *bar, const char *name);
int      TwRemoveAllVars(TwBar *bar);

typedef struct CTwEnumVal
{
    int           Value;
    const char *  Label;
} TwEnumVal;
typedef struct CTwStructMember
{
    const char *  Name;
    TwType        Type;
    size_t        Offset;
    const char *  DefString;
} TwStructMember;
typedef void (* TwSummaryCallback)(char *summaryString, size_t summaryMaxLength, const void *value, void *clientData);

int      TwDefine(const char *def);
TwType   TwDefineEnum(const char *name, const TwEnumVal *enumValues, unsigned int nbValues);
TwType   TwDefineStruct(const char *name, const TwStructMember *structMembers, unsigned int nbMembers, size_t structSize, TwSummaryCallback summaryCallback, void *summaryClientData);

typedef void (* TwCopyCDStringToClient)(char **destinationClientStringPtr, const char *sourceString);
void     TwCopyCDStringToClientFunc(TwCopyCDStringToClient copyCDStringFunc);
void     TwCopyCDStringToLibrary(char **destinationLibraryStringPtr, const char *sourceClientString);
#ifdef __cplusplus
typedef void (* TwCopyStdStringToClient)(std::string& destinationClientString, const std::string& sourceString);
void     TwCopyStdStringToClientFunc(TwCopyStdStringToClient copyStdStringToClientFunc);
void     TwCopyStdStringToLibrary(std::string& destinationLibraryString, const std::string& sourceClientString);
#endif // __cplusplus

typedef enum ETwParamValueType
{
    TW_PARAM_INT32,
    TW_PARAM_FLOAT,
    TW_PARAM_DOUBLE,
    TW_PARAM_CSTRING // Null-terminated array of char (ie, c-string)
} TwParamValueType;
int      TwGetParam(TwBar *bar, const char *varName, const char *paramName, TwParamValueType paramValueType, unsigned int outValueMaxCount, void *outValues);
int      TwSetParam(TwBar *bar, const char *varName, const char *paramName, TwParamValueType paramValueType, unsigned int inValueCount, const void *inValues);


// ----------------------------------------------------------------------------
//  Managment functions and definitions
// ----------------------------------------------------------------------------

typedef enum ETwGraphAPI
{
    //TW_OPENGL           = 1,
    TW_DIRECT3D9        = 2,
    //TW_DIRECT3D10       = 3
} TwGraphAPI;

int      TwInit(TwGraphAPI graphAPI, void *device);
int      TwTerminate();

int      TwDraw();
int      TwWindowSize(int width, int height);

typedef enum ETwKeyModifier
{
    TW_KMOD_NONE        = 0x0000,   // same codes as SDL keysym.mod
    TW_KMOD_SHIFT       = 0x0003,
    TW_KMOD_CTRL        = 0x00c0,
    TW_KMOD_ALT         = 0x0100,
    TW_KMOD_META        = 0x0c00
} TwKeyModifier;
typedef enum EKeySpecial
{
    TW_KEY_BACKSPACE    = '\b',
    TW_KEY_TAB          = '\t',
    TW_KEY_CLEAR        = 0x0c,
    TW_KEY_RETURN       = '\r',
    TW_KEY_PAUSE        = 0x13,
    TW_KEY_ESCAPE       = 0x1b,
    TW_KEY_SPACE        = ' ',
    TW_KEY_DELETE       = 0x7f,
    TW_KEY_UP           = 273,      // same codes and order as SDL keysym.sym
    TW_KEY_DOWN,
    TW_KEY_RIGHT,
    TW_KEY_LEFT,
    TW_KEY_INSERT,
    TW_KEY_HOME,
    TW_KEY_END,
    TW_KEY_PAGE_UP,
    TW_KEY_PAGE_DOWN,
    TW_KEY_F1,
    TW_KEY_F2,
    TW_KEY_F3,
    TW_KEY_F4,
    TW_KEY_F5,
    TW_KEY_F6,
    TW_KEY_F7,
    TW_KEY_F8,
    TW_KEY_F9,
    TW_KEY_F10,
    TW_KEY_F11,
    TW_KEY_F12,
    TW_KEY_F13,
    TW_KEY_F14,
    TW_KEY_F15,
    TW_KEY_LAST
} TwKeySpecial;

int      TwKeyPressed(int key, int modifiers);

typedef enum ETwMouseAction
{
    TW_MOUSE_RELEASED,
    TW_MOUSE_PRESSED  
} TwMouseAction;
typedef enum ETwMouseButtonID
{
    TW_MOUSE_LEFT       = 1,    // same code as SDL_BUTTON_LEFT
    TW_MOUSE_MIDDLE     = 2,    // same code as SDL_BUTTON_MIDDLE
    TW_MOUSE_RIGHT      = 3     // same code as SDL_BUTTON_RIGHT
} TwMouseButtonID;

int      TwMouseButton(TwMouseAction action, TwMouseButtonID button);
int      TwMouseMotion(int mouseX, int mouseY);
int      TwMouseWheel(int pos);

const char * TwGetLastError();
typedef void (* TwErrorHandler)(const char *errorMessage);
void     TwHandleErrors(TwErrorHandler errorHandler);


// ----------------------------------------------------------------------------
//  Helper functions to translate events from some common window management
//  frameworks to AntTweakBar.
//  They call TwKeyPressed, TwMouse* and TwWindowSize for you (implemented in
//  files TwEventWin.c TwEventSDL.c TwEventGLFW.c TwEventGLUT.c)
// ----------------------------------------------------------------------------

//  For Windows message proc
#ifndef _W64    // Microsoft specific (detection of 64 bits portability problems)
#   define _W64
#endif  // _W64
int      TwEventWin(void *wnd, unsigned int msg, unsigned int _W64 wParam, int _W64 lParam);
#define TwEventWin32    TwEventWin // For compatibility with AntTweakBar versions prior to 1.11

//  For libSDL event loop
int      TwEventSDL(const void *sdlEvent);
 
//  For GLFW event callbacks
int      TwEventMouseButtonGLFW(int glfwButton, int glfwAction);
int      TwEventKeyGLFW(int glfwKey, int glfwAction);
int      TwEventCharGLFW(int glfwChar, int glfwAction);
#define TwEventMousePosGLFW     TwMouseMotion
#define TwEventMouseWheelGLFW   TwMouseWheel

//  For GLUT event callbacks (Windows calling convention for GLUT callbacks is cdecl)
#if defined(_WIN32) || defined(_WIN64)
#   define TW_GLUT_CALL __cdecl
#else
#   define TW_GLUT_CALL
#endif
int TW_GLUT_CALL TwEventMouseButtonGLUT(int glutButton, int glutState, int mouseX, int mouseY);
int TW_GLUT_CALL TwEventMouseMotionGLUT(int mouseX, int mouseY);
int TW_GLUT_CALL TwEventKeyboardGLUT(unsigned char glutKey, int mouseX, int mouseY);
int TW_GLUT_CALL TwEventSpecialGLUT(int glutKey, int mouseX, int mouseY);
int      TwGLUTModifiersFunc(int (*glutGetModifiersFunc)(void));
typedef void (TW_GLUT_CALL *GLUTmousebuttonfun)(int glutButton, int glutState, int mouseX, int mouseY);
typedef void (TW_GLUT_CALL *GLUTmousemotionfun)(int mouseX, int mouseY);
typedef void (TW_GLUT_CALL *GLUTkeyboardfun)(unsigned char glutKey, int mouseX, int mouseY);
typedef void (TW_GLUT_CALL *GLUTspecialfun)(int glutKey, int mouseX, int mouseY);

 
// ----------------------------------------------------------------------------
//  Make sure the types have the right sizes
// ----------------------------------------------------------------------------

#define TW_COMPILE_TIME_ASSERT(name, x) typedef int TW_DUMMY_ ## name[(x) * 2 - 1]

TW_COMPILE_TIME_ASSERT(CHAR,    sizeof(char)    == 1);
TW_COMPILE_TIME_ASSERT(SHORT,   sizeof(short)   == 2);
TW_COMPILE_TIME_ASSERT(INT,     sizeof(int)     == 4);
TW_COMPILE_TIME_ASSERT(FLOAT,   sizeof(float)   == 4);
TW_COMPILE_TIME_ASSERT(DOUBLE,  sizeof(double)  == 8);


//  ---------------------------------------------------------------------------


#ifdef  __cplusplus
    }   // extern "C"
#endif  // __cplusplus


#endif  // !defined TW_INCLUDED
