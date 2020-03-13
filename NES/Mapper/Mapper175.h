//////////////////////////////////////////////////////////////////////////
// Mapper175  15-in-1                                                   //
//////////////////////////////////////////////////////////////////////////
class	Mapper175 : public Mapper
{
public:
	Mapper175( NES* parent ) : Mapper(parent) {}

	void	Reset();
	void	Write( WORD addr, BYTE data );
	BYTE	Read( WORD addr );

protected:
	BYTE	reg_dat;
private:
};
