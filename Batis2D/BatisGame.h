#pragma once
#include "ChessBoard.h"
#include<vector>
#include<ostream>

class Boint
{
public:
	static int ap;
	int x,y,mark;
	int n;
	bool operator < (const Boint &m)const {
		if(ap==3 || ap==1){
			if(isBan(x,y,n)^isBan(m.x,m.y,m.n)){
				return !isBan(x,y,n);
			}else if(isCorner(x,y,n)^isCorner(m.x,m.y,m.n)){
				return isCorner(x,y,n);
			}else if(isEdge(x,y,n)^isEdge(m.x,m.y,m.n)){
				return isEdge(x,y,n);
			}
		}
		return mark > m.mark;
    }
	friend std::ostream& operator<<(std::ostream& os,const Boint& bo)
	{
		os<<"["<<bo.x<<","<<bo.y<<"]->"<<bo.mark;
		return os;
	}
private:
	inline bool isCorner(int x, int y,int n) const{

		return ((x==0 && y==0) || (x==0 &&y==n-1) ||
				(x==n-1&&y==0) || (y==n-1 && x==n-1));
	}

	inline bool isEdge(int x,int y ,int n)const {
		return (x==0 || y==0 || x==n-1 || y==n-1);
	}

	inline bool isBan(int x,int y,int n)const {
		return (!isCorner(x,y,n) && ( (x<=1 || x>=n-2) && ( y<=1 || y>=n-2)));
	}
};

class BatisGame
{
public:
	enum Character{
		AI,HUMAN
	};
	enum TurnStatus{
		AUTO,PASS,NORMAL
	};
	enum GameStatus{
		PRE,RUNNING,OVER
	};
	 
	BatisGame(int numberOfPlayer,int boardSize);
	~BatisGame(void);
	
	//Status Query
	int GetBoardSize() const;
	int GetTurn() const;
	int GetActivePlayer() const;
	bool IsGameOver() const;
	int GetNumberOfPlayer() const;
	TurnStatus GetTurnStatus() const;
	GameStatus GetGameStatus() const;

	//Action
	void Start();
	void Next();
	void Show();
	bool Put();
	bool Put(int x,int y);
	bool IsWaitingForInput();
	void Undo(int PlayerID);
	static std::vector<Boint> GetMark(int PlayerID,const ChessBoard& board);
	const ChessBoard* GetBoard()const;
	bool AutoGo();
	std::vector<Boint> GetBTable();
private:
	void BTableRefresh();

	GameStatus GStatus;
	TurnStatus TStatus;
	bool TurnDone;
	int  Turn,ActivePlayer,NumberOfPlayer,BoardSize,Passes;
	Character* Players;
	ChessBoard* Current;
	std::vector<ChessBoard> History;
	std::vector<Boint> BTable;
};

