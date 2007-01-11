/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: winx/Basic.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 14:23:22
// 
// $Id: Basic.h,v 1.9 2006/12/22 10:22:28 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef __WINX_BASIC_H__
#define __WINX_BASIC_H__

#if (0)
#define WINX_NO_DEBUG_NEW
#define WINX_USE_DEFINE_IID
#define DEFINE_IID
#define __uuid
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

// -------------------------------------------------------------------------

#ifndef __STLPATCH_H__
#include "../../../stdext/include/stlpatch.h"
#endif

#ifndef __WINX_WINSDK_BASIC_H__
#include "winsdk/Basic.h"
#endif

#ifndef __WINX_WTL_BASIC_H__
#include "wtl/Basic.h"
#endif

#ifndef __STDEXT_H__
#include "../../../stdext/include/stdext.h"
#endif

// -------------------------------------------------------------------------
// winx_msg

#ifndef winx_msg
#define winx_msg winx_call
#endif

// -------------------------------------------------------------------------
// __WINX

#ifndef __WINX
#define __WINX				winx::
#define __WINX_BEGIN		namespace winx {
#define __WINX_END			}
#endif

#ifndef WINX_VER
#define WINX_VER			0x0100	// Version 1.0
#endif

// -------------------------------------------------------------------------
// WINX_MALLOC/WINX_FREE

#define WINX_MALLOC(cb)		_malloc_dbg(cb, _NORMAL_BLOCK, __FILE__, __LINE__)
#define WINX_FREE(p)		_free_dbg(p, _NORMAL_BLOCK)

// -------------------------------------------------------------------------
// WINX_NEW/WINX_DELETE

#if !defined(WINX_NO_DEBUG_NEW) && defined(_DEBUG)
#define WINX_DEBUG_NEW
#endif

template <class WindowClass>
__forceinline void _WinxDbgDelete(WindowClass* pOb)
{
	pOb->~WindowClass();
	WINX_FREE(pOb);
}

#if defined(WINX_DEBUG_NEW)
#define WINX_NEW(WindowClass)			new(WINX_MALLOC(sizeof(WindowClass))) WindowClass
#define WINX_DELETE(pOb)				_WinxDbgDelete(pOb)
#else
#define WINX_NEW(WindowClass)			new WindowClass
#define WINX_DELETE(pOb)				delete pOb
#endif

// -------------------------------------------------------------------------
// GET_X_LPARAM, GET_Y_LPARAM - window message helper

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                ((int)(short)HIWORD(lp))
#endif

// -------------------------------------------------------------------------
// for MFC Compatibility

#ifndef ASSERT
#define ASSERT(e)			WINX_ASSERT(e)
#endif

#ifndef ASSERT_VALID
#define ASSERT_VALID(pObj)	WINX_ASSERT((pObj) != NULL)
#endif

#ifndef VERIFY
#define VERIFY(e)			WINX_VERIFY(e)
#endif

#ifndef TRACE
#define TRACE				WINX_TRACE
#endif

#ifndef TRACE0
#define TRACE0				WINX_TRACE
#endif

#ifndef TRACE1
#define TRACE1				WINX_TRACE
#endif

#ifndef TRACE2
#define TRACE2				WINX_TRACE
#endif

#ifndef TRACE3
#define TRACE3				WINX_TRACE
#endif

#ifndef DEBUG_ONLY
#define DEBUG_ONLY(e)		WINX_DEBUG_ONLY(e)
#endif

// -------------------------------------------------------------------------
// WINX_PROPERTY - for general use

#define WINX_PROPERTY(Type, Name, Method)									\
	Type __declspec(property(get=Method)) Name

#define WINX_PROPERTY_RW(Type, Name, Get, Put)								\
	Type __declspec(property(get=Get,put=Put)) Name

