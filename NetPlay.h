//
// ネットプレイクラス
//
#ifndef	__CNETPLAY_INCLUDED__
#define	__CNETPLAY_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <mmsystem.h>
#include <winsock.h>

#include "macro.h"
#include "typedef.h"

#include <string>
using namespace std;

#define	WM_NETPLAY		(WM_APP+100)

#define	WM_NETPLAY_HOSTBYNAME	(WM_APP+101)

#define	WM_NETPLAY_ACCEPT	(WM_APP+110)
#define	WM_NETPLAY_CONNECT	(WM_APP+111)
#define	WM_NETPLAY_CLOSE	(WM_APP+112)
#define	WM_NETPLAY_ERROR	(WM_APP+113)

// for BSD style
#ifndef	INVALID_SOCKET
#define	INVALID_SOCKET	-1
#endif

// for Winsock1.x
#ifndef	SD_RECEIVE
#define	SD_RECEIVE	0
#endif
#ifndef	SD_SEND
#define	SD_SEND		1
#endif
#ifndef	SD_BOTH
#define	SD_BOTH		2
#endif

class	CNetPlay
{
public:
	CNetPlay();
	~CNetPlay();

	// 初期化/開放
	BOOL	Initialize( HWND hWnd );
	void	Release();

	// ネットプレイ中？
	BOOL	IsNetPlay() { return m_hWnd?TRUE:FALSE; }
	// 接続中？
	BOOL	IsConnect() { return m_hWnd?m_bConnect:FALSE; }
	// 接続中？
	BOOL	IsServer() { return m_bServer; }

	// 通信レイテンシ
	void	SetLatency( INT nLatency ) { m_nLatency = nLatency; }
	INT	GetLatency() { return m_nLatency; }

	// 非同期処理メッセージ返送ウインドウの設定
	void	SetMsgWnd( HWND hWnd ) { m_hWndMsg = hWnd; }

	// チャットメッセージ受け取りウインドウの設定
	void	SetChatWnd( HWND hWnd ) { m_hWndChat = hWnd; }

	// ホスト名がIPかどうかをチェックする(0:IP -:Error +:Host search)
	INT	ASyncHostCheck( HWND hWnd, const char* lpszHost );
	HRESULT	ASyncWndProc( HWND hWnd, WPARAM wParam, LPARAM lParam );

	// 接続と切断
	BOOL	Connect( BOOL bServer, const char* lpszIP, unsigned short Port );
	void	Disconnect();

	// データ送信 0:受信データ待ち 1以上:受信データあり 0未満:接続切れやエラー
	INT	Send( BYTE data );
	// データ受信
	// 0:受信データ待ち 1以上:受信データあり 0未満:接続切れやエラー
	// タイムアウト無し
	INT	Recv( BYTE& data );
	// タイムアウト有り
	INT	RecvTime( BYTE& data, unsigned long timeout );

	// リングバッファへの取り込み
	BOOL	RecvBuffer();
	// バッファチェック(0:No change  +:frame add  -:no frame)
	INT	BufferCheck();
	// リングバッファのバッファリングバイト数取得
	INT	GetRecvBufferSize();

	// 同期処理
	INT	Sync();
	// プレイヤー状態の更新
	INT	ModifyPlayer( LPBYTE p1, LPBYTE p2 );

	// チャットメッセージ送信
	void	ChatSend( LPCSTR lpStr );

	// Windowsメッセージプロシージャ
	HRESULT	WndProc( HWND hWnd, WPARAM wParam, LPARAM lParam );

	// 通信バッファ
	enum {
		// データブロックサイズ
		SOCKET_BLOCK_SIZE = 8,
		// バッファサイズ
		SOCKET_BUFFER_SIZE = (SOCKET_BLOCK_SIZE*32),
		// 受信時バッファサイズ
		SOCKET_RECEIVE_SIZE = (SOCKET_BLOCK_SIZE*8)
	};
protected:
	// メンバ変数
	HWND	m_hWnd;
	HWND	m_hWndMsg;
	HWND	m_hWndASync;
	HWND	m_hWndChat;

	HANDLE	m_hASyncTask;
	CHAR	m_HostEntry[MAXGETHOSTSTRUCT];

	BOOL	m_bServer;
	BOOL	m_bConnect;	// 接続中？
	INT	m_nLatency;	// レイテンシ(バッファサイズ)
	INT	m_nFrameStep;	// 通信フレームレート
	INT	m_nFrameCount;	// カウンタ

	// Ring buffer
	INT	m_nRingPtr;
	INT	m_nSendPtr;
	INT	m_nRecvPtr;
	INT	m_nRecvSize;
	BYTE	m_SendBuffer[SOCKET_BUFFER_SIZE];
	BYTE	m_RecvBuffer[SOCKET_BUFFER_SIZE];

	// WINSOCK
	WSADATA	m_WSAdata;
	SOCKET	m_SocketConnect;
	SOCKET	m_SocketData;
	SOCKET	m_SocketChat;

	struct sockaddr_in	m_SAddr_Server;
	struct sockaddr_in	m_SAddr_Client;
private:
};

extern	CNetPlay	NetPlay;

#endif	// !__CNETPLAY_INCLUDED__
