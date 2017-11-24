
#include <Windows.h>
#pragma comment(lib,"winmm.lib")

#include <tchar.h>


#include"Direct3D.h"

#include "Sprite.h"
#include "Texture.h"

#include "DirectInput.h"

#include "PieceProcessing.h"

#include <random>


//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndPrc
(
	HWND hWnd,	//�E�B���h�E�̃n���h��
	UINT msg,	//���b�Z�[�W
	WPARAM wParam,
	LPARAM lParam
)
{
	//���b�Z�[�W�����̂��߂̊֐�
	//DispatchMessage�ɂ���ČĂяo�����

	//�E�B���h�E�N���X�̓o�^����
	//�e�E�B���h�E�ɐݒ肳���

	//�A�v���P�[�V���������b�Z�[�W��
	//�擾������Ăяo�����

	//���b�Z�[�W�̗�

	//WM_DESTROY �E�B���h�E���j�����ꂽ
	//			 �~�{�^���������ꂽ���Ȃ�
	//WM_MOVE    �E�B���h�E���ړ�
	//WM_SIZE    �E�B���h�E�T�C�Y�̕ύX
	//��

	//���ʂȏ�����K�v�Ƃ���ꍇ
	//�܂��͎����œƎ��̏������`�������ꍇ
	//�����Ń��b�Z�[�W�̎�ނɉ�����
	//�������s��

	//��肠���� WM_DESTROY����������
	//�I�������̂ݒ�`

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);

		//���b�Z�[�W�L���[��
		//�V����WM_QUIT���b�Z�[�W�𑗂�

		//�����ꃁ�b�Z�[�W�L���[����
		//���o����
		//���C�����[�v���I������
		//(���C�����[�v�̏I��������
		//���̂悤�ɍ��)
		break;
	}

	//�Ǝ��̏������s������
	//�f�t�H���g�̃E�B���h�E�v���V�[�W�������s

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//�E�B���h�E�ݒ�̖��O
static const TCHAR* WC_BASIC = _T("Basic");

