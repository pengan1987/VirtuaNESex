//
// DirectDraw class
//
#include "DebugOut.h"
#include "DirectDraw.h"
#include "COM.h"

#include "MMU.h"

extern BOOL g_bSan2;
extern unsigned char pFont[256*1024];

#define MIN_GROUP 0x6 
#define MIN_INDEX 0x40 
#define MAX_GROUP 0xF 
#define MAX_INDEX 0x76 
int txt[MAX_GROUP][MAX_INDEX] = 
{ 
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x130,0x131,0x126,0x000,0x000,0x151,0x000,0x000,0x39F,0x132,0x0C8,0x133,0x134,0x135,0x3A0,0x3A1,0x3A2,0x152,
0x000,0x000,0x000,0x000,0x000,0x000,0x0FF,0x34D,0x138,0x0C5,0x25B,0x1C2,0x3A3,0x3A4,0x017,0x3A5,0x364,0x029,0x120,0x000,0x000,0x3A6,0x311,0x39F,0x39F,0x39F,0x39F,0x39F,0x39F,0x39F,0x39F,0x39F,
0x39F,0x39F,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x001,0x002,0x003,0x004,0x005,0x006,0x007,0x008,0x009,0x00A,0x00B,0x00C,
0x00D,0x00E,0x00F,0x010,0x011,0x012,0x013,0x014,0x015,0x016,0x017,0x018,0x019,0x01A,0x01B,0x01C,0x01D,0x01E,0x01F,0x020,0x021,0x022,0x023,0x024,0x025,0x026,0x027,0x028,0x029,0x02A,0x02B,0x02C,
0x02D,0x02E,0x02F,0x030,0x1B9,0x1BA,0x1BB,0x1BC,0x1BD,0x1BE,0x1BF,0x1C0,0x1C1,0x1C2,0x1C3,0x1C4,0x1C5,0x1C6,0x1C7,0x1C8,0x1C9,0x1CA,0x1CB,0x1CC,0x1CD,0x1CE,0x1CF,0x1D0,0x1D1,0x1D2,0x1D3,0x1D4,
0x1D5,0x1D6,0x1D7,0x1D8,0x1D9,0x1DA,0x1DB,0x1DC,0x1DD,0x1DE,0x1DF,0x1E0,0x1E1,0x1E2,0x1E3,0x1E4,0x1E5,0x1E6,0x1E7,0x1E8,0x1E9,0x1EA,0x1EB,0x1EC,0x1ED,0x1EE,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x031,0x032,0x033,0x034,0x035,0x036,0x037,0x038,0x039,0x03A,0x03B,0x03C,0x03D,0x03E,0x03F,0x040,0x041,0x042,0x043,0x044,0x045,0x046,0x047,
0x048,0x049,0x04A,0x04B,0x04C,0x04D,0x04E,0x04F,0x050,0x051,0x052,0x053,0x054,0x055,0x056,0x057,0x058,0x059,0x05A,0x05B,0x05C,0x05D,0x05E,0x05F,0x060,0x061,0x1EF,0x1F0,0x1F1,0x1F2,0x1F3,0x1F4,
0x1F5,0x1F6,0x1F7,0x1F8,0x1F9,0x1FA,0x1FB,0x1FC,0x1FD,0x1FE,0x1FF,0x200,0x201,0x202,0x203,0x204,0x205,0x206,0x207,0x208,0x209,0x20A,0x20B,0x20C,0x20D,0x20E,0x20F,0x210,0x211,0x212,0x213,0x214,
0x215,0x216,0x217,0x218,0x219,0x21A,0x21B,0x21C,0x21D,0x21E,0x21F,0x220,0x221,0x222,0x223,0x224,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x062,
0x063,0x064,0x065,0x066,0x067,0x068,0x069,0x06A,0x06B,0x06C,0x06D,0x06E,0x06F,0x070,0x071,0x072,0x073,0x074,0x075,0x076,0x077,0x078,0x079,0x07A,0x07B,0x07C,0x07D,0x07E,0x07F,0x080,0x081,0x082,
0x083,0x084,0x085,0x086,0x087,0x088,0x089,0x08A,0x08B,0x08C,0x08D,0x08E,0x08F,0x090,0x091,0x092,0x225,0x226,0x227,0x228,0x229,0x22A,0x22B,0x22C,0x22D,0x22E,0x22F,0x230,0x231,0x232,0x233,0x234,
0x235,0x236,0x237,0x238,0x239,0x23A,0x23B,0x23C,0x23D,0x23E,0x23F,0x240,0x241,0x242,0x243,0x244,0x245,0x246,0x247,0x248,0x249,0x24A,0x24B,0x24C,0x24D,0x24E,0x24F,0x250,0x251,0x252,0x253,0x254,
0x255,0x256,0x257,0x258,0x259,0x25A,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x093,0x094,0x095,0x096,0x097,0x098,0x099,0x09A,0x09B,0x09C,0x09D,
0x09E,0x09F,0x0A0,0x0A1,0x0A2,0x0A3,0x0A4,0x0A5,0x0A6,0x0A7,0x0A8,0x0A9,0x0AA,0x0AB,0x0AC,0x0AD,0x0AE,0x0AF,0x0B0,0x0B1,0x0B2,0x0B3,0x0B4,0x0B5,0x0B6,0x0B7,0x0B8,0x0B9,0x0BA,0x0BB,0x0BC,0x0BD,
0x0BE,0x0BF,0x0C0,0x0C1,0x0C2,0x0C3,0x25B,0x25C,0x25D,0x25E,0x25F,0x260,0x261,0x262,0x263,0x264,0x265,0x266,0x267,0x268,0x269,0x26A,0x26B,0x26C,0x26D,0x26E,0x26F,0x270,0x271,0x272,0x273,0x274,
0x275,0x276,0x277,0x278,0x279,0x27A,0x27B,0x27C,0x27D,0x27E,0x27F,0x280,0x281,0x282,0x283,0x284,0x285,0x286,0x287,0x288,0x289,0x28A,0x28B,0x28C,0x28D,0x28E,0x28F,0x290,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x0C4,0x0C5,0x0C6,0x0C7,0x0C8,0x0C9,0x0CA,0x0CB,0x0CC,0x0CD,0x0CE,0x0CF,0x0D0,0x0D1,0x0D2,0x0D3,0x0D4,0x0D5,0x0D6,0x0D7,0x0D8,
0x0D9,0x0DA,0x0DB,0x0DC,0x0DD,0x0DE,0x0DF,0x0E0,0x0E1,0x0E2,0x0E3,0x0E4,0x0E5,0x0E6,0x0E7,0x0E8,0x0E9,0x0EA,0x0EB,0x0EC,0x0ED,0x0EE,0x0EF,0x0F0,0x0F1,0x0F2,0x0F3,0x0F4,0x291,0x292,0x293,0x294,
0x295,0x296,0x297,0x298,0x299,0x29A,0x29B,0x29C,0x29D,0x29E,0x29F,0x2A0,0x2A1,0x2A2,0x2A3,0x2A4,0x2A5,0x2A6,0x2A7,0x2A8,0x2A9,0x2AA,0x2AB,0x2AC,0x2AD,0x2AE,0x2AF,0x2B0,0x2B1,0x2B2,0x2B3,0x2B4,
0x2B5,0x2B6,0x2B7,0x2B8,0x2B9,0x2BA,0x2BB,0x2BC,0x2BD,0x2BE,0x2BF,0x2C0,0x2C1,0x2C2,0x2C3,0x2C4,0x2C5,0x2C6,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x0F5,0x0F6,0x0F7,0x0F8,0x0F9,0x0FA,0x0FB,0x0FC,0x0FD,0x0FE,0x0FF,0x100,0x101,0x102,0x103,0x104,0x105,0x106,0x107,0x108,0x109,0x10A,0x10B,0x10C,0x10D,0x10E,0x10F,0x110,0x111,0x112,0x113,
0x114,0x115,0x116,0x117,0x118,0x119,0x11A,0x11B,0x11C,0x11D,0x11E,0x11F,0x120,0x121,0x122,0x123,0x124,0x125,0x2C7,0x2C8,0x2C9,0x2CA,0x2CB,0x2CC,0x2CD,0x2CE,0x2CF,0x2D0,0x2D1,0x2D2,0x2D3,0x2D4,
0x2D5,0x2D6,0x2D7,0x2D8,0x2D9,0x2DA,0x2DB,0x2DC,0x2DD,0x2DE,0x2DF,0x2E0,0x2E1,0x2E2,0x2E3,0x2E4,0x2E5,0x2E6,0x2E7,0x2E8,0x2E9,0x2EA,0x2EB,0x2EC,0x2ED,0x2EE,0x2EF,0x2F0,0x2F1,0x2F2,0x2F3,0x2F4,
0x2F5,0x2F6,0x2F7,0x2F8,0x2F9,0x2FA,0x2FB,0x2FC,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x126,0x127,0x128,0x129,0x12A,0x12B,0x12C,0x12D,0x12E,
0x12F,0x130,0x131,0x132,0x133,0x134,0x135,0x136,0x137,0x138,0x139,0x13A,0x13B,0x13C,0x13D,0x13E,0x13F,0x140,0x141,0x142,0x143,0x144,0x145,0x146,0x147,0x148,0x149,0x14A,0x14B,0x14C,0x14D,0x14E,
0x14F,0x150,0x151,0x152,0x153,0x154,0x155,0x156,0x2FD,0x2FE,0x2FF,0x300,0x301,0x302,0x303,0x304,0x305,0x306,0x307,0x308,0x309,0x30A,0x30B,0x30C,0x30D,0x30E,0x30F,0x310,0x311,0x312,0x313,0x314,
0x315,0x316,0x317,0x318,0x319,0x31A,0x31B,0x31C,0x31D,0x31E,0x31F,0x320,0x321,0x322,0x323,0x324,0x325,0x326,0x327,0x328,0x329,0x32A,0x32B,0x32C,0x32D,0x32E,0x32F,0x330,0x331,0x332,0x000,0x000,
0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x157,0x158,0x159,0x15A,0x15B,0x15C,0x15D,0x15E,0x15F,0x160,0x161,0x162,0x163,0x164,0x165,0x166,0x167,0x168,0x169,
0x16A,0x16B,0x16C,0x16D,0x16E,0x16F,0x170,0x171,0x172,0x173,0x174,0x175,0x176,0x177,0x178,0x179,0x17A,0x17B,0x17C,0x17D,0x17E,0x17F,0x180,0x181,0x182,0x183,0x184,0x185,0x186,0x187,0x333,0x334,
0x335,0x336,0x337,0x338,0x339,0x33A,0x33B,0x33C,0x33D,0x33E,0x33F,0x340,0x341,0x342,0x343,0x344,0x345,0x346,0x347,0x348,0x349,0x34A,0x34B,0x34C,0x34D,0x34E,0x34F,0x350,0x351,0x352,0x353,0x354,
0x355,0x356,0x357,0x358,0x359,0x35A,0x35B,0x35C,0x35D,0x35E,0x35F,0x360,0x361,0x362,0x363,0x364,0x365,0x366,0x367,0x368,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
0x000,0x000,0x000,0x188,0x189,0x18A,0x18B,0x18C,0x18D,0x18E,0x18F,0x190,0x191,0x192,0x193,0x194,0x195,0x196,0x197,0x198,0x199,0x19A,0x19B,0x19C,0x19D,0x19E,0x19F,0x1A0,0x1A1,0x1A2,0x1A3,0x1A4,
0x1A5,0x1A6,0x1A7,0x1A8,0x1A9,0x1AA,0x1AB,0x1AC,0x1AD,0x1AE,0x1AF,0x1B0,0x1B1,0x1B2,0x1B3,0x1B4,0x1B5,0x1B6,0x1B7,0x1B8,0x369,0x36A,0x36B,0x36C,0x36D,0x36E,0x36F,0x370,0x371,0x372,0x373,0x374,
0x375,0x376,0x377,0x378,0x379,0x37A,0x37B,0x37C,0x37D,0x37E,0x37F,0x380,0x381,0x382,0x383,0x384,0x385,0x386,0x387,0x388,0x389,0x38A,0x38B,0x38C,0x38D,0x38E,0x38F,0x390,0x391,0x392,0x393,0x394,
0x395,0x396,0x397,0x398,0x399,0x39A,0x39B,0x39C,0x39D,0x39E,};

