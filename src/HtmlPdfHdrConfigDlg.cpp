//
//////////////////////////////////////////////////////////////////
//
//  Windows Mbox Viewer is a free tool to view, search and print mbox mail archives.
//
// Source code and executable can be downloaded from
//  https://sourceforge.net/projects/mbox-viewer/  and
//  https://github.com/eneam/mboxviewer
//
//  Copyright(C) 2019  Enea Mansutti, Zbigniew Minciel
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the version 3 of GNU Affero General Public License
//  as published by the Free Software Foundation; 
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this program; if not, write to the
//  Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
//  Boston, MA  02110 - 1301, USA.
//
//////////////////////////////////////////////////////////////////
//

// HtmlPdfHdrConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileUtils.h"
#include "HtmlPdfHdrConfigDlg.h"
#include "GenericFontFamilyDlg.h"
#include "afxdialogex.h"


// HtmlPdfHdrConfigDlg dialog

IMPLEMENT_DYNAMIC(HtmlPdfHdrConfigDlg, CDialogEx)

extern const TCHAR *sz_Software_mboxview;

HtmlPdfHdrConfigDlg::HtmlPdfHdrConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HTML_PDF_HDR_DLG, pParent)
{
	m_bPickFamilyFont = 0;
	m_bPickConcreteFont = 0;

	m_HdrFldConfig.LoadFromRegistry();
}

HtmlPdfHdrConfigDlg::~HtmlPdfHdrConfigDlg()
{
}

void HtmlPdfHdrConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_fldListBox);
	//DDX_Radio(pDX, IDC_PICK_FAMILY_FONT, m_bPickFamilyFont);
	DDX_Radio(pDX, IDC_PICK_CONCRETE_FONT, m_bPickConcreteFont);
	DDX_Radio(pDX, IDC_FONT_DFLT, m_HdrFldConfig.m_bHdrFontDflt);
	DDX_Radio(pDX, IDC_HDR_FLD_NAME, m_HdrFldConfig.m_bHdrFldCustomNameFont);
	DDX_Check(pDX, IDC_FLD_NAME_BOLD, m_HdrFldConfig.m_bHdrBoldFldName);
	DDX_Text(pDX, IDC_FLD_SIZE, m_HdrFldConfig.m_nHdrFontSize);
}


BEGIN_MESSAGE_MAP(HtmlPdfHdrConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &HtmlPdfHdrConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PICK_CONCRETE_FONT, &HtmlPdfHdrConfigDlg::OnBnClickedPickConcreteFont)
	ON_BN_CLICKED(IDC_FLD_NAME_BOLD, &HtmlPdfHdrConfigDlg::OnBnClickedFldNameBold)
	ON_EN_CHANGE(IDC_FLD_SIZE, &HtmlPdfHdrConfigDlg::OnEnChangeFldSize)
	ON_BN_CLICKED(IDC_PICK_FAMILY_FONT, &HtmlPdfHdrConfigDlg::OnBnClickedPickFamilyFont)
	ON_BN_CLICKED(IDC_FONT_DFLT, &HtmlPdfHdrConfigDlg::OnBnClickedFontDflt)
	ON_BN_CLICKED(IDC_FONT_CUSTOM, &HtmlPdfHdrConfigDlg::OnBnClickedFontCustom)
	ON_BN_CLICKED(IDC_HDR_FLD_HELP, &HtmlPdfHdrConfigDlg::OnBnClickedHdrFldHelp)
END_MESSAGE_MAP()


// HtmlPdfHdrConfigDlg message handlers

int HtmlPdfHdrConfigDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return 0;
}

void HtmlPdfHdrConfigDlg::LoadData()
{
	m_fldListBox.AddString(_T("SUBJECT"));
	m_fldListBox.AddString(_T("FROM"));
	m_fldListBox.AddString(_T("TO"));
	m_fldListBox.AddString(_T("CC"));
	m_fldListBox.AddString(_T("BCC"));
	m_fldListBox.AddString(_T("DATE"));
	m_fldListBox.AddString(_T("ATTACHMENTS"));

	int i;
	for (i = 0; i < m_fldListBox.GetCount(); i++)
	{
		if (m_HdrFldConfig.m_HdrFldList.IsFldSet(i))
			m_fldListBox.SetCheck(i, BST_CHECKED);
		else
			m_fldListBox.SetCheck(i, BST_UNCHECKED);
	}
}

