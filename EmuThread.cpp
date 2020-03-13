//
// エミュレータスレッドクラス
//
#include "DebugOut.h"

#include "VirtuaNESres.h"
#include "EmuThread.h"
#include "MainFrame.h"
#include "Pathlib.h"

#include "NetPlay.h"

#include "DirectDraw.h"
#include "DirectSound.h"
#include "DirectInput.h"

// 自分自身
CEmuThread	Emu;

// Thisポインタ
CEmuThread*	CEmuThread::g_pThis = NULL;
// ウインドウハンドル
HWND	CEmuThread::g_hWnd = NULL;
// エミュレータオブジェクトポインタ
NES*	CEmuThread::g_nes = NULL;
// WAVEレコーダ
CWaveRec CEmuThread::g_WaveRec;

// NetEvent Queue
deque<NETEV>	CEmuThread::NetEventQueue;
string		CEmuThread::strNetStateName;

// ステータス
INT	CEmuThread::g_Status = CEmuThread::STATUS_NONE;
// スレッドイベントとイベントハンドル
INT	CEmuThread::g_Event = CEmuThread::EV_NONE;
LONG	CEmuThread::g_EventParam = 0;
LONG	CEmuThread::g_EventParam2 = 0;
HANDLE	CEmuThread::g_hEvent = NULL;
HANDLE	CEmuThread::g_hEventAccept = NULL;

// エラーメッセージ
CHAR	CEmuThread::g_szErrorMessage[512];

// ストリングテーブル
LPCSTR	CEmuThread::g_lpSoundMuteStringTable[] = {
	"Master ",
	"Rectangle 1",
	"Rectangle 2",
	"Triangle ",
	"Noise ",
	"DPCM ",
	"Ex CH1 ",
	"Ex CH2 ",
	"Ex CH3 ",
	"Ex CH4 ",
	"Ex CH5 ",
	"Ex CH6 ",
	"Ex CH7 ",
	"Ex CH8 ",
	NULL,
	NULL,
};

// スレッドプライオリティテーブル
INT	CEmuThread::g_PriorityTable[] = {
	THREAD_PRIORITY_IDLE,
	THREAD_PRIORITY_LOWEST,
	THREAD_PRIORITY_BELOW_NORMAL,
	THREAD_PRIORITY_NORMAL,
	THREAD_PRIORITY_ABOVE_NORMAL,
	THREAD_PRIORITY_HIGHEST,
	THREAD_PRIORITY_TIME_CRITICAL
};

CEmuThread::CEmuThread()
{
	g_pThis = this;

	m_hThread = NULL;
	g_Status = STATUS_NONE;
	m_nPriority = 3;	// Normal
	g_nes = NULL;
	m_nPauseCount = 0;
}

CEmuThread::~CEmuThread()
{
	Stop();
}

void	CEmuThread::SetPriority( INT nPriority )
{
	m_nPriority = nPriority;
	if( IsRunning() ) {
		::SetThreadPriority( m_hThread, g_PriorityTable[m_nPriority] );
	}
}

BOOL	CEmuThread::Start( HWND hWnd, NES* nes )
{
	Stop();

	if( !g_hEvent ) {
		if( !(g_hEvent = ::CreateEvent( NULL, FALSE, FALSE, NULL )) ) {
			DEBUGOUT( "CreateEvent failed.\n" );
			goto	_Start_Failed;
		}
	}
	if( !g_hEventAccept ) {
		if( !(g_hEventAccept = ::CreateEvent( NULL, FALSE, FALSE, NULL )) ) {
			DEBUGOUT( "CreateEvent failed.\n" );
			goto	_Start_Failed;
		}
	}

	::ResetEvent( g_hEvent );
	::ResetEvent( g_hEventAccept );

	g_hWnd = hWnd;
	g_nes = nes;
	g_Event = EV_INITIAL;
	g_Status = STATUS_NONE;
	::SetEvent( g_hEvent );
	m_nPauseCount = 0;

	if( !(m_hThread = ::CreateThread( NULL, 0, ThreadProc, 0, 0, &m_dwThreadID )) ) {
		DEBUGOUT( "CreateThread failed.\n" );
		goto	_Start_Failed;
	}

	// スレッドプライオリティの設定
	::SetThreadPriority( m_hThread, g_PriorityTable[m_nPriority] );

	// ちゃんと起動できたか確認の為イベントを待つ
	::WaitForSingleObject( g_hEventAccept, INFINITE );
	g_Status = STATUS_RUN;

//	DEBUGOUT( "CEmuThread:Start() Thread started.\n" );
	return	TRUE;

_Start_Failed:
	CLOSEHANDLE( g_hEvent );
	CLOSEHANDLE( g_hEventAccept );

	DEBUGOUT( "CEmuThread:Start() Thread startup failed!!." );
	return	FALSE;
}