CDirectDraw	DirectDraw;

#define	COMUSE	TRUE
#define	DYNDLL	FALSE

CDirectDraw::BLTFUNC	CDirectDraw::NormalBltTable[] = {
	&CDirectDraw::Render8bpp_Normal,
	&CDirectDraw::Render16bpp_Normal,
	&CDirectDraw::Render24bpp_Normal,
	&CDirectDraw::Render32bpp_Normal,
};
CDirectDraw::BLTFUNC	CDirectDraw::ScanlineBltTable[] = {
	&CDirectDraw::Render8bpp_Scanline,
	&CDirectDraw::Render16bpp_Scanline,
	&CDirectDraw::Render24bpp_Scanline,
	&CDirectDraw::Render32bpp_Scanline,
};
CDirectDraw::BLTFUNC	CDirectDraw::DoubleBltTable[] = {
	&CDirectDraw::Render8bpp_Double,
	&CDirectDraw::Render16bpp_Double,
	&CDirectDraw::Render24bpp_Double,
	&CDirectDraw::Render32bpp_Double,
};
CDirectDraw::BLTFUNC	CDirectDraw::DoubleScanlineBltTable[] = {
	&CDirectDraw::Render8bpp_DoubleScanline,
	&CDirectDraw::Render16bpp_DoubleScanline,
	&CDirectDraw::Render24bpp_DoubleScanline,
	&CDirectDraw::Render32bpp_DoubleScanline,
};
CDirectDraw::BLTFUNC	CDirectDraw::nx2xSaIBltTable[] = {
	&CDirectDraw::Render8bpp_Double,
	&CDirectDraw::nx_2xSaI_16bpp,
	&CDirectDraw::Render24bpp_Double,
	&CDirectDraw::nx_2xSaI_32bpp,
};
CDirectDraw::BLTFUNC	CDirectDraw::nxSuper2xSaIBltTable[] = {
	&CDirectDraw::Render8bpp_Double,
	&CDirectDraw::nx_Super2xSaI_16bpp,
	&CDirectDraw::Render24bpp_Double,
	&CDirectDraw::nx_Super2xSaI_32bpp,
};
CDirectDraw::BLTFUNC	CDirectDraw::nxSuperEagleBltTable[] = {
	&CDirectDraw::Render8bpp_Double,
	&CDirectDraw::nx_SuperEagle_16bpp,
	&CDirectDraw::Render24bpp_Double,
	&CDirectDraw::nx_SuperEagle_32bpp,
};
CDirectDraw::BLTFUNC	CDirectDraw::nxScale2xBltTable[] = {
	&CDirectDraw::Render8bpp_Double,
	&CDirectDraw::nx_Scale2x_16bpp,
	&CDirectDraw::Render24bpp_Double,
	&CDirectDraw::nx_Scale2x_32bpp,
};

