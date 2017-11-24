#pragma once

#include"ExternGV.h"
#include "Direct3D.h"

class PieceProcessing
{
private:

public:
	//駒の状態変数
	OnBoardPieceState PS[MapSize][MapSize];

public:

	~PieceProcessing();

	//駒の移動処理
	void PieceMove(int SetMousePosY, int SetMousePosX, int MousePosY, int MousePosX);
	//駒の選択可能描画
	void PieceChooseable
	     (int SetMousePosY, int SetMousePosX, PiecesType type, bool Promoted, GameOrderMode GOM);
	//駒の選択可能消去
	void PieceChooseableReset();
	//駒の移動処理計算
	void PieceMoveCalculation(int a, int b, int c);

};

PieceProcessing::~PieceProcessing()
{
	//全ての駒の状態が無であることにする（リセット）
	//全ての駒が盤面に出ていない状態にする
	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			//盤面上の駒の状態のリセット
			PS[yCount][xCount].Type = Null;
			PS[yCount][xCount].Promoted = false;
			PS[yCount][xCount].Chooseable = false;

			if (yCount == 2 || yCount == 6)
			{
				PS[yCount][xCount].Type = Pawn;
			}
		}

		//駒のタイプの初期化
		if (yCount == 0 || yCount == 8)
		{
			PS[yCount][0].Type = Lance;
			PS[yCount][8].Type = Lance;
			PS[yCount][1].Type = Knight;
			PS[yCount][7].Type = Knight;
			PS[yCount][2].Type = SilverGeneral;
			PS[yCount][6].Type = SilverGeneral;
			PS[yCount][3].Type = GoldGeneral;
			PS[yCount][5].Type = GoldGeneral;
			PS[yCount][4].Type = King;
		}
		else if (yCount == 1)
		{
			PS[yCount][1].Type = Rook;
			PS[yCount][7].Type = Bishop;
		}
		else if (yCount == 7)
		{
			PS[yCount][1].Type = Bishop;
			PS[yCount][7].Type = Rook;
		}
	}

	//どちらの駒か初期化
	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			if (PS[yCount][xCount].Type != Null)
			{
				if (yCount < 3)
				{
					PS[yCount][xCount].PiecesGOM = Player02;
				}
				else if (yCount >= 6)
				{
					PS[yCount][xCount].PiecesGOM = Player01;
				}
			}
		}
	}
}

//駒の移動処理
void PieceProcessing::PieceMove
(int SetMousePosY, int SetMousePosX, int MousePosY, int MousePosX)
{
	//選択したマスに前の駒の状態を移行
	PS[MousePosY][MousePosX].Type = PS[SetMousePosY][SetMousePosX].Type;
	PS[MousePosY][MousePosX].PiecesGOM = PS[SetMousePosY][SetMousePosX].PiecesGOM;

	//前のマスの駒の状態をリセット
	PS[SetMousePosY][SetMousePosX].Promoted = false;
	PS[SetMousePosY][SetMousePosX].Type = Null;

}

//選択可能領域の描画
void PieceProcessing::PieceChooseable
(int MousePosY, int MousePosX, PiecesType type, bool Promoted, GameOrderMode GOM)
{
	//成っているか
	switch (Promoted)
	{
	case false:

		//駒の種類
		switch (type)
		{

		//歩
		case Pawn:
			PieceMoveCalculation(GOM, MousePosY, MousePosX);//上または下
			break;

		//香車
		case Lance:
			PieceMoveCalculation(GOM + 7, MousePosY, MousePosX);//上×∞または下×∞
			break;

		//桂馬
		case Knight:
			PieceMoveCalculation(GOM + 5, MousePosY, MousePosX);//特殊上または特殊下
			break;

		//銀将
		case SilverGeneral:
			PieceMoveCalculation(GOM, MousePosY, MousePosX);//上または下
			PieceMoveCalculation(3, MousePosY, MousePosX);  //左上右上
			PieceMoveCalculation(4, MousePosY, MousePosX);  //左下右下

			break;

		//金将
		case GoldGeneral:
			PieceMoveCalculation(0, MousePosY, MousePosX);      //上
			PieceMoveCalculation(1, MousePosY, MousePosX);      //下
			PieceMoveCalculation(2, MousePosY, MousePosX);      //左右
			PieceMoveCalculation(GOM + 3, MousePosY, MousePosX);//左上右上または左下右下
			break;

		//飛車
		case Rook:
			PieceMoveCalculation(7, MousePosY, MousePosX);//上×∞
			PieceMoveCalculation(8, MousePosY, MousePosX);//下×∞
			PieceMoveCalculation(9, MousePosY, MousePosX);//左右×∞
			break;

		//角行
		case Bishop:

			break;

		//王,玉
		case King:

			PieceMoveCalculation(0, MousePosY, MousePosX);//上
			PieceMoveCalculation(1, MousePosY, MousePosX);//下
			PieceMoveCalculation(2, MousePosY, MousePosX);//左右
			PieceMoveCalculation(3, MousePosY, MousePosX);//左上右上
			PieceMoveCalculation(4, MousePosY, MousePosX);//左下右下

			break;
		}

		break;

	case true:

		//駒の種類
		switch (type)
		{

		case Pawn:
		case Lance:
		case Knight:

		//と金,成香,成桂,成銀
		case SilverGeneral:

			PieceMoveCalculation(0, MousePosY, MousePosX);      //上
			PieceMoveCalculation(1, MousePosY, MousePosX);      //下
			PieceMoveCalculation(2, MousePosY, MousePosX);      //左右
			PieceMoveCalculation(GOM + 3, MousePosY, MousePosX);//左上右上または左下右下
			break;

		//龍王（成車）
		case Rook:

			PieceMoveCalculation(3, MousePosY, MousePosX);//左上右上
			PieceMoveCalculation(4, MousePosY, MousePosX);//左下右下
			PieceMoveCalculation(7, MousePosY, MousePosX);//上×∞
			PieceMoveCalculation(8, MousePosY, MousePosX);//下×∞
			PieceMoveCalculation(9, MousePosY, MousePosX);//左右×∞
			break;

		//竜馬（成角）
		case Bishop:

			break;
		}

		break;
	}
};

