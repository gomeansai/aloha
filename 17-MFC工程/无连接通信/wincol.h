// wincol.h : main header file for the WINCOL application
//

#if !defined(AFX_WINCOL_H__D62D8311_B866_4D57_9BBF_9481EAFF907A__INCLUDED_)
#define AFX_WINCOL_H__D62D8311_B866_4D57_9BBF_9481EAFF907A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWincolApp:
// See wincol.cpp for the implementation of this class
//

class CWincolApp : public CWinApp
{
public:
	CWincolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWincolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWincolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINCOL_H__D62D8311_B866_4D57_9BBF_9481EAFF907A__INCLUDED_)
