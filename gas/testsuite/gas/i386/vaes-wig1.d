#as: -mvexwig=1
#objdump: -dw
#name: i386 AVX/VAES wig insns
#source: vaes.s

.*: +file format .*


Disassembly of section \.text:

00000000 <_start>:
[ 	]*[a-f0-9]+:	c4 e2 cd dc d4       	vaesenc %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd dc 39       	vaesenc \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd dd d4       	vaesenclast %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd dd 39       	vaesenclast \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd de d4       	vaesdec %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd de 39       	vaesdec \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd df d4       	vaesdeclast %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd df 39       	vaesdeclast \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd dc d4       	vaesenc %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd dc 39       	vaesenc \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd dc 39       	vaesenc \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd dd d4       	vaesenclast %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd dd 39       	vaesenclast \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd dd 39       	vaesenclast \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd de d4       	vaesdec %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd de 39       	vaesdec \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd de 39       	vaesdec \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd df d4       	vaesdeclast %ymm4,%ymm6,%ymm2
[ 	]*[a-f0-9]+:	c4 e2 cd df 39       	vaesdeclast \(%ecx\),%ymm6,%ymm7
[ 	]*[a-f0-9]+:	c4 e2 cd df 39       	vaesdeclast \(%ecx\),%ymm6,%ymm7
#pass
