# VirtuaNESex - The VirtuaNES EXtended

## About

VirtuaNESex was a fork of VirtuaNES maintained by temryu and was published on nesbbs.com, it is one of the most popular NES emulators in Chinese community. Especially it has very good compatibility for NES based home computers, included some most powerful model from YuXing and BBK.

As temryu mentioned, the unif mapper code is from CaH4e3, developer of fceumm, and BBK (Mapper 171) and YuXing (Mapper 169) floppy driver mappers are from fanoble. There is one thing we should note: The Mapper 171 was only documented for Kaiser board right now, but for many Chinese developers Mapper 171 is BBK's floppy controller since that models are quite popular back in time.

If you want to know more about fanoble's initial work on Mapper171(MapperBBK), please refer to this repository: https://github.com/pengan1987/virtuanes

## How to use it?

I'm provide a pre-configured pack if you want to test your build can run YuXing and BBK correctly

https://9game.oss-us-west-1.aliyuncs.com/VirtuaNESex.zip

### For BBK Floppy Drive Model I

Just drag the BBG_apps.img into VirtualNESex window, it will load the BIOS(bbk_bios10.rom) and boot the floppy image automatically.

### For YuXing

First rename the disk image file to YuXing.img and load one of YX-*.nes to VirtualNESex, you can load the disk content using "Fast Disk Load" ("快速调盘" in Chinese) menu option to load software on diskette.

Both BBK and YuXing using GB2312 for DBCS Chinese character support, and both support 1.44MB, MS-DOS FAT formatted diskette.
