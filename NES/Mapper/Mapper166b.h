//////////////////////////////////////////////////////////////////////////
// Mapper166                                                            //
//////////////////////////////////////////////////////////////////////////
class	Mapper166 : public Mapper
{
public:
	Mapper166( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );

//	void	HSync( INT scanline );
	void	Clock( INT cycles );

protected:
	WORD	reg[9];

	BYTE	irq_enable;
	BYTE	irq_counter;
	BYTE	irq_latch;
	BYTE	irq_request;
	INT		irq_clock;

private:
};
