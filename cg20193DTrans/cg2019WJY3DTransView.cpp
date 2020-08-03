
// cg2019WJY3DTransView.cpp : Ccg2019WJY3DTransView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cg2019WJY3DTrans.h"
#endif

#include "gl\gl.h"
#include "gl\glu.h"

#include "DrawScene.h"
#include "cg2019WJY3DTransDoc.h"
#include "cg2019WJY3DTransView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccg2019WJY3DTransView

IMPLEMENT_DYNCREATE(Ccg2019WJY3DTransView, CView)

BEGIN_MESSAGE_MAP(Ccg2019WJY3DTransView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_AUTOPLAY, &Ccg2019WJY3DTransView::OnAutoplay)
	ON_UPDATE_COMMAND_UI(ID_AUTOPLAY, &Ccg2019WJY3DTransView::OnUpdateAutoplay)
	ON_WM_TIMER()
	ON_COMMAND(ID_ONLIGHT, &Ccg2019WJY3DTransView::OnOnlight)
	ON_UPDATE_COMMAND_UI(ID_ONLIGHT, &Ccg2019WJY3DTransView::OnUpdateOnlight)
	ON_COMMAND(ID_ONSHADE, &Ccg2019WJY3DTransView::OnOnshade)
	ON_UPDATE_COMMAND_UI(ID_ONSHADE, &Ccg2019WJY3DTransView::OnUpdateOnshade)
END_MESSAGE_MAP()

// Ccg2019WJY3DTransView 构造/析构

Ccg2019WJY3DTransView::Ccg2019WJY3DTransView()
{
	// TODO: 在此处添加构造代码
	m_drawScene = new CDrawScene(this);
}

Ccg2019WJY3DTransView::~Ccg2019WJY3DTransView()
{
}

BOOL Ccg2019WJY3DTransView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ccg2019WJY3DTransView 绘制

void Ccg2019WJY3DTransView::OnDraw(CDC* /*pDC*/)
{
	Ccg2019WJY3DTransDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawScene();
}


// Ccg2019WJY3DTransView 打印

BOOL Ccg2019WJY3DTransView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccg2019WJY3DTransView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccg2019WJY3DTransView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ccg2019WJY3DTransView 诊断

#ifdef _DEBUG
void Ccg2019WJY3DTransView::AssertValid() const
{
	CView::AssertValid();
}

void Ccg2019WJY3DTransView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccg2019WJY3DTransDoc* Ccg2019WJY3DTransView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccg2019WJY3DTransDoc)));
	return (Ccg2019WJY3DTransDoc*)m_pDocument;
}
#endif //_DEBUG


// Ccg2019WJY3DTransView 消息处理程序
BOOL Ccg2019WJY3DTransView::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),       // size of this pfd
		1,                                   // version number
		PFD_DRAW_TO_WINDOW |                 // support window
		PFD_SUPPORT_OPENGL |                 // support OpenGL
		PFD_DOUBLEBUFFER,                    // double buffered
		PFD_TYPE_RGBA,                       // RGBA type
		24,                                  // 24-bit color depth
		0, 0, 0, 0, 0, 0,                    // color bits ignored
		0,                                   // no alpha buffer
		0,                                   // shift bit ignored
		0,                                   // no accumulation buffer
		0, 0, 0, 0,                          // accum bits ignored
		32,                                  // 32-bit z-buffer
		0,                                   // no stencil buffer
		0,                                   // no auxiliary buffer
		PFD_MAIN_PLANE,                      // main layer
		0,                                   // reserved
		0, 0, 0                              // layer masks ignored
	};
	int pixelformat;

	if ((pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0) {
		MessageBox(_T("ChoosePixelFormat failed"));
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE) {
		MessageBox(_T("SetPixelFormat failed"));
		return FALSE;
	}

	return TRUE;
}



int Ccg2019WJY3DTransView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC       hrc;
	GLfloat      fAspect;

	m_pDC = new CClientDC(this);

	ASSERT(m_pDC != NULL);

	if (!bSetupPixelFormat()) return 0;

	n = ::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);

	//	CreateRGBPalette();

	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);

	glClearDepth(100.0f);
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	GetClientRect(&m_viewRect);

	if (!m_viewRect.bottom)	fAspect = 1.0f;
	else                    fAspect = (GLfloat)m_viewRect.right / m_viewRect.bottom;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, fAspect, 3.0f, 7.0f);
	glMatrixMode(GL_MODELVIEW);

	return 0;

}


void Ccg2019WJY3DTransView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if ((m_viewRect.right != cx) || (m_viewRect.bottom != cy)) {
		//	if (cy > 0)	{
		glViewport(0, 0, cx, cy);

		//			RedrawWindow();

		m_viewRect.right = cx;
		m_viewRect.bottom = cy;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLdouble)cx / cy, 3.0f, 7.0f);
		glMatrixMode(GL_MODELVIEW);
	}
}

void Ccg2019WJY3DTransView::DrawScene()
{
	static BOOL bBusy = FALSE;
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	if (bBusy)	return;

	bBusy = TRUE;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(pDoc->m_translateVector[0],
		pDoc->m_translateVector[1],
		pDoc->m_translateVector[2]);
	glRotatef(pDoc->m_xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(pDoc->m_yAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(pDoc->m_zAngle, 0.0f, 0.0f, 1.0f);
	glScalef(0.3f, 0.3f, 0.3f);   /* Make Scene Smaller Enough to Look */
	DWORD  opStart, opFinish;
	opStart = GetTickCount();        // 记录测试开始时间，单位是毫秒
	if (m_drawScene)  m_drawScene->DrawScene();
	//if (m_cameraView) m_cameraView->DrawScene();

	glPopMatrix();

	glFinish();
	SwapBuffers(wglGetCurrentDC());

	bBusy = FALSE;
	opFinish = GetTickCount();       // 记录测试结束时间，单位是毫秒
	pDoc->m_DrawSceneRunTime = (opFinish - opStart) / 1000.0f;
	// Notify CCgTransControl can process the Project Result to PictureBox 'VIEWPORT'
	pDoc->UpdateAllViews(this);
}


static BOOL m_autoPlay = false;
static BOOL m_onLight = false;
static BOOL m_onShade = false;

void Ccg2019WJY3DTransView::OnAutoplay()
{
	m_autoPlay = !m_autoPlay;

	if (m_autoPlay) SetTimer(1, 50, NULL);
	else            KillTimer(1);
}


void Ccg2019WJY3DTransView::OnUpdateAutoplay(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void Ccg2019WJY3DTransView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DrawScene();

	CView::OnTimer(nIDEvent);
}


void Ccg2019WJY3DTransView::OnOnlight()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	m_onLight = !m_onLight;
	pDoc->m_onLight = m_onLight;
}


void Ccg2019WJY3DTransView::OnUpdateOnlight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_onLight);
}


void Ccg2019WJY3DTransView::OnOnshade()
{
	Ccg2019WJY3DTransDoc* pDoc = (Ccg2019WJY3DTransDoc*)GetDocument();

	m_onShade = !m_onShade;
	pDoc->m_onShade = m_onShade;
}


void Ccg2019WJY3DTransView::OnUpdateOnshade(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_onShade);
}
