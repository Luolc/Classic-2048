
// 2048Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "2048.h"
#include "2048Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C2048Dlg �Ի���


//��ɫ��
#define Color_NavajoWhite	RGB(255, 222, 157)
#define Color_Red			RGB(250, 0, 0)
#define Color_Black			RGB(0, 0, 0)

C2048Dlg::C2048Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C2048Dlg::IDD, pParent)
{
	//ͼ��ѡ��IDI_MainIcon
	//���ʽIcon
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MainIcon);
}

void C2048Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(C2048Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Rebegin, &C2048Dlg::OnClickedRebegin)
	ON_BN_CLICKED(IDC_About, &C2048Dlg::OnClickedAbout)
	ON_BN_CLICKED(IDC_Exit, &C2048Dlg::OnClickedExit)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// C2048Dlg ��Ϣ�������

BOOL C2048Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	
	//��ʼ��2���ݴζ�Ӧ�б�
	int initCellNumber = 1;
	for (int i = 0; i <= 16; ++i) {
		expMap[i] = initCellNumber;
		initCellNumber *= 2;
	}

	srand(time(NULL));	//���������

	RECT rect;
	GetClientRect(&rect);				//��ÿͻ�������
	CDC *dc = GetDC();					//���DC
	majorDC.CreateCompatibleDC(dc);		//��DC�뵱ǰDC����

	bmp.CreateCompatibleBitmap(dc, rect.right, rect.bottom);	//λͼ����
	majorDC.SetBkMode(TRANSPARENT);		//�����ַ�������Ϊ͸��
	majorDC.SelectObject(&bmp);			//ѡ�ó�Աbmp

	pen.CreatePen(PS_SOLID, 1, Color_NavajoWhite);	//���廭�ʣ����ô󱳾�ɫ(���߰׼ӻ�)
	majorDC.SelectObject(&pen);						//ѡ�ó�Աpen

	ReleaseDC(dc);						//�ͷ�DC

	OnClickedRebegin();					//���õ��Rebegin��ť����Ӧ����
	SetTimer(1, 1000 / 60, NULL);		//�趨��ʱ��������ˢ����Ϸ��ͼ�Σ�null��ΪOnTimer����

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C2048Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void C2048Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ���öԻ��򱳾�ɫ
		CRect rect;
		GetClientRect(&rect);							//��ȡ�ͻ�������
		CPaintDC dc(this);								//��ȡ��ǰDC
		dc.FillSolidRect(rect, Color_NavajoWhite);		//�ͻ����ô�ɫ(���߰׼ӻ�)�ޱ߿����
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR C2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C2048Dlg::OnClickedRebegin()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			board[i][j] = 0;			//��Ϸ������

	score = 0;							//��������
	isGameover = false;					//��Ϸ����״̬����Ϊfalse
	generateNewNumber();				//��������������
	generateNewNumber();
}


void C2048Dlg::OnClickedAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();		//����About�Ի���
}


void C2048Dlg::OnClickedExit()
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE, 0, 0);
}


void C2048Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	show();
	CDialogEx::OnTimer(nIDEvent);
}