//////////////////////////////////////////////////////////////////////
// 僨僼僅儖僩僷儗僢僩
//////////////////////////////////////////////////////////////////////
CDirectDraw::PALBUF	CDirectDraw::m_PalDefault[] = {
	0x7F, 0x7F, 0x7F, 0x20, 0x00, 0xB0, 0x28, 0x00, 0xB8, 0x60, 0x10, 0xA0, 0x98, 0x20, 0x78, 0xB0,
	0x10, 0x30, 0xA0, 0x30, 0x00, 0x78, 0x40, 0x00, 0x48, 0x58, 0x00, 0x38, 0x68, 0x00, 0x38, 0x6C,
	0x00, 0x30, 0x60, 0x40, 0x30, 0x50, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0xBC, 0xBC, 0xBC, 0x40, 0x60, 0xF8, 0x40, 0x40, 0xFF, 0x90, 0x40, 0xF0, 0xD8, 0x40, 0xC0, 0xD8,
	0x40, 0x60, 0xE0, 0x50, 0x00, 0xC0, 0x70, 0x00, 0x88, 0x88, 0x00, 0x50, 0xA0, 0x00, 0x48, 0xA8,
	0x10, 0x48, 0xA0, 0x68, 0x40, 0x90, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0xFF, 0xFF, 0xFF, 0x60, 0xA0, 0xFF, 0x50, 0x80, 0xFF, 0xA0, 0x70, 0xFF, 0xF0, 0x60, 0xFF, 0xFF,
	0x60, 0xB0, 0xFF, 0x78, 0x30, 0xFF, 0xA0, 0x00, 0xE8, 0xD0, 0x20, 0x98, 0xE8, 0x00, 0x70, 0xF0,
	0x40, 0x70, 0xE0, 0x90, 0x60, 0xD0, 0xE0, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0xFF, 0xFF, 0xFF, 0x90, 0xD0, 0xFF, 0xA0, 0xB8, 0xFF, 0xC0, 0xB0, 0xFF, 0xE0, 0xB0, 0xFF, 0xFF,
	0xB8, 0xE8, 0xFF, 0xC8, 0xB8, 0xFF, 0xD8, 0xA0, 0xFF, 0xF0, 0x90, 0xC8, 0xF0, 0x80, 0xA0, 0xF0,
	0xA0, 0xA0, 0xFF, 0xC8, 0xA0, 0xFF, 0xF0, 0xA0, 0xA0, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

//////////////////////////////////////////////////////////////////////
// 僉儍儔僋僞
//////////////////////////////////////////////////////////////////////
static	BYTE	lzSight[] = {
#include "lzSight.h"
};
static	BYTE	lzAscii[] = {
#include "lzAscii.h"
};
static	BYTE	lzTVlayer[] = {
#include "lzTVlayer.h"
};

// 儊儌儕偐傜侾僶僀僩庢偭偰偔傞
static __inline INT	mgetc( LPBYTE& inbuf, INT& bufsize )
{
	if( bufsize <= 0 )
		return	-1;
	bufsize--;
	return	(INT)*inbuf++;
}

//
// LZSS夝搥儖乕僠儞
//
#define	N	4096
#define	F	18

static	void	LZdecode( BYTE *inbuf, BYTE *outbuf, INT length )
{
INT	i, j, k, r, c;
WORD	flags;
BYTE	text[N+F-1];
INT	outsize = 0;

	for( i = 0; i < N - F; i++ )
		text[i] = 0;
	r = N - F;
	flags = 0;

	for(;;) {
		if( ((flags >>= 1) & 256) == 0 ) {
			if( (c = mgetc( inbuf, length )) < 0 )
				break;
			flags = c | 0xff00;
		}
		if( flags & 1 ) {
			if( (c = mgetc( inbuf, length )) < 0 )
				break;
			*outbuf++ = c;
			text[r++] = c;
			r &= (N-1);

			outsize++;
		} else {
			if( (i = mgetc( inbuf, length )) < 0 )
				break;
			if( (j = mgetc( inbuf, length )) < 0 )
				break;
			i |= ((j & 0xf0)<<4);
			j  =  (j & 0x0f)+2;
			for( k = 0; k <= j; k++ ) {
				c = text[(i+k) & (N-1)];
				*outbuf++ = c;
				text[r++] = c;
				r &= (N-1);

				outsize++;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// 峔抸/徚柵
//////////////////////////////////////////////////////////////////////

CDirectDraw::CDirectDraw()
{
	font = pFont;
	// General
	m_hWnd           = NULL;

	m_bUseHEL        = FALSE;
	m_bSystemMemory  = FALSE;

	m_bScreenMode    = FALSE;
	m_bGDI           = FALSE;
	m_bAspect        = FALSE;
	m_bAllLine       = FALSE;
	m_bMaxZoom       = FALSE;
	m_bDoubleSize    = FALSE;
	m_bTVFrameMode   = FALSE;
	m_bScanlineMode  = FALSE;
	m_nScanlineColor = 75;
	m_bZapper        = FALSE;
	m_bZapperDraw    = FALSE;

	m_ZapperPosX = m_ZapperPosY = -1;

	m_bWindowVSync   = FALSE;

	m_bChangeMode    = FALSE;
	m_bDraw          = FALSE;

	m_bNoRestore     = FALSE;

	// DirectDraw
	m_hDDraw        = NULL;

	m_lpDD          = NULL;
	m_lpDDPrimary   = NULL;
	m_lpDDBack      = NULL;
	m_lpDDRender    = NULL;
	m_lpDDAscii     = NULL;
	m_lpDDZapper    = NULL;
	m_lpDDTV        = NULL;
	m_lpDDClipper   = NULL;
	m_lpDDClipper2  = NULL;
	m_lpDDPalette   = NULL;

	m_lpRender = NULL;
	m_lpRenderDelta = NULL;
	m_bDeltaUpdate = FALSE;

	m_hPalette = NULL;

	m_DisplayModeNum = 0;

	m_bForceWrite = FALSE;
	// Filter
	m_nBltFilter = 0;

	// 僨僼僅儖僩夝憸搙
	m_dwDisplayWidth  = 640;
	m_dwDisplayHeight = 480;
	m_dwDisplayDepth  = 16;
	m_dwDisplayRate   = 0;	// 儕僼儗僢僔儏儗乕僩巜掕柍偟

	// LineColormode
	::memset( m_LineColormode, 0, sizeof(m_LineColormode) );

	// 僷儗僢僩
	m_nPaletteMode = 0;
	m_bMonoMode = FALSE;

	::memcpy( m_PaletteBuf, m_PalDefault, sizeof(m_PaletteBuf) );

	ZEROMEMORY( m_cpPalette, sizeof(m_cpPalette) );
	ZEROMEMORY( m_mpPalette, sizeof(m_mpPalette) );
	ZEROMEMORY( m_cnPalette, sizeof(m_cnPalette) );
	ZEROMEMORY( m_csPalette, sizeof(m_csPalette) );
	ZEROMEMORY( m_mnPalette, sizeof(m_mnPalette) );
	ZEROMEMORY( m_msPalette, sizeof(m_msPalette) );

	// 偦偺懠
	m_bDiskAccessLamp = FALSE;

	ZEROMEMORY( m_szInfo, sizeof(m_szInfo) );
	ZEROMEMORY( m_szMess, sizeof(m_szMess) );

	// MMX detect
	{
		m_bMMX = FALSE;

		DWORD	flag1, flag2;
		__asm {
			pushfd
			pop	eax
			mov	flag1, eax
			xor	eax, 0x00200000
			push	eax
			popfd
			pushfd
			pop	eax
			mov	flag2, eax
		}

		if( flag1 != flag2 ) {
			DWORD	flag;
			__asm {
				mov	eax, 1
				cpuid
				mov	flag, edx
			}
			if( flag & 0x00800000 ) {
				m_bMMX = TRUE;
			}
		}
	}

	// TEST
//	m_nBltFilter = BLTFILTER_2XSAI;
//	m_nBltFilter = BLTFILTER_SUPER2XSAI;
//	m_nBltFilter = BLTFILTER_SUPEREAGLE;
//	m_nBltFilter = BLTFILTER_SCALE2X;

#if	COMUSE
	COM::AddRef();
#endif
}

CDirectDraw::~CDirectDraw()
{
	ReleaseDDraw();

#if	COMUSE
	COM::Release();
#endif
}

//////////////////////////////////////////////////////////////////////
// 儊儞僶娭悢
//////////////////////////////////////////////////////////////////////

// 僐乕儖僶僢僋娭悢
HRESULT	WINAPI	CDirectDraw::EnumModesCallback( LPDDSURFACEDESC2 lpDDSurfaceDesc, LPVOID lpContext )
{
	// This億僀儞僞傪庴偗庢傞
	CDirectDraw* pDD = (CDirectDraw*)lpContext;

	// 256x240埲壓偺夝憸搙偼彍奜(ex. 320x200)
	if( lpDDSurfaceDesc->dwWidth < 256 || lpDDSurfaceDesc->dwHeight < 240 )
		return	DDENUMRET_OK;

	// 8,16,24,32bit埲奜偼彍奜(4bit偑偁傞偹^^;)
	if( !(lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == 8
	   || lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == 16
	   || lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == 24
	   || lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount == 32) )
		return	DDENUMRET_OK;

	// 僗僋僄傾僒僀僘偺夝憸搙埲奜偼彍奜(ex. 640x400) (1280x1024偼慖戰壜擻偵)
//	if( lpDDSurfaceDesc->dwWidth*3 != lpDDSurfaceDesc->dwHeight*4 && !(lpDDSurfaceDesc->dwWidth == 1280 && lpDDSurfaceDesc->dwHeight == 1024) )
//		return	DDENUMRET_OK;

	// 僒僀僘乛價僢僩怺搙乛儕僼儗僢僔儏儗乕僩乛僺僋僙儖僼僅乕儅僢僩偺僐僺乕
	DISPLAYMODE	mode;
	mode.dwWidth  = lpDDSurfaceDesc->dwWidth;
	mode.dwHeight = lpDDSurfaceDesc->dwHeight;
	mode.dwDepth  = lpDDSurfaceDesc->ddpfPixelFormat.dwRGBBitCount;
	mode.dwRate   = lpDDSurfaceDesc->dwRefreshRate;
	pDD->m_DisplayModes.push_back( mode );

	// 楍嫇悢偑挻偊偨応崌偼僉儍儞僙儖
	if( ++pDD->m_DisplayModeNum > CDirectDraw::DD_DISPLAYMODEMAX-1 ) {
//		DEBUGOUT( "CDirectDraw:Maximum display modes over.\n" );
		return	DDENUMRET_CANCEL;
	}
	return	DDENUMRET_OK;
}

// DirectDraw偺弶婜壔
BOOL	CDirectDraw::InitialDDraw( HWND hWnd )
{
	try {
		// DirectDraw僆僽僕僃僋僩偺嶌惉

#if	!DYNDLL
#if	!COMUSE
		if( !m_bUseHEL ) {
			if( DirectDrawCreateEx(NULL, (LPVOID*)&m_lpDD, IID_IDirectDraw7, NULL) != DD_OK ) {
				m_lpDD = NULL;
				throw	"CDirectDraw:DirectDrawCreateEx failed.";
			}
		} else {
			if( DirectDrawCreateEx((LPGUID)DDCREATE_EMULATIONONLY, (LPVOID*)&m_lpDD, IID_IDirectDraw7, NULL) != DD_OK ) {
				m_lpDD = NULL;
				throw	"CDirectDraw:DirectDrawCreateEx failed.";
			}
		}
#else
		// COM揑棙梡(儔僀僽儔儕傪儕儞僋偡傞偩偗偱婲摦偑抶偔側傞偺偱...)
//		COM::AddRef();

		// CLSID_DirectDraw偱偼柍偔CLSID_DirectDraw7傪巊傢側偄偲懯栚
		if( ::CoCreateInstance( CLSID_DirectDraw7, NULL, CLSCTX_ALL, IID_IDirectDraw7, (LPVOID*)&m_lpDD ) != S_OK ) {
			m_lpDD = NULL;
			throw	"CDirectDraw:DirectX 7.0 or greater is required.";
//			throw	"CDirectDraw:CoCreateInstance failed.";
		}
		if( !m_bUseHEL ) {
			if( m_lpDD->Initialize( NULL ) != DD_OK )
				throw	"CDirectDraw:IDirectDraw7->Initialize failed.";
		} else {
			if( m_lpDD->Initialize( (LPGUID)DDCREATE_EMULATIONONLY ) != DD_OK )	// HEL
				throw	"CDirectDraw:IDirectDraw7->Initialize failed.";
		}
#endif
#else
		// DLL傪摦揑儘乕僪偟偰棙梡
		if( !(m_hDDraw = ::LoadLibrary( "DDRAW.DLL" )) ) {
			throw	"CDirectDraw:DirectX 7.0 or greater is required.";
		}

typedef HRESULT(WINAPI * DIRECTDRAWCREATEEX)( GUID*, VOID**, REFIID, IUnknown* );

		DIRECTDRAWCREATEEX DirectDrawCreateEx = (DIRECTDRAWCREATEEX)GetProcAddress( m_hDDraw, "DirectDrawCreateEx" );
		if( !DirectDrawCreateEx ) {
			::FreeLibrary( m_hDDraw );
			m_hDDraw = NULL;
			throw	"CDirectDraw:DirectX 7.0 or greater is required.";
		}

		if( !m_bUseHEL ) {
			if( DirectDrawCreateEx(NULL, (LPVOID*)&m_lpDD, IID_IDirectDraw7, NULL) != DD_OK ) {
				m_lpDD = NULL;
				throw	"CDirectDraw:DirectX 7.0 or greater is required.";
			}
		} else {
			if( DirectDrawCreateEx((LPGUID)DDCREATE_EMULATIONONLY, (LPVOID*)&m_lpDD, IID_IDirectDraw7, NULL) != DD_OK ) {
				m_lpDD = NULL;
				throw	"CDirectDraw:DirectX 7.0 or greater is required.";
			}
		}
#endif
		// 儌僯僞夝憸搙偺挷嵏
		DDSURFACEDESC2	ddsd;
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		if( m_lpDD->GetDisplayMode( &ddsd ) != DD_OK )
			throw	"CDirectDraw:GetDisplayMode failed.";

		if( ddsd.ddpfPixelFormat.dwRGBBitCount < 8 )
			throw	"CDirectDraw:Unsupported display mode.";

		// 棙梡壜擻側僨傿僗僾儗僀儌乕僪偺庢摼
		m_DisplayModeNum = 0;
		if( m_lpDD->EnumDisplayModes(DDEDM_REFRESHRATES, NULL, (LPVOID)this, (LPDDENUMMODESCALLBACK2)EnumModesCallback) != DD_OK )
			throw	"CDirectDraw:EnumDisplayModes failed.";
		if( !m_DisplayModeNum )
			throw	"CDirectDraw:No display modes available.";

		m_hWnd = hWnd;

		// 僨僼僅儖僩僷儗僢僩愝掕
		memcpy( m_PaletteBuf, m_PalDefault, sizeof(m_PalDefault) );

		// Render screen
		if( !m_lpRender ) {
			if( !(m_lpRender = (LPBYTE)malloc( RENDER_WIDTH*RENDER_HEIGHT )) )
				throw	"CDirectDraw::Out of memory.";

		}
		::memset( m_lpRender, 0x3F, RENDER_WIDTH*RENDER_HEIGHT );

		// Render delta screen
		if( !m_lpRenderDelta ) {
			if( !(m_lpRenderDelta = (LPBYTE)malloc( DELTA_WIDTH*DELTA_HEIGHT*sizeof(DWORD) )) )
				throw	"CDirectDraw::Out of memory.";
		}
		::memset( m_lpRenderDelta, 0xFF, DELTA_WIDTH*DELTA_HEIGHT*sizeof(DWORD) );
		m_bDeltaUpdate = FALSE;

		// LineColormode
		::memset( m_LineColormode, 0, sizeof(m_LineColormode) );
	} catch( char *str ) {
		m_DisplayModeNum = 0;
		RELEASE( m_lpDD );
		::MessageBox( hWnd, str, "ERROR", MB_ICONERROR|MB_OK );

		return	FALSE;
	}

	return	TRUE;
}

// DirectDraw偺奐曻
void	CDirectDraw::ReleaseDDraw()
{
	ReleaseSurface();

	if( m_lpDD ) {
		RELEASE( m_lpDD );
	}
#if	COMUSE
//	COM::Release();
#endif

#if	DYNDLL
	if( m_hDDraw ) {
		m_hDDraw = NULL;
		::FreeLibrary( m_hDDraw );
	}
#endif

	FREE( m_lpRenderDelta );
	FREE( m_lpRender );
//	m_hWnd = NULL;
}

// 僒乕僼僃僗偺弶婜壔
BOOL	CDirectDraw::InitialSurface( BOOL bScreenMode )
{
INT	i;
DDSURFACEDESC2	ddsd;
DDSCAPS2	ddscaps;
DDBLTFX		ddbltfx;

	try {
		if( !m_lpDD )
			throw "CDirectDraw:DirectDraw object uninitialized.";

		m_bGDI = FALSE;
		m_bScreenMode = bScreenMode;
		m_bMessage = FALSE;

		if( !m_bScreenMode ) {
		// 僂僀儞僪僂
			// 嫤挷儌乕僪
			if( m_lpDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL ) != DD_OK )
				throw	"CDirectDraw:SetCooperativeLevel failed.";

			// 僾儔僀儅儕僒乕僼僃僗
			ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
			ddsd.dwSize = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags = DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
			if( m_lpDD->CreateSurface( &ddsd, &m_lpDDPrimary, NULL ) != DD_OK )
				throw	"CDirectDraw:CreateSurface failed.";

			// 僋儕僢僷乕偺嶌惉
			if( m_lpDD->CreateClipper( 0, &m_lpDDClipper, NULL ) != DD_OK )
				throw	"CDirectDraw:CreateClipper failed.";

			m_lpDDClipper->SetHWnd( 0, m_hWnd );
			m_lpDDPrimary->SetClipper( m_lpDDClipper );
			RELEASE( m_lpDDClipper );
		} else {
		// 僼儖僗僋儕乕儞
			// 攔懠儌乕僪
			if( m_lpDD->SetCooperativeLevel( m_hWnd, DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN ) != DD_OK )
				throw	"CDirectDraw:SetCooperativeLevel error";

			// 夋柺夝憸搙偺愝掕
			if( m_lpDD->SetDisplayMode( m_dwDisplayWidth, m_dwDisplayHeight,
						    m_dwDisplayDepth, m_dwDisplayRate,
							DDSDM_STANDARDVGAMODE ) != DD_OK ) {
				// 幐攕偟偨傜儕僼儗僢僔儏儗乕僩巜掕傪柍偔偟偰傕偆堦搙傗偭偰傒傞
				if( m_lpDD->SetDisplayMode( m_dwDisplayWidth, m_dwDisplayHeight,
							    m_dwDisplayDepth, 0,
								DDSDM_STANDARDVGAMODE ) != DD_OK ) {
					throw	"CDirectDraw:SetDisplayMode failed.";
				} else {
					m_dwDisplayRate = 0;
				}
			}

			// 僾儔僀儅儕僒乕僼僃僗偲僶僢僋僒乕僼僃僗偺嶌惉
			ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
			ddsd.dwSize = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
			ddsd.dwBackBufferCount = 1;
			if( m_lpDD->CreateSurface(&ddsd, &m_lpDDPrimary, NULL) != DD_OK )
				throw	"CDirectDraw:CreateSurface failed.";

			ZEROMEMORY( &ddscaps, sizeof(DDSCAPS2) );
			ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
			if( m_lpDDPrimary->GetAttachedSurface(&ddscaps, &m_lpDDBack) != DD_OK )
				throw	"CDirectDraw:GetAttachedSurface failed.";
		}

		// offscreen surface
		m_bForceWrite = FALSE;

		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		if( !m_bSystemMemory ) {
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		} else {
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			m_bForceWrite = TRUE;	// 懡抜儗儞僟儕儞僌偟側偄
		}
		ddsd.dwWidth   = SCREEN_WIDTH*2;
		ddsd.dwHeight  = SCREEN_HEIGHT*2;
		if( m_lpDD->CreateSurface(&ddsd, &m_lpDDRender, NULL) != DD_OK ) {
			// 僔僗僥儉儊儌儕偵傕庢傟側偄偭偰偐....
			if( ddsd.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY )
				throw	"CDirectDraw:CreateSurface failed.";

			// 價僨僆儊儌儕偵庢傟側偄帪丆偟傚偆偑側偄偺偱僔僗僥儉儊儌儕偵庢傞
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			if( m_lpDD->CreateSurface(&ddsd, &m_lpDDRender, NULL) != DD_OK )
				throw	"CDirectDraw:CreateSurface failed.";

			m_bForceWrite = TRUE;	// 懡抜儗儞僟儕儞僌偟側偄
		}
		if( !m_bSystemMemory ) {
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		} else {
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		}
		if( m_lpDD->CreateSurface(&ddsd, &m_lpDDRender2, NULL) != DD_OK ) {
			// 僔僗僥儉儊儌儕偵傕庢傟側偄偭偰偐....
			if( ddsd.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY )
				throw	"CDirectDraw:CreateSurface failed.";

			// 價僨僆儊儌儕偵庢傟側偄帪丆偟傚偆偑側偄偺偱僔僗僥儉儊儌儕偵庢傞
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			if( m_lpDD->CreateSurface(&ddsd, &m_lpDDRender2, NULL) != DD_OK )
				throw	"CDirectDraw:CreateSurface failed.";
		}

		typedef	struct tagDDRGNDATA {
			RGNDATAHEADER	rdh;
			RECT		rect[8];
		} DDRGNDATA;

		DDRGNDATA	rgn;
		rgn.rdh.dwSize = sizeof(RGNDATAHEADER);
		rgn.rdh.iType  = RDH_RECTANGLES;
		rgn.rdh.nCount = 1;
		rgn.rdh.nRgnSize = sizeof(RECT);
		rgn.rdh.rcBound.left   = 0;
		rgn.rdh.rcBound.top    = 0;
		rgn.rdh.rcBound.right  = ddsd.dwWidth;
		rgn.rdh.rcBound.bottom = ddsd.dwHeight;
		rgn.rect[0].left   = 0;
		rgn.rect[0].top    = 0;
		rgn.rect[0].right  = ddsd.dwWidth;
		rgn.rect[0].bottom = ddsd.dwHeight;

		// Rendering surface clipper
		if( m_lpDD->CreateClipper( 0, &m_lpDDClipper2, NULL ) != DD_OK )
			throw	"CDirectDraw:CreateClipper failed.";

		m_lpDDClipper2->SetClipList( (LPRGNDATA)&rgn, 0 );
		m_lpDDRender->SetClipper( m_lpDDClipper2 );

		// Ascii surface(6x6x64character)
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
//		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth   = 6*8;
		ddsd.dwHeight  = 6*8;
		if( m_lpDD->CreateSurface(&ddsd, &m_lpDDAscii, NULL) != DD_OK )
			throw	"CDirectDraw:CreateSurface failed.";
		// Color key
		DDCOLORKEY	ddck;
		ZEROMEMORY( &ddck, sizeof(DDCOLORKEY) );
		ddck.dwColorSpaceLowValue  = 0;
		ddck.dwColorSpaceHighValue = 0;
		m_lpDDAscii->SetColorKey( DDCKEY_SRCBLT, &ddck );

		// Zapper surface
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
//		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth   = 16;
		ddsd.dwHeight  = 16;
		if( m_lpDD->CreateSurface(&ddsd, &m_lpDDZapper, NULL) != DD_OK )
			throw	"CDirectDraw:CreateSurface failed.";
		// Color key
		ZEROMEMORY( &ddck, sizeof(DDCOLORKEY) );
		ddck.dwColorSpaceLowValue  = 0;
		ddck.dwColorSpaceHighValue = 0;
		m_lpDDZapper->SetColorKey( DDCKEY_SRCBLT, &ddck );

		// TV
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
		if( !m_bSystemMemory ) {
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
		} else {
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		}
		ddsd.dwWidth   = 512;
		ddsd.dwHeight  = 480;
		if( m_lpDD->CreateSurface(&ddsd, &m_lpDDTV, NULL) != DD_OK ) {
			// 僔僗僥儉儊儌儕偵傕庢傟側偄偭偰偐....
			if( ddsd.ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY )
				throw	"CDirectDraw:CreateSurface failed.";

			// 價僨僆儊儌儕偵庢傟側偄帪丆偟傚偆偑側偄偺偱僔僗僥儉儊儌儕偵庢傞
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			if( m_lpDD->CreateSurface(&ddsd, &m_lpDDTV, NULL) != DD_OK )
				throw	"CDirectDraw:CreateSurface failed.";
		}

		// Color key
		ZEROMEMORY( &ddck, sizeof(DDCOLORKEY) );
		ddck.dwColorSpaceLowValue  = 0;
		ddck.dwColorSpaceHighValue = 0;
		m_lpDDTV->SetColorKey( DDCKEY_SRCBLT, &ddck );

		// DirectDraw/GDI Palette
		ZEROMEMORY( &m_logPalette, sizeof(m_logPalette) );
		m_logPalette.palVersion    = 0x0300;
		m_logPalette.palNumEntries = 256;

		HDC	hdc = ::GetDC( NULL );
		GetSystemPaletteEntries( hdc, 0, 256, m_logPalette.pe );
		ReleaseDC( NULL, hdc );

		for( i = 0; i < 10; i++ ) {
			m_logPalette.pe[i    ].peFlags = PC_EXPLICIT;
			m_logPalette.pe[i+246].peFlags = PC_EXPLICIT;
		}
		for( i = 10; i < 246; i++ ) {
			m_logPalette.pe[i].peRed   = 0;
			m_logPalette.pe[i].peGreen = 0;
			m_logPalette.pe[i].peBlue  = 0;
			if( i >= 0x10 && i < 0x20 ) {
				m_logPalette.pe[i].peRed   = (i-0x10)*0x10;
				m_logPalette.pe[i].peGreen = (i-0x10)*0x10;
				m_logPalette.pe[i].peBlue  = (i-0x10)*0x10;
				m_logPalette.pe[i].peFlags = PC_RESERVED;
			} else if( i >= 0x40 && i < 0xC0 ) {
				m_logPalette.pe[i].peFlags = PC_RESERVED;
			} else {
				m_logPalette.pe[i].peFlags = PC_NOCOLLAPSE;
			}
		}

		// Surface clear
		ddbltfx.dwSize = sizeof(DDBLTFX);
		ddbltfx.dwFillColor = 0;
		if( m_lpDDBack ) {
			m_lpDDBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);
		}
		m_lpDDRender->Blt(NULL, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);
		m_lpDDRender2->Blt(NULL, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);

		// Palette object
		DDSURFACEDESC2	ddsd;
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		m_lpDDPrimary->GetSurfaceDesc(&ddsd);

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 ) {
			if( !m_bScreenMode ) {
				if( (m_hPalette = CreatePalette( (LOGPALETTE *)&m_logPalette )) == NULL )
					throw	"CDirectDraw:CreatePalette failed.";
			} else {
				if( m_lpDD->CreatePalette( DDPCAPS_8BIT, m_logPalette.pe, &m_lpDDPalette, NULL ) != DD_OK )
					throw	"CDirectDraw:CreatePalette error";
				if( m_lpDDPrimary->SetPalette( m_lpDDPalette ) != DD_OK )
					throw	"CDirectDraw:SetPalette failed.";
			}
		}

		// Palette calculate
		CalcPaletteTable();
		// Palette Realize
		RealizePalette();
		m_bPaletteUpdate = TRUE;

		// Character setup
		SetLZSSChar( lzAscii, m_lpDDAscii );
		SetLZSSChar( lzSight, m_lpDDZapper );
		SetLZSSChar( lzTVlayer, m_lpDDTV );

		// 嵞昤夋偺堊
		m_bDeltaUpdate = TRUE;
	} catch( char *str ) {
		ReleaseSurface();

//		DEBUGOUT( "%s\n", str );
		::MessageBox( m_hWnd, str, "ERROR", MB_ICONERROR|MB_OK );

		return	FALSE;
	}
//	DEBUGOUT( "CDirectDraw:InitialSurface complete.\n" );

	return	TRUE;
}

// 僒乕僼僃僗偺奐曻
BOOL	CDirectDraw::ReleaseSurface( void )
{
	if( !m_lpDD )
		return	FALSE;

	GDIDELETE( m_hPalette );
	RELEASE( m_lpDDPalette );

	RELEASE( m_lpDDClipper2 );
	RELEASE( m_lpDDClipper );

	RELEASE( m_lpDDTV );
	RELEASE( m_lpDDZapper );
	RELEASE( m_lpDDAscii );
	RELEASE( m_lpDDRender2 );
	RELEASE( m_lpDDRender );
	RELEASE( m_lpDDBack );
	RELEASE( m_lpDDPrimary );

	return	TRUE;
}

// 僒乕僼僃僗偺儕僗僩傾僒僽
BOOL	CDirectDraw::RestoreSurfaceSub( LPDIRECTDRAWSURFACE7 lpSurface )
{
	if( lpSurface ) {
		if( lpSurface->IsLost() == DDERR_SURFACELOST ) {
			// 儘僗僩偟偰偄偨傜儕僗僩傾偡傞
//			DEBUGOUT( "CDirectDraw:Restore surface...." );
			if( lpSurface->Restore() == DD_OK ) {
//				DEBUGOUT( "Ok.\n" );
			} else {
//				DEBUGOUT( "Failed.\n" );
			}
			return	FALSE;
		}
	}
	return	TRUE;
}

// 僒乕僼僃僗偺儕僗僩傾
BOOL	CDirectDraw::RestoreSurface()
{
	if( !m_lpDD ) return FALSE;

	if( m_bNoRestore )
		return	FALSE;

	BOOL	bRet = TRUE;

	// 儘僗僩偟偰偨傜撪梕偼幐傢傟偰偄傞偺偱昤夋傪僉儍儞僙儖偟偰偹
	if( !RestoreSurfaceSub( m_lpDDRender ) ) {
		bRet = FALSE;
	}
	if( !RestoreSurfaceSub( m_lpDDRender2 ) ) {
		bRet = FALSE;
	}
	if( !RestoreSurfaceSub( m_lpDDBack ) ) {
		bRet = FALSE;
	}
	if( !RestoreSurfaceSub( m_lpDDPrimary ) ) {
		bRet = FALSE;
	}
	if( !RestoreSurfaceSub( m_lpDDAscii ) ) {
		SetLZSSChar( lzAscii, m_lpDDAscii );
		bRet = FALSE;
	}
	if( !RestoreSurfaceSub( m_lpDDZapper ) ) {
		SetLZSSChar( lzSight, m_lpDDZapper );
		bRet = FALSE;
	}
	if( !RestoreSurfaceSub( m_lpDDTV ) ) {
		SetLZSSChar( lzTVlayer, m_lpDDTV );
		bRet = FALSE;
	}

	if( !bRet ) {
		// 嵞昤夋偺堊
		m_bDeltaUpdate = TRUE;
	}

	return	bRet;
}

//
// 僨傿僗僾儗僀儌乕僪曄峏奐巒
//
BOOL	CDirectDraw::BeginDisplayChange()
{
	if( !m_lpDD )
		return	FALSE;

	// 婛偵曄峏拞傗側偄偐
	if( m_bChangeMode )
		return	FALSE;

	// 曄峏拞偠傖
	m_bChangeMode = TRUE;

	if( m_bScreenMode ) {
		m_lpDD->RestoreDisplayMode();
	}

	// 僒乕僼僃僗偺奐曻
	ReleaseSurface();

	// 嫤挷儌乕僪
	m_lpDD->SetCooperativeLevel( NULL, DDSCL_NORMAL );

	return	TRUE;
}

//
// 僨傿僗僾儗僀儌乕僪曄峏廔椆偲僒乕僼僃僗偺嵞峔抸
//
BOOL	CDirectDraw::EndDisplayChange()
{
	if( !m_lpDD || !m_bChangeMode )
		return	FALSE;

	// Rebuild surface
	if( !InitialSurface( m_bScreenMode ) ) {
		m_bChangeMode = FALSE;
		return	FALSE;
	}

	// Wait
	::Sleep( 250 );

	// Change mode complete
	m_bChangeMode = FALSE;

	return	TRUE;
}

//
// WM_DISPLAYCHANGE僴儞僪儔偐傜屇偽傟傞
//
BOOL	CDirectDraw::OnChangeDisplayMode()
{
	// 帺暘帺恎偱曄峏偟偰偄傞帪偼僉儍儞僙儖
	if( m_bChangeMode )
		return	TRUE;

	// DirectDraw僆僽僕僃僋僩偑柍偗傟偽堄枴柍偟
	if( !m_lpDD )
		return	FALSE;

//	DEBUGOUT( "CDirectDraw:OnChangeDisplayMode\n" );

	// 嫤挷儌乕僪偺僠僃僢僋
	HRESULT	hr = m_lpDD->TestCooperativeLevel();

	if( !m_bScreenMode ) {
	// 僂僀儞僪僂
		if( hr == DDERR_EXCLUSIVEMODEALREADYSET ) {
			// 懠偵攔懠儌乕僪栰榊偑偍傞偺偱偍偟傑偄
//			DEBUGOUT( "CDirectDraw:DDERR_EXCLUSIVEMODEALREADYSET\n" );
			// 儕僗僩傾偟偪傖偄傗傫
			m_bNoRestore = TRUE;
			return	TRUE;
		} else if( hr == DDERR_WRONGMODE || hr == DD_OK ) {
			// 晛捠偺僨傿僗僾儗僀儌乕僪偺曄峏(壗屘偐Window儊僢僙乕僕偩偲DD_OK側帪偑偁傞)
//			DEBUGOUT( "CDirectDraw:DDERR_WRONGMODE\n" );
			// 儕僗僩傾偟偰偊偊傛
			m_bNoRestore = FALSE;

			// 僒乕僼僃僗偺奐曻
			ReleaseSurface();

			// 僒乕僼僃僗偺弶婜壔
			BOOL	bRet = InitialSurface( m_bScreenMode );

			if( bRet ) {
//				DEBUGOUT( "CDirectDraw:InitialSurface ok.\n" );
			} else {
//				DEBUGOUT( "CDirectDraw:InitialSurface failed.\n" );
			}

			return	bRet;
		} else if( hr == DDERR_INVALIDOBJECT ) {
//			DEBUGOUT( "CDirectDraw:DDERR_INVALIDOBJECT\n" );
			return	FALSE;
		} else if( hr == DDERR_NOEXCLUSIVEMODE ) {
//			DEBUGOUT( "CDirectDraw:DDERR_NOEXCLUSIVEMODE\n" );
		} else {
//			DEBUGOUT( "CDirectDraw:Unknown error. hr=%08X偨\n", hr );
		}
		// 椙偔暘偐傜傫偺偱偲傝偁偊偢僒乕僼僃僗傪儕僗僩傾
		m_bNoRestore = FALSE;
		RestoreSurface();

		return	TRUE;
	} else {
	// 僼儖僗僋儕乕儞
		if( hr == DDERR_NOEXCLUSIVEMODE ) {
			// 僼儖僗僋儕乕儞儌乕僪偐傜敳偗偨帪
//			DEBUGOUT( "CDirectDraw:DDERR_NOEXCLUSIVEMODE\n" );
			return	TRUE;
		}

		if( hr == DD_OK ) {
			// 儕僗僩傾偟傠偭偰帠偐...
			// 僒乕僼僃僗傪儕僗僩傾
			RestoreSurface();

			return	TRUE;
		}

		// 晄柧側尨場
		return	FALSE;
	}

	return	TRUE;
}

void	CDirectDraw::SetDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, DWORD dwRate )
{
	m_dwDisplayWidth  = dwWidth;
	m_dwDisplayHeight = dwHeight;
	m_dwDisplayDepth  = dwDepth;
	m_dwDisplayRate   = dwRate;
}

void	CDirectDraw::GetDisplayMode( DWORD& dwWidth, DWORD& dwHeight, DWORD& dwDepth, DWORD& dwRate )
{
	dwWidth  = m_dwDisplayWidth;
	dwHeight = m_dwDisplayHeight;
	dwDepth  = m_dwDisplayDepth;
	dwRate   = m_dwDisplayRate;
}

BOOL	CDirectDraw::GetDisplayMode( INT no, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwDepth, DWORD& dwRate )
{
	if( m_DisplayModes.size() < no )
		return	FALSE;

	dwWidth  = m_DisplayModes[no].dwWidth;
	dwHeight = m_DisplayModes[no].dwHeight;
	dwDepth  = m_DisplayModes[no].dwDepth;
	dwRate   = m_DisplayModes[no].dwRate;

	return	TRUE;
}

INT	CDirectDraw::GetMatchDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, DWORD dwRate )
{
	for( int i = 0; i < m_DisplayModes.size(); i++ ) {
		if( m_DisplayModes[i].dwWidth  == dwWidth
		 || m_DisplayModes[i].dwHeight == dwHeight
		 || m_DisplayModes[i].dwDepth  == dwDepth
		 || m_DisplayModes[i].dwRate   == dwRate )
			return	i;
	}

	return	-1;
}

