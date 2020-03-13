/*----------------------------------------------------------------------*/
/*                                                                      */
/*      DATA TYPE DEFINEs                                               */
/*                                                           Norix      */
/*                                               written     2000/11/09 */
/*                                               last modify ----/--/-- */
/*----------------------------------------------------------------------*/
#ifndef	__TYPEDEF_INCLUDED__
#define	__TYPEDEF_INCLUDED__

typedef	int		BOOL;

typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;
typedef	unsigned long	DWORD;
typedef	unsigned __int64 QWORD;

typedef	unsigned char	UBYTE;
typedef	unsigned short	UWORD;
typedef	unsigned long	ULONG;
typedef	unsigned long	UDWORD;
typedef	unsigned __int64 UQWORD;

typedef	signed char	SBYTE;
typedef	signed short	SWORD;
typedef	signed long	SLONG;
typedef	signed long	SDWORD;
typedef	signed __int64	SQWORD;

typedef	signed int	INT;
typedef	unsigned int	UINT;
typedef	signed short	SHORT;
typedef	signed long	LONG;

typedef	BYTE		*LPBYTE;
typedef	WORD		*LPWORD;
typedef	DWORD		*LPDWORD;

typedef	SBYTE		*LPSBYTE;
typedef	SWORD		*LPSWORD;
typedef	SLONG		*LPSLONG;
typedef	SDWORD		*LPSDWORD;

typedef	UBYTE		*LPUBYTE;
typedef	UWORD		*LPUWORD;
typedef	ULONG		*LPULONG;
typedef	UDWORD		*LPUDWORD;

typedef	SHORT		*LPSHORT;
typedef	LONG		*LPLONG;

// Byte pair
typedef	union	{
	struct	{ BYTE L; BYTE H; } B;
	WORD	W;
} PAIR, *LPPAIR;

#endif
