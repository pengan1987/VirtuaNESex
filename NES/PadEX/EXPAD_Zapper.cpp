//////////////////////////////////////////////////////////////////////////
// Zapper                                                               //
//////////////////////////////////////////////////////////////////////////
void	EXPAD_Zapper::Reset()
{
	zapper_button = 0;
}

BYTE	EXPAD_Zapper::Read4017()
{
BYTE	data = 0x08;

	if( zapper_button ) {
		data |= 0x10;
	}

	if( nes->GetZapperHit() ) {
		if( DirectDraw.GetZapperHit() >= 0x40 )
			data &= ~0x08;
	}
	return	data;
}

void	EXPAD_Zapper::Sync()
{
	nes->GetZapperPos( zapper_x, zapper_y );

	zapper_button = 0;
	if( ::GetAsyncKeyState(VK_LBUTTON)&0x8000 )
		zapper_button = 0xFF;
}

void	EXPAD_Zapper::SetSyncData( INT type, LONG data )
{
	if( type == 0 ) {
		zapper_x = data;
	} else if( type == 1 ) {
		zapper_y = data;
	} else if( type == 2 ) {
		zapper_button = data?0xFF:0x00;
	}
}

LONG	EXPAD_Zapper::GetSyncData( INT type )
{
LONG	data = 0;

	if( type == 0 ) {
		data = zapper_x;
	} else if( type == 1 ) {
		data = zapper_y;
	} else if( type == 2 ) {
		data = zapper_button?0xFF:0x00;
	}
	return	data;
}

