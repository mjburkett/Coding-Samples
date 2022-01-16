
// ChildView.h : interface of the CChildView class
//


#pragma once

#include "Game.h"


/**
 * CChildView window
 */
class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CGame   mGame; ///< The game
	bool mFirstDraw = true; ///< Bool for if window has drawn before
	bool mFirstLevelLoad = true; ///< Bool for if a Level has loaded before
	long long mLastTime = 0; ///< Last time we read the timer
	double mTimeFreq = 0; ///< Rate the timer updates
	std::shared_ptr<CItem> mGrabbedItem = nullptr; ///< Item that was clicked
	bool mRoadCheatActive = false; ///< Determines whether to ignore car collisions
	bool mRiverCheatActive = false; ///< Determines whether to ignore boats and rivers for hero
	
public:
	afx_msg void OnLevelLevel0();
	afx_msg void OnLevelLevel1();
	afx_msg void OnLevelLevel2();
	afx_msg void OnLevelLevel3();
	afx_msg void OnCheatRoadcheat();
	afx_msg void OnCheatRivercheat();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateCheatRoadcheat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCheatRivercheat(CCmdUI* pCmdUI);
};

