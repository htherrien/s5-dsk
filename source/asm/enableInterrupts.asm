;****************************************************************************************
; Copyright Projet S5 - Les S de Sherbrooke
;
; Fichier: enableInterrupts.asm
; Auteur : Hugo Therrien
; Date   : 7 novembre 2017
; Rev.   : 0.1
; Appel  : void enableInterrupts(void);
;
; Active les interruptions
;
;****************************************************************************************

	.def _enableInterrupts

_enableInterrupts
	.asmfunc

	; Enable interrupt 14 (Timer 0)
	MVC	IER,B0
	MVK	0x4000,B1
	OR	B0,B1,B0
	MVC	B0,IER

	; Enable interrupt 15 (Timer 1)
	MVC	IER,B0
	MVKL	0x8000,B1
	MVKH	0x8000,B1
	OR	B0,B1,B0
	MVC	B0,IER

	; Enable nonmaskable interrupts
	MVC	IER,B0
	MVK	0x0002,B1
	OR	B0,B1,B0
	MVC	B0,IER

	; Enable global interrupts
	MVC CSR,B0
	OR 	1,B0,B0
	MVC	B0,CSR


	B	B3
	NOP	5
	.endasmfunc