BOOL	CDirectDraw::IsNowDisplayMode( DWORD dwWidth, DWORD dwHeight, DWORD dwDepth, DWORD dwRate )
{
	if( m_dwDisplayWidth == dwWidth && m_dwDisplayHeight == dwHeight
	 && m_dwDisplayDepth == dwDepth && m_dwDisplayRate == dwRate )
		return	TRUE;
	return	FALSE;
}

// 價僢僩埵抲偺庢摼
void	CDirectDraw::GetBitMask( DWORD val, int& shift, int& bits )
{
	shift = 0;
	while( !(val & (1<<shift)) && (shift<32) ) {
		shift++;
	}

	bits = 32;
	while( !(val & (1<<(bits-1))) && (bits>0) ) {
		bits--;
	}
	bits = bits - shift;
}

static	float	PalConvTbl[][3] = {
	1.00f, 1.00f, 1.00f,
	1.00f, 0.80f, 0.73f,
	0.73f, 1.00f, 0.70f,
	0.76f, 0.78f, 0.58f,
	0.86f, 0.80f, 1.00f,
	0.83f, 0.68f, 0.85f,
	0.67f, 0.77f, 0.83f,
	0.68f, 0.68f, 0.68f,
//	1.00f, 1.00f, 1.00f,
};

// 僷儗僢僩僥乕僽儖偺寁嶼
BOOL	CDirectDraw::CalcPaletteTable()
{
INT	i, j;

	if( !m_lpDD || !m_lpDDPrimary )
		return	FALSE;

	DDSURFACEDESC2	ddsd;
	ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_PIXELFORMAT;
	if( m_lpDDPrimary->GetSurfaceDesc(&ddsd) != DD_OK )
		throw "CDirectDraw:GetSurfaceDesc error";

	INT	Rbit, Gbit, Bbit;
	INT	Rsft, Gsft, Bsft;

	if( ddsd.ddpfPixelFormat.dwRGBBitCount != 8 ) {
		GetBitMask( ddsd.ddpfPixelFormat.dwRBitMask, Rsft, Rbit );
		GetBitMask( ddsd.ddpfPixelFormat.dwGBitMask, Gsft, Gbit );
		GetBitMask( ddsd.ddpfPixelFormat.dwBBitMask, Bsft, Bbit );
	}

	for( j = 0; j < 8; j++ ) {
		for( i = 0; i < 64; i++ ) {
			DWORD	Rn, Gn, Bn;
			DWORD	Rs, Gs, Bs;

			// Normal
			Rn = (DWORD)(PalConvTbl[j][0]*m_PaletteBuf[i].r);
			Gn = (DWORD)(PalConvTbl[j][1]*m_PaletteBuf[i].g);
			Bn = (DWORD)(PalConvTbl[j][2]*m_PaletteBuf[i].b);
			// Scanline
			Rs = (DWORD)(PalConvTbl[j][0]*m_PaletteBuf[i].r*m_nScanlineColor/100.0f);
			Gs = (DWORD)(PalConvTbl[j][1]*m_PaletteBuf[i].g*m_nScanlineColor/100.0f);
			Bs = (DWORD)(PalConvTbl[j][2]*m_PaletteBuf[i].b*m_nScanlineColor/100.0f);

			m_cpPalette[j][i+0x00].rgbRed   = (BYTE)Rn;
			m_cpPalette[j][i+0x00].rgbGreen = (BYTE)Gn;
			m_cpPalette[j][i+0x00].rgbBlue  = (BYTE)Bn;
			m_cpPalette[j][i+0x40].rgbRed   = (BYTE)Rs;
			m_cpPalette[j][i+0x40].rgbGreen = (BYTE)Gs;
			m_cpPalette[j][i+0x40].rgbBlue  = (BYTE)Bs;

			m_cnPalette[j][i] = ((Rn>>(8-Rbit))<<Rsft)|((Gn>>(8-Gbit))<<Gsft)|((Bn>>(8-Bbit))<<Bsft);
			m_csPalette[j][i] = ((Rs>>(8-Rbit))<<Rsft)|((Gs>>(8-Gbit))<<Gsft)|((Bs>>(8-Bbit))<<Bsft);

			// RGB555
			if( Rsft > Bsft ) {
				// RGB555->RGB888偺帪
				m_cfPalette[j][i] = ((Rn>>(8-5))<<10)|((Gn>>(8-5))<<5)|((Bn>>(8-5))<<0);
			} else {
				// BGR555->BGR888偺帪
				m_cfPalette[j][i] = ((Rn>>(8-5))<<0)|((Gn>>(8-5))<<5)|((Bn>>(8-5))<<10);
			}

			// Monochrome
			Rn = (DWORD)(m_PaletteBuf[i&0x30].r);
			Gn = (DWORD)(m_PaletteBuf[i&0x30].g);
			Bn = (DWORD)(m_PaletteBuf[i&0x30].b);
			Rn = 
			Gn = 
			Bn = (DWORD)(0.299f * Rn + 0.587f * Gn + 0.114f * Bn);
			Rn = (DWORD)(PalConvTbl[j][0]*Rn);
			Gn = (DWORD)(PalConvTbl[j][1]*Gn);
			Bn = (DWORD)(PalConvTbl[j][2]*Bn);
			if( Rn > 0xFF ) Rs = 0xFF;
			if( Gn > 0xFF ) Gs = 0xFF;
			if( Bn > 0xFF ) Bs = 0xFF;
			// Scanline
			Rs = (DWORD)(m_PaletteBuf[i&0x30].r*m_nScanlineColor/100.0f);
			Gs = (DWORD)(m_PaletteBuf[i&0x30].g*m_nScanlineColor/100.0f);
			Bs = (DWORD)(m_PaletteBuf[i&0x30].b*m_nScanlineColor/100.0f);
			Rs = 
			Gs = 
			Bs = (DWORD)(0.299f * Rs + 0.587f * Gs + 0.114f * Bs);
			Rs = (DWORD)(PalConvTbl[j][0]*Rs);
			Gs = (DWORD)(PalConvTbl[j][1]*Gs);
			Bs = (DWORD)(PalConvTbl[j][2]*Bs);
			if( Rs > 0xFF ) Rs = 0xFF;
			if( Gs > 0xFF ) Gs = 0xFF;
			if( Bs > 0xFF ) Bs = 0xFF;

			m_mpPalette[j][i+0x00].rgbRed   = (BYTE)Rn;
			m_mpPalette[j][i+0x00].rgbGreen = (BYTE)Gn;
			m_mpPalette[j][i+0x00].rgbBlue  = (BYTE)Bn;
			m_mpPalette[j][i+0x40].rgbRed   = (BYTE)Rs;
			m_mpPalette[j][i+0x40].rgbGreen = (BYTE)Gs;
			m_mpPalette[j][i+0x40].rgbBlue  = (BYTE)Bs;

			m_mnPalette[j][i] = ((Rn>>(8-Rbit))<<Rsft)|((Gn>>(8-Gbit))<<Gsft)|((Bn>>(8-Bbit))<<Bsft);
			m_msPalette[j][i] = ((Rs>>(8-Rbit))<<Rsft)|((Gs>>(8-Gbit))<<Gsft)|((Bs>>(8-Bbit))<<Bsft);

			// RGB555
			if( Rsft > Bsft ) {
				// RGB555->RGB888偺帪
				m_mfPalette[j][i] = ((Rn>>(8-5))<<10)|((Gn>>(8-5))<<5)|((Bn>>(8-5))<<0);
			} else {
				// BGR555->BGR888偺帪
				m_mfPalette[j][i] = ((Rn>>(8-5))<<0)|((Gn>>(8-5))<<5)|((Bn>>(8-5))<<10);
			}
		}
	}

	// 嵞昤夋偺堊
	m_bDeltaUpdate = TRUE;

	return	TRUE;
}