BOOL HtmlPdfHdrConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_fldListBox.SetFont(GetFont());

	CString m_title = _T("HTML/PDF Mail Header Configuration");
	SetWindowText(m_title);

	LoadData();

	if (m_HdrFldConfig.m_bHdrFontDflt == 0)
		OnBnClickedFontDflt();
	else
		OnBnClickedFontCustom();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void HtmlPdfHdrConfigDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CString errorTxt;
	CString txt;
	if ((m_HdrFldConfig.m_nHdrFontSize <= 0) || (m_HdrFldConfig.m_nHdrFontSize > 72))
	{
		txt.Format(_T("Default Font:  Invalid Font Size of %d! Valid Size is >0 and <72\n"), m_HdrFldConfig.m_nHdrFontSize);
		errorTxt.Append(txt);
	}

	if (m_HdrFldConfig.m_HdrFldFontName.m_fontName.IsEmpty())
	{
		errorTxt.Append(_T("Field Name Font:  Empty!\n"));
	}

	if (m_HdrFldConfig.m_HdrFldFontName.m_fontStyleName.IsEmpty())
	{
		errorTxt.Append(_T("Field Name Font Style:  Empty!\n"));
	}

	if ((m_HdrFldConfig.m_HdrFldFontName.m_nFontSize <= 0) || (m_HdrFldConfig.m_HdrFldFontName.m_nFontSize > 72))
	{
		txt.Format(_T("Field Name Font Size:  Invalid Font Size of %d! Valid Size is >0 and <=72\n"), m_HdrFldConfig.m_HdrFldFontName.m_nFontSize);
		errorTxt.Append(txt);
	}

	if (m_HdrFldConfig.m_HdrFldFontText.m_fontName.IsEmpty())
	{
		errorTxt.Append(_T("Field Text Font:  Empty!\n"));
	}

	if (m_HdrFldConfig.m_HdrFldFontText.m_fontStyleName.IsEmpty())
	{
		errorTxt.Append(_T("Field Text Font Style:  Empty!\n"));
	}

	if ((m_HdrFldConfig.m_HdrFldFontText.m_nFontSize <= 0) || (m_HdrFldConfig.m_HdrFldFontText.m_nFontSize > 72))
	{
		txt.Format(_T("Field Text Font Size:  Invalid Font Size of %d! Valid Size is >0 and <=72\n"), m_HdrFldConfig.m_HdrFldFontText.m_nFontSize);
		errorTxt.Append(txt);
	}

	if (!errorTxt.IsEmpty())
	{
		int answer = MessageBox(errorTxt, _T("Error"), MB_APPLMODAL | MB_ICONERROR | MB_OK);
		return;
	}

	m_HdrFldConfig.m_HdrFldList.ClearFldBitmap();

	int i;
	for (i = 0; i < m_fldListBox.GetCount(); i++)
	{
		int chk = m_fldListBox.GetCheck(i);
		if (chk)
			m_HdrFldConfig.m_HdrFldList.SetFld(i);
	}

	m_HdrFldConfig.SaveToRegistry();

	CDialogEx::OnOK();
}

///

HdrFldConfig::HdrFldConfig() :
	m_HdrFldFontName(CString(_T("hdrFldName_"))), 
	m_HdrFldFontText(CString(_T("hdrFldText_")))
{
	m_bHdrFldCustomNameFont = 0;
	m_bHdrFontDflt = 0;
	m_nHdrFontSize = 20;
	m_bHdrBoldFldName = FALSE;
}

////

HdrFldList::HdrFldList()
{
}
HdrFldList::~HdrFldList()
{
}

void HdrFldList::ClearFldBitmap()
{
	m_nFldBitmap = 0x0;
}

void HdrFldList::LoadFldBitmap()
{
	BOOL retval;
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, _T("hdrFldBitmap"), m_nFldBitmap))
	{
		; //all done
	}
	else
	{
		m_nFldBitmap = 0xFFFFFFFF;
		CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, _T("hdrFldBitmap"), m_nFldBitmap);
	}
}

void HdrFldList::LoadFromRegistry()
{
	LoadFldBitmap();
}

void HdrFldList::SaveToRegistry()
{
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, _T("hdrFldBitmap"), m_nFldBitmap);
}

BOOL HdrFldList::IsFldSet(int fldNumber)
{
	DWORD position = 1 << fldNumber;
	if (m_nFldBitmap & position)
		return TRUE;
	else
		return FALSE;
}

void HdrFldList::ClearFld(int fldNumber)
{
	DWORD position = 1 << fldNumber;
	m_nFldBitmap &= ~position;
}

void HdrFldList::SetFld(int fldNumber)
{
	DWORD position = 1 << fldNumber;
	m_nFldBitmap |= position;
}

