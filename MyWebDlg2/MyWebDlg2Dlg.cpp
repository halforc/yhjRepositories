
// MyWebDlg2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyWebDlg2.h"
#include "MyWebDlg2Dlg.h"
#include "afxdialogex.h"
#include "ExDispID.h"
#include "afxinet.h"//用到WinInet类
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


// CMyWebDlg2Dlg 对话框
BEGIN_EVENTSINK_MAP(CMyWebDlg2Dlg, CDHtmlDialog)
	ON_EVENT(CMyWebDlg2Dlg, IDD_MYWEBDLG2_DIALOG, DISPID_STATUSTEXTCHANGE, StatusTextChange, VTS_BSTR)
#ifdef _ONNEWWINDOW2
	ON_EVENT(CMyWebDlg2Dlg, IDD_MYWEBDLG2_DIALOG, DISPID_NEWWINDOW2, OnNewWindow2, VTS_DISPATCH VTS_PBOOL)
#else
	ON_EVENT(CMyWebDlg2Dlg, IDD_MYWEBDLG2_DIALOG, DISPID_NEWWINDOW3, OnNewWindow3, VTS_PDISPATCH VTS_PBOOL VTS_I4 VTS_BSTR VTS_BSTR)
#endif
	//ON_EVENT(CMyWebDlg2Dlg, IDD_MYWEBDLG2_DIALOG, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CMyWebDlg2Dlg, IDD_MYWEBDLG2_DIALOG, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

