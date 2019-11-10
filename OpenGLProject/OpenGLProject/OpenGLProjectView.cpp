﻿#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "OpenGLProject.h"
#endif

#include "OpenGLProjectDoc.h"
#include "OpenGLProjectView.h"
#pragma once
#include "include/tinyobjloader/tiny_obj_loader.h"
#include "include/stb/stb_image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(COpenGLProjectView, CView)

BEGIN_MESSAGE_MAP(COpenGLProjectView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGLProjectView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	// 유저가 추가한 메시지 입니다
	ON_MESSAGE(UWM_CUSTOM1, &COpenGLProjectView::OnUwmCustom1)
	ON_MESSAGE(UWM_CHECKED, &COpenGLProjectView::OnUwmChecked)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_TEST_DIRECTIONAL, &COpenGLProjectView::OnTestDirectional)
	ON_COMMAND(ID_LIGHT_POSITIONAL, &COpenGLProjectView::OnLightPositional)
	ON_COMMAND(ID_LIGHT_SPOTLIGHT, &COpenGLProjectView::OnLightSpotlight)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

bool COpenGLProjectView::SetDevicePixelFormat(HDC hdc) {
	int pixelformat;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_FORMAT |
		PFD_DOUBLEBUFFER |
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		8,
		0,
		8,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE) {
		MessageBox(LPCTSTR("ChoosePixelFormat Failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}

	if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE) {
		MessageBox(LPCTSTR("SetPixelFormat Failed"), LPCTSTR("Error"), MB_OK);
		return FALSE;
	}
	return TRUE;
}

// COpenGLProjectView 생성/소멸
COpenGLProjectView::COpenGLProjectView() noexcept
{
	preX = 0.0;
	preY = 0.0;
	clicked = false;
	// TODO: 여기에 생성 코드를 추가합니다.
}

COpenGLProjectView::~COpenGLProjectView()
{
}

BOOL COpenGLProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// COpenGLProjectView 그리기
void COpenGLProjectView::OnDraw(CDC* /*pDC*/)
{
	COpenGLProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	DrawGLScene();
	Invalidate(FALSE);
}

// COpenGLProjectView 인쇄
void COpenGLProjectView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL COpenGLProjectView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void COpenGLProjectView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void COpenGLProjectView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void COpenGLProjectView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// COpenGLProjectView 진단
#ifdef _DEBUG
void COpenGLProjectView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLProjectDoc* COpenGLProjectView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLProjectDoc)));
	return (COpenGLProjectDoc*)m_pDocument;
}
#endif //_DEBUG

int COpenGLProjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_hDC = GetDC()->m_hDC;

	if (!SetDevicePixelFormat(m_hDC)) {
		return -1;
	}

	m_hglRC = wglCreateContext(m_hDC);
	wglMakeCurrent(m_hDC, m_hglRC);

	// initialize renderring mode

	mOPT_LTG = new bool[nOPT_LTG];
	for (int i = 0; i < nOPT_LTG; i++) {
		mOPT_LTG[i] = TRUE;
	}

	mEN_LTG = new bool[nLTG];
	for (int i = 0; i < nLTG; i++) {
		mEN_LTG[i] = TRUE;
	}

	initGL();

	return 0;
}

void COpenGLProjectView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);
	wglMakeCurrent(m_hDC, NULL);
	wglDeleteContext(m_hglRC);
}

