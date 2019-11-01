// HelloDuilib.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HelloDuilibWnd.h"
#include "HelloDuilib.h"

#define WM_ADDLISTITEM WM_USER + 50
/*
* 存放第二列数据
*/
std::vector<std::string> ClothesName;
std::vector<std::string> ClothesSize;
std::vector<std::string> ClothesLink;
std::vector<std::string> ClothesType;
std::vector<std::string> ClothesMaterial;
struct Prama
{
	HWND hWnd;
	CListUI* pList;
};
class CHelloDuilibWnd :
	public CWindowWnd, public INotifyUI
{
public:

	CPaintManagerUI m_pm;
	virtual void    Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("windowinit")){
		}
		else if (msg.sType == _T("click")){
			if (msg.pSender == m_pCloseBtn)
			{
				PostQuitMessage(0);
				return;
			}
			else if (msg.pSender == m_pMinBtn)
			{
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
				return;
			}
		}
	}
	LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
		CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("clotheslist")));
		if (pList) pList->Add(pListElement);
		return 0;
	}
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MONITORINFO oMonitor = {};
		oMonitor.cbSize = sizeof(oMonitor);
		::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
		CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMaxPosition.x = rcWork.left;
		lpMMI->ptMaxPosition.y = rcWork.top;
		lpMMI->ptMaxSize.x = rcWork.right;
		lpMMI->ptMaxSize.y = rcWork.bottom;

		bHandled = FALSE;
		return 0;
	}
	void Init()
	{
		m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
		m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
	}
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
		if (wParam == SC_CLOSE) {
			::PostQuitMessage(0L);
			bHandled = TRUE;
			return 0;
		}
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if (::IsZoomed(*this) != bZoomed) {
			if (!bZoomed) {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if (pControl) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if (pControl) pControl->SetVisible(true);
			}
			else {
				CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
				if (pControl) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
				if (pControl) pControl->SetVisible(false);
			}
		}
		return lRes;
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		if (uMsg == WM_CREATE)
		{


			LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
			styleValue &= ~WS_CAPTION;
			::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
			m_pm.Init(m_hWnd);
			//m_pm.SetTransparent(100);
			CDialogBuilder builder;
			CControlUI* pRoot = builder.Create(_T("HelloDuilib.xml"), (UINT)0, NULL, &m_pm);
			ASSERT(pRoot && "Failed to parse XML");
			m_pm.AttachDialog(pRoot);
			m_pm.AddNotifier(this);
			Init();
			HRESULT err = ::CoInitialize(NULL);
			if (FAILED(err))
			{
				return __super::HandleMessage(uMsg, wParam, lParam);
			}
			//连接到MS SQL Server 
			/*	_ConnectionPtr pMyConnect = NULL;
			HRESULT hr = pMyConnect.CreateInstance(__uuidof(Connection));
			if (FAILED(hr))
			return __super::HandleMessage(uMsg, wParam, lParam);*/
			//初始化链接参数 
			_bstr_t strConnect = "Provider=SQLNCLI11.1;Persist Security Info=False;User ID=sa;Initial Catalog=mfsDemo;Data Source=localhost;Password=lzt@123456;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Workstation ID=LAPTOP-RMBNQS92;Initial File Name=\"\";Use Encryption for Data=False;Tag with column collation when possible=False;MARS Connection=False;DataTypeCompatibility=0;Trust Server Certificate=False;Application Intent=READWRITE"; //密码

			////执行连接 
			//try
			//{
			//	// Open方法连接字串必须四BSTR或者_bstr_t类型 
			//	pMyConnect->Open(strConnect, "", "", NULL);
			//}
			//catch (_com_error &e)
			//{
			//	::MessageBox(NULL, _T("连接数据库失败"), _T("错误"), NULL);
			//}
			_ConnectionPtr m_pConnection = NULL;
			m_pConnection.CreateInstance(_uuidof(Connection));
			// Open方法连接字串必须四BSTR或者_bstr_t类型 
			m_pConnection->Open(strConnect, "", "", NULL);
			_RecordsetPtr m_pRecordset;
			//初始化Connection指针 
			m_pRecordset.CreateInstance(__uuidof(Recordset));
			//初始化Recordset指针 
			_bstr_t strSql = "select * from F_Clothes";//具体执行的SQL语句
			try
			{
				m_pRecordset = m_pConnection->Execute(_bstr_t(strSql), NULL, adCmdText);//将查询数据导入m_pRecordset数据容器
			}
			catch (_com_error &e)
			{
				::MessageBox(NULL, _T("查询数据库失败"), _T("错误"), NULL);
			}
			int i = 1;
			CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("clotheslist")));
			CDuiString str;
			while (!m_pRecordset->adoEOF)//遍历并读取name列的记录并输出 
			{
				CListTextElementUI* pListElement = new CListTextElementUI;
			
				_bstr_t temp1 = (TCHAR *)(_bstr_t)m_pRecordset->GetFields()->GetItem("F_ClothesName")->Value;
				/*::MessageBox(NULL, temp, NULL, NULL);*/
				ClothesName.push_back((char *)temp1);

				_bstr_t temp2 = (TCHAR *)(_bstr_t)m_pRecordset->GetFields()->GetItem("F_ClothesSize")->Value;
				/*::MessageBox(NULL, temp, NULL, NULL);*/
				ClothesSize.push_back((char *)temp2);

				_bstr_t temp3 = (TCHAR *)(_bstr_t)m_pRecordset->GetFields()->GetItem("F_ClothesLink")->Value;
				/*::MessageBox(NULL, temp, NULL, NULL);*/
				ClothesLink.push_back((char *)temp3);

				_bstr_t temp4 = (TCHAR *)(_bstr_t)m_pRecordset->GetFields()->GetItem("F_ClothesType")->Value;
				/*::MessageBox(NULL, temp, NULL, NULL);*/
				ClothesType.push_back((char *)temp4);

				_bstr_t temp5 = (TCHAR *)(_bstr_t)m_pRecordset->GetFields()->GetItem("F_ClothesMaterial")->Value;
				/*::MessageBox(NULL, temp, NULL, NULL);*/
				ClothesMaterial.push_back((char *)temp5);

				pListElement->SetTag(i);

				if (pListElement != NULL)
				{
			     	if (pList){ 
						pList->Add(pListElement); 
						str.Format(_T("%d"), i);
						pListElement->SetText(0, _bstr_t(i));
						pListElement->SetText(1, temp1);
						pListElement->SetText(2, temp2);
						pListElement->SetText(3, temp3);
						pListElement->SetText(4, temp4);
						pListElement->SetText(5, temp5);
					}
					/*::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);*/
				}
				i++;
				m_pRecordset->MoveNext();
			}

		}
		else if (uMsg == WM_ADDLISTITEM){
			lRes = OnAddListItem(uMsg, wParam, lParam, bHandled);
		}
		else if (uMsg == WM_CLOSE){
			lRes = OnClose(uMsg, wParam, lParam, bHandled); 
		}
		else if (uMsg == WM_GETMINMAXINFO){
			lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); 
		}
		else if (uMsg == WM_SYSCOMMAND){
			lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
		}



		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return __super::HandleMessage(uMsg, wParam, lParam);
	}
protected:
	/*virtual CDuiString GetSkinFolder() { return _T("skin"); };*/
	//virtual CDuiString GetSkinFile()  { return _T("HelloDuilib.xml"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T("HelloDuilib_Wnd"); };

private:
	CButtonUI* m_pMinBtn;
	CButtonUI* m_pCloseBtn;
};





int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+"/skin/");   // 设置资源的默认路径（此处设置为和exe在同一目录）
	CHelloDuilibWnd wnd;
	wnd.Create(NULL, _T("CHelloDuilibWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	wnd.CenterWindow();
	wnd.ShowModal();

	CPaintManagerUI::MessageLoop();

	/*delete wnd;*/

	return 0;
}