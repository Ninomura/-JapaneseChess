#pragma once

#include"ExternGV.h"
#include "Direct3D.h"

class PieceProcessing
{
private:
	bool Breakflag = false;
public:

	//駒の状態変数
	OnBoardPieceState PS[40];
	//取ることが出来るか
	bool Chooseable[MapSize][MapSize];

	~PieceProcessing();

	//駒の移動処理
	void PieceMove(int SetPiece, int MousePosY, int MousePosX);
	//手持ちの駒計算
	void OfPieceMoveCalculation();
	//駒の選択可能描画
	void PieceChooseable(int i, GameOrderMode GOM);
	//駒の選択可能消去
	void PieceChooseableReset();
	//持ち駒を押した時の判断処理
	void OfPieceProcessing(int SetPiece,int Mousecount);

private:
	//駒の移動処理計算
	void PieceMoveCalculation(int type, int i);

};

PieceProcessing::~PieceProcessing()
{
	//全ての駒の状態が無であることにする（リセット）
	//全ての駒が盤面に出ていない状態にする
	for (int Count = 0; Count < 40; Count++)
	{
		//盤面上の駒の状態のリセット
		PS[Count].Promoted = false;
		PS[Count].OnBoard = true;

		if (Count < 18)
		{
			if (Count < 9)
			{
				PS[Count].y = 2;
			}
			else
			{
				PS[Count].y = 6;
			}

			PS[Count].Type = Pawn;
			PS[Count].x = Count % 9;
		}
		else if (Count < 36)
		{
			if (Count < 27){PS[Count].y = 0;}
			else{PS[Count].y = 8;}

			PS[Count].x = Count % 9;
			switch (Count % 9)
			{
			case 0:
				PS[Count].Type = Lance;
				break;
			case 1:
				PS[Count].Type = Knight;
				break;
			case 2:
				PS[Count].Type = SilverGeneral;
				break;
			case 3:
				PS[Count].Type = GoldGeneral;
				break;
			case 4:
				PS[Count].Type = King;
				break;
			case 5:
				PS[Count].Type = GoldGeneral;
				break;
			case 6:
				PS[Count].Type = SilverGeneral;
				break;
			case 7:
				PS[Count].Type = Knight;
				break;
			case 8:
				PS[Count].Type = Lance;
				break;
			}
		}
	}

	//角
	PS[36].Type = Bishop;
	PS[36].y = 1;
	PS[36].x = 7;
	PS[37].Type = Bishop;
	PS[37].y = 7;
	PS[37].x = 1;

	//飛車
	PS[38].Type = Rook;
	PS[38].y = 1;
	PS[38].x = 1;
	PS[39].Type = Rook;
	PS[39].y = 7;
	PS[39].x = 7;

	//どちらの駒か初期化
	for (int Count = 0; Count < 40; Count++)
	{
		if (PS[Count].Type != Null)
		{
			if (PS[Count].y < 3)
			{
				PS[Count].PiecesGOM = Player02;
			}
			else
			{
				PS[Count].PiecesGOM = Player01;
			}
		}
	}
}

//駒の移動処理
void PieceProcessing::PieceMove(int SetPiece, int MousePosY, int MousePosX)
{
	//選択したマスに前の駒の状態を移行
	PS[SetPiece].y = MousePosY;
	PS[SetPiece].x = MousePosX;

	if (PS[SetPiece].OnBoard == false)
	{
		PS[SetPiece].OnBoard = true;
	}
}

