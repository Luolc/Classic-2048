
// 2048Dlg.h : ͷ�ļ�
//

#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
#include "afxwin.h"
using namespace std;

// C2048Dlg �Ի���
class C2048Dlg : public CDialogEx
{
// ����
public:
	C2048Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	enum Dire { LEFT = 1, RIGHT, UP, DOWN };
	//COLORREF bkColor = RGB(255, 222, 157);

	CDC majorDC;		//��DC
	CBitmap bmp;		//λͼ
	//CFont font;			//
	CPen pen;			//����

	int board[4][4];	//��Ϸ�����ּ�¼(��¼2���ݴ�)
	int new_pos;		//�³��ֿ�λ��
	int zoomStep;		//�¿�Ŵ�����
	int score;			//����
	bool isGameover;	//�Ƿ���Ϸ����

	int expMap[17];		//expMap[k] = 2 ^ k

public:
	afx_msg void OnClickedRebegin();
	afx_msg void OnClickedAbout();
	afx_msg void OnClickedExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	// �����µ�����(2��4)
	void generateNewNumber();
	// ��ʾ��Ϸ˲ʱ����
	void show();
	// ������ĳһ������(�۶�)
	bool boardDrop(Dire dire);
	// �ϲ���ͬ����
	bool cellCombine(Dire dire);
	// ��Ӧ�ƶ�����
	void move(Dire dire);
};
