//
// Language plugin support
//
#include "VirtuaNESres.h"

#include "Pathlib.h"
#include "DebugOut.h"

#include "Plugin.h"

typedef	INT	(__cdecl *GETVLPVERSION)();
typedef	void	(__cdecl *GETVLPLANGUAGE)( LPSTR );
typedef LCID	(__cdecl *GETVLPLOCALEID)();

INT	CPlugin::m_PluginID = 0;
INT	CPlugin::m_PluginNum = 0;
CPlugin::PLUGIN	CPlugin::m_Plugin[PLUGIN_MAX];

HINSTANCE CPlugin::LoadPlugin()
{
	return	LoadPluginID( m_PluginID );
}

HINSTANCE CPlugin::LoadPluginID( INT nID )
{
	return ::LoadLibrary( m_Plugin[nID].Path );
}

BOOL	CPlugin::FindPlugin( LPCSTR lpModulePath )
{
	m_PluginNum = 0;

	string	Path = CPathlib::MakePath( lpModulePath, "*.vlp" );

	WIN32_FIND_DATA	find;
	HANDLE hFind = ::FindFirstFile( Path.c_str(), &find );
	if( hFind == INVALID_HANDLE_VALUE )
		return	FALSE;

	HINSTANCE	hDLL;
	GETVLPVERSION	GetVlpVersion = NULL;
	GETVLPLANGUAGE	GetVlpLanguage = NULL;
	GETVLPLOCALEID	GetVlpLocaleID = NULL;

	BOOL	bFind = TRUE;
	while( bFind ) {
		if( (hDLL = ::LoadLibrary( find.cFileName )) ) {
			GetVlpVersion  = (GETVLPVERSION) ::GetProcAddress( hDLL, "GetVlpVersion" );
			GetVlpLanguage = (GETVLPLANGUAGE)::GetProcAddress( hDLL, "GetVlpLanguage" );
			GetVlpLocaleID = (GETVLPLOCALEID)::GetProcAddress( hDLL, "GetVlpLocaleID" );

			if( !(!GetVlpVersion || !GetVlpLanguage || !GetVlpLocaleID) ) {
				INT	ver = GetVlpVersion();

				if( ver >= VIRTUANES_PLUGIN_VERSION ) {
					m_Plugin[m_PluginNum].LocaleID = GetVlpLocaleID();
					string	Path = CPathlib::MakePath( lpModulePath, find.cFileName );
					::strcpy( m_Plugin[m_PluginNum].Path, Path.c_str() );
					GetVlpLanguage( m_Plugin[m_PluginNum].Language );
					m_PluginNum++;
				}
			}

			::FreeLibrary( hDLL );
		}

		bFind = ::FindNextFile( hFind, &find );

		if( m_PluginNum >= PLUGIN_MAX )
			break;
	}

	::FindClose( hFind );

	LCID	lcid = ::GetUserDefaultLCID();
	for( INT no = 0; no < m_PluginNum; no++ ) {
		if( m_Plugin[no].LocaleID == lcid ) {
			m_PluginID = no;
			return	TRUE;
		}
	}

	return	TRUE;
}