void	CEmuThread::Stop()
{
	if( IsRunning() ) {
		::ResetEvent( g_hEventAccept );

		g_Event = EV_EXIT;
		::SetEvent( g_hEvent );

		::WaitForSingleObject( m_hThread, INFINITE );
		CLOSEHANDLE( m_hThread );
		m_hThread = NULL;
		g_Status = STATUS_NONE;

		CLOSEHANDLE( g_hEvent );
		CLOSEHANDLE( g_hEventAccept );

		// ネットプレイ時の切断処理
		NetPlay.Disconnect();

//		DEBUGOUT( "CEmuThread::Stop() Thread stoped.\n" );
	}
}

void	CEmuThread::Pause()
{
	if( IsRunning() ) {
		if( !IsPausing() ) {
			::ResetEvent( g_hEventAccept );
			g_Event = EV_PAUSE;
			::SetEvent( g_hEvent );
			::WaitForSingleObject( g_hEventAccept, INFINITE );
			g_Status = STATUS_PAUSE;
			m_nPauseCount++;
		} else {
			m_nPauseCount++;
		}
//		DEBUGOUT( "CEmuThread::Pause() Thread paused. Count=%d\n", m_nPauseCount );
	}
}

void	CEmuThread::Resume()
{
	if( IsRunning() ) {
		if( IsPausing() ) {
			if( --m_nPauseCount <= 0 ) {
				m_nPauseCount = 0;
				::ResetEvent( g_hEventAccept );
				g_Event = EV_RESUME;
				::SetEvent( g_hEvent );
				::WaitForSingleObject( g_hEventAccept, INFINITE );
				g_Status = STATUS_RUN;
			}
		}
//		DEBUGOUT( "CEmuThread::Resume() Thread resumed. Count=%d\n", m_nPauseCount );
	}
}

void	CEmuThread::Event( EMUEVENT ev )
{
	if( IsRunning() ) {
		::ResetEvent( g_hEventAccept );
		g_Event = ev;
		g_EventParam = 0;
		g_EventParam2 = -1;
		::SetEvent( g_hEvent );
		::WaitForSingleObject( g_hEventAccept, INFINITE );
	}
}

void	CEmuThread::EventParam( EMUEVENT ev, LONG param )
{
	if( IsRunning() ) {
		::ResetEvent( g_hEventAccept );
		g_Event = ev;
		g_EventParam = param;
		g_EventParam2 = -1;
		::SetEvent( g_hEvent );
		::WaitForSingleObject( g_hEventAccept, INFINITE );
	}
}

void	CEmuThread::EventParam2( EMUEVENT ev, LONG param, LONG param2 )
{
	if( IsRunning() ) {
		::ResetEvent( g_hEventAccept );
		g_Event = ev;
		g_EventParam = param;
		g_EventParam2 = param2;
		::SetEvent( g_hEvent );
		::WaitForSingleObject( g_hEventAccept, INFINITE );
	}
}

void	CEmuThread::DiskCommand( BYTE cmd )
{
	switch( cmd ) {
		case	0: // Eject
			if( g_nes->rom->GetDiskNo() > 0 ) {
				g_nes->Command( NES::NESCMD_DISK_EJECT );
				DirectDraw.SetMessageString( "Disk Eject." );
			}
			break;
		case	1: // Disk0 SideA
			if( g_nes->rom->GetDiskNo() > 0 ) {
				g_nes->Command( NES::NESCMD_DISK_0A );
				DirectDraw.SetMessageString( "Change Disk1 SideA." );
			}
			break;
		case	2: // Disk0 SideB
			if( g_nes->rom->GetDiskNo() > 1 ) {
				g_nes->Command( NES::NESCMD_DISK_0B );
				DirectDraw.SetMessageString( "Change Disk1 SideB." );
			}
			break;
		case	3: // Disk1 SideA
			if( g_nes->rom->GetDiskNo() > 2 ) {
				g_nes->Command( NES::NESCMD_DISK_1A );
				DirectDraw.SetMessageString( "Change Disk2 SideA." );
			}
			break;
		case	4: // Disk1 SideB
			if( g_nes->rom->GetDiskNo() > 3 ) {
				g_nes->Command( NES::NESCMD_DISK_1B );
				DirectDraw.SetMessageString( "Change Disk2 SideB." );
			}
			break;
	}
}

