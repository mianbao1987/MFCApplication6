// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MFCApplication5View.h : interface of the CMFCApplication5View class
//

#pragma once

#include "OSG.h"

class CMFCApplication5View : public CView
{
protected: // create from serialization only
	CMFCApplication5View();
	DECLARE_DYNCREATE(CMFCApplication5View)

// Attributes
public:
	CMFCApplication5Doc* GetDocument() const;
public:
	COSG* m_pOSG;
	HANDLE m_hThread;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMFCApplication5View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnButtonFlyGo();
	afx_msg void OnEditFlyJing();
	afx_msg void OnUpdateEditFlyJing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFlyWei(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditFlyHeight(CCmdUI *pCmdUI);
	afx_msg void OnEditFlyWei();
	afx_msg void OnEditFlyHeight();
	afx_msg void OnButtonFly();
};

#ifndef _DEBUG  // debug version in MFCApplication5View.cpp
inline CMFCApplication5Doc* CMFCApplication5View::GetDocument() const
   { return reinterpret_cast<CMFCApplication5Doc*>(m_pDocument); }
#endif

