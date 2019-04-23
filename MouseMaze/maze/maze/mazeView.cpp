
// mazeView.cpp : CmazeView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "maze.h"
#endif

#include "mazeDoc.h"
#include "mazeView.h"
#include <stdlib.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmazeView

IMPLEMENT_DYNCREATE(CmazeView, CView)

BEGIN_MESSAGE_MAP(CmazeView, CView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_MAZE_GENERATEMAZE, &CmazeView::OnGeneratemaze)
	ON_COMMAND(ID_MAZE_ESCAPEMAZE, &CmazeView::OnEscapemaze)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CmazeView 생성/소멸

CmazeView::CmazeView()
{
		bTimer = true;
	PlayerMotion = RIGHT;
	nMazeRow = nMazeCol = 1;
	maze = NULL;
	escape = false;
	bMove = true;
	ModeCheck = 0; 
}

CmazeView::~CmazeView()
{
}

BOOL CmazeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

void CmazeView::drawMaze(CDC &dc) {
	CRect rect;
	GetClientRect(rect);

	if((rect.right/nMazeCol) < (rect.bottom/nMazeRow))
		cellLength = (rect.right-MARGIN*2)/nMazeCol;
	else
		cellLength = (rect.bottom-MARGIN*2)/nMazeRow;

	CPoint lt;
	lt.x = rect.right/2 - cellLength*nMazeCol/2;
	lt.y = rect.bottom/2 - cellLength*nMazeRow/2;

	for(int r = 0; r < nMazeRow; r++)
		for(int c = 0; c < nMazeCol; c++) {
			if(maze == NULL || maze[r][c].state[WEST] != EMPTY) {
				dc.MoveTo(lt.x+c*cellLength, lt.y+r*cellLength);
				dc.LineTo(lt.x+c*cellLength, lt.y+(r+1)*cellLength);
			}

			if(maze == NULL || maze[r][c].state[EAST] != EMPTY) {
				dc.MoveTo(lt.x+(c+1)*cellLength, lt.y+r*cellLength);
				dc.LineTo(lt.x+(c+1)*cellLength, lt.y+(r+1)*cellLength);
			}

			if(maze == NULL || maze[r][c].state[NORTH] != EMPTY) {
				dc.MoveTo(lt.x+c*cellLength, lt.y+r*cellLength);
				dc.LineTo(lt.x+(c+1)*cellLength, lt.y+r*cellLength);
			}

			if(maze == NULL || maze[r][c].state[SOUTH] != EMPTY) {
				dc.MoveTo(lt.x+c*cellLength, lt.y+(r+1)*cellLength);
				dc.LineTo(lt.x+(c+1)*cellLength, lt.y+(r+1)*cellLength);
			}
		}
}

void CmazeView::drawMouse(CDC &dc) {
		CRect rect;
		GetClientRect(rect);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CPoint lt;
		lt.x = rect.right/2 - cellLength*nMazeCol/2;
		lt.y = rect.bottom/2 - cellLength*nMazeRow/2;

		CBrush greenBrush(RGB(0,255,0));
		dc.SelectObject(greenBrush);
		dc.SelectStockObject(NULL_PEN);
		CBitmap PlayerM;
		switch(PlayerMotion) {
		case LEFT :
		PlayerM.LoadBitmapA(IDB_BITMAP4);
			break;
		case RIGHT :
		PlayerM.LoadBitmapA(IDB_BITMAP1);
			break;
		case UP :
		PlayerM.LoadBitmapA(IDB_BITMAP2);
			break;
		case DOWN :
		PlayerM.LoadBitmapA(IDB_BITMAP3);
			break;
		}
		BITMAP bitmap;
		PlayerM.GetBitmap(&bitmap);
		memDC.SelectObject(PlayerM);
		//dc.Ellipse(lt.x+Player.x*cellLength, lt.y+Player.y*cellLength, 
		//	lt.x+(Player.x+1)*cellLength, lt.y+(Player.y+1)*cellLength);
		
		
	if(bMove == false) {
		for(int i = 0; i < ls.size(); i++) {
			dc.Ellipse(lt.x+ls[i].x*cellLength, lt.y+ls[i].y*cellLength,  lt.x+(ls[i].x+1)*cellLength, lt.y + (ls[i].y+1)*cellLength);
		}
	}
	dc.TransparentBlt(lt.x+Player.x*cellLength, lt.y+Player.y*cellLength, 
			cellLength,cellLength, &memDC,
			0,0,bitmap.bmWidth, bitmap.bmHeight, RGB(255,255,255));

}

void CmazeView::OnDraw(CDC* pDC)
{
	if(bTimer) SetTimer(1,150,NULL);
	CRect rect;
	GetClientRect(rect);

	
	CDC backDC;
	backDC.CreateCompatibleDC(pDC);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rect.right, rect.bottom);

	backDC.SelectObject(bitmap);
	backDC.PatBlt(0, 0, rect.right, rect.bottom, WHITENESS);

	drawMaze(backDC);

	if(maze) 
		drawMouse(backDC);


	pDC->BitBlt(0, 0, rect.right, rect.bottom, &backDC, 0, 0, SRCCOPY);
}