void HtmlPdfHdrConfigDlg::OnBnClickedPickConcreteFont()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CWnd *p = GetDlgItem(IDC_PICK_CONCRETE_FONT);
	if (p)
	{
		BOOL chk = ((CButton*)p)->GetCheck();
		if (chk == FALSE)
		{
			return;
		}
	}

	LPLOGFONT lplfInitial = NULL;
	DWORD dwFlags = CF_PRINTERFONTS | CF_SCREENFONTS | CF_NOSCRIPTSEL | CF_USESTYLE;
	//DWORD dwFlags = CF_PRINTERFONTS | CF_SCREENFONTS | CF_NOSCRIPTSEL;
	CDC* pdcPrinter = NULL;
	CWnd* pParentWnd = NULL;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	HdrFldFont *pFont = &m_HdrFldConfig.m_HdrFldFontName;
	if (m_HdrFldConfig.m_bHdrFldCustomNameFont)
		pFont = &m_HdrFldConfig.m_HdrFldFontText;

	CClientDC dc(this); // expects a CWnd that has already been initialized
	int fontSize = pFont->m_nFontSize;
	lf.lfHeight = -MulDiv(fontSize, dc.GetDeviceCaps(LOGPIXELSY), 72);

	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, pFont->m_fontName);

	LOGFONT *plf = &lf;

	CFontDialog fdlg(plf, dwFlags, pdcPrinter, pParentWnd);

	_tcsncpy(fdlg.m_cf.lpszStyle, pFont->m_fontStyleName, 64);
	fdlg.m_cf.lpszStyle[63] = 0;

	INT_PTR ret = fdlg.DoModal();
	if (ret == IDOK)
	{
		CString fn = fdlg.GetFaceName();
		CString sn = fdlg.GetStyleName();

		// GetFaceName() is not always returning the font selected in the font dialog.
		// Frequently the font and the font style names are concatenated
		// Until I find the better solution, try below hack
		CString faceNameRoot;
		if (fn.GetLength() > sn.GetLength())
		{
			int len = fn.GetLength() - sn.GetLength();
			faceNameRoot = fn.Mid(len);
			if (faceNameRoot.CompareNoCase(sn) == 0)
			{
				CString rootFont = fn.Left(len);
				fn = rootFont;
				fn.Trim();
				int deb = 1;
			}
		}


		int fs = fdlg.GetSize();
		int fw = fdlg.GetWeight();
		BOOL isBold = fdlg.IsBold();
		BOOL isItalic = fdlg.IsItalic();

		LOGFONT lf;
		fdlg.GetCurrentFont(&lf);
		//
		CHARFORMAT cf;
		cf.cbSize = sizeof(cf);
		fdlg.GetCharFormat(cf);

		pFont->m_fontName = fn;
		pFont->m_fontStyleName = sn;
		pFont->m_nFontStyle = fw;
		pFont->m_bIsBold = isBold;
		pFont->m_bIsItalic = isItalic;
		pFont->m_nFontSize = fs/10;
		if ((pFont->m_nFontSize <= 0) || (pFont->m_nFontSize > 72))
		{
			CString txt;
			CString errorTxt;
			if (m_HdrFldConfig.m_bHdrFldCustomNameFont == 0)
			{
				txt.Format(_T("Field Name Font Size:  Invalid Font Size of %d! Valid Size is >0 and <=72\n"), pFont->m_nFontSize);
				errorTxt.Append(txt);
				errorTxt.Append(_T("Setting Font Size to 16\n"));
			}
			else
			{
				txt.Format(_T("Field Text Font Size:  Invalid Font Size of %d! Valid Size is >0 and <=72\n"), pFont->m_nFontSize);
				errorTxt.Append(txt);
				errorTxt.Append(_T("Setting Font Size to 16\n"));
			}
			int answer = MessageBox(errorTxt, _T("Error"), MB_APPLMODAL | MB_ICONERROR | MB_OK);
			pFont->m_nFontSize = 16;
		}

		int deb = 1;
	}

	int deb = 1;
}


void HtmlPdfHdrConfigDlg::OnBnClickedFldNameBold()
{
	// TODO: Add your control notification handler code here
	int deb = 1;
}


void HtmlPdfHdrConfigDlg::OnEnChangeFldSize()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	int deb = 1;
}


