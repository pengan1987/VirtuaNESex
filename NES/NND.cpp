/******************************************************************************/
/* Nes Cart Dumper                                                            */
/* tpu<tpunix@126.com>                                                        */
/* last : 2006.11.14                                                          */
/* first: 2003.05.05                                                          */
/******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "mmu.h"

#include "nnd.h"

#define inb(a) _inp((unsigned short)a)
#define outb(a, d) _outp((unsigned short)a, (unsigned short)d)

int InstallGiveIo(void)
{
	HANDLE h;
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT){	//OS=NT/2000
		h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		CloseHandle(h);
		if(h == INVALID_HANDLE_VALUE)
			return 0;
		else
			return 0x2000;
	}else{
		return 0x0098;	//OS=WIN98
	}
}

void set_paddr(int addr)
{
    outb(0x37b, 0);
    outb(0x37c, addr&0xff);
    outb(0x37b, 1);
    outb(0x37c, (addr>>8));
}

void set_vaddr(int addr)
{
    int data;
    data = (~addr)&0x2000;
    data <<= 1;
    addr &= 0x3fff;
    addr |= data;
    outb(0x37b, 4);
    outb(0x37c, addr&0xff);
    outb(0x37b, 5);
    outb(0x37c, addr>>8);
}

void vwrite(int addr, int data)
{
    set_vaddr(addr);
    outb(0x37b, 6);
    outb(0x37c, data);
}

int vread(int addr)
{
	int data;
    set_vaddr(addr);
    outb(0x37b, 6);
    data = inb(0x37c);
    return data;
}

int ppread(int addr)
{
	int data;
	if(addr<0x4000) return vread(addr);
    set_paddr(addr);
    outb(0x37b, 2);
    data = inb(0x37c);
    return data;
}

void ppwrite(int addr, int data)
{
	if(addr<0x4000){
		vwrite(addr, data);
		return;
	}
    set_paddr(addr);
    outb(0x37b, 2);
    outb(0x37c, data);
}

void bus_reset(void)
{
    int data;
    //Reset
    data = inb(0x37a);
    data &= 0xfb;
    outb(0x37a, data);
    data = 0xc4;
    outb(0x37a, data);
    outb(0x37b, 8);
    outb(0x37c, 0);		//auto sync on
//    outb(0x37c, 1);		//auto sync off
    set_vaddr(0x3fff);
}

void main(void)
{
	BYTE filebuf_Prom[64*1024];
	BYTE filebuf_Vrom[16*1024];
    int retv, i;
    
	retv = InstallGiveIo();
	if(retv==0){
		printf("Please install gaveio.sys first!\n");
	}
	bus_reset();

    for(i=0x8000; i<0x10000; i++) filebuf_Prom[i-0x8000] = ppread(i);
	CPU_MEM_BANK[4] = filebuf_Prom;
	CPU_MEM_BANK[5] = filebuf_Prom + 0x2000;
	CPU_MEM_BANK[6] = filebuf_Prom + 0x4000;
	CPU_MEM_BANK[7] = filebuf_Prom + 0x6000;

    for(i=0x0000; i<0x2000; i++) filebuf_Vrom[i] = ppread(i);
	PPU_MEM_BANK[0] = filebuf_Vrom;
	PPU_MEM_BANK[1] = filebuf_Vrom + 0x0400;
	PPU_MEM_BANK[2] = filebuf_Vrom + 0x0800;
	PPU_MEM_BANK[3] = filebuf_Vrom + 0x0c00;
	PPU_MEM_BANK[4] = filebuf_Vrom + 0x1000;
	PPU_MEM_BANK[5] = filebuf_Vrom + 0x1400;
	PPU_MEM_BANK[6] = filebuf_Vrom + 0x1800;
	PPU_MEM_BANK[7] = filebuf_Vrom + 0x1c00;
}