#define WINX_PROP_READONLY(Type, Name, Variant)								\
	Type winx_call _winx_get_##Name() const { return Variant; }				\
	WINX_PROPERTY(Type, Name, _winx_get_##Name)

#define WINX_PROP_READWRITE(Type, Name, Variant)							\
	Type winx_call _winx_get_##Name() const { return Variant; }				\
	void winx_call _winx_put_##Name(Type _winx_v) { Variant = _winx_v; }	\
	WINX_PROPERTY_RW(Type, Name, _winx_get_##Name, _winx_put_##Name)

// -------------------------------------------------------------------------
// WINX_TEXT

#if defined(UNICODE)
#define WINX_TEXT(str)		L ## str
#else
#define WINX_TEXT(str)		str
#endif

// -------------------------------------------------------------------------
// GetModuleHandleEx

inline HMODULE GetModuleHandleEx(LPCVOID lpAddress)
{
	MEMORY_BASIC_INFORMATION mInfo;
	VirtualQuery(lpAddress, &mInfo, sizeof(mInfo));
	return (HMODULE)mInfo.AllocationBase;
}

// -------------------------------------------------------------------------
// GetThisModule

template <class Unused>
class WinxThisModuleT
{
public:
	static HMODULE _g_hInst;
};

inline VOID _DummyFunction() {}

template <class Unused>
HMODULE WinxThisModuleT<Unused>::_g_hInst = GetModuleHandleEx(_DummyFunction);

typedef WinxThisModuleT<void> WinxThisModule;

#define GetThisModule()	WinxThisModule::_g_hInst

// -------------------------------------------------------------------------
// class CComAppInit/COleAppInit - Helper

class CComAppInit
{
public:
	CComAppInit(LPVOID pvReserved = NULL) {
		::CoInitialize(pvReserved);
	}
	~CComAppInit() {
		::CoUninitialize();
	}
};

class COleAppInit
{
public:
	COleAppInit(LPVOID pvReserved = NULL) {
		::OleInitialize(pvReserved);
	}
	~COleAppInit() {
		::OleUninitialize();
	}
};

// -------------------------------------------------------------------------
// class CDebugAppInit - Debug Helper

class CDebugAppInit
{
public:
	static VOID winx_call EnableLeakCheck() {
		_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	}

#if defined(_DEBUG)
	CDebugAppInit() {
		EnableLeakCheck();
	}
#endif
};

// -------------------------------------------------------------------------
// class CComModuleInit - ATL Helper

class CComModuleInit
{
public:
	CComModuleInit(
		_ATL_OBJMAP_ENTRY* p = NULL,
		HINSTANCE hInst = GetThisModule(),
		const GUID* plibid = NULL)
	{
		_Module.Init(p, hInst, plibid);
	}
	~CComModuleInit()
	{
		_Module.Term();
	}
};

// -------------------------------------------------------------------------
// CAppModuleInit - WTL Helper

#if defined(WINX_USE_APPMODULE)

class CAppModuleInit : public WTL::CMessageLoop
{
public:
	CAppModuleInit(
		_ATL_OBJMAP_ENTRY* p = NULL,
		HINSTANCE hInst = GetThisModule(),
		const GUID* plibid = NULL)
	{
		_Module.Init(p, hInst, plibid);
		_Module.AddMessageLoop(this);
	}
	~CAppModuleInit()
	{
		_Module.Term();
	}
};

#endif // defined(WINX_USE_APPMODULE)

// -------------------------------------------------------------------------
// $Log: Basic.h,v $
// Revision 1.9  2006/12/22 10:22:28  xushiwei
// STL-Patch: std::basic_string::_Split function bugfix (vc6)
//
// Revision 1.8  2006/10/18 12:13:46  xushiwei
// stdext as independent component
//
// Revision 1.7  2006/08/26 09:12:37  xushiwei
// vs2005 support
//
// Revision 1.6  2006/08/22 10:36:53  xushiwei
// WINX-Core:
//   Property(Bkgrnd, Accel, Layout), MainFrame support
//   CommandDispatch, CommandState, Demo: User-defined-control(Subclass, Superclass, SuperDialog)
//
// Revision 1.5  2006/08/21 18:45:48  xushiwei
// WINX-Core:
//    Window::Create, MainFrame, StackWindowObject(WINX_STACK_OBJECT) support
//
// Revision 1.4  2006/08/20 05:14:47  xushiwei
// MFC-Compatibility:
//   GdiObject(CDC, CClientDC, CPaintDC, CBitmap, CPalette, etc),  Diagnost(ASSERT, VERIFY, etc)
//   CreditStatic - Demonstrate how to port MFC code to WINX --- see @@code in source code
//
// Revision 1.2  2006/08/19 10:50:55  xushiwei
// WINX-Core: Message, Window, Dialog, Control
//
// Revision 1.1  2006/08/19 09:42:10  xushiwei
// WINX-Basic:
//   BasicTypes(CString, CFindFile, CRecentDocumentList, CComBSTR, etc)
//   Helper(MsgBox, GetThisModule, InitCommonControls, etc)
//   Init(CComAppInit, COleAppInit, CDebugAppInit, CComModuleInit, etc)
//   Resource(GetModuleIniPath, Bitmap, Icon, Menu, Brush, Accelerator, etc)
//   Gdi(Rgn, Pen, Brush, Font, Bitmap, DC, DoubleBuffer, etc)
//   WindowHandle, WindowMap, MessageMap
//   COM Support(Object, StackObject, FakeObject, COLESTR, etc)
//

#endif /* __WINX_BASIC_H__ */