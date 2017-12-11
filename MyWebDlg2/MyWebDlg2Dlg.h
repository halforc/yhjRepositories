
// MyWebDlg2Dlg.h : ͷ�ļ�
//

#pragma once
#include "explorer1.h"
#include <atlbase.h>
#include <Mshtml.h>

#import "C:\windows\system32\mshtml.tlb" // location of mshtml.tlb  
// CMyWebDlg2Dlg �Ի���
class CMyWebDlg2Dlg : public CDHtmlDialog
{
// ����
public:
	CMyWebDlg2Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYWEBDLG2_DIALOG, IDH = IDR_HTML_MYWEBDLG2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	BOOL CreateControlSite(COleControlContainer* pContainer,
		COleControlSite** ppSite, UINT /* nID */, REFCLSID /* clsid */);
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);
	HRESULT TestFunc(IHTMLElement *pElement);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_EVENTSINK_MAP()
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	void Func1(VARIANT& vStr1, VARIANT& vStr2);
public:
	void OnClick(MSHTML::IHTMLEventObj *pEvtObj);//link ����¼�������
	void OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	//void OnNavigateComplete2(LPDISPATCH pDisp, VARIANT FAR* vtURL);
#ifdef _ONNEWWINDOW2
	void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
#else
	void OnNewWindow3(LPDISPATCH* ppDisp, BOOL* Cancel, long dwFlags, LPCTSTR bstrUrlContext, LPCTSTR bstrUrl);
#endif
	afx_msg void OnBnClickedGoback();
	afx_msg void OnBnClickedForward();
	void SuppressScriptError();
	void ExecuteScript(CString &strScript, CString &strLanguage);
	virtual void OnNavigateComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
	void StatusTextChange(BSTR str);
	CString GetHttpFileData(CString strUrl);

	CString GetData(LPCTSTR URL);
	CString getNodeElementData(CString strHtml,CString nodeName);

private:
	CString m_strShow;
public:
	afx_msg void OnBnClickedAddrnavigate();
};