//��肽���E�B���h�E�̐ݒ��
//�o�^����֐�
HRESULT RegistClassEx(HINSTANCE hInstance)
{
	//�E�B���h�E�̐ݒ��ۑ�����\����
	WNDCLASSEX wcex;

	//wcex�̐擪�A�h���X����
	//WNDCLASSEX�̒������̃�������
	//�S��0�Ŗ��߂�
	memset(&wcex, 0, sizeof(WNDCLASSEX));
	//�܂��wcex���g�p���Ă��郁�����̈��
	//�S��0�ŏ��������鏈��

	//�E�B���h�E�̐ݒ�

	//�\���̂̃T�C�Y
	wcex.cbSize = sizeof(WNDCLASSEX);

	//�E�B���h�E�̐ݒ���ɒǉ�����ߑ���t���Ȃ��̂�
	//0�ŌŒ�
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	//Window�̃X�^�C��
	//���ꂼ�ꂪ�r�b�g�P�ʂ̃t���O�ł��邽��
	//�I�A���Z�ō����������̂��X�^�C���̍ŏI�I�Ȑݒ�
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//CS_HREDRAW ���T�C�Y���ς�����Ƃ��E�B���h�E�S�̂̍ĕ`��
	//   VREDRAW �c�T�C�Y���ς�����Ƃ��E�B���h�E�S�̂̍ĕ`��
	//   DBLCLKS �_�u���N���b�N�������������Ƃ��E�B���h�E�ɒʒm

	//WinMain�̃C���X�^���X�n���h��
	wcex.hInstance = hInstance;

	//�E�B���h�E�v���V�[�W���̃A�h���X
	//�֐��|�C���^�̐ݒ�
	//�E�B���h�E�v���V�[�W����OS���Ăяo����悤�ɂ���
	wcex.lpfnWndProc = (WNDPROC)WndPrc;

	//�w�i�F
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//���j���[�@�Ȃ�
	wcex.lpszMenuName = NULL;

	//�E�B���h�E�N���X�̖��O�@���̃E�B���h�E�ݒ�̖��O
	wcex.lpszClassName = WC_BASIC;

	//�A�C�R���@�f�t�H���g�̂��g��
	wcex.hIcon = (HICON)LoadImage(NULL,
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm = NULL;

	//�J�[�\�� �f�t�H���g�̂��̂��g��
	wcex.hCursor = (HCURSOR)LoadImage(NULL,
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED);

	//�ݒ肵���E�B���h�E�N���X��o�^
	//�o�^�����E�B���h�E�ݒ�łȂ���
	//�E�B���h�E����邱�Ƃ��ł��Ȃ�
	return RegisterClassEx(&wcex);
	//HRESULT�^�̎��s���ʂ��߂����
}


//�o�^�����ݒ����ɃE�B���h�E���쐬����
HRESULT MakeWindow
(HINSTANCE hInstance,//WinMain�̎��ʎq
	HWND &refHWnd,	//�E�B���h�E�̎��ʎq
					//�������E�B���h�E�̍쐬���ł�����
					//���̕ϐ��Ɏ��ʎq��������
	int width = WindowWidthSize,	//�N���C�A���g�̈�̕�
	int height = WindowHeightSize)	//�N���C�A���g�̈�̍���
						//�N���C�A���g�̈�̓E�B���h�E�S�̂���
						//�O�g�⃁�j���[�̕��������������ƍ��͎v���Ă�����OK
{
	//�N���C�A���g�̈�̂������������߂�
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;

	//�N���C�A���g�̈��ێ�����̂ɕK�v��
	//�E�B���h�E�̂����������v�Z
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE/*TRUE*/);
	//���������s������rect�̒l���X�V�����

	//WS_OVERLAPPEDWINDOW�̓E�B���h�E�̃X�^�C���̈��

	//�E�B���h�E�̃n���h��
	//�E�B���h�E�̎��ʂɎg�p����
	HWND hWnd;

	hWnd = CreateWindowEx
	(0,						//�E�B���h�E�g���X�^�C��
		WC_BASIC,				//��肽���E�B���h�E�N���X
								//���炩���ߓo�^���ꂽ����
		_T("����"),			//�E�B���h�E�̃^�C�g��
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,			//�ʒux���W �f�t�H���g�̒l
		CW_USEDEFAULT,			//�ʒuy���W �f�t�H���g�̒l
		rect.right - rect.left,	//�E�B���h�E�S�̂̕�
		rect.bottom - rect.top, //�E�B���h�E�S�̂̍���
		NULL,					//�e�E�B���h�E�̃n���h��
		NULL,					//���j���[�̃n���h��
		hInstance,				//WinMain�̃n���h��

		NULL);					//�E�B���h�E�쐬�f�[�^
								//�E�B���h�E�ɍD����32�r�b�g�̃f�[�^��
								//�t�������邱�Ƃ��ł���
								//�ǉ����\���̂ւ̃|�C���^�Ȃ�
								//��t����̂�����Ȏg������
								//����͎g��Ȃ�

								//�E�B���h�E����������ꂽ�����`�F�b�N
								//��������ꂽ��hWnd��NULL�ȊO������͂�
	if (hWnd == NULL)
	{
		return S_FALSE;//HRESULT�^�@���s��\�����ʎq�̈��
	}

	//�E�B���h�E���A�N�e�B�u�ɂ��ĕ\��
	//hWnd�ŃE�B���h�E�̎��ʂ����Ă��邱�Ƃɒ���
	ShowWindow(hWnd, SW_SHOWNORMAL);

	//�N���C�A���g�̈�̍X�V
	UpdateWindow(hWnd);

	//�Q�Ɠn���ɂȂ��Ă��������
	//�E�B���h�E�̎��ʃn���h�����R�s�[����
	//����ɂ���č�������E�B���h�E�̎��ʃn���h����
	//�Ăяo�����̃v���O�����œ��邱�Ƃ��ł���
	refHWnd = hWnd;

	//HRESULT�^�@������\�����ʎq�̈��
	return S_OK;
}


