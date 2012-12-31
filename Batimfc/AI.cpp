#include "StdAfx.h"
#include "AI.h"
#include "ChessBoard.h"
#include <algorithm>
using namespace std;

vector<Boint> GetMark(int PlayerID,const ChessBoard* board)
{
	vector<Boint> dat;
	int size=board->GetBoardSize();
	for(int y=0;y<size;y++)
		for(int x=0;x<size;x++){
			ChessBoard c=*board;
			if(c.Place(PlayerID,x,y)){
				Boint boint;
				boint.x=x;
				boint.y=y;
				boint.n=board->GetBoardSize();
				boint.mark=c.GetStatus(PlayerID);
				dat.push_back(boint);
			}
		}
	return dat;
}

Boint AIStrategy1(int PlayerID,const ChessBoard* board)
{
	vector<Boint> bt=GetMark(PlayerID,board);
	sort(bt.begin(),bt.end());
	return bt[0];
}

AIStrategy Strategies[3]={AIStrategy1};