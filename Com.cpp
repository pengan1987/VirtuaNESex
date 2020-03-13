//
// COM利用サポートクラス
//
#include <objbase.h>
#include "DebugOut.h"
#include "Com.h"

INT	COM::m_nRefCount = 0;

LRESULT	COM::AddRef()
{
	if( !m_nRefCount ) {
		HRESULT	hr;
		if( (hr = ::CoInitialize( NULL )) != S_OK ) {
			DEBUGOUT( "COM::AddRef() CoInitialize failed.\n" );
			return	hr;
		}
		DEBUGOUT( "COM::AddRef() CoInitialize.\n" );
	}
	m_nRefCount++;
	return	0L;
}

void	COM::Release()
{
	if( !m_nRefCount ) {
		DEBUGOUT( "COM::Release() too many released.\n" );
		return;
	}
	if( !(--m_nRefCount) ) {
		::CoUninitialize();
		DEBUGOUT( "COM::AddRef() CoUninitialize.\n" );
	}
}

