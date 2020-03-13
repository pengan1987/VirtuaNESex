//////////////////////////////////////////////////////////////////////////
// Mapper120  Tobidase Daisakusen                                       //
//////////////////////////////////////////////////////////////////////////
class	Mapper120 : public Mapper
{
public:
	Mapper120( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	WriteLow( WORD addr, BYTE data );

protected:
private:
};
