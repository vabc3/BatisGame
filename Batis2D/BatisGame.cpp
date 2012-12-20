#include "BatisGame.h"
#include <cstdio>
#include <algorithm>
using namespace std;

int Boint::ap;

BatisGame::BatisGame(int numberOfPlayer,int boardSize)
{
	Current			= new ChessBoard(numberOfPlayer,boardSize);
	if(Current->isValid()){
		Players			= new Character[numberOfPlayer+1];
		Players[1]		= HUMAN;
		for(int i=1;i<=numberOfPlayer;i++)Players[i]=AI;
		GStatus			= PRE;
		TStatus			= NORMAL;
		TurnDone		= false;
		Turn			= 0;
		ActivePlayer	= 1;
		Passes			= 0;
		NumberOfPlayer	= numberOfPlayer;
		BoardSize		= boardSize;
	}
}

BatisGame::~BatisGame(void)
{
	if(Current && Current->isValid()){
		if(Players) delete [] Players;
		delete Current;
	}
}


vector<Boint> BatisGame::GetMark(int PlayerID,const ChessBoard& board)
{
	vector<Boint> dat;
	int size=board.GetBoardSize();
	for(int y=0;y<size;y++)
		for(int x=0;x<size;x++){
			ChessBoard c=board;
			if(c.Place(PlayerID,x,y)){
				Boint boint;
				boint.x=x;
				boint.y=y;
				boint.n=board.GetBoardSize();
				boint.mark=c.GetStatus(PlayerID);
				dat.push_back(boint);
			}
		}

	sort(dat.begin(),dat.end());
	return dat;
}

void BatisGame::Start()
{
	if(!(Current->isValid())){
		return;
	}

	if(GStatus!=PRE){
		return;
	}

	GStatus	= RUNNING;
	TStatus	= NORMAL;
	Turn	= 1;
	TurnDone= false;
	ActivePlayer=1;
	BTableRefresh();
}

void BatisGame::Next()
{
	if(GStatus!=RUNNING){
		return;
	}

	if(Current->GetStatus(0)==0 || Passes>=NumberOfPlayer){
		GStatus=OVER;
		return;
	}

	if(!TurnDone){
		return;
	}
	if(ActivePlayer == NumberOfPlayer){
		ActivePlayer = 1;
		Turn++;
	}else{
		ActivePlayer++;
	}

	Boint::ap=ActivePlayer;
	BTableRefresh();
	TurnDone = false;

	
}

void BatisGame::BTableRefresh()
{
	BTable=GetMark(ActivePlayer,*Current);
	if(BTable.size()==0){
		TStatus=PASS;
		Passes++;
	}else if(BTable.size()==1){
		TStatus=AUTO;
		Passes=0;
	}else{
		TStatus=NORMAL;
		Passes=0;
	}
}

bool BatisGame::Put()
{
	if(GStatus!=RUNNING){
		return false;
	}

	if(TurnDone){
		return false;
	}

	switch(TStatus){
	case AUTO:
		Put(BTable[0].x,BTable[0].y);
		TurnDone	= true;
		break;
	case PASS:
		TurnDone	= true;
		break;
	case NORMAL:
		switch(Players[ActivePlayer]){
		case AI:
			Put(BTable[0].x,BTable[0].y);
			break;
		case HUMAN:
			break;
		}
		TurnDone	= true;
		break;
	}
	return true;
}

bool BatisGame::Put(int x,int y)
{
	if(GStatus!=RUNNING){
		return false;
	}

	if(TurnDone){
		return false;
	}
	TurnDone= Current->Place(ActivePlayer,x,y);
	return TurnDone;
}

//void Undo(int PlayerID);

int BatisGame::GetTurn() const
{
	return Turn;
}

int BatisGame::GetActivePlayer() const
{
	return ActivePlayer;
}

bool BatisGame::IsGameOver() const
{
	return GStatus==OVER;
}

BatisGame::TurnStatus BatisGame::GetTurnStatus() const
{
	return TStatus;
}


void BatisGame::Show()
{
	Current->Print();
	
	/*printf("游戏状态:%s,轮状态:%s,轮结束:%d,轮数:%d,选手:%d,跳过:%d,历史:%d\n",
		GameStatusMsg[GStatus],TurnStatusMsg[TStatus],TurnDone,Turn,ActivePlayer,Passes,History.size());*/
	/*GameStatus GStatus;
	TurnStatus TStatus;
	bool TurnDone;
	int  Turn,ActivePlayer,NumberOfPlayer,BoardSize;
	Character* Players;
	ChessBoard* Current;
	std::vector<ChessBoard> history;*/
}


int BatisGame::GetBoardSize() const
{
	return BoardSize;
}


const ChessBoard* BatisGame::GetBoard() const
{
	return Current;
}

bool BatisGame::AutoGo() 
{
	if(GStatus!=RUNNING){
		return false;
	}

	if(TurnDone){
		Next();
		return true;
	}

	if(Players[ActivePlayer]==AI){
		Put();
		Next();
	}else{
		if(TStatus== NORMAL){
			return false;
		}else{
			Put();
			Next();
		}
	}
		

	return true;
}

BatisGame::GameStatus BatisGame::GetGameStatus() const
{
	return GStatus;
}

int BatisGame::GetNumberOfPlayer() const
{
	return NumberOfPlayer;
}

std::vector<Boint> BatisGame::GetBTable()
{
	return BTable;
}

bool BatisGame::IsWaitingForInput()
{
	return (!TurnDone&& Players[ActivePlayer]==HUMAN);
}