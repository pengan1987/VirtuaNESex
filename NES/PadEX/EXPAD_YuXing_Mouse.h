//////////////////////////////////////////////////////////////////////////
// YuXing_Mouse                                                         //
//////////////////////////////////////////////////////////////////////////
class	EXPAD_YuXing_Mouse : public EXPAD
{
public:
	EXPAD_YuXing_Mouse( NES* parent ) : EXPAD( parent ) {}

	void	Reset();
	void	Strobe();
	BYTE	Read4016();
	BYTE	Read4017();
	void	Write4016( BYTE data );

	void	Sync();
	void	SetSyncData( INT type, LONG data );
	LONG	GetSyncData( INT type );

protected:
	LONG	mouse_x, mouse_y, mouse_x_old, mouse_y_old, mouse_x_move, mouse_y_move;
	BYTE	mouse_button, mouse_bits, mouse_data, mouse_count;
	BYTE	temp1, reg[8], mouse[8];

private:
};