BOOL	CEmuThread::FrameInput()
{
static	CHAR	szMes[256];

	DirectInput.Poll();
	if( DirectDraw.GetZapperMode() ) {
		LONG	x, y;
		DirectDraw.GetZapperPos( x, y );
		if( g_nes ) {
			g_nes->SetZapperPos( x, y );
		}
	}
	g_nes->pad->Sync();

	if( !NetPlay.IsConnect() ) {
		g_nes->Movie();
	} else {
		DWORD	paddata = g_nes->pad->GetSyncData();
		BYTE	player1[CNetPlay::SOCKET_BLOCK_SIZE], player2[CNetPlay::SOCKET_BLOCK_SIZE];

		player1[0] = player1[1] = player1[2] = player1[3] = 0;
		if( !NetEventQueue.empty() ) {
			NETEV&	ev = NetEventQueue.front();

			switch( ev.Event ) {
				case	EV_HWRESET:
					player1[0] = 0x80;
					break;
				case	EV_SWRESET:
					player1[0] = 0x81;
					break;
				case	EV_DISK_COMMAND:
					player1[0] = 0x88;
					player1[1] = (BYTE)ev.Param;
					break;
				case	EV_STATE_LOAD:
					player1[0] = 0xF0;
					break;
				case	EV_STATE_SAVE:
					player1[0] = 0xF1;
					break;
				default:
					break;
			}
		}

		player1[4] = (BYTE) paddata;
		player1[5] = (BYTE)(paddata>>8);
		player1[6] = (BYTE)(paddata>>16);
		player1[7] = (BYTE)(paddata>>24);

		INT	ret = NetPlay.ModifyPlayer( player1, player2 );

		if( ret < 0 ) {
			// Network Error
			NetPlay.Disconnect();
			return	FALSE;
		} else if( ret == 0 ) {
			// Queueの分は送信したのでFIFOから取り除く
			if( !NetEventQueue.empty() ) {
				NetEventQueue.pop_front();
			}

			// Command check(同時はP1優先)
			BYTE	event = 0;
			BYTE	param = 0;
			if( player1[0] ) {
				event = player1[0];
				param = player1[1];
			} else if( player2[0] ) {
				event = player2[0];
				param = player2[1];
			}
			switch( event ) {
				case	0x80:
					g_nes->Command( NES::NESCMD_HWRESET );
					DirectDraw.SetMessageString( "Hardware reset." );
					break;
				case	0x81:
					g_nes->Command( NES::NESCMD_SWRESET );
					DirectDraw.SetMessageString( "Software reset." );
					break;
				case	0x88:
					DiskCommand( param );
					break;
				case	0xF0:
					if( g_nes->LoadState( strNetStateName.c_str() ) ) {
						::wsprintf( szMes, "Net State Load." );
						DirectDraw.SetMessageString( szMes );
					}
					break;
				case	0xF1:
					if( g_nes->SaveState( strNetStateName.c_str() ) ) {
						::wsprintf( szMes, "Net State Save." );
						DirectDraw.SetMessageString( szMes );
					}
					break;
				default:
					break;
			}
		}

		LPBYTE	p1, p2;
		if( NetPlay.IsServer() ) {
			p1 = player1;
			p2 = player2;
		} else {
			p1 = player2;
			p2 = player1;
		}
//DEBUGOUT( "P1:%02X  P2:%02X  P3:%02X  P4:%02X\n", player1[4], player2[4], player1[5], player2[5] );
		g_nes->pad->SetSyncData( ((DWORD)p1[4])|((DWORD)p2[4]<<8)|((DWORD)p1[5]<<16)|((DWORD)p2[5]<<24) );
	}

	return	TRUE;
}