//選択可能領域の描画
void PieceProcessing::PieceChooseable(int i, GameOrderMode GOM)
{
	//成っているか
	switch (PS[i].Promoted)
	{
	case false:

		//駒の種類
		switch (PS[i].Type)
		{

		//歩
		case Pawn:
			PieceMoveCalculation(GOM, i);//上または下
			break;

		//香車
		case Lance:
			PieceMoveCalculation(GOM + 7, i);//上×∞または下×∞
			break;

		//桂馬
		case Knight:
			PieceMoveCalculation(GOM + 5, i);//特殊上または特殊下
			break;

		//銀将
		case SilverGeneral:
			PieceMoveCalculation(GOM, i);//上または下
			PieceMoveCalculation(3, i);  //左上右上
			PieceMoveCalculation(4, i);  //左下右下

			break;

		//金将
		case GoldGeneral:
			PieceMoveCalculation(0, i);      //上
			PieceMoveCalculation(1, i);      //下
			PieceMoveCalculation(2, i);      //左右
			PieceMoveCalculation(GOM + 3, i);//左上右上または左下右下
			break;

		//飛車
		case Rook:
			PieceMoveCalculation(7, i);//上×∞
			PieceMoveCalculation(8, i);//下×∞
			PieceMoveCalculation(9, i);//左右×∞
			break;

		//角行
		case Bishop:
			PieceMoveCalculation(10, i);//左上左下右上右下×∞
			break;

		//王,玉
		case King:

			PieceMoveCalculation(0, i);//上
			PieceMoveCalculation(1, i);//下
			PieceMoveCalculation(2, i);//左右
			PieceMoveCalculation(3, i);//左上右上
			PieceMoveCalculation(4, i);//左下右下

			break;
		}

		break;

	case true:

		//駒の種類
		switch (PS[i].Type)
		{

		case Pawn:
		case Lance:
		case Knight:

		//と金,成香,成桂,成銀
		case SilverGeneral:

			PieceMoveCalculation(0, i);      //上
			PieceMoveCalculation(1, i);      //下
			PieceMoveCalculation(2, i);      //左右
			PieceMoveCalculation(GOM + 3, i);//左上右上または左下右下
			break;

		//龍王（成車）
		case Rook:

			PieceMoveCalculation(3, i);//左上右上
			PieceMoveCalculation(4, i);//左下右下
			PieceMoveCalculation(7, i);//上×∞
			PieceMoveCalculation(8, i);//下×∞
			PieceMoveCalculation(9, i);//左右×∞
			break;

		//竜馬（成角）
		case Bishop:
			PieceMoveCalculation(0, i);//上
			PieceMoveCalculation(1, i);//下
			PieceMoveCalculation(2, i);//左右
			PieceMoveCalculation(10, i);//左上左下右上右下×∞
			break;
		}

		break;
	}
};

void PieceProcessing::OfPieceMoveCalculation()
{
	for (int i = 0; i < 40; i++)
	{
		if (PS[i].OnBoard == false)
		{
			switch (PS[i].PiecesGOM)
			{
			case Player01:
				switch (PS[i].Type)
				{
				case Pawn:
					break;
				case Lance:
					break;
				case Knight:
					break;
				case SilverGeneral:
					break;
				case GoldGeneral:
					break;
				case Bishop:
					break;
				case Rook:
					break;
				default:
					break;
				}
				break;

			case Player02:
				switch (PS[i].Type)
				{
				case Pawn:
					break;
				case Lance:
					break;
				case Knight:
					break;
				case SilverGeneral:
					break;
				case GoldGeneral:
					break;
				case Bishop:
					break;
				case Rook:
					break;
				default:
					break;
				}
				break;
			}
		}
	}
}

//選択可能領域の消去
void PieceProcessing::PieceChooseableReset()
{
	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			Chooseable[yCount][xCount] = false;

		}
	}
};

