//
// Language Plugin support
//
#ifndef	__CPLUGIN_INCLUDED__
#define	__CPLUGIN_INCLUDED__

#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <mbctype.h>

#include <string>
using namespace std;

#define	PLUGIN_MAX	64

class	CPlugin
{
public:
	typedef	struct tagPLUGIN {
		LCID	LocaleID;
		CHAR	Path[_MAX_PATH];
		CHAR	Language[256];
	} PLUGIN, *LPPLUGIN;

	static	HINSTANCE LoadPlugin();
	static	HINSTANCE LoadPluginID( INT nID );

	static	BOOL	FindPlugin( LPCSTR lpModulePath );

	static	INT	GetPluginNum()	{ return m_PluginNum; }
	static	INT	GetPluginID()	{ return m_PluginID; }
	static	void	SetPluginID( INT nID )	{ m_PluginID = nID; }

	// デフォルトプラグイン
	static	LPCSTR	GetPluginPath()		{ return m_Plugin[m_PluginID].Path; }
	static	LPCSTR	GetPluginLanguage()	{ return m_Plugin[m_PluginID].Language; }
	static	LCID	GetPluginLocaleID()	{ return m_Plugin[m_PluginID].LocaleID; }

	// プラグインID指定
	static	LPCSTR	GetPluginPath( INT nID )	{ return m_Plugin[nID].Path; }
	static	LPCSTR	GetPluginLanguage( INT nID )	{ return m_Plugin[nID].Language; }
	static	LCID	GetPluginLocaleID( INT nID )	{ return m_Plugin[nID].LocaleID; }


protected:
	static	INT	m_PluginID;
	static	INT	m_PluginNum;
	static	PLUGIN	m_Plugin[PLUGIN_MAX];

private:
};

#endif	// !__CPLUGIN_INCLUDED__
