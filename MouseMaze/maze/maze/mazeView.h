
// mazeView.h : CmazeView Ŭ������ �������̽�
//

#pragma once
#include <list>
#include <vector>
using namespace std;



#define MARGIN		10
#define WEST			0
#define EAST			1
#define NORTH		2
#define SOUTH		3
#define WALL			5
#define EMPTY		6
#define BOUNDARY		7
#define VISITED		8
#define UNVISITED		9

typedef struct {
	int state[4];
	int visited;
} CELL;


class CmazeView : public CView
{
protected: // serialization������ ��������ϴ�.
	CmazeView();
	DECLARE_DYNCREATE(CmazeView)

// Ư���Դϴ�.
public:
	CmazeDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CmazeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

private:
	void drawMaze(CDC &);
	void drawMouse(CDC &);
	void generateMaze(int, int);
	bool escapeMaze(int, int);
	void releaseMaze();
	void makeEnterExit();
	bool bTimer;
	int nMazeRow, nMazeCol;
	int cellLength;
	int mazeEnter, mazeExit;
	bool escape;
	bool bMove;
	vector<CPoint> ls;
	CPoint Player;
	CPoint CopyPlayer;
	CELL **maze;
	int PlayerMotion;
	int ModeCheck;
	int MoveCount;
enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
public:
	void PlayerMove() {
		if(MoveCount < 0 || MoveCount > ls.size() ) MoveCount = 0;
		if(MoveCount >= ls.size()) {

			return;
		}if(MoveCount > 1) {
		if(ls[MoveCount].x > ls[MoveCount-1].x) PlayerMotion = RIGHT;
		if(ls[MoveCount].x < ls[MoveCount-1].x) PlayerMotion = LEFT;
		if(ls[MoveCount].y > ls[MoveCount-1].y) PlayerMotion = DOWN;
		if(ls[MoveCount].y < ls[MoveCount-1].y) PlayerMotion = UP;
		}
		
		Player.x = ls[MoveCount].x;
		Player.y = ls[MoveCount].y;
		MoveCount++;
		if(MoveCount == ls.size()) {
			bMove = false;
				PlayerMotion = RIGHT;
		}
		
	Invalidate();
	}
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnGeneratemaze();
	afx_msg void OnEscapemaze();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // mazeView.cpp�� ����� ����
inline CmazeDoc* CmazeView::GetDocument() const
   { return reinterpret_cast<CmazeDoc*>(m_pDocument); }
#endif

