#include "StdAfx.h"
#include "ChessBoard.h"
#include "BatisConstant.h"
#include <string.h>

const int ChessBoard::TraceTable[8][2]={
	{ 0,-1}, { 1,-1},
	{ 1, 0}, { 1, 1},
	{ 0, 1}, {-1, 1},
	{-1, 0}, {-1,-1},
};

void ChessBoard::Init()
{
	Valid			= false;
	Board			= NULL;
	Count			= NULL;
	BoardSize		= 0;
	NumberOfPlayer	= 0;
}

ChessBoard::ChessBoard()
{
	Init();
}

ChessBoard::ChessBoard(int numberOfPlayer,int size)
{
	Init();
	Valid		= true;
	if(numberOfPlayer< BatisConstant::nPlayerMin || numberOfPlayer> BatisConstant::nPlayerMax){
		Valid	= false;
	}else if(size<BatisConstant::nBoardSizeMin || size> BatisConstant::nBoardSizeMax){
		Valid	= false;
	}else if(size<=numberOfPlayer){
		Valid	= false;
	}else if((size-numberOfPlayer)%2 !=0){
		Valid	= false;
	}

	if(Valid){
		NumberOfPlayer	= numberOfPlayer;
		BoardSize		= size;
		Board			= new int[BoardSize*BoardSize];
		Count			= new int[BoardSize+1];
		memset(Board,0,BoardSize*BoardSize*sizeof(int));
		memset(Count,0,(BoardSize+1)*sizeof(int));
		Count[0]		= BoardSize*BoardSize;
		int beg			= (BoardSize-NumberOfPlayer)/2;
		for(int i = beg; i < beg+NumberOfPlayer; i++)
			for(int j = beg; j < beg+NumberOfPlayer; j++)
				Set((i+j*(NumberOfPlayer-1)-beg*2) % NumberOfPlayer + 1,i,j);
	}
}

ChessBoard::~ChessBoard(void)
{
	if(Valid){
		if(Board) delete [] Board;
		if(Count) delete [] Count;
	}
}

ChessBoard& ChessBoard::operator=(const ChessBoard& source)
{
	if(this!=&source){
		Valid			= source.Valid;
		NumberOfPlayer	= source.NumberOfPlayer;
		BoardSize		= source.BoardSize;
		Board			= new int[BoardSize*BoardSize];
		Count			= new int[BoardSize+1];
		memcpy(Board,source.Board,BoardSize*BoardSize*sizeof(int));
		memcpy(Count,source.Count,(BoardSize+1)*sizeof(int));
	}
	return *this;
}

ChessBoard::ChessBoard(const ChessBoard& source)
{
	if(this!=&source)
		*this = source;
}

int ChessBoard::GetNumberOfPlayer() const
{
	return NumberOfPlayer;
}

int ChessBoard::GetBoardSize() const
{
	return BoardSize;
}

bool ChessBoard::Place(int PlayerID,int x,int y)
{
	bool sd[8];
	bool rt=false;
	for(int i=0;i<8;i++){
		sd[i]=Trace(PlayerID,x,y,TraceTable[i][0],TraceTable[i][1]);
		rt|=sd[i];
	}
	if(rt){
		for(int i=0;i<8;i++)
			if(sd[i]){
				int dx=TraceTable[i][0];
				int dy=TraceTable[i][1];
				int px=x+dx;
				int py=y+dy;
				int idx=Location2Index(px,py);
				do{
					Set(PlayerID,px,py);
					px+=dx;py+=dy;
					idx=Location2Index(px,py);
				}while(Board[idx]!=PlayerID);
			}		
		Set(PlayerID,x,y);
	}

	return rt;
}

int ChessBoard::GetStatus(int PlayerID) const
{
	if(PlayerID > NumberOfPlayer || PlayerID < 0)
		throw -1;
	return Count[PlayerID];
}

int ChessBoard::Location2Index(int x, int y) const
{
	return ( x >= 0 && x < BoardSize && y >= 0 && y < BoardSize)?y*BoardSize + x:-1;
}

bool ChessBoard::IsValid() const
{
	return Valid;
}

void ChessBoard::Set(int PlayerID,int x,int y)
{
	int idx	= Location2Index(x,y);
	if(idx>=0){
		Count[Board[idx]]--;
		Board[idx]=PlayerID;
		Count[PlayerID]++;
	}
}

bool ChessBoard::Trace(int PlayerID,int x,int y, int dx, int dy)
{
	
	bool rt=false;

	int idx	= Location2Index(x,y);
	if(idx>=0 && Board[idx]==0){
		int px	= x+dx;
		int py	= y+dy;
		idx		= Location2Index(px,py);
		if(!(idx<0 || Board[idx]==0 || Board[idx]==PlayerID)){
			do{
				px+=dx;
				py+=dy;
				idx=Location2Index(px,py);
			}while(idx>=0 && Board[idx]!=0 && Board[idx]!=PlayerID);

			if (idx>=0 && Board[idx]==PlayerID)
			rt=true;
		}
	}
	return rt;
}

int ChessBoard::GetID(int x,int y) const
{
	int idx	= Location2Index(x,y);
	if(idx<0)
		throw -1;
	return Board[idx];
}
