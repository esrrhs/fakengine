
// watcherDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "watcher.h"
#include "watcherDlg.h"
#include "afxdialogex.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CwatcherDlg 对话框




CwatcherDlg::CwatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CwatcherDlg::IDD, pParent), m_bLBtnDown(FALSE), m_iCurFrmIdx(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GRAPH, m_ctlGraph);
	DDX_Control(pDX, IDC_LIST_NAME, m_ctlListUser);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_buttonMin);
	DDX_Control(pDX, IDC_BUTTON_MAX, m_buttonMax);
	DDX_Control(pDX, IDC_STATIC_DATE, m_staticDate);
	DDX_Text(pDX, IDC_EDIT_INDEX, m_iCurFrmIdx);
}

BEGIN_MESSAGE_MAP(CwatcherDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NAME, &CwatcherDlg::OnItemchangedListName)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CwatcherDlg::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CwatcherDlg::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CwatcherDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, &CwatcherDlg::OnBnClickedButtonLast)
	ON_BN_CLICKED(IDC_BUTTON_GO, &CwatcherDlg::OnBnClickedButtonGo)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CwatcherDlg::OnBnClickedButtonOpen)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_MAX, &CwatcherDlg::OnBnClickedButtonMax)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CwatcherDlg::OnBnClickedButtonMin)
END_MESSAGE_MAP()


// CwatcherDlg 消息处理程序

BOOL CwatcherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_ctlListUser.SetExtendedStyle(LVS_EX_FULLROWSELECT);//| LVS_EX_TWOCLICKACTIVATE);
	//用户数据名
	LVCOLUMN lc;
	lc.mask = LVCF_WIDTH | LVCF_FMT | LVCF_TEXT | LVCF_SUBITEM;
	lc.cx = 200;
	lc.fmt = LVCFMT_RIGHT;
	lc.pszText = "项目";
	lc.iSubItem = 0;
	m_ctlListUser.InsertColumn(0, &lc);
	//用户数据值
	lc.cx = 100;
	lc.fmt = LVCFMT_LEFT;
	lc.pszText = "值";
	lc.iSubItem = 1;
	m_ctlListUser.InsertColumn(1, &lc);
	//用户注释
	lc.cx = 200;
	lc.fmt = LVCFMT_LEFT;
	lc.pszText = "注释";
	lc.iSubItem = 2;
	m_ctlListUser.InsertColumn(2, &lc);

	ShowWindow(SW_NORMAL);

	//接受文件托拽
	DragAcceptFiles();

	// TODO: 在此添加额外的初始化代码
	Parse("gamesvrd.stat");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwatcherDlg::OnPaint()
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
		CDialogEx::OnPaint();
		Draw(m_iCurFrmIdx);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwatcherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CwatcherDlg::Parse(std::string filename)
{
	m_time.clear();
	m_name.clear();
	m_comment.clear();
	m_data.clear();
	m_strdata.clear();
	m_dataMax.clear();
	m_dataMin.clear();
	m_dataMaxIndex.clear();
	m_dataMinIndex.clear();
	m_staticDate.SetWindowTextA("");

	m_bLBtnDown = FALSE;		//左键是否按下
	m_iMouseLastPosX = 0;	//鼠标上一次位置
	m_iCurFrmIdx = 0;

	ParseText(filename);

	for (int i = 0; i < m_name.size(); i++)
	{
		int max = 0;
		int min = 0x7fffffff;
		int maxindex = 0;
		int minindex = 0;
		for (int j = 0; j < m_data.size(); j++)
		{
			int val = m_data[j][i];
			if (val > max)
			{
				max = val;
				maxindex = j;
			}
			if (val < min)
			{
				min = val;
				minindex = j;
			}
		}
		m_dataMax.push_back(max);
		m_dataMin.push_back(min);
		m_dataMaxIndex.push_back(maxindex);
		m_dataMinIndex.push_back(minindex);
	}

	m_ctlListUser.DeleteAllItems();
	for (int i = 0; i < m_name.size(); i++)
	{
		m_ctlListUser.InsertItem(i, m_name[i].c_str());
	}
	CString strData;
	for (int i = 0; i < m_name.size(); i++)
	{
		strData.Format("%s", m_comment[i].c_str());
		m_ctlListUser.SetItemText(i, 2, strData);
	}

	m_ctlListUser.SetSelectedColumn(0);

	UpdateData(FALSE);
	Invalidate(FALSE);

	SetWindowText(filename.c_str());
}

