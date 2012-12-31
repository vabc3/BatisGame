#include "StdAfx.h"
#include "BatisGame.h"
#include <cstdio>
#include <algorithm>
using namespace std;

int Boint::ap;

BatisGame::BatisGame(int numberOfPlayer,int boardSize,int Hnum,int AILevel)
{
	Current				= new ChessBoard(numberOfPlayer,boardSize);
	if(Current->IsValid()){
		Players			= new Character[numberOfPlayer+1];
		for(int i=1;i<=Hnum;i++)Players[i]=HUMAN;
		for(int i=Hnum+1;i<=numberOfPlayer;i++)Players[i]=AI;
		GStatus			= PRE;
		TStatus			= NORMAL;
		TurnDone		= false;
		Turn			= 0;
		ActivePlayer	= 1;
		Passes			= 0;
		NumberOfPlayer	= numberOfPlayer;
		BoardSize		= boardSize;
		Strategy=Strategies[AILevel];
	}
}

BatisGame::~BatisGame(void)
{
	if(Current && Current->IsValid()){
		if(Players) delete [] Players;
		delete Current;
	}
}

void BatisGame::Start()
{
	if(!(Current->IsValid())){
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
	vector<Boint> BTable=GetMark(ActivePlayer,Current);
	if(BTable.size()==0){
		TStatus=PASS;
		Passes++;
	}else if(BTable.size()==1){
		Bone=BTable[0];
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
		Put(Bone.x,Bone.y);
		TurnDone	= true;
		break;
	case PASS:
		TurnDone	= true;
		break;
	case NORMAL:
		switch(Players[ActivePlayer]){
		case AI:
			Put(Strategy(ActivePlayer,Current));
			break;
		case HUMAN:
			break;
		}
		TurnDone	= true;
		break;
	}
	return true;
}

bool BatisGame::Put(Boint bo)
{
	return Put(bo.x,bo.y);
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

TurnStatus BatisGame::GetTurnStatus() const
{
	return TStatus;
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

GameStatus BatisGame::GetGameStatus() const
{
	return GStatus;
}

int BatisGame::GetNumberOfPlayer() const
{
	return NumberOfPlayer;
}

bool BatisGame::IsWaitingForInput() const
{
	return (!TurnDone && Players[ActivePlayer]==HUMAN);
}