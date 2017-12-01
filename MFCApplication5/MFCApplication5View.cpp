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

// MFCApplication5View.cpp : implementation of the CMFCApplication5View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication5.h"
#endif

#include "MFCApplication5Doc.h"
#include "MFCApplication5View.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication5View

IMPLEMENT_DYNCREATE(CMFCApplication5View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication5View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_COMMAND(ID_BUTTON_FLY_GO, &CMFCApplication5View::OnButtonFlyGo)
	ON_COMMAND(ID_EDIT_FLY_JING, &CMFCApplication5View::OnEditFlyJing)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FLY_JING, &CMFCApplication5View::OnUpdateEditFlyJing)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FLY_WEI, &CMFCApplication5View::OnUpdateEditFlyWei)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FLY_HEIGHT, &CMFCApplication5View::OnUpdateEditFlyHeight)
	ON_COMMAND(ID_EDIT_FLY_WEI, &CMFCApplication5View::OnEditFlyWei)
	ON_COMMAND(ID_EDIT_FLY_HEIGHT, &CMFCApplication5View::OnEditFlyHeight)
	ON_COMMAND(ID_BUTTON_FLY, &CMFCApplication5View::OnButtonFly)
END_MESSAGE_MAP()

// CMFCApplication5View construction/destruction

CMFCApplication5View::CMFCApplication5View()
{
	// TODO: add construction code here

}

CMFCApplication5View::~CMFCApplication5View()
{
	m_hThread = 0;
	m_pOSG = NULL;
}

BOOL CMFCApplication5View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCApplication5View drawing

void CMFCApplication5View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication5Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CMFCApplication5View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication5View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication5View diagnostics

#ifdef _DEBUG
void CMFCApplication5View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication5View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication5Doc* CMFCApplication5View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication5Doc)));
	return (CMFCApplication5Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication5View message handlers


int CMFCApplication5View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here


	m_pOSG = new COSG(m_hWnd);



	return 0;
}


BOOL CMFCApplication5View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	if (m_pOSG == NULL)
	{
		return CView::OnEraseBkgnd(pDC);
	}
	else
	{
		return FALSE;
	}

	return CView::OnEraseBkgnd(pDC);
}


void CMFCApplication5View::OnDestroy()
{
	CView::OnDestroy();

	if (m_pOSG)
	{
		m_pOSG->ExitOSG();
		WaitForSingleObject(m_hThread, 10000);
		m_pOSG ? delete m_pOSG : 0;
	}

	m_pOSG = NULL;
	// TODO: Add your message handler code here
}


void CMFCApplication5View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (pFrame)
	{
		CWnd* pWnd = &pFrame->m_wndRibbonBar;
		RECT rect;
		::GetWindowRect(pWnd->GetSafeHwnd(), &rect);

		m_pOSG->InitOSG(rect);
		m_hThread = (HANDLE)_beginthread(COSG::Render, 0, m_pOSG);
	}


	// TODO: Add your specialized code here and/or call the base class
}


void CMFCApplication5View::OnButtonFlyGo()
{
	// TODO: Add your command handler code here
	if (m_pOSG != NULL)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
		CMFCRibbonEdit* pJing = (CMFCRibbonEdit*)(pFrame->m_wndRibbonBar.FindByID(ID_EDIT_FLY_JING));
		CMFCRibbonEdit* pWei = (CMFCRibbonEdit*)(pFrame->m_wndRibbonBar.FindByID(ID_EDIT_FLY_WEI));
		CMFCRibbonEdit* pHeight = (CMFCRibbonEdit*)(pFrame->m_wndRibbonBar.FindByID(ID_EDIT_FLY_HEIGHT));
		CString strJing = pJing->GetEditText();
		CString strWei = pWei->GetEditText();
		CString strHeight = pHeight->GetEditText();

		double jing = wcstod(strJing.GetBuffer(), NULL);
		double wei = wcstod(strWei.GetBuffer(), NULL);
		double height = wcstod(strHeight.GetBuffer(), NULL);
		m_pOSG->FlyGo(jing, wei, height);
	}
}


void CMFCApplication5View::OnEditFlyJing()
{
	// TODO: Add your command handler code here
}


void CMFCApplication5View::OnUpdateEditFlyJing(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CMFCApplication5View::OnUpdateEditFlyWei(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CMFCApplication5View::OnUpdateEditFlyHeight(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CMFCApplication5View::OnEditFlyWei()
{
	// TODO: Add your command handler code here
}


void CMFCApplication5View::OnEditFlyHeight()
{
	// TODO: Add your command handler code here
}


void CMFCApplication5View::OnButtonFly()
{
	// TODO: Add your command handler code here
//	AfxMessageBox(L"start to fly!");
	m_pOSG->DoPreLineNow();
}