void CwatcherDlg::Draw(int offset)
{
	CRect rcCanvas;
	m_ctlGraph.GetClientRect(&rcCanvas);

	CPaintDC dc(&m_ctlGraph);
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,255));

	int iStartX = 0;
	int iStartIndex = 0;

	int iMidX = rcCanvas.Width() / 2;
	if (offset - iMidX < 0)
	{
		//前面有空白
		iStartX = iMidX - offset;
		iStartIndex = 0;
		RECT rect = rcCanvas;
		rect.right = iStartX;
		dc.FillRect(&rect, &br);
	}
	else
	{
		iStartX = 0;
		iStartIndex = offset - iMidX;
	}

	int iEndX = 0;
	int iEndIndex = 0;

	if (offset + (rcCanvas.Width() - iMidX) > m_data.size())
	{
		//后面有空白
		iEndX = iMidX + (m_data.size() - offset);
		iEndIndex = m_data.size();

		RECT rect = rcCanvas;
		rect.left = iEndX;
		dc.FillRect(&rect, &br);
	}
	else
	{
		iEndX = rcCanvas.Width();
		iEndIndex = offset + (rcCanvas.Width() - iMidX);
	}

	int iCurX, iCurIndex;
	for (iCurX = iStartX, iCurIndex = iStartIndex;
		iCurX < iEndX && iCurIndex < iEndIndex;
		iCurX++, iCurIndex++)
	{
		//当前数据帧不画(反正被黑线挡住了, 防止拖动的时候闪烁)
		if (iCurX == iMidX)
		{
			continue;
		}

		//获取被选中的行序号
		POSITION pos = m_ctlListUser.GetFirstSelectedItemPosition();
		if (NULL != pos)
		{
			//只需要被选中的行对应的用户数据
			while(1)
			{
				int col = m_ctlListUser.GetNextSelectedItem(pos);

				if (col < 0)
				{
					break;
				}

				DrawOneFrame(col, iCurIndex, iCurX, rcCanvas.Height() - 1, dc);

			}
		}
		else
		{
			DrawOneFrame(0, iCurIndex, iCurX, rcCanvas.Height() - 1, dc);
		}
	}

	CPen		m_pen;
	m_pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	dc.SelectObject(&m_pen);
	dc.MoveTo(iMidX, 0);
	dc.LineTo(iMidX, rcCanvas.bottom);

	//用户数据
	CString strData;
	for (int i = 0; i < m_name.size(); i++)
	{
		strData.Format("%s", m_strdata[offset][i].c_str());
		m_ctlListUser.SetItemText(i, 1, strData);
	}

	if (offset >= 0 && offset < m_time.size())
	{
		m_staticDate.SetWindowTextA(m_time[offset].c_str());
	}
}

void CwatcherDlg::ParseLineStr(char * str, std::vector<std::string> & vec)
{
	char tmp[10240] = {0};
	strcpy(tmp, str);
	char * tokenPtr = strtok(tmp, "\t");
	vec.clear();
	while (tokenPtr != NULL)
	{
		vec.push_back(tokenPtr);
		tokenPtr = strtok(NULL, "\t");
	}
}

void CwatcherDlg::ParseLineInt(char * str, std::vector<int> & vec)
{
	char tmp[10240] = {0};
	strcpy(tmp, str);
	char * tokenPtr = strtok(tmp, "\t");
	vec.clear();
	while (tokenPtr != NULL)
	{
		vec.push_back(atoi(tokenPtr));
		tokenPtr = strtok(NULL, "\t");
	}
}

void CwatcherDlg::DrawOneFrame( int iCol, int iIndex, int iPosX, int iMaxY, CPaintDC &dc )
{
	if (iCol < 0 || iCol >= m_name.size())
	{
		return;
	}

	if (iIndex < 0 || iIndex >= m_data.size())
	{
		return;
	}

	dc.MoveTo(iPosX, iMaxY);
	CPen m_pen;
	m_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&m_pen);
	dc.LineTo(iPosX, -1);

	//绘制用户数据, 如果有的话
	int iUserDataOffset = m_dataMax[iCol] - m_dataMin[iCol];
	int iCurY = 0;
	if (iUserDataOffset > 0)
	{
		//因为绘制点为两个象素, 为了防止画到框外面去, 把iMaxY减1
		iCurY = int(iMaxY - (__int64)(iMaxY - 1) * (m_data[iIndex][iCol] - m_dataMin[iCol]) / iUserDataOffset);
	}
	else
	{
		iCurY = iMaxY;
	}
	CRect rc(iPosX - 1, iCurY - 1, iPosX + 1, iCurY + 1);
	CBrush m_br;
	static COLORREF colorvec[6] = {RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(0, 255, 255), RGB(255, 0, 255)};
	COLORREF c = colorvec[iCol % 6];
	m_br.CreateSolidBrush(c);
	dc.FillRect(&rc, &m_br);
}


void CwatcherDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//指针是否落在图表范围内
	CRect rcGraph;
	m_ctlGraph.GetWindowRect(&rcGraph);
	ScreenToClient(&rcGraph);
	if (rcGraph.PtInRect(point))
	{
		m_bLBtnDown = TRUE;
		m_iMouseLastPosX = point.x;
		SetCapture();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CwatcherDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	if (m_bLBtnDown)
	{
		m_bLBtnDown = FALSE;
	}
	
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CwatcherDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bLBtnDown && m_data.size() > 0 
		&& point.x != m_iMouseLastPosX)
	{
		//图表被拖拽了
		int iIndexOffset = point.x - m_iMouseLastPosX;
		int iIndexFrameNew;
		if (m_iCurFrmIdx - iIndexOffset < 0)
		{
			iIndexFrameNew = 0;
		}
		else if (m_iCurFrmIdx - iIndexOffset >= m_data.size())
		{
			iIndexFrameNew = m_data.size() - 1;
		}
		else
		{
			iIndexFrameNew = m_iCurFrmIdx - iIndexOffset;
		}
		m_iMouseLastPosX = point.x;

		if (iIndexFrameNew != m_iCurFrmIdx)
		{
			m_iCurFrmIdx = iIndexFrameNew;
			UpdateData(FALSE);
			Invalidate(FALSE);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}



void CwatcherDlg::OnItemchangedListName(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	Invalidate(FALSE);

	int num = 0;
	POSITION pos = m_ctlListUser.GetFirstSelectedItemPosition();
	if (NULL != pos)
	{
		//只需要被选中的行对应的用户数据
		while(1)
		{
			int col = m_ctlListUser.GetNextSelectedItem(pos);

			if (col < 0)
			{
				break;
			}

			num++;
		}
	}

	if (num == 1)
	{
		m_buttonMin.EnableWindow(TRUE);
		m_buttonMax.EnableWindow(TRUE);
	}
	else
	{
		m_buttonMin.EnableWindow(FALSE);
		m_buttonMax.EnableWindow(FALSE);
	}

}


void CwatcherDlg::OnBnClickedButtonFirst()
{
	// TODO: Add your control notification handler code here
	m_iCurFrmIdx = 0;
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CwatcherDlg::OnBnClickedButtonPre()
{
	// TODO: Add your control notification handler code here
	m_iCurFrmIdx--;
	if (m_iCurFrmIdx < 0)
	{
		m_iCurFrmIdx = 0;
	}
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CwatcherDlg::OnBnClickedButtonNext()
{
	// TODO: Add your control notification handler code here
	m_iCurFrmIdx++;
	if (m_iCurFrmIdx > m_data.size() - 1)
	{
		m_iCurFrmIdx = m_data.size() - 1;
	}
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CwatcherDlg::OnBnClickedButtonLast()
{
	// TODO: Add your control notification handler code here
	m_iCurFrmIdx = m_data.size() - 1;
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CwatcherDlg::OnBnClickedButtonGo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_iCurFrmIdx < 0)
	{
		m_iCurFrmIdx = 0;
	}
	if (m_iCurFrmIdx > m_data.size() - 1)
	{
		m_iCurFrmIdx = m_data.size() - 1;
	}
	UpdateData(FALSE);
	Invalidate(FALSE);
}

void CwatcherDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Stat Files (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString			m_strFile;	
	CFileDialog FileDlg(TRUE, "stat", m_strFile, OFN_FILEMUSTEXIST,	szFilter, this);
	if(FileDlg.DoModal() == IDOK)
	{
		Parse(FileDlg.GetPathName().GetString());
	}
}

void CwatcherDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default

	UINT nFiles = ::DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	if (nFiles <= 0)
	{
		return;
	}
	TCHAR szFileName[_MAX_PATH];
	::DragQueryFile(hDropInfo, 0, szFileName, _MAX_PATH);

	Parse(szFileName);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CwatcherDlg::OnBnClickedButtonMax()
{
	// TODO: Add your control notification handler code here

	int num = 0;
	int index = 0;
	POSITION pos = m_ctlListUser.GetFirstSelectedItemPosition();
	if (NULL != pos)
	{
		//只需要被选中的行对应的用户数据
		while(1)
		{
			int col = m_ctlListUser.GetNextSelectedItem(pos);

			if (col < 0)
			{
				break;
			}
			index = col;
			num++;
		}
	}
	if (index < 0 || index >= m_dataMinIndex.size())
	{
		return;
	}
	m_iCurFrmIdx = m_dataMaxIndex[index];
	if (m_iCurFrmIdx < 0)
	{
		m_iCurFrmIdx = 0;
	}
	if (m_iCurFrmIdx > m_data.size() - 1)
	{
		m_iCurFrmIdx = m_data.size() - 1;
	}
	UpdateData(FALSE);
	Invalidate(FALSE);
}


void CwatcherDlg::OnBnClickedButtonMin()
{
	// TODO: Add your control notification handler code here

	int num = 0;
	int index = 0;
	POSITION pos = m_ctlListUser.GetFirstSelectedItemPosition();
	if (NULL != pos)
	{
		//只需要被选中的行对应的用户数据
		while(1)
		{
			int col = m_ctlListUser.GetNextSelectedItem(pos);

			if (col < 0)
			{
				break;
			}
			index = col;
			num++;
		}
	}
	if (index < 0 || index >= m_dataMinIndex.size())
	{
		return;
	}
	m_iCurFrmIdx = m_dataMinIndex[index];
	if (m_iCurFrmIdx < 0)
	{
		m_iCurFrmIdx = 0;
	}
	if (m_iCurFrmIdx > m_data.size() - 1)
	{
		m_iCurFrmIdx = m_data.size() - 1;
	}
	UpdateData(FALSE);
	Invalidate(FALSE);
}

bool CwatcherDlg::ParseText( std::string name )
{
	std::ifstream ifs;
	ifs.open(name);

	if (!ifs.is_open())
	{
		return false;
	}

	int colnum = 0;
	bool ini = false;

	while (!ifs.eof())
	{
		char buff[10240] = {0};
		ifs.getline(buff, 10240);

		if (IsTitle(buff))
		{
			std::vector<int> tmp;
			std::vector<std::string> tmpstr;
			while (!ifs.eof())
			{
				char databuff[10240] = {0};
				ifs.getline(databuff, 10240);

				if (IsEnd(databuff))
				{
					break;
				}

				std::string name;
				int data;
				std::string datastring;
				std::string comment;
				ParseLine(databuff, name, data, datastring, comment);

				if (!ini)
				{
					m_name.push_back(name);
					m_comment.push_back(comment);
				}
				tmp.push_back(data);
				tmpstr.push_back(datastring);
			}
			m_time.push_back(GetTime(buff));
			m_data.push_back(tmp);
			m_strdata.push_back(tmpstr);
			ini = true;
		}
	}
	return true;
}

bool CwatcherDlg::IsTitle( std::string str )
{
	if (!str.empty())
	{
		if (str[0] == '=')
		{
			return true;
		}
	}
	return false;
}

std::string CwatcherDlg::GetTime( std::string str )
{
	CString ss(str.c_str());
	ss = ss.Trim('=');
	return ss.GetString();
}

bool CwatcherDlg::IsEnd( std::string str )
{
	return str.empty();
}

void CwatcherDlg::ParseLine( std::string str, std::string & name, int & data, std::string & datastring, std::string & comment )
{
	char tmp[10240] = {0};
	strcpy(tmp, str.c_str());

	char * tokenPtr = strtok(tmp, "\t");
	if (!tokenPtr)
	{
		return;
	}

	name = tokenPtr;
	tokenPtr = strtok(NULL, "\t");
	if (!tokenPtr)
	{
		return;
	}

	datastring = tokenPtr;
	data = atoi(tokenPtr);
	tokenPtr = strtok(NULL, "\t");
	if (!tokenPtr)
	{
		return;
	}

	comment = tokenPtr;
	tokenPtr = strtok(NULL, "\t");
	if (!tokenPtr)
	{
		return;
	}
}