// 僷儗僢僩僥乕僽儖偺愝掕
void	CDirectDraw::SetPaletteTable( LPBYTE pal )
{
	if( pal )
		memcpy( m_PaletteBuf, pal, sizeof(m_PaletteBuf) );
	else
		memcpy( m_PaletteBuf, m_PalDefault, sizeof(m_PaletteBuf) );

	CalcPaletteTable();

	m_bPaletteUpdate = TRUE;
}

// 僷儗僢僩僥乕僽儖偺愝掕
void	CDirectDraw::SetPaletteTable( RGBQUAD* rgb )
{
	for( INT i = 0; i < 64; i++ ) {
		m_PaletteBuf[i].r = rgb[i].rgbRed;
		m_PaletteBuf[i].g = rgb[i].rgbGreen;
		m_PaletteBuf[i].b = rgb[i].rgbBlue;
	}

	CalcPaletteTable();

	m_bPaletteUpdate = TRUE;
}

// 僷儗僢僩僥乕僽儖偺庢摼
void	CDirectDraw::GetPaletteTable( RGBQUAD* rgb )
{
	for( INT i = 0; i < 64; i++ ) {
		rgb[i].rgbRed   = m_PaletteBuf[i].r;
		rgb[i].rgbGreen = m_PaletteBuf[i].g;
		rgb[i].rgbBlue  = m_PaletteBuf[i].b;
		rgb[i].rgbReserved = 0;
	}
}

