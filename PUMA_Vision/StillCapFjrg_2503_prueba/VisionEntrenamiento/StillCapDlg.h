//------------------------------------------------------------------------------
// File: StillCapDlg.h
//
// Desc: DirectShow sample code - definition of callback and dialog
//       classes for StillCap application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#if !defined(AFX_STILLCAPDLG_H__3067E9D2_B94C_4ED1_99AB_53034129A0DD__INCLUDED_)
#define AFX_STILLCAPDLG_H__3067E9D2_B94C_4ED1_99AB_53034129A0DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CStillCapDlg dialog

class CSampleGrabberCB;

class CStillCapDlg : public CDialog
{
    friend class CSampleGrabberCB;

protected:

    // either the capture live graph, or the capture still graph
    CComPtr< IGraphBuilder > m_pGraph;

    // the playback graph when capturing video
    CComPtr< IGraphBuilder > m_pPlayGraph;

    // the sample grabber for grabbing stills
    CComPtr< ISampleGrabber > m_pGrabber;

    // if you're in still mode or capturing video mode
    bool m_bCapStills;

    // when in video mode, whether capturing or playing back
    int m_nCapState;

    // how many times you've captured
    int m_nCapTimes;

    HRESULT InitStillGraph( );
    HRESULT InitCaptureGraph( TCHAR * pFilename );
    HRESULT InitPlaybackGraph( TCHAR * pFilename ); 

    void GetDefaultCapDevice( IBaseFilter ** ppCap );
    void ClearGraphs( );
    void UpdateStatus(TCHAR *szStatus);
    void Error( TCHAR * pText );
    void PlaySnapSound(void);

// Construction
public:
    CStillCapDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CStillCapDlg)
	enum { IDD = IDD_STILLCAP_DIALOG };
	CListBox	m_liscentrox;
	CListBox	m_lisdistc;
	CListBox	m_lisdistb;
	CListBox	m_lisdista;
	CButton	m_Distor;
	CButton	m_ProcessButon;
	CSliderCtrl	m_DistMaxBlSl;
	CButton	m_CalibCancha;
	CButton	m_CalColor;
	CStatic	m_Static_SP;
	CStatic	m_Static_IP;
	CStatic	m_Static_HP;
	CSliderCtrl	m_SliderSP;
	CSliderCtrl	m_SliderIP;
	CSliderCtrl	m_SliderHP;
	CStatic	m_Static_OP;
	CSliderCtrl	m_SliderOI;
	CStatic	m_Static_OS;
	CSliderCtrl	m_SliderOS;
	CStatic	m_Static_OI;
	CSliderCtrl	m_SliderOH;
	CStatic	m_Static_OH;
	CButton	m_CommButton;
	CIPAddressCtrl	m_IPRemote;
	CIPAddressCtrl	m_IPLocal;
	CListBox	m_FrameRate;
	CSliderCtrl	m_YBlob;
	CSliderCtrl	m_XBlob;
	CSliderCtrl	m_MinRLE;
	CSliderCtrl	m_MinBlob;
	CSliderCtrl	m_SegYR;
	CSliderCtrl	m_SegYG;
	CSliderCtrl	m_SegYB;
	CSliderCtrl	m_SegOR;
	CSliderCtrl	m_SegOG;
	CSliderCtrl	m_SegOB;
	CSliderCtrl	m_SegBR;
	CSliderCtrl	m_SegBG;
	CSliderCtrl	m_SegBB;
	CSliderCtrl	m_Saturation;
	CSliderCtrl	m_Contrast;
	CSliderCtrl	m_Bright;
    CStatic m_StrStatus;
    CStatic m_StillScreen;
    CStatic m_PreviewScreen;
	CString	m_EdBright;
	CString	m_EdContrast;
	CString	m_EdSat;
	CString	m_EdBG;
	CString	m_EdBR;
	CString	m_EdOB;
	CString	m_EdOG;
	CString	m_EdOR;
	CString	m_EdYB;
	CString	m_EdYG;
	CString	m_EdYR;
	CString	m_MinBlobButt;
	int		m_rad;
	CString	m_EdXBlob;
	CString	m_EdYBlob;
	CString	m_IPPort;
	int		m_TeamColor;
	int		m_Color;
	float	m_CanchaX0;
	float	m_CanchaX1;
	float	m_CanchaY0;
	float	m_CanchaY1;
	int		m_MaxDistB;
	int		m_DistMaxBl;
	BOOL	m_VideoP;
	BOOL	m_centro;
	BOOL	m_distorsion;
	//}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CStillCapDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CStillCapDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSnap();
    afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonProcess();
	afx_msg void OnButtonResetPreproc();
	afx_msg void OnButtonRegiReset();
	afx_msg void OnRadioDim();
	afx_msg void OnSelchangeListRefres();
	afx_msg void OnFieldchangedIpaddressIpLocal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFieldchangedIpaddressDest(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusEditIpPort();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnButtonComm();
	afx_msg void OnRadioTeamcolor();
	afx_msg void OnButtonColor();
	afx_msg void OnButtonVercolor();
	afx_msg void OnButtonColorpicker();
	afx_msg void OnButtonCalcolor();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButtonHistogram();
	afx_msg void OnButtonCalibcancha();
	afx_msg void OnButtonCanchadat();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLoad();
	afx_msg void OnCommBurron();
	afx_msg void OnCheckVideop();
	afx_msg void OnButtonDistor();
	afx_msg void OnCheckCentro();
	afx_msg void OnCheckDistor();
	afx_msg void OnSelchangeListDista();
	afx_msg void OnSelchangeListDistb();
	afx_msg void OnSelchangeListx();
	afx_msg void OnSelchangeListDistc();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STILLCAPDLG_H__3067E9D2_B94C_4ED1_99AB_53034129A0DD__INCLUDED_)
