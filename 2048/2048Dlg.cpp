
// 2048Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "2048.h"
#include "2048Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// C2048Dlg 对话框


//颜色表
#define Color_NavajoWhite	RGB(255, 222, 157)
#define Color_Red			RGB(250, 0, 0)
#define Color_Black			RGB(0, 0, 0)

C2048Dlg::C2048Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C2048Dlg::IDD, pParent)
{
	//图标选用IDI_MainIcon
	//多格式Icon
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


// C2048Dlg 消息处理程序

BOOL C2048Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	
	// TODO:  在此添加额外的初始化代码
	
	//初始化2的幂次对应列表
	int initCellNumber = 1;
	for (int i = 0; i <= 16; ++i) {
		expMap[i] = initCellNumber;
		initCellNumber *= 2;
	}

	srand(time(NULL));	//随机数种子

	RECT rect;
	GetClientRect(&rect);				//获得客户区矩形
	CDC *dc = GetDC();					//获得DC
	majorDC.CreateCompatibleDC(dc);		//主DC与当前DC兼容

	bmp.CreateCompatibleBitmap(dc, rect.right, rect.bottom);	//位图兼容
	majorDC.SetBkMode(TRANSPARENT);		//设置字符串背景为透明
	majorDC.SelectObject(&bmp);			//选用成员bmp

	pen.CreatePen(PS_SOLID, 1, Color_NavajoWhite);	//定义画笔，采用大背景色(纳瓦白加黄)
	majorDC.SelectObject(&pen);						//选用成员pen

	ReleaseDC(dc);						//释放DC

	OnClickedRebegin();					//调用点击Rebegin按钮的响应函数
	SetTimer(1, 1000 / 60, NULL);		//设定计时器，周期刷新游戏区图形，null即为OnTimer函数

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C2048Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 设置对话框背景色
		CRect rect;
		GetClientRect(&rect);							//获取客户区矩形
		CPaintDC dc(this);								//获取当前DC
		dc.FillSolidRect(rect, Color_NavajoWhite);		//客户区用纯色(纳瓦白加黄)无边框填充
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C2048Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void C2048Dlg::OnClickedRebegin()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			board[i][j] = 0;			//游戏区重置

	score = 0;							//分数重置
	isGameover = false;					//游戏结束状态重置为false
	generateNewNumber();				//生成两个新数字
	generateNewNumber();
}


void C2048Dlg::OnClickedAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();		//弹出About对话框
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


// 生成新的数字(2或4)
void C2048Dlg::generateNewNumber()
{
	vector<int> blankCells;			//记录当前空格子的位置
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (board[i][j] == 0)
				blankCells.push_back(i * 4 + j);

	if (blankCells.empty()) return;

	int randomIdx = (rand() * rand()) % blankCells.size();
	new_pos = blankCells[randomIdx];	//随机新位置

	board[0][new_pos] = (rand() * rand()) % 100 > 80 ? 2 : 1;	//按概率决定生成2或4
	zoomStep = 0;	//新块放大步骤编号重置
}


