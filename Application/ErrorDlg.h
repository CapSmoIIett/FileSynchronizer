#pragma once


// Диалоговое окно ErrorDlg

class ErrorDlg : public CDialogEx
{
	public:
		ErrorDlg(CString text, CWnd* pParent = nullptr);   // стандартный конструктор
		virtual ~ErrorDlg();

	// Данные диалогового окна
	#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_ERROR };
	#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
		virtual BOOL OnInitDialog();


		DECLARE_MESSAGE_MAP()


		CPngImage image;
		CString ErrorMessage;
		CStatic Picture;
		HICON m_hIcon;

	private:
		DECLARE_DYNAMIC(ErrorDlg)

	
};
