﻿
// OpenGLProjectView.h: COpenGLProjectView 클래스의 인터페이스
//
#ifndef OPENGLPROJECTVIEW_H
#define OPENGLPROJECTVIEW_H

#include "OpenGLProjectDoc.h"

// 사용자 정의 클래스
#include "Controller.h"
#include "Camera.h"
#include "Axis.h"
#include "Object.h"
#include "ObjectController.h"

class COpenGLProjectView : public CView
{
protected: // serialization에서만 만들어집니다.
	COpenGLProjectView() noexcept;
	DECLARE_DYNCREATE(COpenGLProjectView)

// 특성입니다.
public:
	COpenGLProjectDoc* GetDocument() const;

// 작업입니다.
public:
	HDC	m_hDC;
	HGLRC m_hglRC;
public:
	GLfloat cameraX = 0.0;
	bool SetDevicePixelFormat(HDC hdc);
	void initGL(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void DrawGLScene(void);

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~COpenGLProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	bool *mOPT_LTG;
	bool* mEN_LTG; // Enable Lighting
	const int nLTG = 3; // Lighting의 개수
	const int nOPT_LTG = 8; // 조명 옵션의 개수
	GLfloat *light0_position;
	GLfloat preX;
	GLfloat preY;
	bool clicked;
// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnUwmCustom1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUwmChecked(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTestDirectional();
	afx_msg void OnLightPositional();
	afx_msg void OnLightSpotlight();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif

#ifndef _DEBUG  // OpenGLProjectView.cpp의 디버그 버전
inline COpenGLProjectDoc* COpenGLProjectView::GetDocument() const
   { return reinterpret_cast<COpenGLProjectDoc*>(m_pDocument); }
#endif