// CmazeView 진단

#ifdef _DEBUG
void CmazeView::AssertValid() const
{
	CView::AssertValid();
}

void CmazeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmazeDoc* CmazeView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmazeDoc)));
	return (CmazeDoc*)m_pDocument;
}
#endif //_DEBUG


// CmazeView 메시지 처리기
void CmazeView::releaseMaze() {
	if(maze == NULL) return;

	for(int r = 0; r < nMazeRow; r++)
		delete []maze[r];
	delete []maze;
	maze = NULL;
}

void CmazeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	//switch(nChar) {
	//case VK_LEFT:
	//	if(maze[Player.y][Player.x].state[WEST] != BOUNDARY &&  maze[Player.y][Player.x-1].state[EAST] == EMPTY)
	//	Player.x--;
	//	PlayerMotion = LEFT;
	//	break;
	//
	//case VK_RIGHT:
	//	if(maze[Player.y][Player.x].state[EAST] != BOUNDARY &&  maze[Player.y][Player.x+1].state[WEST] == EMPTY)
	//	Player.x++;
	//	PlayerMotion = RIGHT;
	//	break;
	//
	//case VK_UP:
	//	if(maze[Player.y][Player.x].state[NORTH] != BOUNDARY &&  maze[Player.y-1][Player.x].state[SOUTH] == EMPTY)
	//	Player.y--;
	//	PlayerMotion = UP;
	//	break;
	//
	//case VK_DOWN:
	//	if(maze[Player.y][Player.x].state[SOUTH] != BOUNDARY &&  maze[Player.y+1][Player.x].state[NORTH] == EMPTY)
	//	Player.y++;
	//	PlayerMotion = DOWN;
	//	break;
	//}
switch(nChar) {
	case VK_LEFT:
		if(ModeCheck) {
			if(Player.x <= 0 ) return;
	if(Player.x == nMazeCol-1  &&  Player.y == mazeExit) break;
		if(maze[Player.y][Player.x].state[WEST] == EMPTY)
			Player.x--;
		PlayerMotion = 2;
			break;
		}
			if(nMazeCol > 1) nMazeCol--;
			releaseMaze();	
			break;

	case VK_RIGHT:
		if(ModeCheck) {
			if(Player.x >= nMazeCol-1 ) return;
	if(Player.x == nMazeCol-1  &&  Player.y == mazeExit) break;
		if(maze[Player.y][Player.x].state[EAST] == EMPTY)
			Player.x++;
		PlayerMotion = 3;
			break;
		}
			nMazeCol++;
			releaseMaze();	
			break;

	case VK_UP:
		if(ModeCheck) {
			if(Player.y <= 0 ) return;
	if(Player.x == nMazeCol-1  &&  Player.y == mazeExit) break;
	if(maze[Player.y][Player.x].state[NORTH] == EMPTY)
			Player.y--;
	PlayerMotion = 0;
			break;
		}
			if(nMazeRow > 1) nMazeRow--;
			releaseMaze();	
			break;

	case VK_DOWN:
		if(ModeCheck) {
			if(Player.y >= nMazeRow-1 ) return;
	if(Player.x == nMazeCol-1  &&  Player.y == mazeExit) break;
	if(maze[Player.y][Player.x].state[SOUTH] == EMPTY)
			Player.y++;
	PlayerMotion = 1;
			break;
		}
			nMazeRow++;
			releaseMaze();	
			break;
}

	Invalidate();
	if(Player.x == nMazeCol-1  &&  Player.y == mazeExit) {
		AfxMessageBox(TEXT("Clear!"));
		return;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}




void CmazeView::generateMaze(int cr, int cc) {
	maze[cr][cc].visited = VISITED;

	while(1) {
		int count = 0;
		if(cc != 0 && maze[cr][cc-1].visited == UNVISITED) count++;
		if(cc != nMazeCol-1 && maze[cr][cc+1].visited == UNVISITED) count++;
		if(cr != nMazeRow-1 && maze[cr+1][cc].visited == UNVISITED) count++;
		if(cr != 0 && maze[cr-1][cc].visited == UNVISITED) count++;
		if(count == 0) break;

		int dir;
		while(1) {
			dir = rand()%4;
			if(dir == WEST && maze[cr][cc].state[WEST] != BOUNDARY && maze[cr][cc-1].visited == UNVISITED) break;
			if(dir == EAST && maze[cr][cc].state[EAST] != BOUNDARY && maze[cr][cc+1].visited == UNVISITED) break;
			if(dir == SOUTH && maze[cr][cc].state[SOUTH] != BOUNDARY && maze[cr+1][cc].visited == UNVISITED) break;
			if(dir == NORTH && maze[cr][cc].state[NORTH] != BOUNDARY && maze[cr-1][cc].visited == UNVISITED) break;
		}

		switch(dir) {
			case WEST:
					maze[cr][cc].state[WEST] = EMPTY;
					maze[cr][cc-1].state[EAST] = EMPTY;
					generateMaze(cr, cc-1);
					break;

			case EAST:
					maze[cr][cc].state[EAST] = EMPTY;
					maze[cr][cc+1].state[WEST] = EMPTY;
					generateMaze(cr, cc+1);
					break;

			case SOUTH:
					maze[cr][cc].state[SOUTH] = EMPTY;
					maze[cr+1][cc].state[NORTH] = EMPTY;
					generateMaze(cr+1, cc);
					break;

			case NORTH:
					maze[cr][cc].state[NORTH] = EMPTY;
					maze[cr-1][cc].state[SOUTH] = EMPTY;
					generateMaze(cr-1, cc);
					break;
		}
	}
}

