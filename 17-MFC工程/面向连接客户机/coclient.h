// coclient.h : main header file for the COCLIENT application
//

#if !defined(AFX_COCLIENT_H__CAB086B2_A42F_48F2_931A_7AB71A28EF34__INCLUDED_)
#define AFX_COCLIENT_H__CAB086B2_A42F_48F2_931A_7AB71A28EF34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoclientApp:
// See coclient.cpp for the implementation of this class
//

class CCoclientApp : public CWinApp
{
public:
	CCoclientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoclientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCoclientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COCLIENT_H__CAB086B2_A42F_48F2_931A_7AB71A28EF34__INCLUDED_)
