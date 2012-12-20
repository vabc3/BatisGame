#pragma once
//The ChessBoard class
class ChessBoard
{
public:
	ChessBoard();
	ChessBoard(int NumberOfPlayer,int Size);
	~ChessBoard(void);
	ChessBoard(const ChessBoard& source);
	ChessBoard& operator=(const ChessBoard& source);
	
	bool Place(int PlayerID,int x,int y);
	bool IsValid() const;
	int  GetID(int x,int y) const ;
	int  GetStatus(int PlayerID) const;
	int  GetNumberOfPlayer() const;
	int  GetBoardSize() const;
private:
	const static int TraceTable[8][2];
	bool Valid;
	int* Board;
	int* Count;
	int  NumberOfPlayer,BoardSize;
	
	bool Trace(int PlayerID,int x,int y,int dx, int dy);
	int  Location2Index(int x, int y) const;
	void Set(int PlayerID,int x,int y);
	void Init();
};