BEGIN_DHTML_EVENT_MAP(CMyWebDlg2Dlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
	DHTML_EVENT_ONCLICK(_T("TestFunc"), TestFunc)
END_DHTML_EVENT_MAP()

BEGIN_DISPATCH_MAP(CMyWebDlg2Dlg, CDHtmlDialog)
	DISP_FUNCTION(CMyWebDlg2Dlg, "TestFunc", Func1, VT_EMPTY, VTS_VARIANT VTS_VARIANT)
	DISP_FUNCTION_ID(CMyWebDlg2Dlg, "HTMLELEMENTEVENTS2_ONCLICK", DISPID_HTMLELEMENTEVENTS2_ONCLICK,CMyWebDlg2Dlg::OnClick, VT_EMPTY, VTS_DISPATCH)
END_DISPATCH_MAP()


CMyWebDlg2Dlg::CMyWebDlg2Dlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CMyWebDlg2Dlg::IDD, CMyWebDlg2Dlg::IDH, pParent)
	, m_strShow(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyWebDlg2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyWebDlg2Dlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	//ON_BN_CLICKED(IDC_BUTTON2, &CMyWebDlg2Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_GOBACK, &CMyWebDlg2Dlg::OnBnClickedGoback)
	ON_BN_CLICKED(IDC_FORWARD, &CMyWebDlg2Dlg::OnBnClickedForward)
	ON_BN_CLICKED(IDC_ADDRNAVIGATE, &CMyWebDlg2Dlg::OnBnClickedAddrnavigate)
END_MESSAGE_MAP()


// CMyWebDlg2Dlg 消息处理程序

BOOL CMyWebDlg2Dlg::OnInitDialog()
{
	SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR | DOCHOSTUIFLAG_NO3DBORDER);
	CDHtmlDialog::OnInitDialog();

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
	CRect rect;
	GetDlgItem(IDC_BROWSERAREA)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_pBrowserApp->put_Top(rect.top);
	m_pBrowserApp->put_Left(rect.left);
	m_pBrowserApp->put_Width(rect.Width());
	m_pBrowserApp->put_Height(rect.Height());

	//m_pBrowserApp->put_MenuBar(VARIANT_TRUE);
	
	//开启自动化
	EnableAutomation();
	//将接口展示给JS
	SetExternalDispatch(GetIDispatch(TRUE));
	Navigate(_T("http://search.cnki.net/kns/index.html?code=CJFX"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyWebDlg2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CMyWebDlg2Dlg::CreateControlSite(COleControlContainer* pContainer,
	COleControlSite** ppSite, UINT /* nID */, REFCLSID /* clsid */)
{
	return TRUE;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyWebDlg2Dlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyWebDlg2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CMyWebDlg2Dlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CMyWebDlg2Dlg::TestFunc(IHTMLElement* /*pElement*/)
{

	return S_OK;
}

HRESULT CMyWebDlg2Dlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}

void CMyWebDlg2Dlg::Func1(VARIANT& vStr1, VARIANT& vStr2){
	CComVariant varStr1(vStr1), varStr2(vStr2);
	varStr1.ChangeType(VT_BSTR);
	varStr2.ChangeType(VT_BSTR);
	USES_CONVERSION;
	CString strMsg;
	strMsg.Format(_T("varStr1:%s,varStr2:%s"), OLE2T(varStr1.bstrVal), OLE2T(varStr2.bstrVal));
	AfxMessageBox(strMsg);
}

void CMyWebDlg2Dlg::OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel)
{
	TRACE("OnBeforeNavigate2\n");
	Flags; // unused
	TargetFrameName; // unused
	PostData; // unused
	Headers; // unused
	//Cancel; // unused
	//*Cancel = TRUE; //这里非零就可以阻止进一步操作

	CString str(V_BSTR(URL));
	//CString strTemp = str.Left(str.Find(_T("?")));
	//TRACE(strTemp+_T("\n"));
	//if (strTemp == _T("http://search.cnki.net/kns/detail/detail.aspx")){
	//	CString strNew = GetData(str);
	//	if (strNew != m_strShow){
	//		SetDlgItemText(IDC_EDIT1, str);
	//	}
	//}
	OnBeforeNavigate(pDisp, str);
}


//void CMyWebDlg2Dlg::OnNavigateComplete2(LPDISPATCH pDisp, VARIANT FAR* vtURL)
//{
//	TRACE("OnNavigateComplete2\n");
//}

#ifdef _ONNEWWINDOW2
void CMyWebDlg2Dlg::OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel)
{
	CComPtr<IHTMLDocument2> pHTMLDocument2;

	m_pBrowserApp->get_Document((IDispatch **)&pHTMLDocument2);
	if (pHTMLDocument2 != NULL)
	{
		CComPtr<IHTMLElement> pIHTMLElement;
		pHTMLDocument2->get_activeElement(&pIHTMLElement);
		
		if (pIHTMLElement != NULL)
		{
			variant_t url;
			BSTR s[10];
			pIHTMLElement->get_outerText(&s[0]);
			pIHTMLElement->get_innerHTML(&s[1]);
			m_pBrowserApp->get_LocationURL(&s[2]);
			pIHTMLElement->get_className(&s[3]);
			pIHTMLElement->get_id(&s[4]);
			pIHTMLElement->get_innerText(&s[5]);
			pIHTMLElement->get_lang(&s[6]);
			pIHTMLElement->get_outerHTML(&s[7]);
			pIHTMLElement->get_tagName(&s[8]);
			pIHTMLElement->get_title(&s[9]);
			HRESULT hr = pIHTMLElement->getAttribute(L"href", 0, &url);
			if (SUCCEEDED(hr))
			{
				hr = m_pBrowserApp->Navigate2(&url, NULL, NULL, NULL, NULL);

				url.Clear();

				if (SUCCEEDED(hr))
				{
					*Cancel = TRUE;
				}
			}
		}
	}
	TRACE("OnNewWindow2\n");
}
#else
void CMyWebDlg2Dlg::OnNewWindow3(LPDISPATCH* ppDisp, BOOL* Cancel, long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl)
{
	TRACE("OnNewWindow3\n");
	*Cancel = TRUE;
	CString str(bstrUrl);
	GetHttpFileData(str);
	CString strTemp = str.Left(str.Find(_T("?")));
	if (strTemp == _T("http://search.cnki.net/kns/detail/detail.aspx")){
		//CString strNew = GetData(str);
		//if (strNew != m_strShow){
		//	SetDlgItemText(IDC_EDIT1, strNew);
		//}
	}
	else{
	Navigate(bstrUrl);
	}
}
#endif

CString CMyWebDlg2Dlg::GetHttpFileData(CString strUrl)
{
	CInternetSession session;
	CHttpFile *file = NULL;
	CString strHtml = _T(""); //存放网页数据  

	try{
		file = (CHttpFile*)session.OpenURL(strUrl);
	}
	catch (CInternetException * m_pException){
		file = NULL;
		m_pException->m_dwError;
		m_pException->Delete();
		session.Close();
		MessageBox(_T("CInternetException"));
	}
	CString strLine;
	if (file != NULL){
		while (file->ReadString(strLine) != NULL){
			strHtml += strLine;
		}
	}
	else{
		MessageBox(_T("fail"));
	}
	session.Close();
	file->Close();
	delete file;
	file = NULL;
	return strHtml;
}

void CMyWebDlg2Dlg::OnBnClickedGoback()
{
	// TODO:  在此添加控件通知处理程序代码
	if (nullptr != m_pBrowserApp){
		m_pBrowserApp->GoBack();
	}
}


void CMyWebDlg2Dlg::OnBnClickedForward()
{
	// TODO:  在此添加控件通知处理程序代码
	if (nullptr != m_pBrowserApp){
		m_pBrowserApp->GoForward();
	}
}

//屏蔽脚本错误
void CMyWebDlg2Dlg::SuppressScriptError()
{
	// 要执行的屏蔽报错脚本  
	CString strScript = _T("window.onerror=function myonerror(){return true}");
	CString strLanguage("JavaScript");
	ExecuteScript(strScript, strLanguage);
}

void CMyWebDlg2Dlg::ExecuteScript(CString &strScript, CString &strLanguage)
{
	IHTMLDocument2* pIHtmlDoc = NULL;
	GetDHtmlDocument(&pIHtmlDoc);
	if (!pIHtmlDoc) return;

	IHTMLWindow2* pIhtmlwindow2 = NULL;
	pIHtmlDoc->get_parentWindow(&pIhtmlwindow2);
	if (!pIhtmlwindow2) return;

	BSTR bstrScript = strScript.AllocSysString();
	BSTR bstrLanguage = strLanguage.AllocSysString();
	VARIANT pRet;

	// 注入脚本到当前页面  
	pIhtmlwindow2->execScript(bstrScript, bstrLanguage, &pRet);
	::SysFreeString(bstrScript);
	::SysFreeString(bstrLanguage);
	pIhtmlwindow2->Release();
}

void CMyWebDlg2Dlg::OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl)
{
	CDHtmlDialog::OnNavigateComplete(pDisp, szUrl);
	SuppressScriptError();
	// TODO:  在此添加专用代码和/或调用基类
}

