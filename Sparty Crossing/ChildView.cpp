
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "DoubleBufferDC.h"
#include "SpartyCrossing.h"
#include "ChildView.h"
#include "IsCargoVisitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Frame duration in milliseconds
const int FrameDuration = 1;

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_LEVEL_LEVEL0, &CChildView::OnLevelLevel0)
	ON_COMMAND(ID_LEVEL_LEVEL1, &CChildView::OnLevelLevel1)
	ON_COMMAND(ID_LEVEL_LEVEL2, &CChildView::OnLevelLevel2)
	ON_COMMAND(ID_LEVEL_LEVEL3, &CChildView::OnLevelLevel3)
	ON_COMMAND(ID_CHEAT_ROADCHEAT, &CChildView::OnCheatRoadcheat)
	ON_COMMAND(ID_CHEAT_RIVERCHEAT, &CChildView::OnCheatRivercheat)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_CHEAT_ROADCHEAT, &CChildView::OnUpdateCheatRoadcheat)
	ON_UPDATE_COMMAND_UI(ID_CHEAT_RIVERCHEAT, &CChildView::OnUpdateCheatRivercheat)
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

/**
 * Function called when assets are drawn
 */
void CChildView::OnPaint()
{
	CPaintDC paintDC(this);
	CDoubleBufferDC dc(&paintDC); // device context for painting
	Gdiplus::Graphics graphics(dc.m_hDC);

	CRect rect;
	GetClientRect(&rect);


	if (mFirstLevelLoad)
	{
		mFirstLevelLoad = false;
		mGame.OnStartup();
		OnLevelLevel1();
	}

	mGame.OnDraw(&graphics, rect.Width(), rect.Height());

	if (mFirstDraw)
	{
		mFirstDraw = false;
		SetTimer(1, FrameDuration, nullptr);

		//Elapsed Time System Similar to Step 3
		LARGE_INTEGER time, freq;
		QueryPerformanceCounter(&time);
		QueryPerformanceFrequency(&freq);


		mLastTime = time.QuadPart;
		mTimeFreq = double(freq.QuadPart);
	}

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	long long diff = time.QuadPart - mLastTime;
	double elapsed = double(diff) / mTimeFreq;
	mLastTime = time.QuadPart;

	mGame.OnUpdateCall(elapsed);
}

// MENU BAR

// Level Menus

/**
 * Loads Level 0 into game
 */
void CChildView::OnLevelLevel0()
{
	mGame.OnLevelLoad(0);
}

/**
 * Loads level 1 into game
 */
void CChildView::OnLevelLevel1()
{
	mGame.OnLevelLoad(1);
}

/**
 * Loads level 2 into game
 */
void CChildView::OnLevelLevel2()
{
	mGame.OnLevelLoad(2);
}

/**
 * Loads Level 3 into game
 */
void CChildView::OnLevelLevel3()
{
	mGame.OnLevelLoad(3);
	
}

// Cheat Menus


/**
 * Change setting for road cheat
 */
void CChildView::OnCheatRoadcheat()
{
	mRoadCheatActive = !mRoadCheatActive;
	
	mGame.SetRoadCheat(mRoadCheatActive);

	if (!mRoadCheatActive && mGame.GetHero() != nullptr)
	{
		mGame.CrashCheck();
	}
}


/**
 * Change setting for river cheat
 */
void CChildView::OnCheatRivercheat()
{
	mRiverCheatActive = !mRiverCheatActive;


	mGame.SetRiverCheat(mRiverCheatActive);
	if (!mRiverCheatActive && !mGame.GetHeroes().empty()) 
	{

		for (shared_ptr<CHero> hero : mGame.GetHeroes())
		{
			hero->OverBoat();
			hero->CheckDrown();
		}

	}
}

/**
 * Change check mark display for Road Cheat
 * \param pCmdUI UI to be changed
 */
void CChildView::OnUpdateCheatRoadcheat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mRoadCheatActive);
}

/**
 * Change check mark display for River Cheat
 * \param pCmdUI UI to be changed
 */
void CChildView::OnUpdateCheatRivercheat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(mRiverCheatActive);
}


/**
 * Handle timer events
 * \param nIDEvent The timer event ID
 */
void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate();
	CWnd::OnTimer(nIDEvent);
}


/**
 * Erase the background
 *
 * This is disabled to eliminate flicker
 * \param pDC Device context
 * \returns FALSE
 */
BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}


/** 
 * Called when there is a left mouse button press
 * \param nFlags Flags associated with the mouse button press
 * \param point Where the button was pressed
*/
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	mGrabbedItem = mGame.HitTest(point.x, point.y);
	if (mGrabbedItem != nullptr)
	{
		CIsCargoVisitor visitor;
		mGrabbedItem->Accept(&visitor);
		if (visitor.IsCargo())
		{
			if (mGame.GetHeroes().size() == 2)
			{
				if (mGame.GetHero(0)->GetHeldItem() == mGrabbedItem || mGame.GetHero(1)->GetHeldItem() == mGrabbedItem)
				{
					mGame.SwapItem(mGrabbedItem);
					mGame.DropItem(mGrabbedItem, &visitor);
				}
				else
				{
					mGame.PickUpItem(mGrabbedItem, &visitor);
				}
				
			}
			else if (mGame.GetHero()->GetHeldItem() == mGrabbedItem)
			{
				mGame.DropItem(mGrabbedItem, &visitor);
			}
			else
			{
				mGame.PickUpItem(mGrabbedItem, &visitor);
			}
		}
	}
}


/**
 * 
 * \param nChar Contains the character code value of the key.
 * \param nRepCnt The number of times the keystroke is repeated when holding down the key
 * \param nFlags Flags associated with the mouse button press
 */
void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 1=up, 2=right, 3=down, 4=left
	switch (nChar)
	{
	// Moving Hero Up
	case'E':
		mGame.MoveHero(1, 0);
		break;
	case VK_UP:
		mGame.MoveHero(1, -1);
		break;
	// Moving Hero Down
	case'D':
		mGame.MoveHero(3, 0);
		break;
	case VK_DOWN:
		mGame.MoveHero(3, -1);
		break;
	// Moving Hero Right
	case'F':
		mGame.MoveHero(2 , 0);
		break;
	case VK_RIGHT:
		mGame.MoveHero(2, -1);
		break;
	// Moving Hero Left
	case'S':
		mGame.MoveHero(4, 0);
		break;
	case VK_LEFT:
		mGame.MoveHero(4, -1);
		break;
	}
}