//駒の移動処理計算
void PieceProcessing::PieceMoveCalculation(int type,int i)
{
	switch (type)
	{
	//上
	case 0:
		if (PS[i].y > 0)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y - 1 == PS[a].y
				 && PS[i].x == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 1][PS[i].x] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 1][PS[i].x] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y - 1][PS[i].x] = true;
				}
			}
		}

		break;

	//下
	case 1:
		if (PS[i].y < MapSize - 1)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y + 1 == PS[a].y
				 && PS[i].x == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 1][PS[i].x] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 1][PS[i].x] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y + 1][PS[i].x] = true;
				}
			}
		}

		break;

	//左右
	case 2:

		//左
		if (PS[i].x > 0)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y == PS[a].y
				 && PS[i].x - 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][PS[i].x - 1] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][PS[i].x - 1] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y][PS[i].x - 1] = true;
				}
			}
		}

		//右
		if (PS[i].x < MapSize - 1)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y == PS[a].y
					&& PS[i].x + 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][PS[i].x + 1] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][PS[i].x + 1] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y][PS[i].x + 1] = true;
				}
			}
		}

		break;

	//左上右上
	case 3:

		//左上
		if (PS[i].y > 0 && PS[i].x > 0)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y - 1 == PS[a].y
					&& PS[i].x - 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 1][PS[i].x - 1] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 1][PS[i].x - 1] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y - 1][PS[i].x - 1] = true;
				}
			}
		}

		//右上
		if (PS[i].y > 0 && PS[i].x < MapSize - 1)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y - 1 == PS[a].y
					&& PS[i].x + 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 1][PS[i].x + 1] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 1][PS[i].x + 1] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y - 1][PS[i].x + 1] = true;
				}
			}

		}

		break;

	//左下右下
	case 4:

		//左下
		if (PS[i].y < MapSize - 1 && PS[i].x > 0)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y + 1 == PS[a].y
					&& PS[i].x - 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 1][PS[i].x - 1] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 1][PS[i].x - 1] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y + 1][PS[i].x - 1] = true;
				}
			}
		}

		//右下
		if (PS[i].y < MapSize - 1 && PS[i].x < MapSize - 1)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y + 1 == PS[a].y
					&& PS[i].x + 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 1][PS[i].x + 1] = false;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 1][PS[i].x + 1] = true;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y + 1][PS[i].x + 1] = true;
				}
			}
		}

		break;

	//特殊上
	case 5:
		//左上
		if (PS[i].y > 1 && PS[i].x > 0)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y - 2 == PS[a].y
					&& PS[i].x - 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 2][PS[i].x - 1] = false;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y - 2][PS[i].x - 1] = true;
				}
			}
		}

		//右上
		if (PS[i].y > 1 && PS[i].x < MapSize - 1)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y - 2 == PS[a].y
					&& PS[i].x + 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y - 2][PS[i].x + 1] = false;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y - 2][PS[i].x + 1] = true;
				}
			}
		}

		break;

	//特殊下
	case 6:

		//左下
		if (PS[i].y < MapSize - 2 && PS[i].x > 0)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y + 2 == PS[a].y
					&& PS[i].x - 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 2][PS[i].x - 1] = false;
					}

					break;
				}
				else
				{
					Chooseable[PS[i].y + 2][PS[i].x - 1] = true;
				}
			}
		}

		//右下
		if (PS[i].y < MapSize - 2 && PS[i].x < MapSize - 1)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y + 2 == PS[a].y
					&& PS[i].x + 1 == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y + 2][PS[i].x + 1] = false;
					}
					break;
				}
				else
				{
					Chooseable[PS[i].y + 2][PS[i].x + 1] = true;
				}
			}
		}

		break;

	//上×∞
	case 7:
		for (int Count = PS[i].y - 1; Count >= 0; Count--)
		{
			for (int a = 0; a < 40; a++)
			{
				if (Count == PS[a].y
					&& PS[i].x == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[Count][PS[i].x] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[Count][PS[i].x] = true;
						Breakflag = true;
						break;
					}
				}
				else
				{
					Chooseable[Count][PS[i].x] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}
		}

		break;

	//下×∞
	case 8:
		for (int Count = PS[i].y + 1; Count < MapSize; Count++)
		{
			for (int a = 0; a < 40; a++)
			{
				if (Count == PS[a].y
					&& PS[i].x == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[Count][PS[i].x] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[Count][PS[i].x] = true;
						Breakflag = true;
						break;
					}

				}
				else
				{
					Chooseable[Count][PS[i].x] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}
		}

		break;

	//左右×∞
	case 9:
		//左
		for (int Count = PS[i].x - 1; Count >= 0; Count--)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y == PS[a].y
					&& Count == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][Count] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][Count] = true;
						Breakflag = true;
						break;
					}
				}
				else
				{
					Chooseable[PS[i].y][Count] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}
		}

		//右
		for (int Count = PS[i].x + 1; Count < MapSize; Count++)
		{
			for (int a = 0; a < 40; a++)
			{
				if (PS[i].y == PS[a].y
					&& Count == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][Count] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[PS[i].y][Count] = true;
						Breakflag = true;
						break;
					}
				}
				else
				{
					Chooseable[PS[i].y][Count] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}
		}

		break;

	//左上左下右上右下×∞
	case 10:

		int CountX;

		//左上
		CountX = PS[i].x - 1;
		for (int CountY = PS[i].y - 1; CountY >= 0; CountY--)
		{
			if (CountX < 0)
			{
				break;
			}

			for (int a = 0; a < 40; a++)
			{
				if (CountY == PS[a].y
					&& CountX == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = true;
						Breakflag = true;
						break;
					}
				}
				else
				{
					Chooseable[CountY][CountX] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}

			CountX--;
		}

		//右上
		CountX = PS[i].x + 1;
		for (int CountY = PS[i].y - 1; CountY >= 0; CountY--)
		{
			if (CountX >= MapSize)
			{
				break;
			}

			for (int a = 0; a < 40; a++)
			{
				if (CountY == PS[a].y
					&& CountX == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = true;
						Breakflag = true;
						break;
					}
				}
				else
				{
					Chooseable[CountY][CountX] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}

			CountX++;
		}

		//左下
		CountX = PS[i].x - 1;
		for (int CountY = PS[i].y + 1; CountY < MapSize; CountY++)
		{
			if (CountX < 0)
			{
				break;
			}

			for (int a = 0; a < 40; a++)
			{
				if (CountY == PS[a].y
					&& CountX == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = true;
						Breakflag = true;
						break;
					}

				}
				else
				{
					Chooseable[CountY][CountX] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}

			CountX--;
		}

		//右下

		CountX = PS[i].x + 1;

		for (int CountY = PS[i].y + 1; CountY < MapSize; CountY++)
		{
			if (CountX >= MapSize)
			{
				break;
			}

			for (int a = 0; a < 40; a++)
			{
				if (CountY == PS[a].y
					&& CountX == PS[a].x)
				{
					if (PS[i].PiecesGOM == PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = false;
						Breakflag = true;
						break;
					}
					else if (PS[i].PiecesGOM != PS[a].PiecesGOM
						&& PS[a].OnBoard == true)
					{
						Chooseable[CountY][CountX] = true;
						Breakflag = true;
						break;
					}
				}
				else
				{
					Chooseable[CountY][CountX] = true;
				}
			}

			if (Breakflag == true)
			{
				Breakflag = false;
				break;
			}

			CountX++;

		}

		break;
	}

};

