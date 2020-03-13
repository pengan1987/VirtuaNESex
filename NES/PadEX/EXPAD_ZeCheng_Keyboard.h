//////////////////////////////////////////////////////////////////////////
// ZeCheng Keyboard                                                     //
//                                                   written by Temryu  //
//                                                     last 2013/09/12  //
//////////////////////////////////////////////////////////////////////////

class	EXPAD_ZeCheng_Keyboard : public EXPAD
{
public:
	EXPAD_ZeCheng_Keyboard( NES* parent ) : EXPAD( parent ) {}

	void	Reset();
	BYTE	Read4017();
	void	Write4016( BYTE data );

protected:
	BOOL	tb;
	BYTE	ScanNo;
	BYTE	count4017;
private:
};
