//////////////////////////////////////////////////////////////////////////
// Mapper000                                                            //
//////////////////////////////////////////////////////////////////////////
class	Mapper000 : public Mapper
{
public:
	Mapper000( NES* parent ) : Mapper(parent) {}

	void	Reset();
//	BYTE	ReadLow ( WORD addr );
//	void	WriteLow( WORD addr, BYTE data );
	void	Write( WORD addr, BYTE data );

protected:
private:
};
