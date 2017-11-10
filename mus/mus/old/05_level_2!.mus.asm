
; Tabla de instrumentos
TABLA_PAUTAS: DW PAUTA_0,PAUTA_1,PAUTA_2,PAUTA_3,PAUTA_4,PAUTA_5,0,PAUTA_7,PAUTA_8,PAUTA_9,PAUTA_10,PAUTA_11,PAUTA_12,PAUTA_13,PAUTA_14,PAUTA_15,PAUTA_16

; Tabla de efectos
TABLA_SONIDOS: DW SONIDO0,SONIDO1,SONIDO2,SONIDO3,SONIDO4,SONIDO5,SONIDO6,SONIDO7,SONIDO8,SONIDO9

;Pautas (instrumentos)
;Instrumento 'Piano'
PAUTA_0:	DB	8,0,7,0,6,0,7,0,129
;Instrumento 'Piano Reverb'
PAUTA_1:	DB	10,0,11,0,10,0,9,0,8,0,8,0,10,0,10,0,10,0,10,0,9,0,9,0,9,0,9,0,136
;Instrumento 'Fade In FX'
PAUTA_2:	DB	2,0,3,0,4,8,4,-1,9,-4,9,20,9,-24,8,4,132
;Instrumento 'Guitar 1'
PAUTA_3:	DB	13,0,13,0,11,0,11,0,9,0,10,1,10,1,10,-1,10,-1,10,0,10,0,134
;Instrumento 'Guitar 2'
PAUTA_4:	DB	13,0,13,0,12,0,11,0,9,0,10,1,10,0,10,0,10,-1,10,0,10,0,134
;Instrumento 'Eco guitar'
PAUTA_5:	DB	7,0,7,0,7,0,6,0,6,0,6,0,6,0,6,0,5,0,5,0,5,0,5,0,5,0,4,0,4,0,4,0,4,0,3,0,0,0,129
;Instrumento 'Solo Guitar'
PAUTA_7:	DB	75,0,10,0,10,0,10,0,9,0,10,1,10,0,10,-1,10,0,10,0,10,-1,10,0,10,1,10,0,10,0,138
;Instrumento 'Eco Solo Guitar'
PAUTA_8:	DB	70,0,6,0,6,0,5,0,5,0,5,0,5,0,5,0,4,0,4,0,4,0,4,0,4,0,4,0,3,0,3,0,3,0,3,0,3,0,3,0,2,0,2,0,2,0,1,0,129
;Instrumento 'Slap Bass'
PAUTA_9:	DB	45,0,12,4,11,-4,10,4,9,-5,10,0,129
;Instrumento 'Robo'
PAUTA_10:	DB	7,-1,23,5,23,9,6,0,22,5,22,9,5,0,21,5,21,9,4,0,20,5,20,9,6,0,22,5,22,9,131
;Instrumento 'Chip'
PAUTA_11:	DB	70,0,7,0,40,0,7,0,6,0,10,0,129
;Instrumento 'Clipclop'
PAUTA_12:	DB	13,0,12,0,11,0,11,0,11,0,11,0,11,0,11,0,10,0,10,0,10,0,10,0,10,0,138
;Instrumento 'Eco'
PAUTA_13:	DB	9,0,8,0,7,0,8,0,129
;Instrumento 'Harmonica'
PAUTA_14:	DB	42,0,11,0,12,0,11,0,10,0,11,0,129
;Instrumento 'Onda'
PAUTA_15:	DB	73,0,10,0,11,0,12,0,11,0,10,0,9,0,9,0,129
;Instrumento 'Teeth'
PAUTA_16:	DB	73,0,10,0,42,0,9,0,8,0,9,0,9,0,9,0,9,0,8,0,8,0,8,0,8,0,136

;Efectos
;Efecto 'bass drum'
SONIDO0:	DB	209,60,0,15,124,0,255
;Efecto 'bass drum vol 2'
SONIDO1:	DB	186,58,0,0,102,0,162,131,0,255
;Efecto 'drum'
SONIDO2:	DB	231,46,0,115,43,1,100,42,2,255
;Efecto 'drum 2'
SONIDO3:	DB	19,63,0,0,13,1,0,10,1,0,8,1,255
;Efecto 'hithat'
SONIDO4:	DB	0,12,1,0,6,1,255
;Efecto 'hit hat 2'
SONIDO5:	DB	0,12,1,255
;Efecto 'Bongo 1'
SONIDO6:	DB	186,30,0,232,25,0,0,40,0,69,38,0,255
;Efecto 'Bongo 2'
SONIDO7:	DB	69,46,0,186,41,0,46,56,0,232,54,0,255
;Efecto 'drum 3'
SONIDO8:	DB	232,44,0,0,92,5,23,108,5,232,124,5,255
;Efecto 'Lava 1'
SONIDO9:	DB	0,0,0,255

;Frecuencias para las notas
DATOS_NOTAS: DW 0,0
DW 1711,1614,1524,1438,1358,1281,1210,1142,1078,1017
DW 960,906,855,807,762,719,679,641,605,571
DW 539,509,480,453,428,404,381,360,339,320
DW 302,285,269,254,240,227,214,202,190,180
DW 170,160,151,143,135,127,120,113,107,101
DW 95,90,85,80,76,71,67,64,60,57
