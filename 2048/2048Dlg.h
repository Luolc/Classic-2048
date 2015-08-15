
// 2048Dlg.h : 头文件
//

#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include "afxwin.h"
using namespace std;

// C2048Dlg 对话框
class C2048Dlg : public CDialogEx
{
// 构造
public:
	C2048Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	enum Dire { LEFT = 1, RIGHT, UP, DOWN };
	//COLORREF bkColor = RGB(255, 222, 157);

	CDC majorDC;		//主DC
	CBitmap bmp;		//位图
	//CFont font;			//
	CPen pen;			//画笔

	int board[4][4];	//游戏区数字记录(记录2的幂次)
	int new_pos;		//新出现块位置
	int zoomStep;		//新块放大步骤编号
	int score;			//分数
	bool isGameover;	//是否游戏结束

	int expMap[17];		//expMap[k] = 2 ^ k

public:
	afx_msg void OnClickedRebegin();
	afx_msg void OnClickedAbout();
	afx_msg void OnClickedExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	// 生成新的数字(2或4)
	void generateNewNumber();
	// 显示游戏瞬时界面
	void show();
	// 数字向某一方向滑落(聚堆)
	bool boardDrop(Dire dire);
	// 合并相同数字
	bool cellCombine(Dire dire);
	// 响应移动操作
	void move(Dire dire);
};
