//////////////////////////////////////////////////////////////////////////
// Kingwon Keyboard                                                     //
//                                                   written by Temryu  //
//                                                     last 2013/09/12  //
//////////////////////////////////////////////////////////////////////////

class	EXPAD_Kingwon_Keyboard : public EXPAD
{
public:
	EXPAD_Kingwon_Keyboard( NES* parent ) : EXPAD( parent ) {}

	void	Reset();
	BYTE	Read4017();
	void	Write4016( BYTE data );

protected:
	BYTE	ScanNo;
	BYTE	count4017;
private:
};
