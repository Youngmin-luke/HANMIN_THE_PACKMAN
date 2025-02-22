﻿#ifndef OPENGLPROJECTVIEW_H
#define OPENGLPROJECTVIEW_H

#include "OpenGLProjectDoc.h"

// 사용자 정의 클래스
#include "IOManager.h"
#include "Camera.h"
#include "Axis.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Shader.h"
#include "Light.h"
#include "SystemMangement.h"
#include "Skybox.h"
#include "plane.h"
#include "ShaderManager.h"

using namespace CameraProperties;
using namespace DefaultPlane;

class COpenGLProjectView : public CView
{
protected: // serialization에서만 만들어집니다.
	COpenGLProjectView() noexcept;
	DECLARE_DYNCREATE(COpenGLProjectView)
public:
	COpenGLProjectDoc* GetDocument() const;
	

public:
	HDC	m_hDC;
	HGLRC m_hglRC;

public:
	bool SetDevicePixelFormat(HDC hdc);
	void initGL(void);
	void ReSizeGLScene(GLsizei width, GLsizei height);
	void DrawGLScene(void);

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

	//월드에 생성될 모든 인스턴스의 선언부입니다.
private:
	Shader glslShader;
	Shader skyboxShader;
	Shader rayTracingShader;
	Shader planeShader;
	Skybox skybox0;
	Light light0;
	Light light1;
	Camera camera;
	plane defaultPlane;
	Object airobj;
	IOManager ioManager;
	ShaderManager* shaderManager;

	// Ray Tracing Rendering에 사용될 vertex의 Array 입니다
	glm::vec3 *vertexArr;
	GLfloat test[3] = { 10,0,0 };
	// vertexArr에 데이터를 채웁니다 initGL에서 실행됩니다
	// 큐브 두개와 구 하나를 그립니다
	void makeArr(void);

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShaderPhongShading();
	afx_msg void OnShaderRayTracingRendering();
	afx_msg void OnUpdateShaderPhongshading(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShaderRaytracingrendering(CCmdUI* pCmdUI);
};

#endif


#ifndef _DEBUG  // OpenGLProjectView.cpp의 디버그 버전
inline COpenGLProjectDoc* COpenGLProjectView::GetDocument() const
{
	return reinterpret_cast<COpenGLProjectDoc*>(m_pDocument);
}

#endif