// 僷儗僢僩僼傽僀儖偺峏怴
void	CDirectDraw::SetPaletteFile( LPCTSTR fname )
{
	// 僷儗僢僩僼傽僀儖偺峏怴
	if( strlen( fname ) > 0 ) {
		FILE	*fp;
		if( (fp = ::fopen( fname, "rb" )) ) {
			BYTE	palbuf[192];
			// 僒僀僘暘撉傒崬傒
			if( ::fread( palbuf, 192, 1, fp ) == 1 ) {
				// 僷儗僢僩偺曄峏偲寁嶼
				SetPaletteTable( palbuf );
			} else {
				// 撉傒偒傟側偐偭偨帪偼僨僼僅儖僩
				SetPaletteTable( (LPBYTE)NULL );
			}
			FCLOSE(fp);
		} else {
			// 奐偗側偐偭偨帪偼僨僼僅儖僩
			SetPaletteTable( (LPBYTE)NULL );
		}
	} else {
		// 柤慜偑柍偄帪偼僨僼僅儖僩
		SetPaletteTable( (LPBYTE)NULL );
	}
}

// 僼儖僗僋儕乕儞儌乕僪偱偺GDI僂僀儞僪僂昞帵愝掕
BOOL	CDirectDraw::SetFullScreenGDI( BOOL bMode )
{
	// 擮偺堊僠僃僢僋
	if( !m_lpDD || !m_lpDDPrimary )
		return	FALSE;

	if( m_bScreenMode ) {
		if( !m_bGDI ) {
			if( bMode ) {
				RELEASE( m_lpDDClipper ); // 堦墳
				if( m_lpDD->CreateClipper(0, &m_lpDDClipper, NULL) == DD_OK ) {
					m_lpDDClipper->SetHWnd( 0, m_hWnd );
					m_lpDDPrimary->SetClipper( m_lpDDClipper );
					if( m_lpDD->FlipToGDISurface() == DD_OK ) {
						m_bGDI = TRUE;
					} else {
						RELEASE( m_lpDDClipper );
						return	FALSE;
					}
				}
			}
		} else {
			if( !bMode ) {
				RELEASE( m_lpDDClipper );
				m_bGDI = FALSE;
			}
		}
	}
	return	TRUE;
}

void	CDirectDraw::RealizePalette()
{
	if( !m_lpDD || !m_lpDDPrimary )
		return;

	if( !m_bScreenMode ) {
		DDSURFACEDESC2	ddsd;
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		m_lpDDPrimary->GetSurfaceDesc(&ddsd);

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 ) {
			HDC	hdc;
			hdc = ::GetDC( m_hWnd );
			::SelectPalette( hdc, m_hPalette, FALSE );
			::RealizePalette( hdc );
			::ReleaseDC( m_hWnd, hdc );

			m_bPaletteUpdate = TRUE;
		}
	}
}

// 昤夋(Windows儊僢僙乕僕梡)
void	CDirectDraw::OnScreenDraw()
{
	if( !m_bScreenMode ) {
	// Window mode
		Blt();
		Flip();
	} else {
	// Fullscreen mode
		if( m_bGDI ) {
			Blt();
			Flip();
		}
	}
}

void	CDirectDraw::SetPaletteMode( INT nMode, BOOL bMono )
{
	if( (m_nPaletteMode != nMode) || (m_bMonoMode != bMono) ) {
		m_bPaletteUpdate = TRUE;
	}
	m_nPaletteMode = nMode;
	m_bMonoMode    = bMono;
}

// 昞帵拞偺僷儗僢僩僥乕僽儖偺庢摼
void	CDirectDraw::GetPaletteData( RGBQUAD* rgb )
{
INT	i;
	if( !m_bMonoMode ) {
		for( i = 0; i < 64; i++ ) {
			rgb[i     ] = m_cpPalette[m_nPaletteMode][i];
			rgb[i+0x40] = m_mpPalette[m_nPaletteMode][i];
		}
	} else {
		for( i = 0; i < 64; i++ ) {
			rgb[i     ] = m_mpPalette[m_nPaletteMode][i];
			rgb[i+0x40] = m_mpPalette[m_nPaletteMode][i];
		}
	}
}

void	CDirectDraw::Blt()
{
INT	i;
DDSURFACEDESC2	ddsd;

	if( !m_lpDD || !m_lpDDPrimary )
		return;
	// ScreenMode changing?
	if( m_bChangeMode )
		return;
	// Surface lost check & restore
	if( !RestoreSurface() )
		return;

		//在此之前，m_lpRender已经被写入了内容，后面的部分只是将它放上ｄｘ表面
	LPBYTE	lpRdr = m_lpRender+8;
	
	if(g_bSan2) ViewChars(lpRdr);	//霸王大陆

	ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	if( m_lpDDPrimary->GetSurfaceDesc(&ddsd) != DD_OK )
		return;

	// Palette copy
	if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 ) {
		if( !(m_LineColormode[1]&0x80) ) {
			for( i = 0; i < 128; i++ ) {
				m_logPalette.pe[i+0x40].peRed   = m_cpPalette[m_nPaletteMode][i].rgbRed;
				m_logPalette.pe[i+0x40].peGreen = m_cpPalette[m_nPaletteMode][i].rgbGreen;
				m_logPalette.pe[i+0x40].peBlue  = m_cpPalette[m_nPaletteMode][i].rgbBlue;
			}
		} else {
			for( i = 0; i < 128; i++ ) {
				m_logPalette.pe[i+0x40].peRed   = m_mpPalette[m_nPaletteMode][i].rgbRed;
				m_logPalette.pe[i+0x40].peGreen = m_mpPalette[m_nPaletteMode][i].rgbGreen;
				m_logPalette.pe[i+0x40].peBlue  = m_mpPalette[m_nPaletteMode][i].rgbBlue;
			}
		}
		if( !m_bScreenMode ) {
			::AnimatePalette( m_hPalette, 0, 256, m_logPalette.pe );
		}
	}

	// Size calculate
	BOOL	bDoubleWidth = FALSE;
	BOOL	bDoubleHeight = FALSE;

	RECT	rcW;
	rcW.left   = 0;
	rcW.top    = 0;
	rcW.right  = SCREEN_WIDTH;
	rcW.bottom = SCREEN_HEIGHT;
	if( m_bDoubleSize || (m_nBltFilter && IsMMX()) ) {
		rcW.right  *= 2;
		rcW.bottom *= 2;
		bDoubleWidth  = TRUE;
		bDoubleHeight = TRUE;
	} else if( m_bScanlineMode ) {
		rcW.bottom *= 2;
		bDoubleHeight = TRUE;
	}

	// Render function
	BLTFUNC* bltfunc;
	if( !m_nBltFilter || !IsMMX() ) {
		if( !m_bDoubleSize ) {
			if( !m_bScanlineMode ) {
				bltfunc = NormalBltTable;
			} else {
				bltfunc = ScanlineBltTable;
			}
		} else {
			if( !m_bScanlineMode ) {
				bltfunc = DoubleBltTable;
			} else {
				bltfunc = DoubleScanlineBltTable;
			}
		}
	} else {
		switch( m_nBltFilter ) {
			case	BLTFILTER_2XSAI:
				bltfunc = nx2xSaIBltTable;
				break;
			case	BLTFILTER_SUPER2XSAI:
				bltfunc = nxSuper2xSaIBltTable;
				break;
			case	BLTFILTER_SUPEREAGLE:
				bltfunc = nxSuperEagleBltTable;
				break;
			case	BLTFILTER_SCALE2X:
				bltfunc = nxScale2xBltTable;
				break;
			default:
				break;
		}
	}

	BOOL	bFilter = FALSE;
