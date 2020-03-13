//////////////////////////////////////////////////////////////////////////
// Mapper162                                                            //
//////////////////////////////////////////////////////////////////////////

class	Mapper162 : public Mapper
{
public:
	Mapper162( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	WriteLow(WORD addr, BYTE data);
	void	Write( WORD addr, BYTE data );
	void	HSync( INT scanline );

protected:
	BYTE	reg5000;
	BYTE	reg5100;
	BYTE	reg5200;
	BYTE	reg5300;

private:
	void	SetBank_CPU();
	void	SetBank_PPU();
};
