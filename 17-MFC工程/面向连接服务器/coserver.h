// coserver.h : main header file for the COSERVER application
//

#if !defined(AFX_COSERVER_H__231CF971_5CDC_4CC4_82DF_5DE6DD4A1CF0__INCLUDED_)
#define AFX_COSERVER_H__231CF971_5CDC_4CC4_82DF_5DE6DD4A1CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoserverApp:
// See coserver.cpp for the implementation of this class
//

class CCoserverApp : public CWinApp
{
public:
	CCoserverApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoserverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCoserverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COSERVER_H__231CF971_5CDC_4CC4_82DF_5DE6DD4A1CF0__INCLUDED_)
