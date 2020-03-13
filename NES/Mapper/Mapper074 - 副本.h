//////////////////////////////////////////////////////////////////////////
// Mapper074                                                            //
//////////////////////////////////////////////////////////////////////////
class	Mapper074 : public Mapper
{
public:
	Mapper074( NES* parent ) : Mapper(parent) {}

	
	void	Reset();
	BYTE	ReadLow ( WORD addr );
	void	WriteLow( WORD addr, BYTE data );
	void	Write( WORD addr, BYTE data );

	void	HSync( INT scanline );

	void	PPU_Latch( WORD addr );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	reg[8];
	BYTE	prg0, prg1, prg2, prg3;
	BYTE	chr01, chr23, chr4, chr5, chr6, chr7;
	BYTE	chr1, chr3;
	BYTE	we_sram;
	BYTE	sp_rom, bank;
	BYTE	patch;

	BYTE	irq_type;
	BYTE	irq_enable;
	BYTE	irq_counter;
	BYTE	irq_latch;
	BYTE	irq_request;

	BYTE	reg5000;
	BYTE	reg5001;
	BYTE	reg5002;
	BYTE	reg5003;
	BYTE	reg5010;
	BYTE	reg5011;
	BYTE	reg5012;
	BYTE	reg5013;
	BYTE	reg5FF3;
	BYTE	reg6000;
	BYTE	reg6010;
	BYTE	reg6013;
	BYTE	WF_sn_5000;
	BYTE	WF_sn_5010;
	BYTE	WF_sn_5013;
	BYTE	RF_sn_6000;
	BYTE	RF_sn_6010;
	BYTE	RF_sn_6013;

	BYTE	WX_WRAM00[8*1024];
	BYTE	WX_WRAM01[8*1024];
	BYTE	WX_WRAM02[8*1024];
	BYTE	WX_WRAM03[8*1024];

//	BYTE	adr5000buf[4*1024];
	BYTE	adr6000buf[8*1024];

private:
	void	SetBank_CPU();
	void	SetBank_PPU();
	void	SetBank_PPUSUB( int bank, int page );
};