//選択可能領域の消去
void PieceProcessing::PieceChooseableReset()
{
	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			PS[yCount][xCount].Chooseable = false;
		}
	}
};

//駒の移動処理計算
void PieceProcessing::PieceMoveCalculation(int a, int b, int c)
{
	switch (a)
	{
	//上
	case 0:
		if (b > 0)
		{
			if (PS[b - 1][c].PiecesGOM == PS[b][c].PiecesGOM&&PS[b - 1][c].Type != Null)
			{
				;
			}
			else
			{
				PS[b - 1][c].Chooseable = true;
			}
		}
		break;

	//下
	case 1:
		if (b < MapSize - 1)
		{
			if (PS[b + 1][c].PiecesGOM == PS[b][c].PiecesGOM&&PS[b + 1][c].Type != Null)
			{
				;
			}
			else
			{
				PS[b + 1][c].Chooseable = true;
			}
		}
		break;

	//左右
	case 2:

		if (c > 0)
		{
			//左
			if (PS[b][c - 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b][c - 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b][c - 1].Chooseable = true;
			}
		}

		//右
		if (c < MapSize - 1)
		{
			if (PS[b][c + 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b][c + 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b][c + 1].Chooseable = true;
			}
		}

		break;

	//左上右上
	case 3:

		//左上
		if (b > 0 && c > 0)
		{
			if (PS[b - 1][c - 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b - 1][c - 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b - 1][c - 1].Chooseable = true;
			}
		}

		//右上
		if (b > 0 && c < MapSize - 1)
		{
			if (PS[b - 1][c + 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b - 1][c + 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b - 1][c + 1].Chooseable = true;
			}
		}

		break;

	//左下右下
	case 4:

		//左下
		if (b < MapSize - 1 && c > 0)
		{
			if (PS[b + 1][c - 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b + 1][c - 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b + 1][c - 1].Chooseable = true;
			}
		}

		//右下
		if (b < MapSize - 1 && c < MapSize - 1)
		{
			if (PS[b + 1][c + 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b + 1][c + 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b + 1][c + 1].Chooseable = true;
			}
		}

		break;

	//特殊上
	case 5:
		//左上
		if (b > 1 && c > 0)
		{
			if (PS[b - 2][c - 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b - 2][c - 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b - 2][c - 1].Chooseable = true;
			}
		}

		//右上
		if (b > 1 && c < MapSize - 1)
		{
			if (PS[b - 2][c + 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b - 2][c + 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b - 2][c + 1].Chooseable = true;
			}
		}

		break;

	//特殊下
	case 6:

		//左下
		if (b < MapSize - 2 && c > 0)
		{
			if (PS[b + 2][c - 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b + 2][c - 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b + 2][c - 1].Chooseable = true;
			}
		}

		//右下
		if (b < MapSize - 2 && c < MapSize - 1)
		{
			if (PS[b + 2][c + 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b + 2][c + 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b + 2][c + 1].Chooseable = true;
			}
		}

		break;

	//上×∞
	case 7:
		for (int Count = b - 1; Count >= 0; Count--)
		{
			if (PS[Count][c].PiecesGOM == PS[b][c].PiecesGOM&&PS[Count][c].Type != Null)
			{
				break;
			}
			else if (PS[Count][c].PiecesGOM != PS[b][c].PiecesGOM&&PS[Count][c].Type != Null)
			{
				PS[Count][c].Chooseable = true;
				break;
			}
			else
			{
				PS[Count][c].Chooseable = true;
			}
		}
		break;

	//下×∞
	case 8:
		for (int Count = b + 1; Count < MapSize; Count++)
		{
			if (PS[Count][c].PiecesGOM == PS[b][c].PiecesGOM&&PS[Count][c].Type != Null)
			{
				break;
			}
			else if (PS[Count][c].PiecesGOM != PS[b][c].PiecesGOM&&PS[Count][c].Type != Null)
			{
				PS[Count][c].Chooseable = true;
				break;
			}
			else
			{
				PS[Count][c].Chooseable = true;
			}
		}
		break;

	//左右×∞
	case 9:
		//左
		for (int Count = c - 1; Count >= 0; Count--)
		{
			if (PS[b][Count].PiecesGOM == PS[b][c].PiecesGOM&&PS[b][Count].Type != Null)
			{
				break;
			}
			else if (PS[b][Count].PiecesGOM != PS[b][c].PiecesGOM&&PS[b][Count].Type != Null)
			{
				PS[b][Count].Chooseable = true;
				break;
			}
			else
			{
				PS[b][Count].Chooseable = true;
			}
		}

		//右
		for (int Count = c + 1; Count < MapSize; Count++)
		{
			if (PS[b][Count].PiecesGOM == PS[b][c].PiecesGOM&&PS[b][Count].Type != Null)
			{
				break;
			}
			else if (PS[b][Count].PiecesGOM != PS[b][c].PiecesGOM&&PS[b][Count].Type != Null)
			{
				PS[b][Count].Chooseable = true;
				break;
			}
			else
			{
				PS[b][Count].Chooseable = true;
			}
		}
		break;
	}
};