void HtmlPdfHdrConfigDlg::OnBnClickedPickFamilyFont()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	CWnd *p = GetDlgItem(IDC_PICK_FAMILY_FONT);
	if (p)
	{
		BOOL chk = ((CButton*)p)->GetCheck();
		if (chk == FALSE)
		{
			return;
		}
	}

	GenericFontFamilyDlg fdlg(this);

	HdrFldFont *pFont = &m_HdrFldConfig.m_HdrFldFontName;
	if (m_HdrFldConfig.m_bHdrFldCustomNameFont)
		pFont = &m_HdrFldConfig.m_HdrFldFontText;

	fdlg.m_genericFontName = pFont->m_genericFontName;

	INT_PTR ret = fdlg.DoModal();

	if (ret == IDOK)
	{

		pFont->m_genericFontName = fdlg.m_genericFontName;

		int deb = 1;
	}
	return;
}


void HtmlPdfHdrConfigDlg::OnBnClickedFontDflt()
{
	// TODO: Add your control notification handler code here

	CWnd *p = GetDlgItem(IDC_PICK_FAMILY_FONT);
	if (p)
	{
		p->EnableWindow(FALSE);
	}

	p = GetDlgItem(IDC_PICK_CONCRETE_FONT);
	if (p)
	{
		p->EnableWindow(FALSE);
	}

	p = GetDlgItem(IDC_HDR_FLD_NAME);
	if (p)
	{
		p->EnableWindow(FALSE);
	}

	p = GetDlgItem(IDC_HDR_FLD_TEXT);
	if (p)
	{
		p->EnableWindow(FALSE);
	}

	p = GetDlgItem(IDC_FLD_NAME_BOLD);
	if (p)
	{
		p->EnableWindow(TRUE);
	}

	p = GetDlgItem(IDC_FLD_SIZE);
	if (p)
	{
		p->EnableWindow(TRUE);
	}

	int deb = 1;
}


void HtmlPdfHdrConfigDlg::OnBnClickedFontCustom()
{
	// TODO: Add your control notification handler code here

	CWnd *p = GetDlgItem(IDC_PICK_FAMILY_FONT);
	if (p)
	{
		p->EnableWindow(TRUE);
	}

	p = GetDlgItem(IDC_PICK_CONCRETE_FONT);
	if (p)
	{
		p->EnableWindow(TRUE);
	}

	p = GetDlgItem(IDC_HDR_FLD_NAME);
	if (p)
	{
		p->EnableWindow(TRUE);
	}

	p = GetDlgItem(IDC_HDR_FLD_TEXT);
	if (p)
	{
		p->EnableWindow(TRUE);
	}

	p = GetDlgItem(IDC_FLD_NAME_BOLD);
	if (p)
	{
		p->EnableWindow(FALSE);
	}

	p = GetDlgItem(IDC_FLD_SIZE);
	if (p)
	{
		p->EnableWindow(FALSE);
	}

	int deb = 1;
}

void HdrFldConfig::SaveToRegistry()
{

	CString id = "hdrFld_";
	CString param = id + "FontStyleDflt";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bHdrFontDflt);
	param = id + "BoldFontName";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bHdrBoldFldName);

	param = id + "FontSize";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_nHdrFontSize);

	m_HdrFldList.SaveToRegistry();
	m_HdrFldFontName.SaveToRegistry();
	m_HdrFldFontText.SaveToRegistry();
}

void HdrFldConfig::LoadFromRegistry()
{
	CString id = "hdrFld_";
	BOOL retval;

	CString param = id + "FontStyleDflt";
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bHdrFontDflt))
	{
		;
	}

	param = id + "FontSize";
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_nHdrFontSize))
	{
		;
	}

	param = id + "BoldFontName";
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bHdrBoldFldName))
	{
		;
	}

	m_HdrFldList.LoadFromRegistry();
	m_HdrFldFontName.LoadFromRegistry();
	m_HdrFldFontText.LoadFromRegistry();
}

HdrFldFont::HdrFldFont(CString &id)
{
	m_id = id;
	SetDflts();
}

void HdrFldFont::SetDflts()
{
	m_genericFontName = "sans-serif";
	m_fontName = "Arial";
	m_fontStyleName = "Regular";
	m_nFontStyle = 400;
	m_bIsBold = FALSE;
	m_bIsItalic = FALSE;
	m_nFontSize = 16;
}

void HdrFldFont::SaveToRegistry()
{
	CString id = m_id;
	CString param = id + "GenericFontName";
	CProfile::_WriteProfileString(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_genericFontName);

	param = id + "FontName";
	CProfile::_WriteProfileString(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_fontName);

	param = id + "FontStyleName";
	CProfile::_WriteProfileString(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_fontStyleName);

	param = id + "FontStyle";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_nFontStyle);

	param = id + "FontIsBold";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bIsBold);

	param = id + "FontIsItalic";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bIsItalic);

	param = id + "FontSize";
	CProfile::_WriteProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_nFontSize);
}