void COpenGLProjectView::initGL()
{
	TRACE0("initGL 시작\n");

	// GLEW를 사용하기 전에 먼저 초기화합니다
	GLenum err = glewInit();
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	Shader ourShader("VertexShader.glsl", "FragmentShader.glsl");
	progId = ourShader.getID();
	ourShader.use();

	GLfloat light1_position[] = { -4.0, -4.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	GLfloat light1_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	GLfloat light1_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	GLfloat light1_specular[] = { 0.0,0.0,1.0,1.0 };
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	GLfloat light2_position[] = { -4.0, -4.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	GLfloat light2_ambient[] = { 1.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	GLfloat light2_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	GLfloat light2_specular[] = { 0.0,0.0,1.0,1.0 };
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
	GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);

	// ObjectController::LoadObject(ourShader, "../OpenGLProject/Asset/IronMan.obj");
	ObjectController::LoadObject(ourShader, "../OpenGLProject/Asset/Kizuna/kizunaai.obj");
	// Object iron = ObjectController::FindObject(std::string("IronMan.obj"));
	TRACE0("로딩 종료\n");

	// Camera::Initialize();
}

// OnCreate 이후에 적어도 한번 호출된다
void COpenGLProjectView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	ReSizeGLScene(cx, cy);
}

// OnSize에서 호출된다
void COpenGLProjectView::ReSizeGLScene(GLsizei width, GLsizei height)
{
	// don't want a divide by zero
	if (height == 0)
		height = 1;

	Camera::ReSize(width, height);

	// set modeview matrix
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPushMatrix();
}

void COpenGLProjectView::DrawGLScene(void)
{
	// claer screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Camera::Convert();
	Axis::Draw();
	ObjectController::DrawObjects(progId);

	// swap buffer
	SwapBuffers(m_hDC);
}

afx_msg LRESULT COpenGLProjectView::OnUwmCustom1(WPARAM wParam, LPARAM lParam)
{
	//CString msg = _T("");
	//msg.Format(_T("Button Click!"));

	//AfxMessageBox(msg);

	// camera view configuration
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	// glColor3f(1, 0, 0);
	glVertex3f(-1, -.5f, 1);
	// glColor3f(0, 1, 0);
	glVertex3f(0, .5f, 0);
	// glColor3f(0, 0, 1);
	glVertex3f(1, -.5f, 0);

	glVertex3f(0, .5f, 0);
	glVertex3f(1, -.5f, 0);
	glVertex3f(-1, -.5f, -1);
	glEnd();
	SwapBuffers(m_hDC);
	//AfxGetMainWnd()->UpdateWindow();

	return 0;
}

afx_msg LRESULT COpenGLProjectView::OnUwmChecked(WPARAM wParam, LPARAM lParam)
{
	int index = lParam - ID_PROPERTIES_CHECKBOX1;
	mOPT_LTG[index] = wParam;
	return 0;
}

void COpenGLProjectView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	preX = point.x - 4.0;
	preY = point.y - 4.0;
	clicked = true;
	CView::OnLButtonDown(nFlags, point);
}

void COpenGLProjectView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	clicked = false;
	GLfloat light0_position[] = { -4.0, -4.0, 1.0, 1.0 };
	if (!mOPT_LTG[0])
		for (int i = 0; i < 4; i++)
			light0_position[i] = 0.0;
	CView::OnLButtonUp(nFlags, point);
}

void COpenGLProjectView::OnRButtonDown(UINT nFlags, CPoint point)
{
	Controller::setRClick(TRUE);
	Controller::setpreClickPoint(point);

	CView::OnRButtonDown(nFlags, point);
}
void COpenGLProjectView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	Controller::setRClick(FALSE);

	// 우클릭 완료시 벗어나려면 주석을 지워야한다
	// ClientToScreen(&point);
	// OnContextMenu(this, point);
}

void COpenGLProjectView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//if (clicked) {
	//	GLfloat light0_position[] = { (preX - point.x)/100, (preY - point.y)/100, 1.0, 1.0 };
	//	if (!mOPT_LTG[0])
	//		for (int i = 0; i < 4; i++)
	//			light0_position[i] = 0.0;
	//}
	//preX = (preX - point.x) / 100;
	//preY = (preY - point.y) / 100;
	if (Controller::getRClick()) {
		Camera::setCaRo(Camera::getCaRo() + point - Controller::getpreClickPoint());
		Controller::setpreClickPoint(point);
	}

	// swap buffer
	CView::OnMouseMove(nFlags, point);
}

void COpenGLProjectView::OnTestDirectional()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	mEN_LTG[0] = !mEN_LTG[0];
}

void COpenGLProjectView::OnLightPositional()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	mEN_LTG[1] = !mEN_LTG[1];
}

void COpenGLProjectView::OnLightSpotlight()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	mEN_LTG[2] = !mEN_LTG[2];
}

// 키보드 입력을 처리합니다
void COpenGLProjectView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case 'W':
		Controller::setW(TRUE);
		break;
	case 'A':
		Controller::setA(TRUE);
		break;
	case 'S':
		Controller::setS(TRUE);
		break;
	case 'D':
		Controller::setD(TRUE);
	}

	Controller::CameraTrans();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
// 키보드 입력을 처리합니다
void COpenGLProjectView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case 'W':
		Controller::setW(FALSE);
		break;
	case 'A':
		Controller::setA(FALSE);
		break;
	case 'S':
		Controller::setS(FALSE);
		break;
	case 'D':
		Controller::setD(FALSE);
	}

	Controller::CameraTrans();
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}