//�G���g���[�|�C���g
//�v���O�����̊J�n�֐�

int _stdcall WinMain
(HINSTANCE hInstance,	//�v���O���������ʂ���n���h��
	HINSTANCE hPrevInstance,//�A�v���P�[�V�����̑O�̃C���X�^���X�n���h��
							//win32���Ə��NULL
	LPSTR lpCmdLine,		//�R�}���h���C������
	int nCmdShow)		//�E�B���h�E�̕\�����
{
	//���b�Z�[�W�{�b�N�X
	MessageBox(NULL,		//�E�B���h�E�̃n���h�� 
		TEXT("�e�X�g"),		//�{��
		TEXT("�e�X�g-�^�C�g��"),//�^�C�g��
		MB_OK);				//���b�Z�[�W�{�b�N�X�̃^�C�v
							//MB_OK  ok�̃{�^�����\��

	if (FAILED(RegistClassEx(hInstance)))
	{
		MessageBox(NULL,
			TEXT("�E�B���h�E�N���X�ݒ莸�s"),
			TEXT("�e�X�g-�^�C�g��"),
			MB_OK);
		return 0;
	}

	HWND hWnd = NULL;

	//�������E�B���h�E���쐬������
	//hWnd�ɃE�B���h�E�̎��ʃn���h��������
	if (FAILED(MakeWindow(hInstance, hWnd)))
	{
		MessageBox(NULL,
			TEXT("�E�B���h�E�쐬���s"),
			TEXT("�e�X�g-�^�C�g��"),
			MB_OK);
		return 0;
	}

	//Direct3D���Ǘ�����N���X(�V���O���g��)�ւ̎Q�Ƃ��擾
	Direct3D& d3d = Direct3D::GetInstance();

	//Direct3DDevice�̍쐬�����݂�
	if (d3d.TryCreate(hWnd))
	{
		MessageBox(NULL,
			TEXT("Direct3D Device�쐬����"),
			TEXT("�e�X�g-�^�C�g��"),
			MB_OK);
	}

	d3d.SetRenderState(RENDERSTATE::RENDER_ALPHATEST);

	DirectInput * pDi = DirectInput::GetInstance();
	pDi->Init(hWnd);

	//��̃T�C�Y
	int PieceImageSize = BoardImageSize / MapSize - 6;

	//�}�E�X�t���O
	bool MouseFlag;
	//�E�B���h�E�t���O
	bool WindowFlag;
	//�E�B���h�E�}�E�X���W
	float WindowMousePosX, WindowMousePosY;
	//��̐؂�ւ��t���O
	bool TypeChangeFlag;
	//�������u�Ԃ̃}�E�X���W
	int SetMousePosX, SetMousePosY;
	//���݂̃}�E�X���W
	int MousePosX, MousePosY;

	//�Ֆʂ̉摜��ݒ�
	Sprite spriteImgBoard;
	Texture imgBoard;
	spriteImgBoard.SetSize(BoardImageSize, BoardImageSize);
	spriteImgBoard.SetPos(BoardImageSize / 2, BoardImageSize / 2);

	imgBoard.Load(_T("Texture/board.png"));

	//�E�B���h�E�̉摜��ݒ�
	Sprite spriteImgWindow;
	Texture imgWindow;
	spriteImgWindow.SetSize(300, 150);
	spriteImgWindow.SetPos(350, 300);

	imgWindow.Load(_T("Texture/window.png"));

	//��̉摜��ݒ�
	Sprite spriteImgPiece[MapSize][MapSize];
	Texture imgPiece[MapSize][MapSize];

	//�J�[�\���̉摜��ݒ�
	Sprite spriteImgCursor[MapSize][MapSize];
	Texture imgCursor[MapSize][MapSize];

	for (int yCount = 0; yCount < MapSize; yCount++)
	{
		for (int xCount = 0; xCount < MapSize; xCount++)
		{
			//��
			spriteImgPiece[yCount][xCount].SetSize(PieceImageSize, PieceImageSize);

			imgPiece[yCount][xCount].Load(_T("Texture/piece.png"));
			imgPiece[yCount][xCount].SetDivide(8, 2);

			//�J�[�\��
			spriteImgCursor[yCount][xCount].SetSize(PieceImageSize, PieceImageSize);

			imgCursor[yCount][xCount].Load(_T("Texture/cursor.png"));
		}
	}

	//���������Z�����ɐݒ�
	Game_Mode Mode = GameStartProcessing;

	//�����̏��Ԃ��v���C���[�P�ɐݒ�
	GameOrderMode GOM = Player01;
	//������̐錾
	OfBoardPieceState OBPS;

	PieceProcessing PP;

	MSG msg = {};

	//���C���v���O����-------------------------------------------------
	//quit���b�Z�[�W���o�Ă���܂Ń��[�v���J��Ԃ�
	//quit���b�Z�[�W�͏�L�̃E�B���h�E�v���V�[�W�����瑗�M
	//���M�̏����Ȃǂ̓E�B���h�E�v���V�[�W�����m�F
	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ���
			//��������b�Z�[�W�L���[�Ƀ|�X�g����
			TranslateMessage(&msg);

			//���b�Z�[�W�̊��蓖��
			//�E�B���h�E�v���V�[�W���Ń��b�Z�[�W������
			DispatchMessage(&msg);
		}
		else
		{
			pDi->Update();//�L�[��Ԃ̍X�V

			switch (Mode)
			{

			//�X�^�[�g���
			case Game_Mode::StartScreenProcessing:

				break;

			//��������
			case Game_Mode::GameStartProcessing:

				PP.~PieceProcessing();

				MouseFlag = false;
				TypeChangeFlag = false;

				//�v���C���[����Ɉړ�
				Mode = PlayerProcessing;
				break;

			//�v���C���[����(�I���\����)
			case Game_Mode::PlayerProcessing:

				if (TypeChangeFlag == true)
				{
					PP.PieceMove(SetMousePosY,SetMousePosX,MousePosY,MousePosX);
					PP.PieceChooseableReset();

					TypeChangeFlag = false;
					MouseFlag = false;
					WindowFlag = false;

					switch (GOM)
					{
					case Player01:
						GOM = Player02;
						break;
					case Player02:
						GOM = Player01;
						break;
					}
					break;
				}
				
				if (MouseFlag == false && pDi->MouseButton(0))
				{
					MouseFlag = true;

					//�}�E�X���W���擾
					Vector2<int> mousePos = pDi->MousePosition();

					SetMousePosX = (mousePos.X() - 32.0) / PieceImageSize;
					SetMousePosY = (mousePos.Y() - 32.0) / PieceImageSize;

					if (MousePosX < 9&&MousePosY < 9)
					{
						if (PP.PS[SetMousePosY][SetMousePosX].PiecesGOM == GOM)
						{
							//�I���\�`��
							PP.PieceChooseable
							(SetMousePosY, SetMousePosX,
							 PP.PS[SetMousePosY][SetMousePosX].Type,
							 PP.PS[SetMousePosY][SetMousePosX].Promoted,
							 GOM);
						}
					}
				}
				else if (MouseFlag == true && pDi->MouseButton(0))
				{
					;
				}
				else if (PP.PS[SetMousePosY][SetMousePosX].PiecesGOM == GOM && MouseFlag == true && !(pDi->MouseButton(0)))
				{

					//�}�E�X���W���擾
					Vector2<int> mousePos = pDi->MousePosition();

					MousePosX = (mousePos.X() - 32.0) / PieceImageSize;
					MousePosY = (mousePos.Y() - 32.0) / PieceImageSize;

					if (MousePosX < 9 && MousePosY < 9)
					{
						if (PP.PS[MousePosY][MousePosX].Chooseable == true)
						{
							//�����Ύ�����𑝂₷
							if (PP.PS[MousePosY][MousePosX].Type != Null)
							{
								;
							}

							//���邩�ǂ����̔��f���Ăяo��
							//����ȏ�i�߂Ȃ��Ȃ��͎����I�ɐ���
							if (((PP.PS[SetMousePosY][SetMousePosX].Type == Pawn
							  || PP.PS[SetMousePosY][SetMousePosX].Type == Lance)
							  && ((GOM == Player01 && MousePosY <= 0)
							  || (GOM == Player02 && MousePosY >= MapSize - 1)))

							  || ((PP.PS[SetMousePosY][SetMousePosX].Type == Knight)
							  && ((GOM == Player01 && MousePosY <= 1)
							  || (GOM == Player02 && MousePosY >= MapSize - 2)))

							  && PP.PS[SetMousePosY][SetMousePosX].Promoted == false)
							{
								PP.PS[MousePosY][MousePosX].Promoted = true;
								Mode = PlayerProcessing;
							}
							else if (GOM == Player01 && (MousePosY < 3 || SetMousePosY < 3))
							{
								Mode = PromotedWindowProcessing;
							}
							else if (GOM == Player02 && (MousePosY > 5 || SetMousePosY > 5))
							{
								Mode = PromotedWindowProcessing;
							}
							else
							{
								PP.PS[MousePosY][MousePosX].Promoted = PP.PS[SetMousePosY][SetMousePosX].Promoted;
							}
							
							TypeChangeFlag = true;
						}
						else
						{
							PP.PieceChooseableReset();
							MouseFlag = false;
						}
					}
				}
				else if (MouseFlag == true && !(pDi->MouseButton(0)))
				{
					PP.PieceChooseableReset();
					MouseFlag = false;
				}

				break;

			//�Ֆʏ���
			case Game_Mode::BoardProcess:

				//�v���C���[����(�I���\����)�Ɉړ�
				Mode = PlayerProcessing;

				break;

			//���邩�ǂ����̏����i�E�B���h�E�`��j
			case Game_Mode::PromotedWindowProcessing:

				if (PP.PS[SetMousePosY][SetMousePosX].Type != King
					&& PP.PS[SetMousePosY][SetMousePosX].Type != GoldGeneral
					&& PP.PS[SetMousePosY][SetMousePosX].Promoted == false)
				{
					WindowFlag = true;

					if (MouseFlag == false && pDi->MouseButton(0))
					{
						MouseFlag = true;
						//�}�E�X���W���擾
						Vector2<int> mousePos = pDi->MousePosition();
					}
					else if (MouseFlag == true && pDi->MouseButton(0))
					{
						;
					}
					else if (MouseFlag == true && !(pDi->MouseButton(0)))
					{
						//�}�E�X���W���擾
						Vector2<int> mousePos = pDi->MousePosition();

						WindowMousePosX = mousePos.X();
						WindowMousePosY = mousePos.Y();

						if (WindowMousePosX >=229&& WindowMousePosX < 341
						  && WindowMousePosY >= 312 && WindowMousePosY < 344)
						{
							PP.PS[MousePosY][MousePosX].Promoted = true;
							Mode = PlayerProcessing;
							WindowFlag = false;
						}
						else if (WindowMousePosX >= 356 && WindowMousePosX < 468
							&& WindowMousePosY >= 312 && WindowMousePosY < 344)
						{
							PP.PS[MousePosY][MousePosX].Promoted = PP.PS[SetMousePosY][SetMousePosX].Promoted;
							Mode = PlayerProcessing;
							WindowFlag = false;
						}

						MouseFlag = false;

					}
				}
				else
				{
					PP.PS[MousePosY][MousePosX].Promoted = PP.PS[SetMousePosY][SetMousePosX].Promoted;
					Mode = PlayerProcessing;
				}

				break;

			//�Q�[���I��
			case Game_Mode::GameEndProcessing:

				break;

			}

			//�`�揈��
			//�o�b�N�o�b�t�@�̃N���A
			if (SUCCEEDED(d3d.BeginScene()))
			{
				d3d.ClearScreen();

				//�Ղ̕`��
				spriteImgBoard.Draw(imgBoard);

				//��̕`��
				for (int yCount = 0; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						//��邱�Ƃ��\�ł���ΑI���\�J�[�\����`��
						if (PP.PS[yCount][xCount].Chooseable == true)
						{
							//���W�ݒ�
							spriteImgCursor[yCount][xCount].SetPos
							((float)xCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 33.0,
								(float)yCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 33.0);

							//�`��
							spriteImgCursor[yCount][xCount].Draw(imgCursor[yCount][xCount]);
						}

						//���̋�Ֆʂɏo�Ă��邩�ǂ���
						//������̏�Ԃ����ł͂Ȃ���
						if (PP.PS[yCount][xCount].Type != Null)
						{
							//���̋�G���Ɓi90�x�j���]����
							switch (PP.PS[yCount][xCount].PiecesGOM)
							{
							case Player01:
								spriteImgPiece[yCount][xCount].SetAngle(0);
								break;
							case Player02:
								spriteImgPiece[yCount][xCount].SetAngle(3.1415/1);
								break;;
							}

							switch (PP.PS[yCount][xCount].Promoted)
							{

							//�����Ă��Ȃ�
							case false:

								//���̋�̎�ނ�
								switch (PP.PS[yCount][xCount].Type)
								{
								//��
								case Pawn:
									imgPiece[yCount][xCount].SetNum(0, 0);
									break;
								//����
								case Lance:
									imgPiece[yCount][xCount].SetNum(2, 0);
									break;
								//�j�n
								case Knight:
									imgPiece[yCount][xCount].SetNum(4, 0);
									break;
								//�⏫
								case SilverGeneral:
									imgPiece[yCount][xCount].SetNum(6, 0);
									break;
								//����
								case GoldGeneral:
									imgPiece[yCount][xCount].SetNum(0, 1);
									break;
								//���
								case Rook:
									imgPiece[yCount][xCount].SetNum(2, 1);
									break;
								//�p�s
								case Bishop:
									imgPiece[yCount][xCount].SetNum(4, 1);
									break;
								//��,��
								case King:
									//�ǂ���̋
									switch (PP.PS[yCount][xCount].PiecesGOM)
									{
										//��
									case Player01:
										imgPiece[yCount][xCount].SetNum(6, 1);
										break;
										//��
									case Player02:
										imgPiece[yCount][xCount].SetNum(7, 1);
										break;
									}
									break;
								}

								break;

							//�����Ă���
							case true:

								//���̋�̎�ނ�
								switch (PP.PS[yCount][xCount].Type)
								{
								//�Ƌ�
								case Pawn:
									imgPiece[yCount][xCount].SetNum(1, 0);
									break;
								//����
								case Lance:
									imgPiece[yCount][xCount].SetNum(3, 0);
									break;
								//���j
								case Knight:
									imgPiece[yCount][xCount].SetNum(5, 0);
									break;
								//����
								case SilverGeneral:
									imgPiece[yCount][xCount].SetNum(7, 0);
									break;
								//�����i���ԁj
								case Rook:
									imgPiece[yCount][xCount].SetNum(3, 1);
									break;
								//���n�i���p�j
								case Bishop:
									imgPiece[yCount][xCount].SetNum(5, 1);
									break;
								}

								break;
							}

							//���W�ݒ�
							spriteImgPiece[yCount][xCount].SetPos
							((float)xCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 32.0,
								(float)yCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 32.0);

							//�`��
							spriteImgPiece[yCount][xCount].Draw(imgPiece[yCount][xCount]);
						}
					}
				}

				//�E�B���h�E�̕`��
				if (WindowFlag == true)
				{
					spriteImgWindow.Draw(imgWindow);
				}

				//�`��I���̍��}
				d3d.EndScene();
				//�o�b�N�o�b�t�@���t�����g�֔��f
				d3d.Present();
			}
		}
	}

	return 0;
};
