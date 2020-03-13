//////////////////////////////////////////////////////////////////////////
// Mapper116 CartSaint : —H—VAV‹­—ñ“`                                   //
//////////////////////////////////////////////////////////////////////////
class	Mapper116 : public Mapper
{
public:
	Mapper116( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	WriteLow( WORD addr, BYTE data );
	void	Write( WORD addr, BYTE data );

	void	HSync( INT scanline );

	// For state save
	BOOL	IsStateSave() { return TRUE; }
	void	SaveState( LPBYTE p );
	void	LoadState( LPBYTE p );

protected:
	BYTE	mode;
	BYTE	vrc2_chr[8], vrc2_prg[2], vrc2_mirr;
	BYTE	mmc3_regs[10], mmc3_ctrl, mmc3_mirr;
	BYTE	mmc1_regs[4], mmc1_buffer, mmc1_shift;
	BYTE	IRQCount, IRQLatch, IRQa;

private:
	void	SetBank_CPU();
	void	SetBank_PPU();
	void	SetBank_MIR();
};
