# s5-dsp
Code CCS de la partie DSP du projet de souris accélérométrique 'Mini-S'.

# Communication PIC <-> DSP

Pour assurer une réutilisation du code, utliser des formats comme **uint8_t**, uint16_t ou uint32_t de la librairie <stdint.h> pour déclarer des variables de la taille d'un ou plusieurs octets.

## Encodage

L'encodage choisi est le [*Consistent Overhead Byte Stuffing* (COBS)](https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing). Cet encodage utilise l'octet 0x00 pour définir la fin d'une trame, ce qui permet au récepteur de se synchroniser si il perd le lien ou (re)démarre.

## Format des trames

Le format des messages doit être prédéfini et commun aux deux périphériques. Un classeur Excel disponible sur le Team de l'équipe permet de documenter et mettre en commun les trames échangées entre le PIC et le DSP. Les 7 LSB du premier octet de chaque trame (après celui d'overhead) définit le type de trame. Les types disponible vont de 0x00 à 0x7F (128 messages possible). Le MSB du premier octet définit si il s'agit d'une trame avec un CRC (1) ou si il s'agit d'une trame sans CRC (0). La fonction CRC utilisée est le CRC-8 et l'octet réservé à ce dernier se situe à la fin du message.

## Timing

La communication UART entre le DSP et le PIC se fait à 115200 bauds, 8-bits, 1 start bit, 2 stop bits, pas de parité.

### Timing et framing de l'accéléromètre

Pour une fréquence d'échantillonage de 400 Hz, chaque trame ne doit pas dépasser la taille de 115200\*8/11/400 = 209 octets, ce qui est amplement suffisant.
