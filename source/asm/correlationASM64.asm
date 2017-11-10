;****************************************************************************************
; APPEL :
; int* correlationASM_optimise_bc64ech_glissante(int* p_signal, int p_signal_ref[], long long* p_sortie, int* p_moyenne);
;
;
; Parametres
; A4 : Pointeur vers la valeur la plus récente                 (p_signal1)
; B4 : Pointeur vers le premier element du signal de réference (p_signal_ref)
; A6 : Pointeur vers la valeur de sortie                       (p_sortie)
; B6 : Pointeur vers la moyenne du signal a correler           (p_moyenne)
;
; Variables locales
; B0    : compteur i
; A0    : copie du compteur i
; A3:A2 : accumulateur
; A5    : p_signal1[i]
; B5    : p_signal_ref[i]
; B7    : abs(p_signal1[i])
; B9:B8 : somme
; A9    : p_signal1[i]*p_signal_ref[i]
;
; Valeur de retour:
; Le pointeur de tampon circulaire (signal a correler) incrementé
;
; Code C de référence (avec buffer circulaire) :
; int* correlation64(int* p_signal1,
;                    int* p_signal_ref,
;                    long long* p_sortie,
;                    int* p_moyenne)
;{
;  int i = 64;
;  long long accumulateur = 0, somme = 0;
;  p_signal1++;
;  while(i--)
;  {
;    accumulateur += p_signal1[i]*p_signal_ref[i];
;	 somme += abs(p_signal1[i]);
;  }
;  *p_sortie = accumulateur;
;  *p_moyenne = (int)somme >> 6; // Diviser par 64
;}
;
;****************************************************************************************

	.def	_correlationASM64

_correlationASM64:

; Mettre 64 dans B0
    MVK  0x40, B0

; Initilisation de l'AMR buffer ciculaire taille 64 entiers sur A4
	MVC	AMR, B5
	STW	B5,*--B15
	MVKL 0xE00002, B9	; Utiliser A4 comme pointeur sur échantillons
	MVKH 0xE00002, B9	; Utiliser BK1 avec une longueur de 256 octets
	MVC B9, AMR

; longl long accumulateur = 0;
	MVK 0, A2
	MVK 0, A3

; long long somme = 0;
	MVK 0, B9
	MVK 0, B8

; A4 pointe vers le plus vieil element (p_signal1++)
  ||ADDAW A4,1,A4       ; Retorune le pointeur vers le prochain echantillon

BOUCLE_WHILE_I:

; i--
	SUB B0, 1, B0

; Copie de I dans le registre A0
    MV	B0, A0

; Chargement de p_signal1[i]
    LDW *+A4[A0], A5

; Chargement de p_signal_ref[i]
  ||LDW *+B4[B0], B5
    NOP 4

; p_signal1[i]*p_signal_ref[i]
    MPYI A5, B5, A9
	NOP 4
    [B0] B BOUCLE_WHILE_I
; somme += abs(p_signal1[i]);
	ABS  A5, B7
	ADD  B9:B8, B7, B9:B8
	NOP  2
; accumulateur += p_signal1[i]*p_signal_ref[i];
    ADD A9, A3:A2, A3:A2

END_BOUCLE_WHILE_I:

; *p_sortie = accumulateur;
    STW A2, *A6
    STW A3, *+A6[1]

; Rétablir le contenu de l'AMR
	B B3
  ||LDW *B15++, B5
; somme >> 6; // Diviser par 64
	SHR B9:B8, 6, B9:B8
;  *p_moyenne = somme >> 6; // Diviser par 64
	STW B8, *B6
	NOP	2
	MVC	B5, AMR