void CMyWebDlg2Dlg::StatusTextChange(BSTR str)
{
/*	CString str1((LPCTSTR)str);
	CString strTemp = str1.Left(str1.Find(_T("?")));
	TRACE(strTemp+_T("\n"));
	if (strTemp == _T("http://search.cnki.net/kns/detail/detail.aspx")){
		CString strNew = GetData(str1);
		if (strNew != m_strShow){
			SetDlgItemText(IDC_EDIT1, str1);
		}	
	}*/	
}

CString CMyWebDlg2Dlg::getNodeElementData(CString strHtml,CString nodeName)
{
	return nodeName;
}

CString CMyWebDlg2Dlg::GetData(LPCTSTR URL)
{
	CString strText = _T("");
	IHTMLDocument2* pHtmlDoc = (IHTMLDocument2*)m_spHtmlDoc;
	if (nullptr != pHtmlDoc){
		CComPtr<IHTMLDocument3> pDocument3 = NULL;
		pHtmlDoc->QueryInterface(IID_IHTMLDocument3, (void**)&pDocument3);
		CComPtr<IHTMLElement> tagElement;
		pDocument3->get_documentElement(&tagElement);
		CComBSTR bstr;
		tagElement->get_innerHTML(&bstr);
		strText = bstr;
		int nStart = strText.Find(_T("name=\"ChDivSummary\">")) + 2;
		int nEnd = strText.Find(_T("< / span><span><a id = \"ChDivSummaryMore")) + 2;
		strText = strText.Mid(nStart, nEnd - nStart);
		//隐藏指定ID  
		//str.Replace(_T("#head{"), _T("#head{display:none;"));
		//tagElement->put_innerHTML(CComBSTR(str));
	}
	return strText;
}

void CMyWebDlg2Dlg::OnClick(MSHTML::IHTMLEventObj *pEvtObj)
{
	TRACE("OnClick\n");
}

void CMyWebDlg2Dlg::OnBnClickedAddrnavigate()
{
	// TODO:  在此添加控件通知处理程序代码
	CString strAddr(_T(""));
	GetDlgItemText(IDC_EDITADDRESS, strAddr);
	if (_T("") != strAddr){
		Navigate(strAddr);
	}
}
