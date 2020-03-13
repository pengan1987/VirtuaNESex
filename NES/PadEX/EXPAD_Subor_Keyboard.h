//////////////////////////////////////////////////////////////////////////
// Subor Keyboard                                                       //
//////////////////////////////////////////////////////////////////////////
class	EXPAD_Subor_Keyboard : public EXPAD
{
public:
	EXPAD_Subor_Keyboard( NES* parent ) : EXPAD( parent ) {}

	void	Reset();

	BYTE	Read4016();
	BYTE	Read4017();
	void	Write4016( BYTE data );

protected:
	BOOL	bOut;
	BYTE	ScanNo;
private:
};
