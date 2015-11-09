#pragma once
#pragma warning(disable : 4251)



#include <ObjBase.h>
#include <TCHAR.h>



#define DECL_INTERFACE(name)		typedef struct __declspec(novtable) name
#define DECL_STRUCT(name)			typedef struct name
#define DECL_CLASS(name)			typedef class name
#define DECL_FUNC(func)				__declspec(nothrow) HRESULT __stdcall func
#define DECL_FUNC_T(value, func)	__declspec(nothrow) value __stdcall func
#define BASE_INHERITANCE(child, parent)						DECL_INTERFACE(child) : public parent
#define DECL_INHERITANCE(level, child, parent)				DECL_##level(child) : public parent
#define DECL_EXINHERITANCE(level, child, parent1, parent2)	DECL_##level(child) : public parent1, parent2



#ifndef HDEVSTDLIB_EXPORTS
#define HDEFBASE_EXPORT __declspec(dllexport)
#else
#define HDEFBASE_EXPORT
#endif



#define DLL_DECL_INTERFACE(name)	typedef struct HDEFBASE_EXPORT __declspec(novtable) name
#define DLL_DECL_STRUCT(name)		typedef struct HDEFBASE_EXPORT name
#define DLL_DECL_CLASS(name)		typedef class HDEFBASE_EXPORT name
#define DLL_DECL_FUNC(func)			HDEFBASE_EXPORT DECL_FUNC(func)
#define DLL_DECL_FUNC_T(type, func)	HDEFBASE_EXPORT DECL_FUNC_T(type, func)
#define DLL_BASE_INHERITANCE(child, parent)						DLL_DECL_INTERFACE(child) : public parent
#define DLL_DECL_INHERITANCE(level, child, parent)				DLL_DECL_##level(child) : public parent
#define DLL_DECL_EXINHERITANCE(level, child, parent1, parent2)	DLL_DECL_##level(child) : public parent1, parent2



#define TEMPLATE_INTERFACE(type, name)			template<type> struct __declspec(novtable) name
#define TEMPLATE_STRUCT(type, name)				template<type> struct name
#define TEMPLATE_CLASS(type, name)				template<type> class name
#define TEMPLATE_FUNC(type, func)				template<type> __declspec(nothrow) HRESULT __stdcall func
#define TEMPLATE_FUNC_T(type, value, func)		template<type> __declspec(nothrow) value __stdcall func
#define TEMPLATE_BASE_INHERITANCE(type, child, parent)					TEMPLATE_INTERFACE(type, child) : public parent
#define TEMPLATE_INHERITANCE(level, type, child, parent)				TEMPLATE_##level(type, child) : public parent
#define TEMPLATE_EXINHERITANCE(level, type, child, parent1, parent2)	TEMPLATE_##level(type, child) : public parent1, parent2



#define INTERFACE_DECL_FUNC(func)			__declspec(nothrow) virtual HRESULT func
#define INTERFACE_DECL_FUNC_T(type, func)	__declspec(nothrow) virtual type func
#define	CLASS_DECL_CONSTRUCTOR(_class)		explicit _class
#define	CLASS_DECL_DESTRUCTOR(_class)		virtual ~_class
#define CLASS_DECL_STRUCTOR(_class)			\
	CLASS_DECL_DESTRUCTOR(_class)(VOID); \
	CLASS_DECL_CONSTRUCTOR(_class)
#define CLASS_DECL_FUNC(func)				__declspec(nothrow) HRESULT func
#define CLASS_DECL_FUNC_T(type, func)		__declspec(nothrow) type func
#define CLASS_DECL_OPEN_FUNC(func)			__declspec(nothrow) static HRESULT func
#define CLASS_DECL_OPEN_FUNC_T(type, func)	__declspec(nothrow) static type func



#define REALIZE_FUNC(func)							HRESULT __stdcall func
#define REALIZE_FUNC_T(value, func)					value __stdcall func
#define CLASS_REALIZE_CONSTRUCTOR(_class)			_class::_class
#define CLASS_REALIZE_DESTRUCTOR(_class)			_class::~_class
#define CLASS_REALIZE_FUNC(_class,_func)			HRESULT _class::_func
#define CLASS_REALIZE_FUNC_T(_class, _type, _func)	_type _class::_func



#define ADD_FLAG(atrr, flag)	(atrr | flag)
#define SUB_FLAG(atrr, flag)	(atrr ^ flag)
#define IS_FLAG(atrr, flag)	(atrr & flag)



#define IF_SUCCEEDED(hr)	if(SUCCEEDED(hr))
#define IF_FAILED(hr)		if(FAILED(hr))
#define IF_TRUE(b)			if(b)
#define IF_FALSE(b)			if(b == NULL)
#define IF_NOT_NULL(b)		IF_TRUE(b)
#define IF_NULL(b)			IF_FALSE(b)
#define IF_SIGNAL(b)		IF_TRUE(b)
#define IF_NONSIGNAL(b)		IF_FALSE(b)



#define DEL_POINTER(p)			IF_NOT_NULL(p){delete p;p=NULL;}
#define DEL_POINTER_ARRAY(p)	IF_NOT_NULL(p){delete [] p;p=NULL;}
#define DEL_HANDLE(p)			IF_NOT_NULL(p){CloseHandle(p);p=NULL;}
#define DEL_COM(p)				IF_NOT_NULL(p){p->Release();p=NULL;}
#define ZERO_COM(p)				IF_NOT_NULL(p){while(p->Release());p=NULL;}
#define DEL_DECLARE(p)			IF_NOT_NULL(p){p->release();p=NULL;}
#define ZERO_DECLARE(p)			IF_NOT_NULL(p){while(p->release());p=NULL;}



#define INIT_FUNC			{return 0;}
#define INIT_FUNC_T(value)	{return value;}



namespace h
{
	DLL_DECL_INTERFACE(h_3DScreen)			*ptr_Screen;
	DLL_DECL_INTERFACE(h_Renderer)			*ptr_Renderer;
	DLL_DECL_INTERFACE(h_F3DCamera)			*ptr_F3DCamera;
	DLL_DECL_INTERFACE(h_D3DCamera)			*ptr_D3DCamera;
	DLL_DECL_CLASS(h_FTimer)				*ptr_FTimer;
	DLL_DECL_CLASS(h_DTimer)				*ptr_DTimer;
	DLL_DECL_INTERFACE(h_MouseEvent)		*ptr_MouseEvent;
	DLL_DECL_INTERFACE(h_MouseListener)		*ptr_MouseListener;
	DLL_DECL_INTERFACE(h_MouseDevice)		*ptr_MouseDevice;
	DLL_DECL_INTERFACE(h_KeyboardEvent)		*ptr_KeyboardEvent;
	DLL_DECL_INTERFACE(h_KeyboardListener)	*ptr_KeyboardListener;
	DLL_DECL_INTERFACE(h_KeyboardDevice)	*ptr_KeyboardDevice;
};