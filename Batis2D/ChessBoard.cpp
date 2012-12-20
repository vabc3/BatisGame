#include "ChessBoard.h"
#include "BatisConstants.h"
#include <iostream>
using namespace std;

int ChessBoard::TraceTable[8][2]={
	{ 0,-1}, { 1,-1},
	{ 1, 0}, { 1, 1},
	{ 0, 1}, {-1, 1},
	{-1, 0}, {-1,-1},
};

void ChessBoard::init()
{
	Valid			= false;
	Board			= NULL;
	Count			= NULL;
	BoardSize		= 0;
	NumberOfPlayer	= 0;
	
}

ChessBoard::ChessBoard()
{
	init();
}

ChessBoard::ChessBoard(int numberOfPlayer,int size)
{
	init();
	Init(numberOfPlayer,size);
}

void ChessBoard::Init(int numberOfPlayer,int size)
{
	
	Valid		= true;
	if(numberOfPlayer< NumberOfPlayerMin || numberOfPlayer> NumberOfPlayerMax){
		Valid	= false;
	}else if(size<BoardSizeMin || size> BoardSizeMax){
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
		memset(Count,0,BoardSize*sizeof(int));
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
		memcpy(Count,source.Count,BoardSize*sizeof(int));
	}
	return *this;
}

ChessBoard::ChessBoard(const ChessBoard& source)
{
	if(this!=&source)
		*this	= source;
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
	//Debug("Try palce"+int2string(PlayerID)+"@"+int2string(x)+","+int2string(y));
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
	int rt	= -1;
	if( x >= 0 && x < BoardSize && y >= 0 && y < BoardSize)
		rt = y*BoardSize + x;
	return rt;
}

bool ChessBoard::isValid()
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

void ChessBoard::Print()
{
	for(int i=0;i<BoardSize;i++){
		for(int j=0;j<BoardSize;j++)
			cout<<Board[Location2Index(j,i)];
		cout<<endl;
	}
	cout<<"[";
	for(int i=0;i<NumberOfPlayer;i++)
		cout<<Count[i]<<",";
	cout<<Count[NumberOfPlayer]<<"]\n";
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
	//Debug("Tracing P" + int2string(PlayerID)+"@("+int2string(x)+","+int2string(y)+")="+int2string(dx)+","+int2string(dy));
	//Debug(rt);
	return rt;
}

int ChessBoard::GetID(int x,int y) const
{
	return Board[Location2Index(x,y)];
}