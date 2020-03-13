//
// COM利用サポートクラス
//
#ifndef	__CCOM_INCLUDED__
#define	__CCOM_INCLUDED__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class	COM
{
public:
	static	LRESULT	AddRef();
	static	void	Release();

protected:
	static	INT	m_nRefCount;
};

#endif	// !__CCOM_INCLUDED__

