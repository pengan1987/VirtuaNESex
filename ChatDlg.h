//
// チャットダイアログクラス
//
#ifndef	__CCHATDLG_INCLUDED__
#define	__CCHATDLG_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
using namespace std;

#include "Wnd.h"

class	CChatDlg : public CWnd
{
public:
	// Override from CWnd
	BOOL	Create( HWND hWndParent );
	void	Destroy();

	// Set Message
	void	SetEditText();
protected:
	// Message map
	DLG_MESSAGE_MAP()
	DLGMSG		OnInitDialog( DLGMSGPARAM );
	DLGMSG		OnDestroy( DLGMSGPARAM );
	DLGMSG		OnClose( DLGMSGPARAM );
	DLGMSG		OnActivate( DLGMSGPARAM );
	DLGMSG		OnSetCursor( DLGMSGPARAM );
	DLGMSG		OnSize( DLGMSGPARAM );

	DLGMSG		OnControlColorStatic( DLGMSGPARAM );

	DLGMSG		OnCopyData( DLGMSGPARAM );

	DLGCMD		OnMessageFocus( DLGCMDPARAM );

	DLGCMD		OnOK( DLGCMDPARAM );
	DLGCMD		OnCancel( DLGCMDPARAM );
	DLGCMD		OnSend( DLGCMDPARAM );
	//

	// 位置
	RECT		m_rcClient;	// クライアントエリア
	RECT		m_rcMessage;	// メッセージ枠
	RECT		m_rcEdit;	// エディット枠
	RECT		m_rcButton;	// 送信ボタン

private:
};

#endif	// !__CCHATDLG_INCLUDED__

