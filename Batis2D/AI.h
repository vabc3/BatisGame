#pragma once
#include "ChessBoard.h"
#include <vector>

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
typedef class Boint (*AIStrategy)(int,const ChessBoard*);
extern AIStrategy Strategies[];
extern std::vector<Boint> GetMark(int PlayerID,const ChessBoard* board);