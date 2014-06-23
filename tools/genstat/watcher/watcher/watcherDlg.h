
// watcherDlg.h : 头文件
//

#pragma once

#include <vector>
#include <string>


// CwatcherDlg 对话框
class CwatcherDlg : public CDialogEx
{
// 构造
public:
	CwatcherDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WATCHER_DIALOG };
	CStatic	m_ctlGraph;
	CListCtrl m_ctlListUser;
	CButton m_buttonMax;
	CButton m_buttonMin;
	CStatic m_staticDate;
	int m_iCurFrmIdx;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void Parse(std::string name);
	void Draw(int offset);
	void ParseLineStr(char * str, std::vector<std::string> & vec);
	void ParseLineInt(char * str, std::vector<int> & vec);
	void DrawOneFrame(int iCol, int iIndex, int iPosX, int iMaxY, CPaintDC &dc);

	bool ParseText(std::string name);
	bool IsTitle(std::string str);
	bool IsEnd(std::string str);
	std::string GetTime(std::string str);
	void ParseLine(std::string str, std::string & name, int & data, std::string & datastring, std::string & comment);
// 实现
protected:
	HICON m_hIcon;

	std::vector<std::string> m_time;
	std::vector<std::string> m_name;
	std::vector<std::string> m_comment;
	std::vector<std::vector<int>> m_data;
	std::vector<std::vector<std::string>> m_strdata;
	std::vector<int> m_dataMax;
	std::vector<int> m_dataMin;
	std::vector<int> m_dataMaxIndex;
	std::vector<int> m_dataMinIndex;

	BOOL		m_bLBtnDown;		//左键是否按下
	int			m_iMouseLastPosX;	//鼠标上一次位置

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnItemchangedListName(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFirst();
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonLast();
	afx_msg void OnBnClickedButtonGo();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedButtonMax();
	afx_msg void OnBnClickedButtonMin();
};