//	LPBYTE	lpRdr = m_lpRender+8;

	ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize  = sizeof(DDSURFACEDESC2);

	if( m_bForceWrite ) {
	// 懡抜儗儞僟儕儞僌柍偟
		if( m_lpDDRender->Lock( NULL, &ddsd, 0, NULL ) == DD_OK ) {
			switch( ddsd.ddpfPixelFormat.dwRGBBitCount ) {
				case	8:
					(this->*bltfunc[0])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, TRUE );
					break;
				case	16:
					(this->*bltfunc[1])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, TRUE );
					break;
				case	24:
					(this->*bltfunc[2])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, TRUE );
					break;
				case	32:
					(this->*bltfunc[3])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, TRUE );
					break;
				default:
					break;
			}

			m_lpDDRender->Unlock( NULL );
			m_bDeltaUpdate = FALSE;
		}
	} else {
		if( m_lpDDRender2->Lock( NULL, &ddsd, 0, NULL ) == DD_OK ) {
			switch( ddsd.ddpfPixelFormat.dwRGBBitCount ) {
				case	8:
					(this->*bltfunc[0])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, m_bDeltaUpdate );
					break;
				case	16:
					(this->*bltfunc[1])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, m_bDeltaUpdate );
					break;
				case	24:
					(this->*bltfunc[2])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, m_bDeltaUpdate );
					break;
				case	32:
					(this->*bltfunc[3])( lpRdr, &m_lpRenderDelta[DELTA_WIDTH*2*sizeof(DWORD)], ddsd, m_bDeltaUpdate );
					break;
				default:
					break;
			}

			m_lpDDRender2->Unlock( NULL );
			m_bDeltaUpdate = FALSE;

			m_lpDDRender->Blt( &rcW, m_lpDDRender2, &rcW, 0, NULL );
		}
	}

	// TV榞
	if( m_bTVFrameMode ) {
		m_lpDDRender->Blt( &rcW, m_lpDDTV, NULL, DDBLT_KEYSRC, NULL );
	}

	// Infomation string
	if( strlen(m_szInfo) > 0 ) {
		INT	x, y, o;

		x = bDoubleWidth ? 16 : 8;
		o = m_bAllLine ? 0 : 8;
		y = bDoubleHeight ? 8+o*2 : 4+o;

		RenderString( x, y, m_szInfo );
	}
	// Message string
	if( m_bMessage ) {
		if( timeGetTime()-m_dwMessageTime > 1500 ) {
			m_bMessage = FALSE;
		}
		if( strlen(m_szMess) > 0 ) {
			INT	x, y, o;

			x = bDoubleWidth ? 16 : 8;
			o = m_bAllLine ? 8 : 0;
			y = bDoubleHeight ? (SCREEN_HEIGHT*2-36+o*2) : (SCREEN_HEIGHT-18+o);

			RenderString( x, y, m_szMess );
		}
	}
	// DiskAccessLamp
	if( m_bDiskAccessLamp ) {
		INT	x, y, o;

		x = bDoubleWidth ? SCREEN_WIDTH*2-20 : SCREEN_WIDTH-10;
		o = m_bAllLine ? 0 : 8;
		y = bDoubleHeight ? 8+o*2 : 4+o;

		RenderString( x, y, "\\" );
	}

	if( m_bZapper && m_bZapperDraw ) {
		if( m_ZapperPosX >= 0 && m_ZapperPosX < SCREEN_WIDTH && m_ZapperPosY >= 0 && m_ZapperPosY < SCREEN_HEIGHT ) {
			RECT	rcS, rcZ;

			SetRect( &rcZ, 0, 0, 16, 16 );
			if( !bDoubleWidth ) {
				rcS.left   = m_ZapperPosX-(rcZ.right-rcZ.left)/2;
				rcS.right  = m_ZapperPosX+(rcZ.right-rcZ.left)/2;
			} else {
				rcS.left   = m_ZapperPosX*2-(rcZ.right-rcZ.left);
				rcS.right  = m_ZapperPosX*2+(rcZ.right-rcZ.left);
			}
			if( !bDoubleHeight ) {
				rcS.top    = m_ZapperPosY-(rcZ.bottom-rcZ.top)/2;
				rcS.bottom = m_ZapperPosY+(rcZ.bottom-rcZ.top)/2;
			} else {
				rcS.top    = m_ZapperPosY*2-(rcZ.bottom-rcZ.top);
				rcS.bottom = m_ZapperPosY*2+(rcZ.bottom-rcZ.top);
			}

			m_lpDDRender->Blt( &rcS, m_lpDDZapper, &rcZ, DDBLT_KEYSRC, NULL );
		}
	}
}

void	CDirectDraw::Flip()
{
	if( !m_lpDD || !m_lpDDPrimary )
		return;
	// ScreenMode changing?
	if( m_bChangeMode )
		return;
	// Surface lost check & restore
	if( !RestoreSurface() )
		return;

	DDSURFACEDESC2	ddsd;
	ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	m_lpDDPrimary->GetSurfaceDesc(&ddsd);

	// Size calculate
	BOOL	bDoubleWidth = FALSE;
	BOOL	bDoubleHeight = FALSE;
	if( m_bDoubleSize || (m_nBltFilter && IsMMX()) ) {
		bDoubleWidth  = TRUE;
		bDoubleHeight = TRUE;
	} else if( m_bScanlineMode ) {
		bDoubleHeight = TRUE;
	}

	RECT	rcS, rcC;
	if( !bDoubleWidth ) {
		rcS.left  = 0;
		rcS.right = SCREEN_WIDTH;
	} else {
		rcS.left  = 0;
		rcS.right = SCREEN_WIDTH*2;
	}
	if( !m_bAllLine ) {
		rcS.top    = 8;
		rcS.bottom = SCREEN_HEIGHT-8;
	} else {
		rcS.top    = 0;
		rcS.bottom = SCREEN_HEIGHT;
	}
	if( bDoubleHeight ) {
		rcS.top    *= 2;
		rcS.bottom *= 2;
	}

	if( !m_bScreenMode ) {
	// Window mode
		::GetClientRect( m_hWnd, &rcC );
		::ClientToScreen( m_hWnd, (POINT*)&rcC.left );
		::ClientToScreen( m_hWnd, (POINT*)&rcC.right );

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 ) {
			HDC	hdc;
			if( m_lpDDPrimary->GetDC( &hdc ) == DD_OK ) {
				::SelectPalette( hdc, m_hPalette, FALSE );
				::RealizePalette( hdc );
				m_lpDDPrimary->ReleaseDC( hdc );
			}
		}
	} else {
	// Fullscreen mode
		if( !m_bMaxZoom ) {
			DDBLTFX	ddbltfx;
			ddbltfx.dwSize = sizeof(DDBLTFX);
			ddbltfx.dwFillColor = 0;
			m_lpDDBack->Blt(NULL, NULL, NULL, DDBLT_COLORFILL|DDBLT_WAIT, &ddbltfx);

			// Position offset caluclate
			LONG	swidth, sheight;
			LONG	dwidth, dheight;
			LONG	hmul, vmul;

			if( !m_bAspect )  swidth = SCREEN_WIDTH;
			else		  swidth = 320;
			if( !m_bAllLine ) sheight = SCREEN_HEIGHT-16;
			else		  sheight = SCREEN_HEIGHT;

			dwidth  = (LONG)ddsd.dwWidth;
			dheight = (LONG)ddsd.dwHeight;
			hmul = dwidth  / swidth;
			vmul = dheight / sheight;

			if( vmul < hmul ) hmul = vmul;
			else		  vmul = hmul;

			rcC.left   = (dwidth -swidth *hmul)/2;
			rcC.top    = (dheight-sheight*vmul)/2;
			rcC.right  = rcC.left+swidth *hmul;
			rcC.bottom = rcC.top +sheight*vmul;
		} else {
			// Maximum zoom
			rcC.left   = 0;
			rcC.top    = 0;
			rcC.right  = (LONG)ddsd.dwWidth;
			rcC.bottom = (LONG)ddsd.dwHeight;
		}
	}

	if( !m_bScreenMode ) {
	// Window mode
		if( m_bWindowVSync ) {
			HRESULT	hr;
			while( TRUE ) {
				hr = m_lpDD->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, NULL );
				if( hr == DD_OK )
					break;
				if( hr == DDERR_SURFACELOST ) {
					break;
				}
				if( hr != DDERR_WASSTILLDRAWING ) {
					break;
				}
			}
//			m_lpDDPrimary->Blt( NULL, m_lpDDBack, NULL, DDBLT_WAIT, NULL );
		}

		m_lpDDPrimary->Blt( &rcC, m_lpDDRender, &rcS, DDBLT_WAIT, NULL );

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 ) {
			if( m_bPaletteUpdate ) {
				m_bPaletteUpdate = FALSE;
				::AnimatePalette( m_hPalette, 0, 256, m_logPalette.pe );
			}
		}
	} else {
	// Fullscreen mode
		m_lpDDBack->Blt( &rcC, m_lpDDRender, &rcS, DDBLT_WAIT, NULL );

		if( ddsd.ddpfPixelFormat.dwRGBBitCount == 8 ) {
			if( m_bPaletteUpdate ) {
				m_bPaletteUpdate = FALSE;
				m_lpDDPalette->SetEntries( 0, 0, 256, m_logPalette.pe );
			}
		}

		if( m_bFlip ) {
			if( !m_bGDI ) {
				HRESULT	hr;
				while( TRUE ) {
					hr = m_lpDDPrimary->Flip( NULL, DDFLIP_WAIT );
					if( hr == DD_OK )
						break;
					if( hr == DDERR_SURFACELOST ) {
						break;
					}
					if( hr != DDERR_WASSTILLDRAWING ) {
						break;
					}
				}
			} else {
				HRESULT	hr;
				while( TRUE ) {
					hr = m_lpDD->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, NULL );
					if( hr == DD_OK )
						break;
					if( hr == DDERR_SURFACELOST ) {
						break;
					}
					if( hr != DDERR_WASSTILLDRAWING ) {
						break;
					}
				}
				m_lpDDPrimary->Blt( NULL, m_lpDDBack, NULL, DDBLT_WAIT, NULL );
			}
		} else {
			m_lpDDPrimary->Blt( NULL, m_lpDDBack, NULL, DDBLT_WAIT, NULL );
		}
	}
}

BYTE	CDirectDraw::GetZapperHit()
{
	if( m_bZapper ) {
		if( m_ZapperPosX >= 0 && m_ZapperPosX < SCREEN_WIDTH && m_ZapperPosY >= 0 && m_ZapperPosY < SCREEN_HEIGHT ) {
			BYTE	c = m_lpRender[8+m_ZapperPosX+RENDER_WIDTH*m_ZapperPosY];
			DWORD	Yn = (DWORD)(0.299f * m_cpPalette[0][c].rgbRed + 0.587f * m_cpPalette[0][c].rgbGreen + 0.114f * m_cpPalette[0][c].rgbBlue);
			if( Yn > 0xFF )
				Yn = 0xFF;
			return	(BYTE)Yn;
		}
	}
	return	0x00;
}

void	CDirectDraw::GetZapperPos( LONG& x, LONG& y )
{
	x = y = -1;

	if( !m_lpDD || !m_lpDDPrimary )
		return;
	// ScreenMode changing?
	if( m_bChangeMode )
		return;

	if( !m_bZapper )
		return;

	RECT	rcS, rcC;

	// Size calculate
	rcS.left  = 0;
	rcS.right = SCREEN_WIDTH;
	if( !m_bAllLine ) {
		rcS.top    = 8;
		rcS.bottom = SCREEN_HEIGHT-8;
	} else {
		rcS.top    = 0;
		rcS.bottom = SCREEN_HEIGHT;
	}
	if( m_bScanlineMode ) {
		rcS.top    *= 2;
		rcS.bottom *= 2;
	}

	if( !m_bScreenMode ) {
	// Window mode
		::GetClientRect( m_hWnd, &rcC );
		::ClientToScreen( m_hWnd, (POINT*)&rcC.left );
		::ClientToScreen( m_hWnd, (POINT*)&rcC.right );
	} else {
	// Fullscreen mode
		DDSURFACEDESC2	ddsd;
		ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
		ddsd.dwSize = sizeof(DDSURFACEDESC2);
		if( m_lpDDPrimary->GetSurfaceDesc(&ddsd) != DD_OK )
			return;

		if( !m_bMaxZoom ) {
			// Position offset caluclate
			LONG	swidth, sheight;
			LONG	dwidth, dheight;
			LONG	hmul, vmul;

			if( !m_bAspect )  swidth = SCREEN_WIDTH;
			else		  swidth = 320;
			if( !m_bAllLine ) sheight = SCREEN_HEIGHT-16;
			else		  sheight = SCREEN_HEIGHT;

			dwidth  = (LONG)ddsd.dwWidth;
			dheight = (LONG)ddsd.dwHeight;
			hmul = dwidth  / swidth;
			vmul = dheight / sheight;

			if( vmul < hmul ) hmul = vmul;
			else		  vmul = hmul;

			rcC.left   = (dwidth -swidth *hmul)/2;
			rcC.top    = (dheight-sheight*vmul)/2;
			rcC.right  = rcC.left+swidth *hmul;
			rcC.bottom = rcC.top +sheight*vmul;
		} else {
			// Maximum zoom
			rcC.left   = 0;
			rcC.top    = 0;
			rcC.right  = (LONG)ddsd.dwWidth;
			rcC.bottom = (LONG)ddsd.dwHeight;
		}

	}

	POINT	mp;
	FLOAT	hz, vz;

	::GetCursorPos( &mp );

	if( mp.x >= rcC.left && mp.x < rcC.right
	 && mp.y >= rcC.top && mp.y < rcC.bottom ) {
		hz = (FLOAT)(rcS.right-rcS.left)/(rcC.right-rcC.left);
		vz = (FLOAT)(rcS.bottom-rcS.top)/(rcC.bottom-rcC.top);
		x = (LONG)((mp.x-rcC.left)*hz)+rcS.left;
		if( !m_bScanlineMode ) {
			y = (LONG)((mp.y-rcC.top) *vz)+rcS.top;
		} else {
			y = (LONG)(((mp.y-rcC.top) *vz)+rcS.top)/2;
		}

		if( x > SCREEN_WIDTH-1 )
			x = SCREEN_WIDTH-1;
		if( y > SCREEN_HEIGHT-1 )
			y = SCREEN_HEIGHT-1;
	} else {
		x = y = -1;
	}

	m_ZapperPosX = x;
	m_ZapperPosY = y;
}