BOOL C2048Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam)
		{
		case VK_LEFT:	move(LEFT);		return true;
		case VK_RIGHT:	move(RIGHT);	return true;
		case VK_UP:		move(UP);		return true;
		case VK_DOWN:	move(DOWN);		return true;
		default:
			break;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


// �����µ�����(2��4)
void C2048Dlg::generateNewNumber()
{
	vector<int> blankCells;			//��¼��ǰ�ո��ӵ�λ��
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (board[i][j] == 0)
				blankCells.push_back(i * 4 + j);

	if (blankCells.empty()) return;

	int randomIdx = (rand() * rand()) % blankCells.size();
	new_pos = blankCells[randomIdx];	//�����λ��

	board[0][new_pos] = (rand() * rand()) % 100 > 80 ? 2 : 1;	//�����ʾ�������2��4
	zoomStep = 0;	//�¿�Ŵ���������
}


// ��ʾ��Ϸ˲ʱ����
void C2048Dlg::show()
{
	// ���Ӳ�����
	struct NumberCell {
		int num;				//�����������
		COLORREF bkColor;		//����ɫ
		COLORREF textColor;		//ǰ��ɫ
		int fontHeight;			//����߶�
		int fontWidth;			//������
	};

	//��ͬ���ֿ����ɫ����
	const static NumberCell cells[] = {
		1,			RGB(204, 192, 179),		RGB(204, 192, 179),		1,		1,
		2,			RGB(238, 230, 210),		RGB(119, 110, 100),		70,		28,
		4,			RGB(237, 224, 200),		RGB(119, 110, 100),		70,		28,
		8,			RGB(242, 177, 121),		RGB(249, 242, 242),		70,		28,
		16,			RGB(245, 148, 100),		RGB(249, 242, 242),		66,		25,
		32,			RGB(246, 124, 95),		RGB(249, 242, 242),		66,		25,
		64,			RGB(246, 94, 59),		RGB(249, 242, 242),		66,		25,
		128,		RGB(237, 207, 114),		RGB(249, 242, 242),		63,		20,
		256,		RGB(237, 204, 97),		RGB(249, 242, 242),		63,		20,
		512,		RGB(237, 200, 80),		RGB(249, 242, 242),		63,		20,
		1024,		RGB(237, 197, 63),		RGB(249, 242, 242),		56,		16,
		2048,		RGB(43, 132, 98),		RGB(249, 242, 242),		56,		16,
		4096,		RGB(250, 56, 108),		RGB(249, 242, 242),		56,		16,
		8192,		RGB(129, 148, 200),		RGB(249, 242, 242),		56,		16,
		16384,		RGB(255, 0, 0),			RGB(249, 242, 242),		56,		16,
		32768,		RGB(0, 255, 0),			RGB(249, 242, 242),		56,		16,
		65536,		RGB(128, 128, 0),		RGB(249, 242, 242),		56,		16,
	};

	static const int cellWidth = 120;	//���ӳ���
	static const int pos_x = 10;		//��Ϸ�������ϽǸ���x����
	static const int pos_y = 10;		//��Ϸ�������ϽǸ���y����
	static const int gapWidth = 10;		//��϶����

	CBrush bkBrush(Color_NavajoWhite);	//������Ϸ��������ˢ(���߰׼ӻ�)
	majorDC.SelectObject(&bkBrush);		//ѡ�ñ���ˢ
	RECT gameField = { pos_x - gapWidth, pos_y - gapWidth,		//������Ϸ��
		pos_x + 4 * gapWidth + 4 * cellWidth, pos_y + 4 * gapWidth + 4 * cellWidth };
	majorDC.Rectangle(&gameField);		//���Ʒ��Ǿ��α��ߣ����ñ���ˢ���

	//���Ƹ���
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			//int exp = i * 4 + j + 1;						//�������
			
			int exp = board[i][j];							//2���ݴ�
			CBrush cellBkBrush(cells[exp].bkColor);			//������ǰ���ӱ���ɫ��ˢ
			majorDC.SelectObject(&cellBkBrush);				//ѡ�øû�ˢ
			majorDC.SetTextColor(cells[exp].textColor);		//�趨��ǰ����������ɫ

			RECT cellRect;									//������Ӿ��β���������
			cellRect.left = pos_x + i * (cellWidth + gapWidth);
			cellRect.top = pos_y + j * (cellWidth + gapWidth);
			cellRect.right = cellRect.left + cellWidth;
			cellRect.bottom = cellRect.top + cellWidth;

			
			// �³��ֿ�ķŴ󶯻�ʵ��
			if (i * 4 + j == new_pos) {
				const static int zoomArg[] = {			//ƫ��ԭ���ӱ��صľ���
					-24, -20, -16, -12, -8, -4, 0,
				};
				int delta = zoomArg[zoomStep];			//��ǰƫ�ƾ���
				cellRect.left	-= delta;				//ƫ������
				cellRect.right	+= delta;
				cellRect.top	-= delta;
				cellRect.bottom += delta;

				++zoomStep;								//�Ŵ����Ÿ���
				
				//�Ŵ󶯻�����������new_posΪ���ɴ�
				if (zoomStep >= sizeof(zoomArg) / sizeof(int))
					new_pos = -1;
			}
			majorDC.Rectangle(&cellRect);				//�����ӱ���ɫ

			//��������
			if (exp > 0) {
				CString numberStr;
				numberStr.Format(_T("%d"), expMap[exp]);	//�����Ƶ�����(CString��ʽ)
				
				CFont numberFont;							//���嵱ǰ���������ʽ
				numberFont.CreateFont(cells[exp].fontHeight, cells[exp].fontWidth, 0, 0,
					FW_EXTRABOLD, false, false, false, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
				majorDC.SelectObject(&numberFont);			//ѡ�ø�����
				
				//����
				majorDC.DrawText(numberStr, &cellRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		}

	RECT rect;
	GetClientRect(&rect);		//��ȡ�ͻ�������
	//"SCORE"���������ھ���
	RECT scoreTitleRect = { gameField.right, pos_y + cellWidth,
		rect.right, pos_y + cellWidth * 3 / 2 + gapWidth };
	//����ֵ���ڵľ���
	RECT scoreValueRect = { gameField.right, scoreTitleRect.bottom,
		rect.right, pos_y + 2 * (cellWidth + gapWidth) };

	//������������
	CFont scoreFont;
	scoreFont.CreateFont(56, 16, 0, 0, 
		FW_EXTRABOLD, false, false, false, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Algerian"));
	majorDC.SelectObject(&scoreFont);

	majorDC.SelectObject(&bkBrush);			//����ѡ�ñ���ˢ
	majorDC.SetTextColor(Color_Black);		//�趨����������ɫ(��)
	majorDC.Rectangle(&scoreTitleRect);		//��䱳��ɫ
	majorDC.Rectangle(&scoreValueRect);

	CString scoreStr;
	scoreStr.Format(_T("%d"), score);		//�����Ƶķ���(CString��ʽ)
	majorDC.DrawText(_T("SCORE"), &scoreTitleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	majorDC.DrawText(scoreStr, &scoreValueRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (isGameover)
	{
		//gameover��������
		CFont gameoverFont;	
		gameoverFont.CreateFont(80, 32, 0, 0, 
			FW_EXTRABOLD, false, false, false, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
		majorDC.SelectObject(&gameoverFont);
		
		majorDC.SetTextColor(Color_Red);		//�趨gameover������ɫ(��)
		majorDC.DrawText(_T("Game Over!"), &gameField, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	//���»��Ƶ�ͼ�θ��ǵ��ͻ���
	CDC *dc = GetDC();
	dc->BitBlt(0, 0, gameField.right, gameField.bottom, &majorDC, 0, 0, SRCCOPY);
	dc->BitBlt(scoreTitleRect.left, scoreTitleRect.top, scoreTitleRect.right - scoreTitleRect.left,
		cellWidth + gapWidth, &majorDC, scoreTitleRect.left, scoreTitleRect.top, SRCCOPY);
	ReleaseDC(dc);
}


// ������ĳһ������(�۶�)���������Ч���䷵��true
bool C2048Dlg::boardDrop(Dire dire)
{
	//��ģ���㷨
	bool flag = false;
	switch (dire)
	{
	case C2048Dlg::LEFT:
		for (int j = 0; j < 4; ++j) {
			int pos = 0;
			for (int i = 0; i < 4; ++i) {
				if (board[pos][j] == 0) {
					for (int k = pos; k < 3; ++k) {
						board[k][j] = board[k + 1][j];
						if (board[k][j] > 0)
							flag = true;
					}
					board[3][j] = 0;
				}
				else {
					++pos;
				}
			}
		}
		break;
	case C2048Dlg::RIGHT:
		for (int j = 0; j < 4; ++j) {
			int pos = 3;
			for (int i = 0; i < 4; ++i) {
				if (board[pos][j] == 0) {
					for (int k = pos; k > 0; --k) {
						board[k][j] = board[k - 1][j];
						if (board[k][j] > 0)
							flag = true;
					}
					board[0][j] = 0;
				}
				else {
					--pos;
				}
			}
		}
		break;
	case C2048Dlg::UP:
		for (int i = 0; i < 4; ++i) {
			int pos = 0;
			for (int j = 0; j < 4; ++j) {
				if (board[i][pos] == 0) {
					for (int k = pos; k < 3; ++k) {
						board[i][k] = board[i][k + 1];
						if (board[i][k] > 0)
							flag = true;
					}
					board[i][3] = 0;
				}
				else {
					++pos;
				}
			}
		}
		break;
	case C2048Dlg::DOWN:
		for (int i = 0; i < 4; ++i) {
			int pos = 3;
			for (int j = 0; j < 4; ++j) {
				if (board[i][pos] == 0) {
					for (int k = pos; k > 0; --k) {
						board[i][k] = board[i][k - 1];
						if (board[i][k] > 0)
							flag = true;
					}
					board[i][0] = 0;
				}
				else {
					--pos;
				}
			}
		}
		break;
	default:
		break;
	}
	return flag;
}


// �ϲ���ͬ���֣��������Ч�ϲ�����true
bool C2048Dlg::cellCombine(Dire dire)
{
	//��ģ���㷨
	bool flag = false;
	switch (dire)
	{
	case C2048Dlg::LEFT:
		for (int j = 0; j < 4; ++j)
			for (int i = 0; i < 3; ++i) {
				if (board[i][j] == 0) continue;
				if (board[i][j] == board[i + 1][j]) {
					flag = true;
					board[i][j] += 1;
					board[i + 1][j] = 0;
					score += expMap[board[i][j]];
				}
			}
		break;
	case C2048Dlg::RIGHT:
		for (int j = 0; j < 4; ++j)
			for (int i = 3; i > 0; --i) {
				if (board[i][j] == 0) continue;
				if (board[i][j] == board[i - 1][j]) {
					flag = true;
					board[i][j] += 1;
					board[i - 1][j] = 0;
					score += expMap[board[i][j]];
				}
			}
		break;
	case C2048Dlg::UP:
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 3; ++j) {
				if (board[i][j] == 0) continue;
				if (board[i][j] == board[i][j + 1]) {
					flag = true;
					board[i][j] += 1;
					board[i][j + 1] = 0;
					score += expMap[board[i][j]];
				}
			}
		break;
	case C2048Dlg::DOWN:
		for (int i = 0; i < 4; ++i)
			for (int j = 3; j > 0; --j) {
				if (board[i][j] == 0) continue;
				if (board[i][j] == board[i][j - 1]) {
					flag = true;
					board[i][j] += 1;
					board[i][j - 1] = 0;
					score += expMap[board[i][j]];
				}
			}
		break;
	default:
		break;
	}
	return flag;
}


// ��Ӧ�ƶ�����
void C2048Dlg::move(Dire dire)
{
	if (isGameover) return;

	bool isValidMove = false;				//��¼�Ƿ�Ϊ��Ч�ƶ�
	isValidMove |= boardDrop(dire);			//����
	isValidMove |= cellCombine(dire);		//�ϲ���ͬ����
	isValidMove |= boardDrop(dire);			//�ٻ���
	if (isValidMove)						//����Ч�ƶ�������������
		generateNewNumber();

	//�жϵ�ǰ�����Ƿ���Ϸ����
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (board[i][j] == 0)
				return;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 4; ++j)
			if (board[i][j] == board[i + 1][j])
				return;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 3; ++j)
			if (board[i][j] == board[i][j + 1])
				return;
	isGameover = true;
}