// 显示游戏瞬时界面
void C2048Dlg::show()
{
	// 格子参数类
	struct NumberCell {
		int num;				//格子里的数字
		COLORREF bkColor;		//背景色
		COLORREF textColor;		//前景色
		int fontHeight;			//字体高度
		int fontWidth;			//字体宽度
	};

	//不同数字块的颜色参数
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

	static const int cellWidth = 120;	//格子长度
	static const int pos_x = 10;		//游戏区，左上角格子x坐标
	static const int pos_y = 10;		//游戏区，左上角格子y坐标
	static const int gapWidth = 10;		//间隙长度

	CBrush bkBrush(Color_NavajoWhite);	//定义游戏区背景画刷(纳瓦白加黄)
	majorDC.SelectObject(&bkBrush);		//选用背景刷
	RECT gameField = { pos_x - gapWidth, pos_y - gapWidth,		//定义游戏区
		pos_x + 4 * gapWidth + 4 * cellWidth, pos_y + 4 * gapWidth + 4 * cellWidth };
	majorDC.Rectangle(&gameField);		//绘制方角矩形边线，并用背景刷填充

	//绘制格子
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			//int exp = i * 4 + j + 1;						//调试语句
			
			int exp = board[i][j];							//2的幂次
			CBrush cellBkBrush(cells[exp].bkColor);			//创建当前格子背景色画刷
			majorDC.SelectObject(&cellBkBrush);				//选用该画刷
			majorDC.SetTextColor(cells[exp].textColor);		//设定当前格子数字颜色

			RECT cellRect;									//定义格子矩形并计算坐标
			cellRect.left = pos_x + i * (cellWidth + gapWidth);
			cellRect.top = pos_y + j * (cellWidth + gapWidth);
			cellRect.right = cellRect.left + cellWidth;
			cellRect.bottom = cellRect.top + cellWidth;

			
			// 新出现块的放大动画实现
			if (i * 4 + j == new_pos) {
				const static int zoomArg[] = {			//偏离原格子边沿的距离
					-24, -20, -16, -12, -8, -4, 0,
				};
				int delta = zoomArg[zoomStep];			//当前偏移距离
				cellRect.left	-= delta;				//偏移修正
				cellRect.right	+= delta;
				cellRect.top	-= delta;
				cellRect.bottom += delta;

				++zoomStep;								//放大步骤编号更新
				
				//放大动画结束，重置new_pos为不可达
				if (zoomStep >= sizeof(zoomArg) / sizeof(int))
					new_pos = -1;
			}
			majorDC.Rectangle(&cellRect);				//填充格子背景色

			//绘制数字
			if (exp > 0) {
				CString numberStr;
				numberStr.Format(_T("%d"), expMap[exp]);	//欲绘制的数字(CString格式)
				
				CFont numberFont;							//定义当前数字字体格式
				numberFont.CreateFont(cells[exp].fontHeight, cells[exp].fontWidth, 0, 0,
					FW_EXTRABOLD, false, false, false, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
					CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
				majorDC.SelectObject(&numberFont);			//选用该字体
				
				//绘制
				majorDC.DrawText(numberStr, &cellRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			}
		}

	RECT rect;
	GetClientRect(&rect);		//获取客户区矩形
	//"SCORE"字样的所在矩形
	RECT scoreTitleRect = { gameField.right, pos_y + cellWidth,
		rect.right, pos_y + cellWidth * 3 / 2 + gapWidth };
	//分数值所在的矩形
	RECT scoreValueRect = { gameField.right, scoreTitleRect.bottom,
		rect.right, pos_y + 2 * (cellWidth + gapWidth) };

	//分数字体设置
	CFont scoreFont;
	scoreFont.CreateFont(56, 16, 0, 0, 
		FW_EXTRABOLD, false, false, false, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Algerian"));
	majorDC.SelectObject(&scoreFont);

	majorDC.SelectObject(&bkBrush);			//重新选用背景刷
	majorDC.SetTextColor(Color_Black);		//设定分数字体颜色(黑)
	majorDC.Rectangle(&scoreTitleRect);		//填充背景色
	majorDC.Rectangle(&scoreValueRect);

	CString scoreStr;
	scoreStr.Format(_T("%d"), score);		//欲绘制的分数(CString格式)
	majorDC.DrawText(_T("SCORE"), &scoreTitleRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	majorDC.DrawText(scoreStr, &scoreValueRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	if (isGameover)
	{
		//gameover字体设置
		CFont gameoverFont;	
		gameoverFont.CreateFont(80, 32, 0, 0, 
			FW_EXTRABOLD, false, false, false, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
			CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_MODERN, _T("Arial"));
		majorDC.SelectObject(&gameoverFont);
		
		majorDC.SetTextColor(Color_Red);		//设定gameover字体颜色(红)
		majorDC.DrawText(_T("Game Over!"), &gameField, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	//将新绘制的图形覆盖到客户区
	CDC *dc = GetDC();
	dc->BitBlt(0, 0, gameField.right, gameField.bottom, &majorDC, 0, 0, SRCCOPY);
	dc->BitBlt(scoreTitleRect.left, scoreTitleRect.top, scoreTitleRect.right - scoreTitleRect.left,
		cellWidth + gapWidth, &majorDC, scoreTitleRect.left, scoreTitleRect.top, SRCCOPY);
	ReleaseDC(dc);
}


// 数字向某一方向滑落(聚堆)，如果是有效滑落返回true
bool C2048Dlg::boardDrop(Dire dire)
{
	//纯模拟算法
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


// 合并相同数字，如果是有效合并返回true
bool C2048Dlg::cellCombine(Dire dire)
{
	//纯模拟算法
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


// 响应移动操作
void C2048Dlg::move(Dire dire)
{
	if (isGameover) return;

	bool isValidMove = false;				//记录是否为有效移动
	isValidMove |= boardDrop(dire);			//滑落
	isValidMove |= cellCombine(dire);		//合并相同数字
	isValidMove |= boardDrop(dire);			//再滑落
	if (isValidMove)						//是有效移动则生成新数字
		generateNewNumber();

	//判断当前局面是否游戏结束
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