void PieceProcessing::OfPieceProcessing(int SetPiece, int Mousecount)
{
		if (Mousecount < 7 && PS[SetPiece].PiecesGOM == Player02)
		{
			switch (Mousecount % 7)
			{
			case 0:
				for (int yCount = 0; yCount < MapSize - 1; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				for (int yCount = 0; yCount < MapSize - 1; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true
								&& PS[a].Type == Pawn
								&& PS[a].Promoted == false
								&& PS[a].PiecesGOM == Player02)
							{
								for (int yCount2 = 0; yCount2 < MapSize - 1; yCount2++)
								{
									Chooseable[yCount2][xCount] = false;
								}

								break;
							}
						}
					}
				}

				break;

			case 1:
				for (int yCount = 0; yCount < MapSize - 1; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				break;

			case 2:
				for (int yCount = 0; yCount < MapSize - 2; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				break;

			default:
				for (int yCount = 0; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				break;
			}
		}
		else if (Mousecount >= 7 && PS[SetPiece].PiecesGOM == Player01)
		{
			switch (Mousecount % 7)
			{
			case 0:
				for (int yCount = 0; yCount < MapSize - 1; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				for (int yCount = 1; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true
								&& PS[a].Type == Pawn
								&& PS[a].Promoted == false
								&& PS[a].PiecesGOM == Player01)
							{
								for (int yCount2 = 0; yCount2 < MapSize - 1; yCount2++)
								{
									Chooseable[yCount2][xCount] = false;
								}

								break;
							}
						}
					}
				}

				break;

			case 1:
				for (int yCount = 1; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				break;

			case 2:
				for (int yCount = 2; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				break;

			default:
				for (int yCount = 0; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						for (int a = 0; a < 40; a++)
						{
							Chooseable[yCount][xCount] = true;

							if (PS[a].y == yCount
								&& PS[a].x == xCount
								&& PS[a].OnBoard == true)
							{
								Chooseable[yCount][xCount] = false;
								break;
							}
						}

					}
				}

				break;
			}
		}
};