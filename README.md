# HI-TECH-C-Z80-Cross-Compiler

This is the HI-TECH Z80 C Cross Compiler (MS-DOS) v4.11

On the Facebook Zilog Z80 DIY group, Chris A Hills requested
and was granted permission from Microchip Technology Inc. to
release this software for cross-compiling CP/M Z80 programs
under MS-DOS.

The external download link for this software is

http://www.safetycritical.info/pub/Microchip_Hitech_C_Z80.zip

and it is also available from GitHub from
[here](https://raw.githubusercontent.com/agn453/HI-TECH-C-Z80-Cross-Compiler/master/Microchip_Hitech_C_Z80.zip).

Please read and keep a copy of the licence agreement in the
LICENSE file with all copies of these files.

This version runs under x86 MS-DOS (or equivalent).  If you're wanting
a version that runs on a Z80 under CP/M - please use the updated and
enhanced version derived from HI-TECH C Compiler for Z80 v3.09 from

https://github.com/agn453/HI-TECH-Z80-C

### Source files extracted

I've extracted the library source files from the Huffman encoded .HUF archives
into the *cpm*, *float*, *gen*, *romstdio* and *stdio* folders.

Documentation is in the *manuals* folder.

To install to MS-DOS, copy the files from the *diskA* and *diskB* folders
(and all their subfolders - preserving the directory heirarchy) into 
a empty directory on the MS-DOS hard disk and run the install program.

```
rem  Assumes files are located in C:\KITS\HITECHC
cd c:\
mkdir ins
xcopy /s c:\kits\hitechc\diska\*.* \ins
xcopy /s c:\kits\hitechc/diskb\*.* \ins
rem Mount the folder as drive I:
substi i: \ins
i:
install
```

When prompted, specify 'I' as the drive letter containing the distribution,
and choose the default path of C:\HITECH to install the files.

When done, remember to review changes to the AUTOEXEC.BAT file and

```
c:
subst i: /d
```

