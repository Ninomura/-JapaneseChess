#pragma once

#include"ExternGV.h"
#include "Direct3D.h"

class PieceProcessing
{
private:
	bool Breakflag = false;
public:

	//��̏�ԕϐ�
	OnBoardPieceState PS[40];
	//��邱�Ƃ��o���邩
	bool Chooseable[MapSize][MapSize];

	~PieceProcessing();

	//��̈ړ�����
	void PieceMove(int SetPiece, int MousePosY, int MousePosX);
	//�莝���̋�v�Z
	void OfPieceMoveCalculation();
	//��̑I���\�`��
	void PieceChooseable(int i, GameOrderMode GOM);
	//��̑I���\����
	void PieceChooseableReset();
	//����������������̔��f����
	void OfPieceProcessing(int SetPiece,int Mousecount);

private:
	//��̈ړ������v�Z
	void PieceMoveCalculation(int type, int i);

};

PieceProcessing::~PieceProcessing()
{
	//�S�Ă̋�̏�Ԃ����ł��邱�Ƃɂ���i���Z�b�g�j
	//�S�Ă̋�Ֆʂɏo�Ă��Ȃ���Ԃɂ���
	for (int Count = 0; Count < 40; Count++)
	{
		//�Ֆʏ�̋�̏�Ԃ̃��Z�b�g
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

	//�p
	PS[36].Type = Bishop;
	PS[36].y = 1;
	PS[36].x = 7;
	PS[37].Type = Bishop;
	PS[37].y = 7;
	PS[37].x = 1;

	//���
	PS[38].Type = Rook;
	PS[38].y = 1;
	PS[38].x = 1;
	PS[39].Type = Rook;
	PS[39].y = 7;
	PS[39].x = 7;

	//�ǂ���̋������
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

//��̈ړ�����
void PieceProcessing::PieceMove(int SetPiece, int MousePosY, int MousePosX)
{
	//�I�������}�X�ɑO�̋�̏�Ԃ��ڍs
	PS[SetPiece].y = MousePosY;
	PS[SetPiece].x = MousePosX;

	if (PS[SetPiece].OnBoard == false)
	{
		PS[SetPiece].OnBoard = true;
	}
}

//�I���\�̈�̕`��
void PieceProcessing::PieceChooseable(int i, GameOrderMode GOM)
{
	//�����Ă��邩
	switch (PS[i].Promoted)
	{
	case false:

		//��̎��
		switch (PS[i].Type)
		{

		//��
		case Pawn:
			PieceMoveCalculation(GOM, i);//��܂��͉�
			break;

		//����
		case Lance:
			PieceMoveCalculation(GOM + 7, i);//��~���܂��͉��~��
			break;

		//�j�n
		case Knight:
			PieceMoveCalculation(GOM + 5, i);//�����܂��͓��ꉺ
			break;

		//�⏫
		case SilverGeneral:
			PieceMoveCalculation(GOM, i);//��܂��͉�
			PieceMoveCalculation(3, i);  //����E��
			PieceMoveCalculation(4, i);  //�����E��

			break;

		//����
		case GoldGeneral:
			PieceMoveCalculation(0, i);      //��
			PieceMoveCalculation(1, i);      //��
			PieceMoveCalculation(2, i);      //���E
			PieceMoveCalculation(GOM + 3, i);//����E��܂��͍����E��
			break;

		//���
		case Rook:
			PieceMoveCalculation(7, i);//��~��
			PieceMoveCalculation(8, i);//���~��
			PieceMoveCalculation(9, i);//���E�~��
			break;

		//�p�s
		case Bishop:
			PieceMoveCalculation(10, i);//���㍶���E��E���~��
			break;

		//��,��
		case King:

			PieceMoveCalculation(0, i);//��
			PieceMoveCalculation(1, i);//��
			PieceMoveCalculation(2, i);//���E
			PieceMoveCalculation(3, i);//����E��
			PieceMoveCalculation(4, i);//�����E��

			break;
		}

		break;

	case true:

		//��̎��
		switch (PS[i].Type)
		{

		case Pawn:
		case Lance:
		case Knight:

		//�Ƌ�,����,���j,����
		case SilverGeneral:

			PieceMoveCalculation(0, i);      //��
			PieceMoveCalculation(1, i);      //��
			PieceMoveCalculation(2, i);      //���E
			PieceMoveCalculation(GOM + 3, i);//����E��܂��͍����E��
			break;

		//�����i���ԁj
		case Rook:

			PieceMoveCalculation(3, i);//����E��
			PieceMoveCalculation(4, i);//�����E��
			PieceMoveCalculation(7, i);//��~��
			PieceMoveCalculation(8, i);//���~��
			PieceMoveCalculation(9, i);//���E�~��
			break;

		//���n�i���p�j
		case Bishop:
			PieceMoveCalculation(0, i);//��
			PieceMoveCalculation(1, i);//��
			PieceMoveCalculation(2, i);//���E
			PieceMoveCalculation(10, i);//���㍶���E��E���~��
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

//�I���\�̈�̏���
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

//��̈ړ������v�Z
void PieceProcessing::PieceMoveCalculation(int type,int i)
{
	switch (type)
	{
	//��
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

	//��
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

	//���E
	case 2:

		//��
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

		//�E
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

	//����E��
	case 3:

		//����
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

		//�E��
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

	//�����E��
	case 4:

		//����
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

		//�E��
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

	//�����
	case 5:
		//����
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

		//�E��
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

	//���ꉺ
	case 6:

		//����
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

		//�E��
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

	//��~��
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

	//���~��
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

	//���E�~��
	case 9:
		//��
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

		//�E
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

	//���㍶���E��E���~��
	case 10:

		int CountX;

		//����
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

		//�E��
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

		//����
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

		//�E��

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