void HdrFldFont::LoadFromRegistry()
{
	BOOL retval;
	CString id = m_id;

	CString param = id + _T("GenericFontName");
	if (retval = CProfile::_GetProfileString(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_genericFontName))
	{
		;
	}

	param = id + _T("FontName");
	if (retval = CProfile::_GetProfileString(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_fontName))
	{
		;
	}

	param = id + _T("FontStyleName");
	if (retval = CProfile::_GetProfileString(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_fontStyleName))
	{
		;
	}

	param = id + _T("FontStyle");
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_nFontStyle))
	{
		;
	}

	param = id + _T("FontIsBold");
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bIsBold))
	{
		;
	}

	param = id + _T("FontIsItalic");
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_bIsItalic))
	{
		;
	}

	param = id + _T("FontSize");
	if (retval = CProfile::_GetProfileInt(HKEY_CURRENT_USER, sz_Software_mboxview, param, m_nFontSize))
	{
		;
	}
	int deb = 1;
}


void HtmlPdfHdrConfigDlg::OnBnClickedHdrFldHelp()
{
	// TODO: Add your control notification handler code here

	CString HelpPath = FileUtils::GetMboxviewTempPath("MboxHelp");

	CString htmlHelpFileFile = "HTML_PDF_HdrConfigHelp.htm";
	CString fullPath = HelpPath + "\\" + htmlHelpFileFile;

	CFile fp;
	CFileException ExError;
	if (!fp.Open(fullPath, CFile::modeWrite | CFile::modeCreate, &ExError))
	{
		CString exErrorStr = FileUtils::GetFileExceptionErrorAsString(ExError);

		CString txt = _T("Could not create \"") + fullPath;
		txt += _T("\" file.\n");
		txt += exErrorStr;

		//TRACE(_T("%s\n"), txt);

		HWND h = NULL; // we don't have any window yet
		int answer = ::MessageBox(h, txt, _T("Error"), MB_APPLMODAL | MB_ICONERROR | MB_OK);
		return;
	}

	CString htmlHdr;

	htmlHdr += "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html;charset=US-ASCII\"></head><body style=\'margin-left:0.5cm\'>";
	htmlHdr += "<br><font size=\"+2\"><b>Mbox Viewer HTML/PDF Mail Header Configuration Help</b></font><br><br>";

	fp.Write((LPCSTR)htmlHdr, htmlHdr.GetLength());

	CString text;

	text.Empty();
	text.Append(
		"Please review the User Guide provided with the package for additional details on the HTML/PDF Mail Header Confguration feature.<br>"
		"<br>"
		"The HTML/PDF Header Configuration dialog allows users to select mail header fields for output and to configure fonts for the field name and field text.<br>"
		"There are two options to configure fonts: Default and Custom.<br><br>"
		"The Default option allows to configure:<br>"
		"&nbsp;&nbsp;&nbsp;&nbsp;1) the font size which be applied to both the field name and text.<br>"
		"&nbsp;&nbsp;&nbsp;&nbsp;2) the bold font style which will be applied to the field names only.<br>"
		"<br>"
		"The Custom option allows to configure:<br>"
		"<br>"
		"&nbsp;&nbsp;&nbsp;&nbsp;1) the same or different font for the header field names and text.<br>"
		"&nbsp;&nbsp;&nbsp;&nbsp;2) the font dialog allows to configure the font name, style and size.<br>"
		"&nbsp;&nbsp;&nbsp;&nbsp;2) the failback generic font in addition to the primary font configured via the font dialog.<br>"
		"<br>"
		"See the following links for an overview of fonts and font families.<br><br>"

		"<a href=\"https://www.w3.org/TR/css-fonts-3/#font-matching-algorithm\">HTML Fonts</a><br>"
		"<a href=\"https://www.granneman.com/webdev/coding/css/fonts-and-formatting\">Fonts & Formatting</a>"
	);
	fp.Write((LPCSTR)text, text.GetLength());


	text.Empty();
	text.Append(
		"<br><br>"
		"<br><br>"
	);
	fp.Write((LPCSTR)text, text.GetLength());

	CString htmlEnd = "\r\n</body></html>";
	fp.Write((LPCSTR)htmlEnd, htmlEnd.GetLength());

	fp.Close();

	ShellExecute(NULL, _T("open"), fullPath, NULL, NULL, SW_SHOWNORMAL);

	int deb = 1;
}
