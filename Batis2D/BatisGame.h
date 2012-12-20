#pragma once
#include "ChessBoard.h"
#include "AI.h"

enum Character{
	AI,HUMAN
};
enum TurnStatus{
	AUTO,PASS,NORMAL
};
enum GameStatus{
	PRE,RUNNING,OVER
};

class BatisGame
{
public:
	BatisGame(int numberOfPlayer,int boardSize,int Hnum=0,int AILevel=0);
	~BatisGame(void);
	
	//Status Query
	int GetNumberOfPlayer() const;
	int GetBoardSize() const;
	int GetTurn() const;
	int GetActivePlayer() const;
	bool IsGameOver() const;
	bool IsWaitingForInput();
	
	TurnStatus GetTurnStatus() const;
	GameStatus GetGameStatus() const;

	//Action
	void Start();
	void Next();
	bool Put();
	bool Put(Boint bo);
	bool Put(int x,int y);
	bool AutoGo();
	const ChessBoard* GetBoard()const;
	//void Undo(int PlayerID);
private:
	AIStrategy Strategy;
	void BTableRefresh();
	GameStatus GStatus;
	TurnStatus TStatus;
	bool TurnDone;
	int  Turn,ActivePlayer,NumberOfPlayer,BoardSize,Passes;
	Character* Players;
	ChessBoard* Current;
	Boint Bone;
	//std::vector<ChessBoard> History;
};

