
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

	int Count;

	int SetPiece;
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

	//�莝���Ղ̉摜��ݒ�
	Sprite spriteImgOfBoard[2];
	Texture imgOfBoard[2];
	spriteImgOfBoard[0].SetSize(OfBoardImageSize, OfBoardImageSize);
	spriteImgOfBoard[0].SetPos(BoardImageSize+((WindowWidthSize - BoardImageSize)/2),
		OfBoardImageSize / 2);

	spriteImgOfBoard[1].SetSize(OfBoardImageSize, OfBoardImageSize);
	spriteImgOfBoard[1].SetPos(BoardImageSize+((WindowWidthSize - BoardImageSize)/2),
		OfBoardImageSize / 2 + WindowHeightSize - OfBoardImageSize);

	imgOfBoard[0].Load(_T("Texture/of_board.png"));
	imgOfBoard[1].Load(_T("Texture/of_board.png"));

	//�E�B���h�E�̉摜��ݒ�
	Sprite spriteImgWindow;
	Texture imgWindow;
	spriteImgWindow.SetSize(300, 150);
	spriteImgWindow.SetPos(350, 300);

	imgWindow.Load(_T("Texture/window.png"));

	//��̉摜��ݒ�
	Sprite spriteImgPiece[40];
	Texture imgPiece[40];

	for (int Count = 0; Count < 40; Count++)
	{
		//��
		spriteImgPiece[Count].SetSize(PieceImageSize, PieceImageSize);
		imgPiece[Count].Load(_T("Texture/piece.png"));
		imgPiece[Count].SetDivide(8, 2);
	}

	//�莝���̋�̉摜��ݒ�
	Sprite spriteImgOfPiece[14];
	Texture imgOfPiece[14];

	for (int Count = 0; Count < 14; Count++)
	{
		//��
		spriteImgOfPiece[Count].SetSize(PieceImageSize, PieceImageSize);
		spriteImgOfPiece[Count].SetPos(BoardImageSize + (Count % 7 % 4 + 1)*((WindowWidthSize - BoardImageSize) / 6) + PieceImageSize / 2,
			Count / 7 * 320 + ((Count - Count / 7 * 7) / 4) * PieceImageSize * 2 + PieceImageSize);
		imgOfPiece[Count].Load(_T("Texture/piece.png"));
		imgOfPiece[Count].SetDivide(8, 2);

		imgOfPiece[Count].SetNum(((Count % 7) * 2) % 8, (Count - Count / 7 * 7) / 4);
	}

	//�莝���̋�̉摜��ݒ�
	Sprite spriteImgOfPieceNumber[14];
	Texture imgOfPieceNumber[14];

	Sprite spriteImgOfPieceNumber2[14];
	Texture imgOfPieceNumber2[14];

	for (int Count = 0; Count < 14; Count++)
	{
		//�P��
		spriteImgOfPieceNumber[Count].SetSize(PieceImageSize/4, PieceImageSize/3);
		spriteImgOfPieceNumber[Count].SetPos(BoardImageSize + (Count % 7 % 4 + 1)*((WindowWidthSize - BoardImageSize) / 6) + PieceImageSize / 2,
			Count / 7 * 320 + ((Count - Count / 7 * 7) / 4) * PieceImageSize * 2 + PieceImageSize*1.7);
		imgOfPieceNumber[Count].Load(_T("Texture/number.png"));
		imgOfPieceNumber[Count].SetDivide(10, 1);

		//�Q��
		spriteImgOfPieceNumber2[Count].SetSize(PieceImageSize / 4, PieceImageSize / 3);
		spriteImgOfPieceNumber2[Count].SetPos(BoardImageSize + (Count % 7 % 4 + 1)*((WindowWidthSize - BoardImageSize) / 6) + PieceImageSize / 2 - PieceImageSize / 4,
			Count / 7 * 320 + ((Count - Count / 7 * 7) / 4) * PieceImageSize * 2 + PieceImageSize*1.7);
		imgOfPieceNumber2[Count].Load(_T("Texture/number.png"));
		imgOfPieceNumber2[Count].SetDivide(10, 1);
	}

	//�J�[�\���̉摜��ݒ�
	Sprite spriteImgCursor[MapSize][MapSize];
	Texture imgCursor[MapSize][MapSize];

	for (int CountY = 0; CountY < MapSize; CountY++)
	{
		for (int CountX = 0; CountX < MapSize; CountX++)
		{
			//�J�[�\��
			spriteImgCursor[CountY][CountX].SetSize(PieceImageSize, PieceImageSize);
			imgCursor[CountY][CountX].Load(_T("Texture/cursor.png"));
		}
	}

	//���������Z�����ɐݒ�
	Game_Mode Mode = GameStartProcessing;

	//�����̏��Ԃ��v���C���[�P�ɐݒ�
	GameOrderMode GOM = Player01;

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
				PP.PieceChooseableReset();

				MouseFlag = false;
				TypeChangeFlag = false;

				//�v���C���[����Ɉړ�
				Mode = PlayerProcessing;
				break;

			//�v���C���[����(�I���\����)
			case Game_Mode::PlayerProcessing:

				if (TypeChangeFlag == true)
				{
					PP.PieceMove(SetPiece,MousePosY,MousePosX);
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

					if (SetMousePosX < 9&& SetMousePosY < 9)
					{

						for (SetPiece = 0; SetPiece < 40; SetPiece++)
						{
							if (PP.PS[SetPiece].y == SetMousePosY
								&& PP.PS[SetPiece].x == SetMousePosX
								&& PP.PS[SetPiece].PiecesGOM == GOM
								&&PP.PS[SetPiece].OnBoard == true)
							{
								//�I���\�`��
								PP.PieceChooseable
								(SetPiece, GOM);
								break;
							}
						}
					}
					else
					{

						SetMousePosX = mousePos.X();
						SetMousePosY = mousePos.Y();

						for (int Mousecount = 0; Mousecount < 14; Mousecount++)
						{
							if (SetMousePosX >= (BoardImageSize + (Mousecount % 7 % 4 + 1)*((WindowWidthSize - BoardImageSize) / 6) + PieceImageSize / 2) - PieceImageSize / 2
							 && SetMousePosX <= (BoardImageSize + (Mousecount % 7 % 4 + 1)*((WindowWidthSize - BoardImageSize) / 6) + PieceImageSize / 2) + PieceImageSize / 2
							 && SetMousePosY >= (Mousecount / 7 * 320 + ((Mousecount - Mousecount / 7 * 7) / 4) * PieceImageSize * 2 + PieceImageSize) - PieceImageSize / 2
							 && SetMousePosY <= (Mousecount / 7 * 320 + ((Mousecount - Mousecount / 7 * 7) / 4) * PieceImageSize * 2 + PieceImageSize) + PieceImageSize / 2)
							{
								if(Mousecount < 7 && GOM == Player02
									|| Mousecount >= 7 && GOM == Player01)
								{
									for (SetPiece = 0; SetPiece < 40; SetPiece++)
									{
										if ((Mousecount % 7 == 0 && PP.PS[SetPiece].Type != Pawn
											|| Mousecount % 7 == 1 && PP.PS[SetPiece].Type != Lance
											|| Mousecount % 7 == 2 && PP.PS[SetPiece].Type != Knight
											|| Mousecount % 7 == 3 && PP.PS[SetPiece].Type != SilverGeneral
											|| Mousecount % 7 == 4 && PP.PS[SetPiece].Type != GoldGeneral
											|| Mousecount % 7 == 5 && PP.PS[SetPiece].Type != Rook
											|| Mousecount % 7 == 6 && PP.PS[SetPiece].Type != Bishop)){}
										else
										{
											if ((PP.PS[SetPiece].OnBoard == false)
												&& ((Mousecount < 7 && PP.PS[SetPiece].PiecesGOM == Player02)
													|| Mousecount >= 7 && PP.PS[SetPiece].PiecesGOM == Player01))
											{
												PP.OfPieceProcessing(SetPiece, Mousecount);
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				else if (MouseFlag == true && pDi->MouseButton(0))
				{
					;
				}
				else if (PP.PS[SetPiece].PiecesGOM == GOM && MouseFlag == true && !(pDi->MouseButton(0)))
				{

					//�}�E�X���W���擾
					Vector2<int> mousePos = pDi->MousePosition();

					MousePosX = (mousePos.X() - 32.0) / PieceImageSize;
					MousePosY = (mousePos.Y() - 32.0) / PieceImageSize;

					if (MousePosX < 9 && MousePosY < 9)
					{
						if (PP.Chooseable[MousePosY][MousePosX] == true)
						{
							//�����Ύ�����𑝂₷
							for (int i = 0; i < 40; i++)
							{
								if(PP.PS[i].y == MousePosY
									&& PP.PS[i].x == MousePosX
									&& PP.PS[i].PiecesGOM != GOM
									&& PP.PS[i].OnBoard == true
									&& PP.PS[i].Type == King)
								{
									Mode = GameEndProcessing;
									break;
								}
								else if (PP.PS[i].y == MousePosY
									&& PP.PS[i].x == MousePosX
									&& PP.PS[i].PiecesGOM != GOM
									&& PP.PS[i].OnBoard == true)
								{
									PP.PS[i].PiecesGOM = GOM;
									PP.PS[i].OnBoard = false;
									PP.PS[i].Promoted = false;
									break;
								}
							}

							//���邩�ǂ����̔��f���Ăяo��
							//����ȏ�i�߂Ȃ��Ȃ��͎����I�ɐ���
							if (((PP.PS[SetPiece].Type == Pawn
							  || PP.PS[SetPiece].Type == Lance)
							  && ((GOM == Player01 && MousePosY <= 0)
							  || (GOM == Player02 && MousePosY >= MapSize - 1)))

							  || ((PP.PS[SetPiece].Type == Knight)
							  && ((GOM == Player01 && MousePosY <= 1)
							  || (GOM == Player02 && MousePosY >= MapSize - 2)))

							  && PP.PS[SetPiece].Promoted == false)
							{
								PP.PS[SetPiece].Promoted = true;
							}
							else if (GOM == Player01 && (MousePosY < 3 || SetMousePosY < 3) && PP.PS[SetPiece].OnBoard == true)
							{
								if (Mode != GameEndProcessing)
								{
									Mode = PromotedWindowProcessing;
								}
							}
							else if (GOM == Player02 && (MousePosY > 5 || SetMousePosY > 5) && PP.PS[SetPiece].OnBoard == true)
							{
								if (Mode != GameEndProcessing)
								{
									Mode = PromotedWindowProcessing;
								}
							}
							
							TypeChangeFlag = true;
						}
						else
						{
							PP.PieceChooseableReset();
						}

						MouseFlag = false;
					}
					else
					{
						PP.PieceChooseableReset();
						MouseFlag = false;
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

				if (PP.PS[SetPiece].Type != King
					&& PP.PS[SetPiece].Type != GoldGeneral
					&& PP.PS[SetPiece].Promoted == false)
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
							PP.PS[SetPiece].Promoted = true;
						}

						Mode = PlayerProcessing;
						WindowFlag = false;
						MouseFlag = false;

					}
				}
				else
				{
					Mode = PlayerProcessing;
				}

				break;

			//�Q�[���I��
			case Game_Mode::GameEndProcessing:

				if (pDi->MouseButton(0))
				{
					Mode = GameStartProcessing;
				}

				break;

			}

			//�`�揈��
			//�o�b�N�o�b�t�@�̃N���A
			if (SUCCEEDED(d3d.BeginScene()))
			{
				d3d.ClearScreen();

				//�Ղ̕`��
				spriteImgBoard.Draw(imgBoard);

				for (int i = 0; i < 2; i++)
				{
					spriteImgOfBoard[i].Draw(imgOfBoard[i]);
				}

				for (int i = 0; i < 14; i++)
				{
					spriteImgOfPiece[i].Draw(imgOfPiece[i]);

					Count = 0;

					for (int a = 0; a < 40; a++)
					{
						if ((i % 7 == 0 && PP.PS[a].Type == Pawn
							|| i % 7 == 1 && PP.PS[a].Type == Lance
							|| i % 7 == 2 && PP.PS[a].Type == Knight
							|| i % 7 == 3 && PP.PS[a].Type == SilverGeneral
							|| i % 7 == 4 && PP.PS[a].Type == GoldGeneral
							|| i % 7 == 5 && PP.PS[a].Type == Rook
							|| i % 7 == 6 && PP.PS[a].Type == Bishop)) 
						{
							if (((i < 7 && PP.PS[a].PiecesGOM == Player02)
								|| (i >= 7 && PP.PS[a].PiecesGOM == Player01))
								&& PP.PS[a].OnBoard == false)
							{
								Count++;
							}
						}

					}

					imgOfPieceNumber[i].SetNum(Count%10, 1);

					if (Count >= 10)
					{
						imgOfPieceNumber2[i].SetNum(Count / 10, 1);
						spriteImgOfPieceNumber2[i].Draw(imgOfPieceNumber2[i]);
					}

					spriteImgOfPieceNumber[i].Draw(imgOfPieceNumber[i]);
				}

				//��̕`��
				for (int yCount = 0; yCount < MapSize; yCount++)
				{
					for (int xCount = 0; xCount < MapSize; xCount++)
					{
						//��邱�Ƃ��\�ł���ΑI���\�J�[�\����`��
						if (PP.Chooseable[yCount][xCount] == true)
						{
							//���W�ݒ�
							spriteImgCursor[yCount][xCount].SetPos
							((float)xCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 33.0,
								(float)yCount * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 33.0);

							//�`��
							spriteImgCursor[yCount][xCount].Draw(imgCursor[yCount][xCount]);
						}
					}
				}

				for (int Count = 0; Count < 40; Count++)
				{
					//���̋�Ֆʂɏo�Ă��邩�ǂ���
					//������̏�Ԃ����ł͂Ȃ���
					if (PP.PS[Count].OnBoard == true)
					{
						//���̋�G���Ɓi90�x�j���]����
						switch (PP.PS[Count].PiecesGOM)
						{
						case Player01:
							spriteImgPiece[Count].SetAngle(0);
							break;
						case Player02:
							spriteImgPiece[Count].SetAngle(3.1415 / 1);
							break;;
						}

						switch (PP.PS[Count].Promoted)
						{

						//�����Ă��Ȃ�
						case false:

							//���̋�̎�ނ�
							switch (PP.PS[Count].Type)
							{
								//��
							case Pawn:
								imgPiece[Count].SetNum(0, 0);
								break;
								//����
							case Lance:
								imgPiece[Count].SetNum(2, 0);
								break;
								//�j�n
							case Knight:
								imgPiece[Count].SetNum(4, 0);
								break;
								//�⏫
							case SilverGeneral:
								imgPiece[Count].SetNum(6, 0);
								break;
								//����
							case GoldGeneral:
								imgPiece[Count].SetNum(0, 1);
								break;
								//���
							case Rook:
								imgPiece[Count].SetNum(2, 1);
								break;
								//�p�s
							case Bishop:
								imgPiece[Count].SetNum(4, 1);
								break;
								//��,��
							case King:
								//�ǂ���̋
								switch (PP.PS[Count].PiecesGOM)
								{
									//��
								case Player01:
									imgPiece[Count].SetNum(6, 1);
									break;
									//��
								case Player02:
									imgPiece[Count].SetNum(7, 1);
									break;
								}
								break;
							}

							break;

							//�����Ă���
						case true:

							//���̋�̎�ނ�
							switch (PP.PS[Count].Type)
							{
								//�Ƌ�
							case Pawn:
								imgPiece[Count].SetNum(1, 0);
								break;
								//����
							case Lance:
								imgPiece[Count].SetNum(3, 0);
								break;
								//���j
							case Knight:
								imgPiece[Count].SetNum(5, 0);
								break;
								//����
							case SilverGeneral:
								imgPiece[Count].SetNum(7, 0);
								break;
								//�����i���ԁj
							case Rook:
								imgPiece[Count].SetNum(3, 1);
								break;
								//���n�i���p�j
							case Bishop:
								imgPiece[Count].SetNum(5, 1);
								break;
							}

							break;
						}

						//���W�ݒ�
						spriteImgPiece[Count].SetPos
						((float)PP.PS[Count].x * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 32.0,
							(float)PP.PS[Count].y * (float)PieceImageSize + (float)PieceImageSize / 2.0 + 32.0);

						//�`��
						spriteImgPiece[Count].Draw(imgPiece[Count]);
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
