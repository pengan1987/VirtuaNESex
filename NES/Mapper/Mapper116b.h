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
	uint32 mode;
	
	uint8 vrc2_chr[8];
	uint8 vrc2_prg[2];
	uint8 vrc2_nmt;
	uint8 vrc2_padding;
	
	uint8 mmc3_banks[10];
	uint8 mmc3_ctrl;
	uint8 mmc3_nmt;

	uint8 mmc1_regs[4];
	uint8 mmc1_buffer;
	uint8 mmc1_shifter;
	uint8 mmc1_padding[2];
	uint8 irq_enable ;	// Disable
	uint8 irq_counter ;
	uint8 irq_latch ;
	
	uint8 exPreg;

private:
	void NES_mapper116_MMC3_set_CPU_banks();
	void NES_mapper116_MMC3_set_Nmt();
	void NES_mapper116_MMC3_set_PPU_banks();

	
	void Mapper116_Poke_Vrc2_8000(uint32 address,uint32 data);
	void Mapper116_Poke_Vrc2_9000(uint32 address,uint32 data);
	void Mapper116_Poke_Vrc2_B000(uint32 address,uint32 data);
	void Mapper116_Poke_Mmc3_C000(uint32 address,uint32 data);
	void Mapper116_Poke_Mmc3_E000(uint32 address,uint32 data);
	void Mapper116_Poke_Mmc3_8000(uint32 address,uint32 data);
	void Mapper116_Poke_Mmc3_A000(uint32 address,uint32 data);
	void Mapper116_Poke_Mmc1_8000(uint32 address,uint32 data);
};