void	CDirectDraw::SetZapperPos( LONG x, LONG y )
{
	m_ZapperPosX = x;
	m_ZapperPosY = y;
}

// Infomation string
void	CDirectDraw::SetInfoString( LPCSTR str )
{
	if( str ) {
		if( strlen(str) > INFOSTR_SIZE ) {
			memcpy( m_szInfo, str, INFOSTR_SIZE );
			m_szInfo[INFOSTR_SIZE] = '\0';
		} else {
			strcpy( m_szInfo, str );
		}
	} else {
		m_szInfo[0] = '\0';
	}
}

// Message string
void	CDirectDraw::SetMessageString( LPCSTR str )
{
	if( str ) {
		if( strlen(str) > INFOSTR_SIZE ) {
			memcpy( m_szMess, str, INFOSTR_SIZE );
			m_szMess[INFOSTR_SIZE] = '\0';
		} else {
			strcpy( m_szMess, str );
		}
		m_bMessage = TRUE;
		m_dwMessageTime = ::timeGetTime();
	} else {
		m_bMessage = FALSE;
		m_szMess[0] = '\0';
	}
}

void	CDirectDraw::RenderString( INT x, INT y, LPCSTR str )
{
	if( !m_lpDD || !m_lpDDPrimary )
		return;
	// ScreenMode changing?
	if( m_bChangeMode )
		return;

	if( !str )
		return;

	// Size calculate
	BOOL	bDoubleWidth = FALSE;
	BOOL	bDoubleHeight = FALSE;
	if( m_bDoubleSize || (m_nBltFilter && IsMMX()) ) {
		bDoubleWidth  = TRUE;
		bDoubleHeight = TRUE;
	} else if( m_bScanlineMode ) {
		bDoubleHeight = TRUE;
	}

	RECT	rcS, rcW;
	INT	ch;
	INT	xadd;

	rcW.left = x;
	rcW.top  = y;
	if( !bDoubleWidth ) {
		rcW.right = x+6;
		xadd = 6;
	} else {
		rcW.right = x+12;
		xadd = 12;
	}
	if( !bDoubleHeight ) {
		rcW.bottom = y+6;
	} else {
		rcW.bottom = y+12;
	}

	while( *str ) {
		ch = toupper(*str)-0x20;
		rcS.left   = (ch%8)*6;
		rcS.right  = rcS.left+6;
		rcS.top    = (ch/8)*6;
		rcS.bottom = rcS.top+6;

		m_lpDDRender->Blt( &rcW, m_lpDDAscii, &rcS, DDBLT_KEYSRC, NULL );

		rcW.left  += xadd;
		rcW.right += xadd;
		str++;
	}
}

// LZSS BMP偐傜僒乕僼僃僗傊偺僐僺乕
void	CDirectDraw::SetLZSSChar( LPBYTE lpLZ, LPDIRECTDRAWSURFACE7 lpDDSurface )
{
LPBYTE	lpBuf = NULL;
LPBITMAPINFOHEADER pbi;
LPBYTE	lpPix;
HDC	hDC;

	if( !(lpBuf = (LPBYTE)malloc( *((LONG*)lpLZ) )) )
		throw	"Out of memory.";

	LZdecode( &lpLZ[8], lpBuf, *((LONG*)&lpLZ[4]) );

	pbi = (LPBITMAPINFOHEADER)lpBuf;
	if( pbi->biBitCount < 16 )
		lpPix = ((LPBYTE)pbi)+pbi->biSize+(1<<pbi->biBitCount)*sizeof(RGBQUAD);
	else
		lpPix = (LPBYTE)pbi+pbi->biSize;

	DDSURFACEDESC2	ddsd;
	ZEROMEMORY( &ddsd, sizeof(DDSURFACEDESC2) );
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	lpDDSurface->GetSurfaceDesc(&ddsd);

	// BMP偼崅偝偑儅僀僫僗側帠偑偁傞偺偱....
	LONG	height = pbi->biHeight;
	if( height < 0 )
		height = -height;

	if( lpDDSurface->GetDC( &hDC ) == DD_OK ) {
		StretchDIBits( hDC, 0, 0, ddsd.dwWidth, ddsd.dwHeight,
				    0, 0, pbi->biWidth, height, lpPix, (LPBITMAPINFO)lpBuf, DIB_RGB_COLORS, SRCCOPY );
		lpDDSurface->ReleaseDC( hDC );
	}
	FREE( lpBuf );
}

#include "Render.h"

void	CDirectDraw::ViewChars( unsigned char * lpRdr )	//书写浮动的文本
{
	unsigned char tileid=0, bktileid=1,count=0,offset=0;
	unsigned int i, j = 0,pos=0;
	unsigned int gridy=0, scrolly = 0, scrollyy = 0;
	unsigned int BASE_VRAM = 0x2000, MAX_INDEX_R = MAX_INDEX, F_SPECIAL = 0;
	unsigned int Line_Start = 30, Line_End = 30;

	#define LINESKIP(n) ((0x7FFF - ((RAM[0x69+(n<<1)]&0x7F)<<8) - RAM[0x68+(n<<1)])*3)/2728
		
	switch(RAM[0x60])
		{
			case 0x1	:	//城内武将资料
				if(RAM[0xBA] == 0x08)
				{
					if(RAM[0x6A] == 0x70)
					{
						Line_Start = 21;//5 + LINESKIP(0) + LINESKIP(1)+ LINESKIP(2) + LINESKIP(3) + LINESKIP(4);
						{
							scrollyy = (RAM[0x98]&0x07) + 2;
						}
						gridy = (RAM[0x9B]*0x100+RAM[0x9A]+RAM[0x97]*0x400-0x2000)/0x20 - Line_Start;
						if(RAM[0xBB] != 0x09)
						{
							MAX_INDEX_R = MIN_INDEX;
						}
					}
					else
					{
						Line_Start = 21;
						if(RAM[0x98]<=RAM[0x99])
						{
							scrollyy = (RAM[0x98]+2)&0x07;
						}
						else
						{
							scrollyy = ((RAM[0x98]+6)&0x07)+4;
						}
						gridy = (RAM[0x9B]*0x100+RAM[0x9A]-0x2000)/0x20 - Line_Start;
						if(RAM[0xBB] != 0x09)
						{
							MAX_INDEX_R = MIN_INDEX;
						}
					}
				}
				break;
			case 0x3	:	//全国地图
				if(RAM[0xBA] == 0x08)
				{
					Line_Start = 0x280/0x20;
					gridy = (RAM[0x9B]*0x100+RAM[0x9A]-0x2000)/0x20 - Line_Start;
					if(RAM[0xBB] != 0x09)
					{
						MAX_INDEX_R = MIN_INDEX;
					}
				}
				break;
			case 0x5	:	//大战场，战斗总结
				if(RAM[0xBA] == 0x08)
				{
					if(RAM[0xAF] == 0x08)
					{
						Line_Start = 0;
					}
					else
					{
						Line_Start = 20;			//扫描线开始行数
						gridy = 32 - Line_Start;	//命名表对扫描线的偏移
					}
					if(RAM[0xBB] != 0x09)
					{
						MAX_INDEX_R = MIN_INDEX;
					}
				}
				break;
			case 0x6	:	
				BASE_VRAM = 0x2400;
				if(RAM[0xBA] == 0x08) 
				{
					Line_Start =0;
					if(RAM[0xBB] != 0x09)
					{
						MAX_INDEX_R = MIN_INDEX;
					}
				}
				break;
			case 0x7	:	//用计
				if(RAM[0xBA] == 0x08)
				{
					Line_Start = 1 + LINESKIP(0) + LINESKIP(1) + LINESKIP(2) + LINESKIP(3);
					gridy = (RAM[0x9B]*0x100+RAM[0x9A]+RAM[0x97]*0x400-0x2000)/0x20 - Line_Start;
					if(RAM[0xBB] != 0x09)
					{
						MAX_INDEX_R = MIN_INDEX;
					}
				}
				break;
			case 0x8	:
				if(RAM[0xBD] == 0xB3) 
				{
					Line_Start =14;
					F_SPECIAL = 1;
				}
				break;
			case 0xA	:
			case 0xB	:
				if(RAM[0xBA] == 0x08)
				{
					Line_Start = 20;
					gridy = (RAM[0x9B]*0x100+RAM[0x9A]+RAM[0x97]*0x400-0x2000)/0x20 - Line_Start;
					if(RAM[0xBB] != 0x09)
					{
						MAX_INDEX_R = MIN_INDEX;
					}
				}
				break;
	}

	RGBQUAD	m_Palette[256];
	DirectDraw.GetPaletteData( m_Palette );

	for(j = Line_Start + gridy; j < Line_End + gridy; j ++)	//scan memory
	{
		if(j == 20 && RAM[0x60] == 0x05 && RAM[0xCB] == 0x09)
		{
			gridy += 12;
			j += 12;
		}
		count=0;
		offset=0;
		bktileid = 1;
		for(i = 0; i < 32; i ++)
		{
			pos = (i & 0x1F) + (j << 5);

			if(pos > 0x800)
			{
				pos -= 0x400;
				scrolly = scrollyy + 16;
			}
			else
			{
				scrolly = scrollyy;
			}
			tileid = VRAM[pos + BASE_VRAM - 0x2000];
			
			//汉字显示
			if(tileid < MAX_GROUP)
			{
				bktileid = tileid;
				if(bktileid < 5)
				{
					offset = 0;
					count = 0;
				}
				else
				{
					offset += 8;
				}
			}
			else if(((tileid < MAX_INDEX_R) && (F_SPECIAL == 0)) || ((F_SPECIAL == 1) && (tileid > 0xC7)))
			{
				//访问属性表，获得文字颜色
				int ataddr;
				ataddr = 0x03C0 + ((j-(j&3))<<1) + (i>>2);
				unsigned char atbyte = VRAM[ataddr];
				unsigned char color;

				switch((i&3)+((j&3)<<2))
				{
					case 0:
					case 1:
					case 4:
					case 5:
						//bit 0,1
						color = (((atbyte&0x03)<<2)|0);
						break;	
					case 2:
					case 3:
					case 6:
					case 7:
						//bit 2,3
						color = ((atbyte&0x0C)|0);
						break;	
					case 8:
					case 9:
					case 12:
					case 13:
						//bit 4,5
						color = (((atbyte&0x30)>>2)|0);
						break;
					case 10:
					case 11:
					case 14:
					case 15:
						//bit 6,7
						color = (((atbyte&0xC0)>>4)|0);
						break;
				}
				if(F_SPECIAL == 1)
				{
					tileid -= 0x90;
					color = 7;
				}

				int x = (i<<3) +(count*6) - offset;
				int y = ((j - gridy)<<3) - scrolly - 2;
				unsigned int index = txt[bktileid][tileid];
				long maxpos = (RENDER_WIDTH * RENDER_HEIGHT - 8);

				for(int i = 0; i < 16; i ++)
				{
					for(int j = 0; j < 16; j ++)
					{
						long pos = (y + i) * RENDER_WIDTH + j + x;

						if(pos < maxpos)
						{
							if(j < 8 && (font[index * 32 + i * 2]&(0x80>>j)))
								lpRdr[pos] = BGPAL[color];
							if(j >= 8 && (font[index * 32 + i * 2 + 1]&(0x80>>(j - 8))))
								lpRdr[pos] = BGPAL[color];
						}
					}
				}
				count++;
			}
			else
			{
				count = 0;
				offset =0;
			}
		}
	}
}