DWORD WINAPI CEmuThread::ThreadProc( LPVOID lpParam )
{
INT	i;
INT	Ev;
LONG	Param, Param2;
BOOL	bLoop = TRUE;
BOOL	bPause = FALSE;		// Thread pause
BOOL	bEmuPause = FALSE;	// Emulation pause
BOOL	bThrottle = FALSE;	// Emulation throttle
INT	nFrameSkip = 0;		// Emulation frameskip
BOOL	bOneStep = FALSE;	// Emulation one step
BOOL	bSleep = FALSE;		// Sleep use

INT	frameskipno;
double	frame_time = 0.0f;
double	frame_period;
DWORD	start_time, current_time, now_time;
LONG	sleep_time;

// FPS
INT	nFrameCount = 0;
DWORD	dwFrameTime[32];
DWORD	FPS = 0;

// Str
CHAR	szStr[256];

// Netplay
NETEV	netev;
INT	nNetTimeoutCount = 0;

	while( bLoop ) {
		try {
			bOneStep = FALSE;

			Ev = EV_NONE;
			if( WAIT_OBJECT_0 == ::WaitForSingleObject(g_hEvent,0) ) {
				Ev = g_Event;
				Param = g_EventParam;
				Param2 = g_EventParam2;
			}

			switch( Ev ) {
				case	EV_NONE:
					break;
				case	EV_INITIAL:
					DirectDraw.SetMessageString( "Emulation start." );
					DirectSound.StreamPlay();
					frame_time = 0.0f;
					start_time = ::timeGetTime();
					if( g_nes ) {
						g_nes->ppu->SetScreenPtr( DirectDraw.GetRenderScreen(), DirectDraw.GetLineColormode() );
					}
					::SetEvent( g_hEventAccept );
					break;
				case	EV_EXIT:
					DirectSound.StreamStop();
					g_WaveRec.Stop();
					bLoop = FALSE;
					return	0;
				case	EV_PAUSE:
					DirectSound.StreamPause();
					bPause = TRUE;
					::SetEvent( g_hEventAccept );
					break;
				case	EV_RESUME:
					if( !bEmuPause )
						DirectSound.StreamResume();
					frame_time = 0.0f;
					start_time = ::timeGetTime();
					bPause = FALSE;
					::SetEvent( g_hEventAccept );
					break;
				case	EV_FULLSCREEN_GDI:
					DirectDraw.SetFullScreenGDI( (BOOL)Param );
					::SetEvent( g_hEventAccept );
					break;

				case	EV_HWRESET:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( !g_nes->IsMoviePlay() ) {
								g_nes->Command( NES::NESCMD_HWRESET );
								DirectDraw.SetMessageString( "Hardware reset." );
							}
						} else {
							netev.Event = EV_HWRESET;
							netev.Param = 0;
							NetEventQueue.push_back( netev );
						}
					}
					::SetEvent( g_hEventAccept );
					break;
				case	EV_SWRESET:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( !g_nes->IsMoviePlay() ) {
								g_nes->Command( NES::NESCMD_SWRESET );
								DirectDraw.SetMessageString( "Software reset." );
							}
						} else {
							netev.Event = EV_SWRESET;
							netev.Param = 0;
							NetEventQueue.push_back( netev );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_NETPLAY_START:
					if( g_nes ) {
						// 一応
						g_nes->MovieStop();

						string	pathstr;
						if( Config.path.bStatePath ) {
							pathstr = CPathlib::CreatePath( CApp::GetModulePath(), Config.path.szStatePath );
							::CreateDirectory( pathstr.c_str(), NULL );
						DEBUGOUT( "Path: %s\n", pathstr.c_str() );
						} else {
							pathstr = g_nes->rom->GetRomPath();
						}
						strNetStateName = CPathlib::MakePathExt( pathstr.c_str(), g_nes->rom->GetRomName(), "stn" );
						DEBUGOUT( "NetState Path: %s\n", strNetStateName.c_str() );

						g_nes->Command( NES::NESCMD_HWRESET );
						DirectDraw.SetMessageString( "Netplay start!" );
					}
					bThrottle = FALSE;

					// Queueをクリア
					NetEventQueue.clear();
					// 最初はSync
					NetPlay.Sync();

					::SetEvent( g_hEventAccept );
					break;

				case	EV_EMUPAUSE:
					if( !NetPlay.IsConnect() ) {
						bEmuPause = !bEmuPause;
						if( bEmuPause ) {
							DirectSound.StreamPause();
						} else if( !bPause ) {
							DirectSound.StreamResume();
						}
						DirectDraw.SetMessageString( "Pause." );
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_ONEFRAME:
					if( !NetPlay.IsConnect() ) {
						bEmuPause = TRUE;
						DirectSound.StreamPause();
						bOneStep = TRUE;
						DirectDraw.SetMessageString( "One Frame." );
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_THROTTLE:
					if( !NetPlay.IsConnect() ) {
						bThrottle = !bThrottle;
						if( bThrottle ) {
							DirectDraw.SetMessageString( "Throttle ON." );
						} else {
							DirectDraw.SetMessageString( "Throttle OFF." );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_FRAMESKIP_AUTO:
					if( !NetPlay.IsConnect() ) {
						DirectDraw.SetMessageString( "FrameSkip Auto." );
						nFrameSkip = 0;
					}
					::SetEvent( g_hEventAccept );
					break;
				case	EV_FRAMESKIP_UP:
					if( !NetPlay.IsConnect() ) {
						if( nFrameSkip < 20 )
							nFrameSkip++;
						::wsprintf( szStr, "FrameSkip %d", nFrameSkip );
						DirectDraw.SetMessageString( szStr );
					}
					::SetEvent( g_hEventAccept );
					break;
				case	EV_FRAMESKIP_DOWN:
					if( !NetPlay.IsConnect() ) {
						if( nFrameSkip )
							nFrameSkip--;
						if( nFrameSkip ) {
							::wsprintf( szStr, "FrameSkip %d", nFrameSkip );
							DirectDraw.SetMessageString( szStr );
						} else {
							DirectDraw.SetMessageString( "FrameSkip Auto." );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_STATE_LOAD:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->LoadState( (const char*)Param ) ) {
								if( Param2 < 0 )
									::wsprintf( szStr, "State Load." );
								else
									::wsprintf( szStr, "State Load #%d", Param2 );
								DirectDraw.SetMessageString( szStr );
							}
						} else {
							netev.Event = EV_STATE_LOAD;
							netev.Param = 0;
							NetEventQueue.push_back( netev );
						}
					}
					::SetEvent( g_hEventAccept );
					break;
				case	EV_STATE_SAVE:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->SaveState( (const char*)Param ) ) {
								if( Param2 < 0 )
									::wsprintf( szStr, "State Save." );
								else
									::wsprintf( szStr, "State Save #%d", Param2 );
								DirectDraw.SetMessageString( szStr );
							}
						} else {
							netev.Event = EV_STATE_SAVE;
							netev.Param = 0;
							NetEventQueue.push_back( netev );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_DISK_COMMAND:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							DiskCommand( Param );
						} else {
							netev.Event = EV_DISK_COMMAND;
							netev.Param = Param;
							NetEventQueue.push_back( netev );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_EXCONTROLLER:
					if( g_nes )
						g_nes->CommandParam( NES::NESCMD_EXCONTROLLER, Param );
					::SetEvent( g_hEventAccept );
					break;

				case	EV_SOUND_MUTE:
					{
					if( g_nes )
						if( g_nes->CommandParam( NES::NESCMD_SOUND_MUTE, Param ) ) {
							::wsprintf( szStr, "%s Enable.", g_lpSoundMuteStringTable[Param] );
						} else {
							::wsprintf( szStr, "%s Mute.", g_lpSoundMuteStringTable[Param] );
						}
						DirectDraw.SetMessageString( szStr );
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_MOVIE_PLAY:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->MoviePlay( (const char*)Param ) ) {
								DirectDraw.SetMessageString( "Movie replay." );
							}
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_MOVIE_REC:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->MovieRec( (const char*)Param ) ) {
								DirectDraw.SetMessageString( "Movie record." );
							}
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_MOVIE_RECAPPEND:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->MovieRecAppend( (const char*)Param ) ) {
								DirectDraw.SetMessageString( "Movie append record." );
							}
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_MOVIE_STOP:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							g_nes->MovieStop();
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_SNAPSHOT:
					if( g_nes ) {
						if( g_nes->Snapshot() ) {
							DirectDraw.SetMessageString( "Snap shot." );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_WAVEREC_START:
					if( g_nes ) {
						DWORD	nRate, nBits;
						DirectSound.GetSamplingRate( nRate, nBits );
						g_WaveRec.Start( (LPSTR)Param, nRate, nBits, FALSE );
					}
					DirectDraw.SetMessageString( "Wave recording start." );
					::SetEvent( g_hEventAccept );
					break;

				case	EV_WAVEREC_STOP:
					if( g_nes ) {
						g_WaveRec.Stop();
						DirectDraw.SetMessageString( "Wave recording stop." );
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_TAPE_PLAY:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->TapePlay( (const char*)Param ) ) {
								DirectDraw.SetMessageString( "Tape play." );
							}
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_TAPE_REC:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							if( g_nes->TapeRec( (const char*)Param ) ) {
								DirectDraw.SetMessageString( "Tape record." );
							}
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_TAPE_STOP:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							g_nes->TapeStop();
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_BARCODE:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							g_nes->SetBarcodeData( (LPBYTE)Param, (INT)Param2  );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_TURBOFILE:
					if( g_nes ) {
						if( !NetPlay.IsConnect() ) {
							g_nes->SetTurboFileBank( (INT)Param  );
						}
					}
					::SetEvent( g_hEventAccept );
					break;

				case	EV_MESSAGE_OUT:
					DirectDraw.SetMessageString( (LPSTR)Param );
					::SetEvent( g_hEventAccept );
					break;

				default:
					DEBUGOUT( "ThreadProc:Unknown event.\n" );
					::SetEvent( g_hEventAccept );
					break;
			}
		} catch( CHAR* str ) {
			bPause = TRUE;
			::strcpy( g_szErrorMessage, str );
			::PostMessage( g_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)g_szErrorMessage );
			::SetEvent( g_hEventAccept );
#ifndef	_DEBUG
		} catch(...) {
			bPause = TRUE;
			::PostMessage( g_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)CApp::GetErrorString( IDS_ERROR_UNKNOWN ) );
			::SetEvent( g_hEventAccept );
#endif
		}

		if( bPause ) {
			// イベント発生の為
			DirectInput.Poll();

			// かっこ悪い…
			CMainFrame::OnKeyControl();

			// 呼ばないとWindowsが重くなるので(NT系以外は特に)
			::Sleep( 20 );
		} else {
			try {
				INT	nNetBuffer = 0;
				BOOL	bNoFrame = FALSE;
				BOOL	bAddFrame = FALSE;

				bSleep = TRUE;

				if( !NetPlay.IsConnect() ) {
					// 通常
					BOOL	bKeyThrottle = FALSE;
					// キーチェック
					{
						BYTE*	pKey = (BYTE*)DirectInput.m_Sw;
						WORD*	pShortCutKey = Config.shortcut.nShortCut;
						INT*	pShortCutKeyID = Config.ShortcutKeyID;
						for( INT i = 0; pShortCutKeyID[i*3+0]; i++ ) {
							if( pShortCutKeyID[i*3+0] == ID_KEYTHROTTLE ) {
								if( (pKey[pShortCutKey[pShortCutKeyID[i*3+2]    ]] & 0x80) && pShortCutKey[pShortCutKeyID[i*3+2]    ]
								 || (pKey[pShortCutKey[pShortCutKeyID[i*3+2]+128]] & 0x80) && pShortCutKey[pShortCutKeyID[i*3+2]+128] ) {
									bKeyThrottle = TRUE;
								}
								break;
							}
						}
					}

					if( Config.general.bScreenMode ) {
					// FullScreen
//						if( Config.graphics.bSyncDraw ) {
						if( Config.graphics.bSyncDraw && Config.graphics.bSyncNoSleep ) {
							bSleep = FALSE;
						} else if( Config.emulator.bAutoFrameSkip ) {
							bSleep = TRUE;
						} else {
							bSleep = FALSE;
						}
					} else {
					// Window
//						if( Config.graphics.bWindowVSync ) {
						if( Config.graphics.bWindowVSync && Config.graphics.bSyncNoSleep ) {
							bSleep = FALSE;
						} else if( Config.emulator.bAutoFrameSkip ) {
							bSleep = TRUE;
						} else {
							bSleep = FALSE;
						}
					}

					frame_period = 1000.0/g_nes->nescfg->FrameRate;

					// フレームスキップ数の計算
					current_time = ::timeGetTime();
					now_time     = current_time - start_time;

					if( Config.emulator.bAutoFrameSkip && bSleep ) {
						if( g_nes->IsDiskThrottle() ) {
							frame_period = g_nes->nescfg->FramePeriod/10.0f;
						} else if( !nFrameSkip ) {
						// Auto
							double	fps = 0.0;
							if( (bThrottle||bKeyThrottle) ) {
								fps = (double)Config.emulator.nThrottleFPS;
							} else {
								fps = g_nes->nescfg->FrameRate;
							}
							if( fps < 0.0 )   fps = 60.0;
							if( fps > 600.0 ) fps = 600.0;
							frame_period = 1000.0/fps;
						} else {
							frame_period = (1000.0/g_nes->nescfg->FrameRate)/(nFrameSkip+1);
						}

						if( !nFrameSkip && !g_nes->IsDiskThrottle() ) {
							frameskipno = (INT)(((double)now_time-frame_time) / frame_period);
							if( frameskipno < 0 || frameskipno > 20 ) {
								frameskipno = 1;
								frame_time  = 0.0;
								start_time  = ::timeGetTime();
							}
						} else if( g_nes->IsDiskThrottle() ) {
							frameskipno = 10;
							frame_time  = 0.0;
							start_time  = ::timeGetTime();
						} else {
							if( nFrameSkip < 0 )
								frameskipno = 1;
							else
								frameskipno = nFrameSkip+1;
						}
					} else {
						if( g_nes->IsDiskThrottle() ) {
							frameskipno = 10;
						}
						if( bThrottle||bKeyThrottle ) {
							frameskipno = (INT)(((double)Config.emulator.nThrottleFPS+30)/60.0);
						} else {
							// オートを外してVSYNC同期の時にもフレームスキップをする為の措置
							if( nFrameSkip < 0 ) {
								frameskipno = 1;
							} else {
								frameskipno = nFrameSkip+1;
							}
						}
					}
				} else {
					bSleep = TRUE;

					// ネットプレイ中はフレームスキップ不可
					frame_period = 1000.0/g_nes->nescfg->FrameRate;

					// フレームスキップ数の計算
					current_time = ::timeGetTime();
					now_time     = current_time - start_time;

					frameskipno = (INT)(((double)now_time-frame_time) / frame_period);
					if( frameskipno > 20-1 ) {
						frameskipno = 20;
						frame_time  = 0.0f;
						start_time  = ::timeGetTime();
					}

					if( !NetPlay.RecvBuffer() ) {
						bPause = TRUE;
						goto	_emulate_error;
					}

					// バッファ不足で少し遅らせるべきかどうかのチェック
					INT	ret = NetPlay.BufferCheck();
					if( ret > 0 ) {
						bAddFrame = TRUE;
						if( frameskipno > 1 ) {
							frameskipno++;
						} else {
							frameskipno = 2;
						}
					}
					if( ret < 0 ) {
						bNoFrame = TRUE;
					}

					// Timeout check
					if( bNoFrame ) {
						nNetTimeoutCount++;
						if( nNetTimeoutCount > 10*60 ) {
							NetPlay.Disconnect();
							bPause = TRUE;
							goto	_emulate_error;
						}
					} else {
						nNetTimeoutCount = 0;
					}

					nNetBuffer = NetPlay.GetRecvBufferSize();
				}

				// Emulation
				if( (!bEmuPause || bOneStep) && g_nes ) {
					g_nes->ppu->SetScreenPtr( DirectDraw.GetRenderScreen(), DirectDraw.GetLineColormode() );

					if( !bOneStep ) {
						for( i = 0; i < frameskipno-1; i++ ) {
							if( !bNoFrame ) {
								// Skip frames
								if( !FrameInput() ) {
									bPause = TRUE;
									goto	_emulate_error;
								}
								g_nes->EmulateFrame( FALSE );
							}
							if( !bAddFrame ) {
								frame_time += frame_period;
							}
							bAddFrame = FALSE;

							// Sound streaming
							StreamProcess( bEmuPause );
						}
					}
					if( !bNoFrame ) {
						if( !FrameInput() ) {
							bPause = TRUE;
							goto	_emulate_error;
						}
						g_nes->EmulateFrame( TRUE );
					}
					frame_time += frame_period;
				} else {
					// イベント発生の為
					DirectInput.Poll();

					for( i = 0; i < frameskipno-1; i++ ) {
						frame_time += frame_period;
					}
					frame_time += frame_period;
				}

				// かっこ悪い…
				CMainFrame::OnKeyControl();

//				// 描画
//				if( g_nes->ppu->GetExtMonoMode() ) {
//					DirectDraw.SetPaletteMode( (PPUREG[1]&PPU_BGCOLOR_BIT)>>5, 0 );
//				} else {
//					DirectDraw.SetPaletteMode( (PPUREG[1]&PPU_BGCOLOR_BIT)>>5, PPUREG[1]&PPU_COLORMODE_BIT );
//				}
				// ディスクアクセスランプ
				DirectDraw.SetDiskAccessLamp( (Config.graphics.bDiskAccessLamp && g_nes->mapper->ExCmdRead( Mapper::EXCMDRD_DISKACCESS ))?TRUE:FALSE );
				// blit
				DirectDraw.Blt();

				// Sound streaming
				StreamProcess( bEmuPause );

				// 暇な時間待ち
				sleep_time = (frame_time+frame_period) - (LONG)(::timeGetTime() - start_time);
				if( bSleep && (sleep_time > 0) ) {
					::Sleep( sleep_time-1 );
				} else {
					::Sleep( 0 );
				}

				DirectDraw.Flip();

#if	_DEBUGOUT
{
static	BOOL	bPalettePut = FALSE;

	if( ::GetAsyncKeyState( VK_ESCAPE ) & 0x8000 ) {
		if( !bPalettePut ) {
			int	i;
			DEBUGOUT( "BG\t" );
			for( i = 0; i < 16; i++ ) {
				DEBUGOUT( "%02X ", BGPAL[i] );
			}
			DEBUGOUT( "\n" );
			DEBUGOUT( "SP\t" );
			for( i = 0; i < 16; i++ ) {
				DEBUGOUT( "%02X ", SPPAL[i] );
			}
			DEBUGOUT( "\n" );
		}
		bPalettePut = TRUE;
	} else {
		bPalettePut = FALSE;
	}
}
#endif

				// FPS表示
				dwFrameTime[nFrameCount] = ::timeGetTime();
				if( ++nFrameCount > 32-1 ) {
					nFrameCount = 0;
					if( Config.graphics.bFPSDisp ) {
						if( dwFrameTime[32-1]-dwFrameTime[0] > 0 ) {
							FPS = 1000*10*(32-1)/(dwFrameTime[32-1]-dwFrameTime[0]);
						} else {
							FPS = 0;
						}
					} else {
						FPS = 0;
					}
				}
				if( Config.graphics.bFPSDisp ) {
#if	!(defined(_DEBUG)||defined(_DEBUGOUT))
					sprintf( szStr, "FPS:%d.%01d", FPS/10, FPS%10 );
#else
					if( !NetPlay.IsConnect() ) {
						sprintf( szStr, "FPS:%d.%01d", FPS/10, FPS%10 );
					} else {
						sprintf( szStr, "FPS:%d.%01d  NET:%3d  NOFRM:%d  TOUT:%3d", FPS/10, FPS%10, nNetBuffer, bNoFrame, nNetTimeoutCount );
					}
#endif
					DirectDraw.SetInfoString( szStr );
				} else {
					DirectDraw.SetInfoString( NULL );
				}
_emulate_error:;
			} catch( CHAR* str ) {
				bPause = TRUE;
				::strcpy( g_szErrorMessage, str );
				::PostMessage( g_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)g_szErrorMessage );
#ifndef	_DEBUG
			} catch(...) {
				bPause = TRUE;
				::PostMessage( g_hWnd, WM_VNS_ERRORMSG, 0, (LPARAM)CApp::GetErrorString( IDS_ERROR_UNKNOWN ) );
#endif
			}
		}
	}

	g_WaveRec.Stop();

	return	0;
}

void	CEmuThread::StreamProcess( BOOL bPause )
{
	if( g_pThis && g_nes && !bPause && !DirectSound.IsStreamPause() ) {
		DWORD	dwWrite, dwSize;
		LPVOID	lpLockPtr;
		DWORD	dwLockSize;
		if( DirectSound.GetStreamLockPosition( &dwWrite, &dwSize ) ) {
			if( DirectSound.StreamLock( dwWrite, dwSize, &lpLockPtr, &dwLockSize, NULL, NULL, 0 ) ) {
				g_nes->apu->Process( (LPBYTE)lpLockPtr, dwLockSize );
				g_WaveRec.Out( lpLockPtr, dwLockSize );
				DirectSound.StreamUnlock( lpLockPtr, dwLockSize, NULL, NULL );
			}
		}
	}
}

