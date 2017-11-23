;****************************************************************************************
; APPEL :
; int* moyenneMobileASM64(int* x, long* y, int taille);
;
; NE PAS APPELER DIRECTEMENT!!!!!!!!
;
; Parametres
; A4    : Pointeur vers la nouvelle valeur du tampon d'entrée, aligné sur 64 int  (x[])
; B4    : Pointeur vers la nouvelle valeur de sortie (à diviser par taille)       (y)
; A6    : Taille de la moyenne mobile                                             (taille)
;
; Variables locales
; A1    : compteur N
; A3:A2 : accumulateur
; A5    : x[-N]
; B8    : Valeur initiale de l'AMR
;
; Valeur de retour:
; Le pointeur du tampon d'entrée incrémenté.
;
; Code C de référence (avec buffer circulaire sur x) :
;int* moyenneMobileASM64(int* x, long* y, int taille);
;{
;  long accumulateur = 0;
;  int N = taille;
;  while(N--)
;  {
;     accumulateur += x[-N];
;  }
;  *y = accumulateur; // Ne pas oublier de diviser par la taille en C!!!
;  return x++;
;}
;
;****************************************************************************************

	.def	_moyenneMobileASM64

_moyenneMobileASM64:

;  int N = taille
    MV  A6, A1

;  long accumulateur = 0;
	MVK 0, A2
	MVK 0, A3

; Initilisation de l'AMR buffer ciculaire taille 64 entiers sur A4
	MVC	AMR, B8
	MVKL 0xE00002, B9	; Utiliser A4 comme pointeur sur échantillons
	MVKH 0xE00002, B9	; Utiliser BK1 avec une longueur de 256 octets
	MVC B9, AMR


; N--
	SUB A1, 1, A1

BOUCLE_WHILE_N:

    [A1] B BOUCLE_WHILE_N

; x[-N]
  ||LDW *-A4[A1], A5

; N--
	SUB A1, 1, A1

	NOP 3

; accumulateur += x[-N]
	SADD A3:A2,A5,A3:A2

END_BOUCLE_WHILE_N:

   B B3

;  *y = accumulateur
	STW A2, *B4
	STB A3, *+B4[4]

; return x++
    ADDAW A4,1,A4       ; Retorune le pointeur vers le prochain echantillon

; Rétablir le contenu de l'AMR
	MVC	B8, AMR

	NOP 1





