//////////////////////////////////////////////////////////////////////////
// Zapper                                                               //
//////////////////////////////////////////////////////////////////////////
class	EXPAD_YuXingMouse : public EXPAD
{
public:
	EXPAD_YuXingMouse( NES* parent ) : EXPAD( parent ) {}

	void	Reset();

	BYTE	Read4017();

	void	Sync();
	void	SetSyncData( INT type, LONG data );
	LONG	GetSyncData( INT type );

protected:
	LONG	zapper_x, zapper_y;
	BYTE	zapper_button;

private:
};