void CmazeView::makeEnterExit() {
	mazeEnter = rand()%nMazeRow;
	mazeExit  = rand()%nMazeRow;

	maze[mazeEnter][0].state[WEST] = EMPTY;
	maze[mazeExit][nMazeCol-1].state[EAST] = EMPTY;
}

void CmazeView::OnGeneratemaze()
{
	ls.clear();
	if(maze) releaseMaze();

	maze = new CELL*[nMazeRow];
	for(int r = 0; r < nMazeRow; r++) {
		maze[r] = new CELL[nMazeCol];
	}

	for(int r = 0; r < nMazeRow; r++) {
		for(int c = 0; c < nMazeCol; c++) {
			for(int i = 0; i < 4; i++) maze[r][c].state[i] = WALL;
			if(r == 0) maze[r][c].state[NORTH] = BOUNDARY;
			if(c == 0) maze[r][c].state[WEST] = BOUNDARY;
			if(r == nMazeRow-1) maze[r][c].state[SOUTH] = BOUNDARY;
			if(c == nMazeCol-1) maze[r][c].state[EAST] = BOUNDARY;

			maze[r][c].visited = UNVISITED;
		}
	}

	srand(time(NULL));
	generateMaze(rand()%nMazeRow, rand()%nMazeCol);
	makeEnterExit();
	Player.x = 0;
	Player.y = mazeEnter;
	ModeCheck = 1;
	Invalidate();
}

	bool CmazeView::escapeMaze(int cr, int cc) {
		
		ls.push_back(CPoint(cc,cr));
	if(cc == nMazeCol-1  &&  cr == mazeExit) return true;
		maze[cr][cc].visited = VISITED;
		//Invalidate();

		
	//	UpdateWindow();
		while(1) {
			int count = 0;
			if(cc != 0 && maze[cr][cc].state[WEST] == EMPTY && maze[cr][cc-1].visited == UNVISITED) count++;
			if(cc != nMazeCol-1 && maze[cr][cc].state[EAST] == EMPTY && maze[cr][cc+1].visited == UNVISITED) count++;
			if(cr != nMazeRow-1 && maze[cr][cc].state[SOUTH] == EMPTY && maze[cr+1][cc].visited == UNVISITED) count++;
			if(cr != 0 && maze[cr][cc].state[NORTH] == EMPTY && maze[cr-1][cc].visited == UNVISITED) count++;
			if(count == 0) break;

			int dir;
			while(1) {
				dir = rand()%4;
				if(dir == WEST && cc != 0 && maze[cr][cc].state[WEST] == EMPTY && maze[cr][cc-1].visited == UNVISITED) break;
				if(dir == EAST && maze[cr][cc].state[EAST] == EMPTY && maze[cr][cc+1].visited == UNVISITED) break;
				if(dir == SOUTH && maze[cr][cc].state[SOUTH] == EMPTY && maze[cr+1][cc].visited == UNVISITED) break;
				if(dir == NORTH && maze[cr][cc].state[NORTH] == EMPTY && maze[cr-1][cc].visited == UNVISITED) break;
			}

			switch(dir) {
			case WEST:
				(CopyPlayer).x--;
				if(escapeMaze(cr, cc-1) == true) return true;
				(CopyPlayer).x++;
				break;

			case EAST:
				(CopyPlayer).x++;
				if(escapeMaze(cr, cc+1) == true) return true;
				(CopyPlayer).x--;
				break;

			case SOUTH:
				(CopyPlayer).y++;
				if(escapeMaze(cr+1, cc) == true) return true;
				(CopyPlayer).y--;
				break;

			case NORTH:
				(CopyPlayer).y--;
				if(escapeMaze(cr-1, cc) == true) return true;
				else (CopyPlayer).y++;
				break;
			}

		}
		ls.pop_back();
		ModeCheck = 0;
		return false;
	}

void CmazeView::OnEscapemaze()
{

	ls.clear();
	MoveCount = 0;
	bMove = true;
	if(maze == NULL) return;

	Player.x = 0;
	Player.y = mazeEnter;
	
	for(int r = 0; r < nMazeRow; r++) 
		for(int c = 0; c < nMazeCol; c++)
			maze[r][c].visited = UNVISITED;
	CopyPlayer = Player;
	escapeMaze(mazeEnter, 0);

	
}


void CmazeView::OnTimer(UINT_PTR nIDEvent)
{
	bTimer = false;
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(bMove) {
		PlayerMove();
		
	}
	CView::OnTimer(nIDEvent);
}
