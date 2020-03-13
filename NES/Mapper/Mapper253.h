//////////////////////////////////////////////////////////////////////////
// Mapper253                                                            //
//////////////////////////////////////////////////////////////////////////
class	Mapper253 : public Mapper
{
public:
	Mapper253( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

	void	Clock( INT cycles );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	VRAM_switch;
	BYTE	rom_type;
	BYTE	reg[9];
	BYTE	irq_enable;
	BYTE	irq_counter;
	BYTE	irq_latch;
	INT		irq_clock;
private:
	void	SetBank_PPUSUB( int bank, int page );
};
