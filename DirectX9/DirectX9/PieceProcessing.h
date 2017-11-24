#pragma once

#include"ExternGV.h"
#include "Direct3D.h"

class PieceProcessing
{
private:

public:
	//��̏�ԕϐ�
	OnBoardPieceState PS[MapSize][MapSize];

public:

	~PieceProcessing();

	//��̈ړ�����
	void PieceMove(int SetMousePosY, int SetMousePosX, int MousePosY, int MousePosX);
	//��̑I���\�`��
	void PieceChooseable
	     (int SetMousePosY, int SetMousePosX, PiecesType type, bool Promoted, GameOrderMode GOM);
	//��̑I���\����
	void PieceChooseableReset();
	//��̈ړ������v�Z
	void PieceMoveCalculation(int a, int b, int c);

};

PieceProcessing::~PieceProcessing()
{
	//�S�Ă̋�̏�Ԃ����ł��邱�Ƃɂ���i���Z�b�g�j
	//�S�Ă̋�Ֆʂɏo�Ă��Ȃ���Ԃɂ���
	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			//�Ֆʏ�̋�̏�Ԃ̃��Z�b�g
			PS[yCount][xCount].Type = Null;
			PS[yCount][xCount].Promoted = false;
			PS[yCount][xCount].Chooseable = false;

			if (yCount == 2 || yCount == 6)
			{
				PS[yCount][xCount].Type = Pawn;
			}
		}

		//��̃^�C�v�̏�����
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

	//�ǂ���̋������
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

//��̈ړ�����
void PieceProcessing::PieceMove
(int SetMousePosY, int SetMousePosX, int MousePosY, int MousePosX)
{
	//�I�������}�X�ɑO�̋�̏�Ԃ��ڍs
	PS[MousePosY][MousePosX].Type = PS[SetMousePosY][SetMousePosX].Type;
	PS[MousePosY][MousePosX].PiecesGOM = PS[SetMousePosY][SetMousePosX].PiecesGOM;

	//�O�̃}�X�̋�̏�Ԃ����Z�b�g
	PS[SetMousePosY][SetMousePosX].Promoted = false;
	PS[SetMousePosY][SetMousePosX].Type = Null;

}

//�I���\�̈�̕`��
void PieceProcessing::PieceChooseable
(int MousePosY, int MousePosX, PiecesType type, bool Promoted, GameOrderMode GOM)
{
	//�����Ă��邩
	switch (Promoted)
	{
	case false:

		//��̎��
		switch (type)
		{

		//��
		case Pawn:
			PieceMoveCalculation(GOM, MousePosY, MousePosX);//��܂��͉�
			break;

		//����
		case Lance:
			PieceMoveCalculation(GOM + 7, MousePosY, MousePosX);//��~���܂��͉��~��
			break;

		//�j�n
		case Knight:
			PieceMoveCalculation(GOM + 5, MousePosY, MousePosX);//�����܂��͓��ꉺ
			break;

		//�⏫
		case SilverGeneral:
			PieceMoveCalculation(GOM, MousePosY, MousePosX);//��܂��͉�
			PieceMoveCalculation(3, MousePosY, MousePosX);  //����E��
			PieceMoveCalculation(4, MousePosY, MousePosX);  //�����E��

			break;

		//����
		case GoldGeneral:
			PieceMoveCalculation(0, MousePosY, MousePosX);      //��
			PieceMoveCalculation(1, MousePosY, MousePosX);      //��
			PieceMoveCalculation(2, MousePosY, MousePosX);      //���E
			PieceMoveCalculation(GOM + 3, MousePosY, MousePosX);//����E��܂��͍����E��
			break;

		//���
		case Rook:
			PieceMoveCalculation(7, MousePosY, MousePosX);//��~��
			PieceMoveCalculation(8, MousePosY, MousePosX);//���~��
			PieceMoveCalculation(9, MousePosY, MousePosX);//���E�~��
			break;

		//�p�s
		case Bishop:

			break;

		//��,��
		case King:

			PieceMoveCalculation(0, MousePosY, MousePosX);//��
			PieceMoveCalculation(1, MousePosY, MousePosX);//��
			PieceMoveCalculation(2, MousePosY, MousePosX);//���E
			PieceMoveCalculation(3, MousePosY, MousePosX);//����E��
			PieceMoveCalculation(4, MousePosY, MousePosX);//�����E��

			break;
		}

		break;

	case true:

		//��̎��
		switch (type)
		{

		case Pawn:
		case Lance:
		case Knight:

		//�Ƌ�,����,���j,����
		case SilverGeneral:

			PieceMoveCalculation(0, MousePosY, MousePosX);      //��
			PieceMoveCalculation(1, MousePosY, MousePosX);      //��
			PieceMoveCalculation(2, MousePosY, MousePosX);      //���E
			PieceMoveCalculation(GOM + 3, MousePosY, MousePosX);//����E��܂��͍����E��
			break;

		//�����i���ԁj
		case Rook:

			PieceMoveCalculation(3, MousePosY, MousePosX);//����E��
			PieceMoveCalculation(4, MousePosY, MousePosX);//�����E��
			PieceMoveCalculation(7, MousePosY, MousePosX);//��~��
			PieceMoveCalculation(8, MousePosY, MousePosX);//���~��
			PieceMoveCalculation(9, MousePosY, MousePosX);//���E�~��
			break;

		//���n�i���p�j
		case Bishop:

			break;
		}

		break;
	}
};

//�I���\�̈�̏���
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

//��̈ړ������v�Z
void PieceProcessing::PieceMoveCalculation(int a, int b, int c)
{
	switch (a)
	{
	//��
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

	//��
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

	//���E
	case 2:

		if (c > 0)
		{
			//��
			if (PS[b][c - 1].PiecesGOM == PS[b][c].PiecesGOM&&PS[b][c - 1].Type != Null)
			{
				;
			}
			else
			{
				PS[b][c - 1].Chooseable = true;
			}
		}

		//�E
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

	//����E��
	case 3:

		//����
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

		//�E��
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

	//�����E��
	case 4:

		//����
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

		//�E��
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

	//�����
	case 5:
		//����
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

		//�E��
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

	//���ꉺ
	case 6:

		//����
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

		//�E��
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

	//��~��
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

	//���~��
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

	//���E�~��
	case 9:
		//��
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

		//�E
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