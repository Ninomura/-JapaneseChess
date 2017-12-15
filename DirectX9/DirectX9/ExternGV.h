#pragma once //�C���N���[�h�K�[�h

#define WindowWidthSize 960  //�E�B���h�E��X��
#define WindowHeightSize 620 //�E�B���h�E��Y��
#define BoardImageSize 620   //�Ֆʂ�\���摜�T�C�Y
#define OfBoardImageSize 300
#define MapSize 9            //�Ֆʂ̃T�C�Y��

//�摜�n���h��
extern int
ImgChooseable,  //�I���\�̉摜
ImgStartScreen, //�X�^�[�g��ʂ̉摜
ImgGameEnd;     //�I����ʂ̉摜

//�Q�[���̃��[�h
enum Game_Mode
{
	StartScreenProcessing,   //�X�^�[�g��ʂ̐ݒ�
	GameStartProcessing,     //��������
	PlayerProcessing,        //�v���C���[����i�I���\�����j
	BoardProcess,            //�Ֆʏ���
	PromotedWindowProcessing,//�����ʏ���
	GameEndProcessing,       //�ŏI����
};

//���Ԃ̏�ԁA��̏��
enum GameOrderMode
{
	Player01,
	Player02
};

//�Ֆʂ̎��
enum PiecesType
{
//--------------------------------
	Pawn,          //��
	Lance,         //����
	Knight,        //�j�n
	SilverGeneral, //�⏫
	GoldGeneral,   //����
	Bishop,        //�p�s
	Rook,          //���
	King,          //��,��
//---------------------------------
	Null           //��
//----------------------------------
};

//�Ֆʏ�̋�̏��
typedef struct
{
	//��̍��W
	int y, x;
	//�N�̋
	GameOrderMode PiecesGOM;
	//��̎��
	PiecesType Type;
	//�����Ă��邩
	bool Promoted;
	//�Ֆʏォ
	bool OnBoard;
	
} OnBoardPieceState;
