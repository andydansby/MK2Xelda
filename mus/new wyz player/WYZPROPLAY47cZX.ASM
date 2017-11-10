ORG $C000

; ZX PSG proPLAYER V 0.3 - WYZ 19.03.2016


; ASSEMBLE WITH ASMSX



; THE DATA TO BE VARIED:
; * SOUND BUFFER WHERE THE MUSCLE FILE IS TOTALLY DECODED
; * NUMBER OF SONG.
; * TABLE OF SONGS





;______________________________________________________

; BEFORE STARTING (WITH DI):
INIT_BUFFERS:
        CALL    PLAYER_OFF
        
        LD      HL,BUFFER_DEC   	;* $80 BYTES FREE RAM MEMORY 
        LD      [CANAL_A],HL		;* $20 BYTES x CHANNEL SHOULD BE ENOUGH

        LD      HL,BUFFER_DEC+$10
        LD      [CANAL_B],HL
        
        LD      HL,BUFFER_DEC+$20
        LD      [CANAL_C],HL 

        LD      HL,BUFFER_DEC+$30
        LD      [CANAL_P],HL
RET

INICIO:
        CALL    ROUT

		LD	HL, PSG_REG
		LD	DE, PSG_REG_SEC
		LD	BC, 14
		LDIR
		CALL    REPRODUCE_SONIDO
		CALL    PLAY
        CALL    REPRODUCE_EFECTO
RET		

; REPRODUCE PERCUSION / SOUND
REPRODUCE_SONIDO:

		LD      HL,INTERR   
                BIT     2,[HL]          ;IS THIS PERCUSSION ACTIVATED?
                RET     Z
                LD      HL,[PUNTERO_SONIDO]
                LD      A,[HL]
                CP      $FF
                JR      Z,FIN_SONIDO
                LD	DE,[SFX_L]
                LD      [DE],A
                INC     HL
                LD      A,[HL]
                RRCA
                RRCA
                RRCA
                RRCA
                AND     00001111B
                LD	DE,[SFX_H]
                LD      [DE],A
                LD      A,[HL]
                AND     00001111B
                LD	DE,[SFX_V]
                LD      [DE],A
                
                INC     HL
                LD      A,[HL]
                LD	B,A
                BIT	7,A				;09.08.13 BIT MORE SIGNIFICANT ACTIVE ENVELOPES
                JR	Z,NO_ENVOLVENTES_SONIDO
                LD	A,$12
                LD	[DE],A
                INC	HL
                LD      A,[HL]
                LD	[PSG_REG_SEC+11],A
                INC	HL
                LD      A,[HL]
                LD	[PSG_REG_SEC+12],A
                INC	HL
                LD      A,[HL]
                CP	1
                JR	Z,NO_ENVOLVENTES_SONIDO		;DO NOT WRITE THE ENVIRONMENT IF YOUR VALUE IS 1
                LD	[PSG_REG_SEC+13],A                


NO_ENVOLVENTES_SONIDO:

		LD	A,B
		RES	7,A
		AND     A
                JR      Z,NO_RUIDO
                LD      [PSG_REG_SEC+6],A
                LD      A,[SFX_MIX]
                JR      SI_RUIDO
NO_RUIDO: 	XOR	A
		LD      [PSG_REG_SEC+6],A
		LD      A,10111000B
SI_RUIDO:       LD      [PSG_REG_SEC+7],A
       
                INC     HL
                LD      [PUNTERO_SONIDO],HL
                RET
FIN_SONIDO:     LD      HL,INTERR
                RES     2,[HL]
       		LD	A,[ENVOLVENTE_BACK]		;DO NOT RESTORE THE ENVIRONMENT IF YES 0
       		AND	A
       		JR	Z,FIN_NOPLAYER
       		;xor	a ; ***
       		LD	[PSG_REG_SEC+13],A			;08.13 RESTORING THE ENVIRONMENT AFTER THE SFX
       		
FIN_NOPLAYER:	LD      A,10111000B
       		LD      [PSG_REG_SEC+7],A

                RET                        
		


;BUFFER BACK SOUND TO PSG

ROUT:		LD	A,[PSG_REG+13]			
                AND	A			;ES CERO?
                JR	Z,NO_BACKUP_ENVOLVENTE
                LD	[ENVOLVENTE_BACK],A	;08.13 / SAVE THE ENVIRONMENT IN THE BACKUP


NO_BACKUP_ENVOLVENTE:

		
		

;RETURN SOUND BUFFER TO SPECTRUM PSG

	      	XOR     A
ROUT_A0:        LD      DE,$FFBF
                LD      BC,$FFFD
                LD      HL,PSG_REG_SEC
LOUT:           OUT     [C],A
                LD      B,E
                OUTI 
                LD      B,D
                INC     A
                CP      13
                JR      NZ,LOUT
                OUT     [C],A
                LD      A,[HL]
                AND     A
                RET     Z
                LD      B,E
                OUTI
                XOR     A
                LD      [PSG_REG_SEC+13],A
                LD	[PSG_REG+13],A
                RET


;START THE SOUND Nº [A]

INICIA_SONIDO:  ;CP	8		;SFX SPEECH
		;JP	Z,SLOOP		;

		LD      HL,TABLA_SONIDOS
                CALL    EXT_WORD
                LD      [PUNTERO_SONIDO],HL
                LD      HL,INTERR
                SET     2,[HL]
                RET
;PLAYER OFF

PLAYER_OFF:	XOR	A			;***** IMPORTANT NO MUSIC ****
		LD	[INTERR],A
		;LD	[FADE],A		;Only if there is fade out

CLEAR_PSG_BUFFER:
		LD	HL,PSG_REG
		LD	DE,PSG_REG+1
		LD	BC,14
		LD	[HL],A
		LDIR
		
		LD      A,10111000B		; **** JUST IN CASE ****
		LD      [PSG_REG+7],A
		
		LD	HL,PSG_REG
		LD	DE,PSG_REG_SEC
		LD	BC,14
		LDIR		

		CALL	ROUT
		RET


;LOAD A SONG
;IN:[A]=SONG NUMBER

CARGA_CANCION:
	LD      HL,INTERR
    SET     1,[HL]
    LD      HL,SONG
    LD      [HL],A          ;Nº A

;DECODE
;IN-> INTERR 0 ON
;     SONG
;CARGA CANCION SI/NO
DECODE_SONG:    
    LD      A,[SONG]
;THE HEAD OF THE SONG
;BYTE 0=TEMPO
    LD      HL,TABLA_SONG
    CALL    EXT_WORD
    
    ; decompress            
    ld		de, BUFFER_UNPACK
    push 	de
    push 	ix
    push 	iy
    call 	depack
    pop		iy
    pop		ix
    pop		de
    ld		hl, BUFFER_UNPACK
    ; fin
    
    LD      A, [HL]
    LD      [TEMPO],A
    DEC     A
    LD      [TTEMPO],A
    
;HEADER BYTE 1
;[-|-|-|-|  3-1 | 0  ]
;[-|-|-|-|FX CHN|LOOP]

    INC	HL		;LOOP 1=ON/0=OFF?
    LD	A,[HL]
    BIT	0,A
    JR	Z,NPTJP0
    PUSH	HL
    LD	HL,INTERR
    SET	4,[HL]
    POP	HL

    ;SELECTING THE RHYTHM EFFECT CHANNEL

NPTJP0:
    AND	00000110B 
    RRA
    ;LD	[SELECT_CANAL_P],A

    PUSH	HL
    LD	HL,TABLA_DATOS_CANAL_SFX
    CALL    EXT_WORD
    PUSH	HL
    POP	IX
    LD	E,[IX+0]
    LD	D,[IX+1]
    LD	[SFX_L],DE

    LD	E,[IX+2]
    LD	D,[IX+3]
    LD	[SFX_H],DE

    LD	E,[IX+4]
    LD	D,[IX+5]
    LD	[SFX_V],DE

    LD	A,[IX+6]
    LD	[SFX_MIX],A
    POP	HL
    
    INC	HL		;2 BYTES RESERVED
    INC	HL
    INC	HL

; SEARCH AND SAVE START OF THE CHANNELS IN THE MUS MODULE (OPTIMIZE****************)
;ADD LOOP OFFSET

    PUSH	HL			;IX HOME OFFSETS LOOP BY CHANNEL
    POP	IX
    
    LD	DE,$0008		;UP TO CHANNEL A
    ADD	HL,DE
    
    
    LD	[PUNTERO_P_DECA],HL	;HOME CHANNEL KEEPS POINTER
    LD	E,[IX+0]
    LD	D,[IX+1]
    ADD	HL,DE
    LD	[PUNTERO_L_DECA],HL	;LOOP START KEEPS POINTER

    CALL	BGICMODBC1
    LD	[PUNTERO_P_DECB],HL
    LD	E,[IX+2]
    LD	D,[IX+3]
    ADD	HL,DE
    LD	[PUNTERO_L_DECB],HL

    CALL	BGICMODBC1
    LD	[PUNTERO_P_DECC],HL
    LD	E,[IX+4]
    LD	D,[IX+5]
    ADD	HL,DE
    LD	[PUNTERO_L_DECC],HL
    
    CALL	BGICMODBC1
    LD	[PUNTERO_P_DECP],HL
    LD	E,[IX+6]
    LD	D,[IX+7]
    ADD	HL,DE
    LD	[PUNTERO_L_DECP],HL
    
                
;READ NOTES
;[|][|||||] LENGTH \ NOTE

INIT_DECODER:   LD      DE,[CANAL_A]
    LD      [PUNTERO_A],DE
    LD	HL,[PUNTERO_P_DECA]
    CALL    DECODE_CANAL    	;CHANNEL A
    LD	[PUNTERO_DECA],HL
    
    LD      DE,[CANAL_B]
    LD      [PUNTERO_B],DE
    LD	HL,[PUNTERO_P_DECB]
    CALL    DECODE_CANAL    	;CHANNEL B
    LD	[PUNTERO_DECB],HL
    
    LD      DE,[CANAL_C]
    LD      [PUNTERO_C],DE
    LD	HL,[PUNTERO_P_DECC]
    CALL    DECODE_CANAL    	;CHANNEL C
    LD	[PUNTERO_DECC],HL
    
    LD      DE,[CANAL_P]
    LD      [PUNTERO_P],DE
    LD	HL,[PUNTERO_P_DECP]
    CALL    DECODE_CANAL    	;CHANNEL P
    LD	[PUNTERO_DECP],HL

RET

;SEARCH CHANNEL START

BGICMODBC1:	XOR	A			;SEARCH BY BYTE 0
		LD	E,$3F			;INSTRUMENT CODE 0
		LD	B,$FF			;THE MODULE MUST HAVE A LENGTH LESS THAN $FF00 ... o_O!
		CPIR
		
		DEC	HL
		DEC	HL
		LD	A,E			;IS THE INSTRUMENT 0??
		CP	[HL]
		INC	HL
		INC	HL
		JR	Z,BGICMODBC1

		DEC	HL
		DEC	HL
		DEC	HL
		LD	A,E			;VOLUME 0??
		CP	[HL]
		INC	HL
		INC	HL
		INC	HL
		JR	Z,BGICMODBC1
		RET

;DECODES A CHANNEL NOTES
;IN [DE]=ADDRESS DESTINATION
;NOTA=0 FIN CHANNEL
;NOTA=1 SILENCE
;NOTA=2 length
;NOTA=3 COMMAND I

DECODE_CANAL:   LD      A,[HL]
                AND     A               ;FIN CHANNEL?
                JR      Z,FIN_DEC_CANAL
                CALL    GETLEN

                CP      00000001B       ;is SILENT?
                JR      NZ,NO_SILENCIO
                SET     6,A
                JR      NO_MODIFICA
                
NO_SILENCIO:    CP      00111110B       ;the length?
                JR      NZ,NO_PUNTILLO
                OR      A
                RRC     B
                XOR     A
                JR      NO_MODIFICA

NO_PUNTILLO:    CP      00111111B       ;FIN CHANNEL?
                JR      NZ,NO_MODIFICA
                BIT     0,B             ;COMMON = INSTRUMENT?
                JR      Z,NO_INSTRUMENTO   
                LD      A,11000001B     ;INSTRUMENT CODE      
                LD      [DE],A
                INC     HL
                INC     DE
                LD      A,[HL]          ;INSTRUMENT NO.
                LD      [DE],A
                INC     DE
                INC	HL
                LD      A,[HL]          ;RELATIVE VOLUME OF THE INSTRUMENT
                LD      [DE],A
                INC     DE
                INC	HL
                JR      DECODE_CANAL
                
NO_INSTRUMENTO: BIT     2,B
                JR      Z,NO_ENVOLVENTE
                LD      A,11000100B     ;ENVELOPE CODE
                LD      [DE],A
                INC     DE
                INC	HL
                LD	A,[HL]
                LD	[DE],A
                INC	DE
                INC	HL
                JR      DECODE_CANAL
     
NO_ENVOLVENTE:  BIT     1,B
                JR      Z,NO_MODIFICA           
                LD      A,11000010B     ;EFFECT CODE
                LD      [DE],A                  
                INC     HL                      
                INC     DE                      
                LD      A,[HL]                  
                CALL    GETLEN   
                
NO_MODIFICA:    LD      [DE],A
                INC     DE
                XOR     A
                DJNZ    NO_MODIFICA
		SET     7,A
		SET 	0,A
                LD      [DE],A
                INC     DE
                INC	HL
                RET			;** JR      DECODE_CHANNEL
                
FIN_DEC_CANAL:  SET     7,A
                LD      [DE],A
                INC     DE
                RET

GETLEN:         LD      B,A
                AND     00111111B
                PUSH    AF
                LD      A,B
                AND     11000000B
                RLCA
                RLCA
                INC     A
                LD      B,A
                LD      A,10000000B
DCBC0:          RLCA
                DJNZ    DCBC0
                LD      B,A
                POP     AF
                RET
                
                

        
                
;PLAY __________________________________________________


PLAY:          	LD      HL,INTERR       ;PLAY BIT 1 ON?
                BIT     1,[HL]
                RET     Z
;TEMPO          
                LD      HL,TTEMPO       ;TEMPO COUNTER
                INC     [HL]
                LD      A,[TEMPO]
                CP      [HL]
                JR      NZ,PAUTAS
                LD      [HL],0
                
;INTERPRETA      
                LD      IY,PSG_REG
                LD      IX,PUNTERO_A
                LD      BC,PSG_REG+8
                CALL    LOCALIZA_NOTA
                LD      IY,PSG_REG+2
                LD      IX,PUNTERO_B
                LD      BC,PSG_REG+9
                CALL    LOCALIZA_NOTA
                LD      IY,PSG_REG+4
                LD      IX,PUNTERO_C
                LD      BC,PSG_REG+10
                CALL    LOCALIZA_NOTA
                LD      IX,PUNTERO_P    ;THE CHANNEL OF EFFECTS MASKS ANOTHER CHANNEL
                CALL    LOCALIZA_EFECTO              

;PAUTAS 
                
PAUTAS:         LD      IY,PSG_REG+0
                LD      IX,PUNTERO_P_A
                LD      HL,PSG_REG+8
                CALL    PAUTA           ;CHANNEL GUIDE A
                LD      IY,PSG_REG+2
                LD      IX,PUNTERO_P_B
                LD      HL,PSG_REG+9
                CALL    PAUTA           ;CHANNEL GUIDE B
                LD      IY,PSG_REG+4
                LD      IX,PUNTERO_P_C
                LD      HL,PSG_REG+10
                CALL    PAUTA           ;CHANNEL GUIDE C                

                RET
                

               


;LOCATE CHANNEL NOTE A
;IN [PUNTERO_A]

;LOCATE CHANNEL NOTE A
;IN [PUNTERO_A]

LOCALIZA_NOTA:  LD      L,[IX+PUNTERO_A-PUNTERO_A]	;HL=[PUNTERO_A_C_B]
                LD      H,[IX+PUNTERO_A-PUNTERO_A+1]
                LD      A,[HL]
                AND     11000000B      			;COMMAND?
                CP      11000000B
                JR      NZ,LNJP0

;BIT[0]=INSTRUMENTO
                
COMANDOS:       LD      A,[HL]
                BIT     0,A             		;INSTRUMENT
                JR      Z,COM_EFECTO

                INC     HL
                LD      A,[HL]          		;NUMBER OF GUIDELINES
                INC     HL
                LD	E,[HL]				
                
                PUSH	HL				;;TEMPO ******************
                LD	HL,TEMPO
                BIT	5,E
                JR	Z,NO_DEC_TEMPO
                DEC	[HL]
NO_DEC_TEMPO:	BIT	6,E
		JR	Z,NO_INC_TEMPO
		INC	[HL]
NO_INC_TEMPO:	RES	5,E				;ALWAYS RESET THE TIME BITS		
		RES	6,E
		POP	HL
                
		LD      [IX+VOL_INST_A-PUNTERO_A],E	;REGISTRATION OF RELATIVE VOLUME
                INC	HL
                LD      [IX+PUNTERO_A-PUNTERO_A],L
                LD      [IX+PUNTERO_A-PUNTERO_A+1],H
                LD      HL,TABLA_PAUTAS
                CALL    EXT_WORD
                LD      [IX+PUNTERO_P_A0-PUNTERO_A],L
                LD      [IX+PUNTERO_P_A0-PUNTERO_A+1],H
                LD      [IX+PUNTERO_P_A-PUNTERO_A],L
                LD      [IX+PUNTERO_P_A-PUNTERO_A+1],H
                LD      L,C
                LD      H,B
                RES     4,[HL]        			;DELETE ENVELOPE EFFECT
                XOR     A
                LD      [PSG_REG_SEC+13],A
                LD	[PSG_REG+13],A
                ;LD	[ENVOLVENTE_BACK],A		;08.13 / RESET THE ENVIRONMENT BACKUP
                JR      LOCALIZA_NOTA

COM_EFECTO:     BIT     1,A             		;SOUND EFFECT
                JR      Z,COM_ENVOLVENTE

                INC     HL
                LD      A,[HL]
                INC     HL
                LD      [IX+PUNTERO_A-PUNTERO_A],L
                LD      [IX+PUNTERO_A-PUNTERO_A+1],H
                CALL    INICIA_SONIDO
                RET

COM_ENVOLVENTE: BIT     2,A
                RET     Z               		;IGNORE - ERROR          
           
                INC     HL
                LD	A,[HL]			;LOAD ENVELOPE CODE
                LD	[ENVOLVENTE],A
                INC     HL
                LD      [IX+PUNTERO_A-PUNTERO_A],L
                LD      [IX+PUNTERO_A-PUNTERO_A+1],H
                LD      L,C
                LD      H,B
                LD	[HL],00010000B          	;LIGHTS INVOLVING EFFECT
                JR      LOCALIZA_NOTA
                
              
LNJP0:          LD      A,[HL]
                INC     HL
                BIT     7,A
                JR      Z,NO_FIN_CANAL_A	;
                BIT	0,A
                JR	Z,FIN_CANAL_A

FIN_NOTA_A:	LD      E,[IX+CANAL_A-PUNTERO_A]
		LD	D,[IX+CANAL_A-PUNTERO_A+1]	;BUFFER POINTER TO HOME
		LD	[IX+PUNTERO_A-PUNTERO_A],E
		LD	[IX+PUNTERO_A-PUNTERO_A+1],D
		LD	L,[IX+PUNTERO_DECA-PUNTERO_A]	;LOAD POINTER DECODER
		LD	H,[IX+PUNTERO_DECA-PUNTERO_A+1]
		PUSH	BC
                CALL    DECODE_CANAL    		;CHANNEL DECODING
                POP	BC
                LD	[IX+PUNTERO_DECA-PUNTERO_A],L	;KEEPING POINTER DECODER
                LD	[IX+PUNTERO_DECA-PUNTERO_A+1],H
                JP      LOCALIZA_NOTA
                
FIN_CANAL_A:    LD	HL,INTERR			;LOOP?
                BIT	4,[HL]              
                JR      NZ,FCA_CONT
                POP	AF
                JP	PLAYER_OFF
                

FCA_CONT:	LD	L,[IX+PUNTERO_L_DECA-PUNTERO_A]	;INITIAL POINT DECODER LOAD
		LD	H,[IX+PUNTERO_L_DECA-PUNTERO_A+1]
		LD	[IX+PUNTERO_DECA-PUNTERO_A],L
		LD	[IX+PUNTERO_DECA-PUNTERO_A+1],H
		JR      FIN_NOTA_A
                
NO_FIN_CANAL_A: LD      [IX+PUNTERO_A-PUNTERO_A],L        	;[PUNTERO_A_B_C] = HL GUARD POINTER
                LD      [IX+PUNTERO_A-PUNTERO_A+1],H
                AND     A               		;DO NOT REPEAT NOTE YES NOTE = 0
                JR      Z,FIN_RUTINA
                BIT     6,A             		;SILENCE?
                JR      Z,NO_SILENCIO_A
                LD	A,[BC]
                AND	00010000B
                JR	NZ,SILENCIO_ENVOLVENTE
                
                XOR     A
                LD	[BC],A				;RESET VOLUME OF CORRESPONDING CHIP
                LD	[IY+0],A
                LD	[IY+1],A
		RET
		
SILENCIO_ENVOLVENTE:
		LD	A,$FF
                LD	[PSG_REG+11],A
                LD	[PSG_REG+12],A               
                XOR	A
                LD	[PSG_REG+13],A                               
                LD	[IY+0],A
                LD	[IY+1],A
                RET

NO_SILENCIO_A:  LD	[IX+REG_NOTA_A-PUNTERO_A],A	;REGISTRO DE LA NOTA DEL CANAL         
		CALL    NOTA            		;REPRODUCE NOTA
                LD      L,[IX+PUNTERO_P_A0-PUNTERO_A]     ;HL=[PUNTERO_P_A0] RESETEA PAUTA 
                LD      H,[IX+PUNTERO_P_A0-PUNTERO_A+1]
                LD      [IX+PUNTERO_P_A-PUNTERO_A],L       ;[PUNTERO_P_A]=HL
                LD      [IX+PUNTERO_P_A-PUNTERO_A+1],H
FIN_RUTINA:     RET


;LOCALIZA EFECTO
;IN HL=[PUNTERO_P]

LOCALIZA_EFECTO:LD      L,[IX+0]       ;HL=[PUNTERO_P]
                LD      H,[IX+1]
                LD      A,[HL]
                CP      11000010B
                JR      NZ,LEJP0

                INC     HL
                LD      A,[HL]
                INC     HL
                LD      [IX+00],L
                LD      [IX+01],H
                CALL    INICIA_SONIDO
                RET
            
              
LEJP0:          INC     HL
                BIT     7,A
                JR      Z,NO_FIN_CANAL_P	;
                BIT	0,A
                JR	Z,FIN_CANAL_P
FIN_NOTA_P:	LD      DE,[CANAL_P]
		LD	[IX+0],E
		LD	[IX+1],D
		LD	HL,[PUNTERO_DECP]	;CARGA PUNTERO DECODER
		PUSH	BC
		CALL    DECODE_CANAL    	;DECODIFICA CANAL
		POP	BC
                LD	[PUNTERO_DECP],HL	;GUARDA PUNTERO DECODER
                JP      LOCALIZA_EFECTO
                
FIN_CANAL_P:	LD	HL,[PUNTERO_L_DECP]	;CARGA PUNTERO INICIAL DECODER
		LD	[PUNTERO_DECP],HL
		JR      FIN_NOTA_P
                
NO_FIN_CANAL_P: LD      [IX+0],L        ;[PUNTERO_A_B_C]=HL GUARDA PUNTERO
                LD      [IX+1],H
                RET

; PAUTA DE LOS 3 CANALES
; IN:[IX]:PUNTERO DE LA PAUTA
;    [HL]:REGISTRO DE VOLUMEN
;    [IY]:REGISTROS DE FRECUENCIA

; FORMATO PAUTA	
;	    7    6     5     4   3-0                        3-0  
; BYTE 1 [LOOP|OCT-1|OCT+1|ORNMT|VOL] - BYTE 2 [ | | | |PITCH/NOTA]

PAUTA:          BIT     4,[HL]        ;SI LA ENVOLVENTE ESTA ACTIVADA NO ACTUA PAUTA
                RET     NZ

		LD	A,[IY+0]
		LD	B,[IY+1]
		OR	B
		RET	Z


                PUSH	HL
           
PCAJP4:         LD      L,[IX+0]
                LD      H,[IX+1]         
		LD	A,[HL]
		
		BIT     7,A		;LOOP / EL RESTO DE BITS NO AFECTAN
                JR      Z,PCAJP0
                AND     00011111B       ;MÁXIMO LOOP PAUTA [0,32]X2!!!-> PARA ORNAMENTOS
                RLCA			;X2
                LD      D,0
                LD      E,A
                SBC     HL,DE
                LD      A,[HL]

PCAJP0:		BIT	6,A		;OCTAVA -1
		JR	Z,PCAJP1
		LD	E,[IY+0]
		LD	D,[IY+1]

		AND	A
		RRC	D
		RR	E
		LD	[IY+0],E
		LD	[IY+1],D
		JR	PCAJP2
		
PCAJP1:		BIT	5,A		;OCTAVA +1
		JR	Z,PCAJP2
		LD	E,[IY+0]
		LD	D,[IY+1]

		AND	A
		RLC	E
		RL	D
		LD	[IY+0],E
		LD	[IY+1],D		




PCAJP2:		LD	A,[HL]
		BIT	4,A
		JR	NZ,PCAJP6	;ORNAMENTOS SELECCIONADOS

		INC     HL		;______________________ FUNCION PITCH DE FRECUENCIA__________________		
		PUSH	HL
		LD	E,A
		LD	A,[HL]		;PITCH DE FRECUENCIA
		LD	L,A
		AND	A
		LD	A,E
		JR	Z,ORNMJP1

                LD	A,[IY+0]	;SI LA FRECUENCIA ES 0 NO HAY PITCH
                ADD	A,[IY+1]
                AND	A
                LD	A,E
                JR	Z,ORNMJP1
                

		BIT	7,L
		JR	Z,ORNNEG
		LD	H,$FF
		JR	PCAJP3
ORNNEG:		LD	H,0
		
PCAJP3:		LD	E,[IY+0]
		LD	D,[IY+1]
		ADC	HL,DE
		LD	[IY+0],L
		LD	[IY+1],H
		JR	ORNMJP1


PCAJP6:		INC	HL		;______________________ FUNCION ORNAMENTOS__________________	
		
		PUSH	HL
		PUSH	AF
		LD	A,[IX+REG_NOTA_A-PUNTERO_P_A]	;RECUPERA REGISTRO DE NOTA EN EL CANAL
		LD	E,[HL]		;
		ADC	A, E		;+- NOTA 
		CALL	TABLA_NOTAS
		POP	AF	
		
		
ORNMJP1:	POP	HL
		
		INC	HL
                LD      [IX+0],L
                LD      [IX+1],H
PCAJP5: POP HL
        AND 15
        LD  C, A
        LD  A,[IX+VOL_INST_A-PUNTERO_P_A]   ;VOLUMEN RELATIVO
        BIT 4, A
        JR  Z, PCAJP9
        OR  $F0
PCAJP9: ADD A, C
        JP  P,PCAJP7
        LD  A, 1        ;NO SE EXTINGUE EL VOLUMEN
PCAJP7: CP 15
        JP M, PCAJP8
        LD A, 15
PCAJP8: LD      [HL],A
        RET



;NOTA : REPRODUCE UNA NOTA
;IN [A]=CODIGO DE LA NOTA
;   [IY]=REGISTROS DE FRECUENCIA


NOTA:		LD      L,C
                LD      H,B
                BIT     4,[HL]
     		LD      B,A
                JR	NZ,EVOLVENTES
      		LD	A,B
TABLA_NOTAS:    LD      HL,DATOS_NOTAS		;BUSCA FRECUENCIA
		CALL	EXT_WORD
		LD      [IY+0],L
                LD      [IY+1],H
                RET




;IN [A]=CODIGO DE LA ENVOLVENTE
;   [IY]=REGISTRO DE FRECUENCIA

EVOLVENTES:     LD      HL,DATOS_NOTAS
		;SUB	12
		RLCA                    ;X2
                LD      D,0
                LD      E,A
                ADD     HL,DE
                LD	E,[HL]
		INC	HL
		LD	D,[HL]
		
		PUSH	DE
		LD	A,[ENVOLVENTE]		;FRECUENCIA DEL CANAL ON/OFF
		RRA
		JR	NC,FRECUENCIA_OFF
		LD      [IY+0],E
                LD      [IY+1],D
		JR	CONT_ENV
				
FRECUENCIA_OFF:	LD 	DE,$0000
		LD      [IY+0],E
                LD      [IY+1],D
					;CALCULO DEL RATIO (OCTAVA ARRIBA)
CONT_ENV:	POP	DE
		PUSH	AF
		PUSH	BC
		AND	00000011B
		LD	B,A
		;INC	B
		
		;AND	A			;1/2
		RR	D
		RR	E
CRTBC0:		;AND	A			;1/4 - 1/8 - 1/16
		RR	D
		RR	E
		DJNZ	CRTBC0
		LD	A,E
                LD      [PSG_REG+11],A
                LD	A,D
                AND	00000011B
                LD      [PSG_REG+12],A
		POP	BC
                POP	AF			;SELECCION FORMA DE ENVOLVENTE
                
                RRA
                AND	00000110B		;$08,$0A,$0C,$0E
                ADD	A, 8                
                LD      [PSG_REG+13],A
           	LD	[ENVOLVENTE_BACK],A
                RET


;EXTRAE UN WORD DE UNA TABLA
;IN:[HL]=DIRECCION TABLA
;   [A]= POSICION
;OUT[HL]=WORD

EXT_WORD:       LD      D,0
                RLCA
                LD      E,A
                ADD     HL,DE
                LD      E,[HL]
                INC     HL
                LD      D,[HL]
                EX      DE,HL
                RET

                
INICIA_EFECTO:	LD	A,B
		LD      HL,TABLA_EFECTOS
                CALL    EXT_WORD
                LD      [PUNTERO_EFECTO],HL
		LD      HL,INTERR
                SET     3,[HL]
                RET       

                
;;;;;;;; SOUND EFFECTS ;;;;;;;;;;;
                
REPRODUCE_EFECTO:

                LD      HL,INTERR   
                BIT     3,[HL]          	;ESTA ACTIVADO EL EFECTO?
                RET     Z
                LD      HL,[PUNTERO_EFECTO]
                LD      A,[HL]
                CP      $FF
                JP      Z,FIN_EFECTO
                LD	B,A			;FRECUENCIA FINO
                INC     HL
                LD	A,[HL]
                RRCA
                RRCA
                RRCA
                RRCA
                AND     00001111B
                LD	C,A			;	FRECUENCIA GRAVE
		;LD      A,10111000B		;	ELIMINA RUIDO
       		;LD      [PSG_REG_SEC+7],A
                LD      A,[HL]
                DEC	A			;DEC A PARA BAJR VOLUMEN!! O PONER VARIABLE
                ;DEC	A
                AND     00001111B

                LD	D,A			;VOLUMEN
                INC     HL			;INCREMENTA Y GUARDA EL PUNTERO
                LD      [PUNTERO_EFECTO],HL     
           	LD	IX,PSG_REG_SEC
                LD	A,[CANAL_EFECTOS]	;SELECCION DE CANAL *********
                CP	1
                JR	Z,RS_CANALA
                CP	2
		JR	Z,RS_CANALB
		
RS_CANALC:  	LD      [IX+4],B
		LD      [IX+5],C
                LD      [IX+10],D
                RET		
		
RS_CANALA:	LD      [IX+0],B
		LD      [IX+1],C
                LD      [IX+8],D
                RET
                
RS_CANALB:	LD      [IX+2],B
		LD      [IX+3],C
                LD      [IX+9],D
                RET
                
FIN_EFECTO:     LD      HL,INTERR
                RES     3,[HL]			;DESACTIVA EFECTO
                RET         
                
; APLIB
; aPPack decompressor
; original source by dwedit
; very slightly adapted by utopian
; optimized by Metalbrain

;hl = source
;de = dest

depack:		ld	ixl,128
apbranch1:	ldi
aploop0:	ld	ixh,1		;LWM = 0
aploop:		call 	ap_getbit
		jr 	nc,apbranch1
		call 	ap_getbit
		jr 	nc,apbranch2
		ld 	b,0
		call 	ap_getbit
		jr 	nc,apbranch3
		ld	c,16		;get an offset
apget4bits:	call 	ap_getbit
		rl 	c
		jr	nc,apget4bits
		jr 	nz,apbranch4
		ld 	a,b
apwritebyte:	ld 	(de),a		;write a 0
		inc 	de
		jr	aploop0
apbranch4:	and	a
		ex 	de,hl 		;write a previous byte (1-15 away from dest)
		sbc 	hl,bc
		ld 	a,(hl)
		add	hl,bc
		ex 	de,hl
		jr	apwritebyte
apbranch3:	ld 	c,(hl)		;use 7 bit offset, length = 2 or 3
		inc 	hl
		rr 	c
		ret 	z		;if a zero is encountered here, it is EOF
		ld	a,2
		adc	a,b
		push 	hl
		ld	iyh,b
		ld	iyl,c
		ld 	h,d
		ld 	l,e
		sbc 	hl,bc
		ld 	c,a
		jr	ap_finishup2
apbranch2:	call 	ap_getgamma	;use a gamma code * 256 for offset, another gamma code for length
		dec 	c
		ld	a,c
		sub	ixh
		jr 	z,ap_r0_gamma		;if gamma code is 2, use old r0 offset,
		dec 	a
		;do I even need this code?
		;bc=bc*256+(hl), lazy 16bit way
		ld 	b,a
		ld 	c,(hl)
		inc 	hl
		ld	iyh,b
		ld	iyl,c

		push 	bc
		
		call 	ap_getgamma

		ex 	(sp),hl		;bc = len, hl=offs
		push 	de
		ex 	de,hl

		ld	a,4
		cp	d
		jr 	nc,apskip2
		inc 	bc
		or	a
apskip2:	ld 	hl,127
		sbc 	hl,de
		jr 	c,apskip3
		inc 	bc
		inc 	bc
apskip3:		pop 	hl		;bc = len, de = offs, hl=junk
		push 	hl
		or 	a
ap_finishup:	sbc 	hl,de
		pop 	de		;hl=dest-offs, bc=len, de = dest
ap_finishup2:	ldir
		pop 	hl
		ld	ixh,b
		jr 	aploop

ap_r0_gamma:	call 	ap_getgamma		;and a new gamma code for length
		push 	hl
		push 	de
		ex	de,hl

		ld	d,iyh
		ld	e,iyl
		jr 	ap_finishup


ap_getbit:	ld	a,ixl
		add	a,a
		ld	ixl,a
		ret	nz
		ld	a,(hl)
		inc	hl
		rla
		ld	ixl,a
		ret

ap_getgamma:	ld 	bc,1
ap_getgammaloop:	call 	ap_getbit
		rl 	c
		rl 	b
		call 	ap_getbit
		jr 	c,ap_getgammaloop
		ret          
            

;RHYTHM EFFECT CHANNEL SELECTOR DATA CHART
TABLA_DATOS_CANAL_SFX:

		DW	SELECT_CANAL_A,SELECT_CANAL_B,SELECT_CANAL_C


;BYTE 0:SFX_L	
;BYTE 1:SFX_H	
;BYTE 2:SFX_V	
;BYTE 3:SFX_MIX

SELECT_CANAL_A:	DW	PSG_REG_SEC+0,PSG_REG_SEC+1,PSG_REG_SEC+8
		DB	10110001B
		
SELECT_CANAL_B:	DW	PSG_REG_SEC+2,PSG_REG_SEC+3,PSG_REG_SEC+9
		DB	10101010B
		
SELECT_CANAL_C:	DW	PSG_REG_SEC+4,PSG_REG_SEC+5,PSG_REG_SEC+10
		DB	10011100B


;____________________________________


INTERR:         DB     0	        ;INTERRUPTORES 1=ON 0=OFF
                                        ;BIT 0=CARGA CANCION ON/OFF
                                        ;BIT 1=PLAYER ON/OFF
                                        ;BIT 2=EFECTOS ON/OFF
                                        ;BIT 3=SFX ON/OFF
                                        ;BIT 4=LOOP
;CONTROL OF INTERRUPTIONS

;HOOK:           DB     0

;MUSICA **** EL ORDEN DE LAS VARIABLES ES FIJO ******



SONG:           DB     0	       ;DBNº DE CANCION
TEMPO:          DB     0	       ;DB TEMPO
TTEMPO:         DB     0	       ;DB CONTADOR TEMPO

PUNTERO_A:      DW     0	       ;DW PUNTERO DEL CANAL A
PUNTERO_B:      DW     0	       ;DW PUNTERO DEL CANAL B
PUNTERO_C:      DW     0	       ;DW PUNTERO DEL CANAL C

CANAL_A:        DW     0		;DW DIRECION DE INICIO DE LA MUSICA A
CANAL_B:        DW     0      		;DW DIRECION DE INICIO DE LA MUSICA B
CANAL_C:        DW     0      		;DW DIRECION DE INICIO DE LA MUSICA C

PUNTERO_P_A:    DW     0      		;DW PUNTERO PAUTA CANAL A
PUNTERO_P_B:    DW     0	        ;DW PUNTERO PAUTA CANAL B
PUNTERO_P_C:    DW     0	        ;DW PUNTERO PAUTA CANAL C

PUNTERO_P_A0:   DW     0      		;DW INI PUNTERO PAUTA CANAL A
PUNTERO_P_B0:   DW     0      		;DW INI PUNTERO PAUTA CANAL B
PUNTERO_P_C0:   DW     0      		;DW INI PUNTERO PAUTA CANAL C

PUNTERO_P_DECA:	DW	0		;DW PUNTERO DE INICIO DEL DECODER CANAL A
PUNTERO_P_DECB:	DW	0		;DW PUNTERO DE INICIO DEL DECODER CANAL B
PUNTERO_P_DECC:	DW	0		;DW PUNTERO DE INICIO DEL DECODER CANAL C

PUNTERO_DECA:	DW	0	;DW PUNTERO DECODER CANAL A
PUNTERO_DECB:	DW	0	;DW PUNTERO DECODER CANAL B
PUNTERO_DECC:	DW	0	;DW PUNTERO DECODER CANAL C       

REG_NOTA_A:	DB	0	;DB REGISTRO DE LA NOTA EN EL CANAL A
VOL_INST_A:	DB	0	;DB VOLUMEN RELATIVO DEL INSTRUMENTO DEL CANAL A
REG_NOTA_B:	DB	0	;DB REGISTRO DE LA NOTA EN EL CANAL B
VOL_INST_B:	DB	0	;DB VOLUMEN RELATIVO DEL INSTRUMENTO DEL CANAL B					;VACIO
REG_NOTA_C:	DB	0	;DB REGISTRO DE LA NOTA EN EL CANAL C
VOL_INST_C:	DB	0	;DB VOLUMEN RELATIVO DEL INSTRUMENTO DEL CANAL C

PUNTERO_L_DECA:	DW	0	;DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL A
PUNTERO_L_DECB:	DW	0	;DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL B
PUNTERO_L_DECC:	DW	0	;DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL C

;CANAL DE EFECTOS DE RITMO - ENMASCARA OTRO CANAL

PUNTERO_P:      DW      0      	;DW PUNTERO DEL CANAL EFECTOS
CANAL_P:        DW      0      	;DW DIRECION DE INICIO DE LOS EFECTOS
PUNTERO_P_DECP:	DW	0	;DW PUNTERO DE INICIO DEL DECODER CANAL P
PUNTERO_DECP:	DW	0	;DW PUNTERO DECODER CANAL P
PUNTERO_L_DECP:	DW	0	;DW PUNTERO DE INICIO DEL LOOP DEL DECODER CANAL P
;SELECT_CANAL_P	DB	INTERR+$36	;DB SELECCION DE CANAL DE EFECTOS DE RITMO


SFX_L:		DW	0	;DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO BAJO
SFX_H:		DW	0	;DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO ALTO
SFX_V:		DW	0	;DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO VOLUMEN
SFX_MIX:	DW	0	;DW DIRECCION BUFFER EFECTOS DE RITMO REGISTRO MIXER


;PERCUSIONES
N_SONIDO:        DB     0      ;DB : NUMERO DE SONIDO
PUNTERO_SONIDO:  DW     0      ;DW : PUNTERO DEL SONIDO QUE SE REPRODUCE

;SFX
N_EFECTO	       	DB 0   ;DB : NUMERO DE SONIDO
PUNTERO_EFECTO		DW 0	;DW : PUNTERO DEL SONIDO QUE SE REPRODUCE;REPRODUCE EFECTOS
CANAL_EFECTOS		DB 1	;DB : 1:CANAL A - 2:CANAL B - OTRO:CANAL C

;DB [13] BUFFERs DE REGISTROS DEL PSG

PSG_REG:         DS	16
PSG_REG_SEC:	 DS	16
ENVOLVENTE:	 DB	0		;DB : FORMA DE LA ENVOLVENTE
               				;BIT 0	  : FRECUENCIA CANAL ON/OFF
               				;BIT 1-2  : RATIO 
               				;BIT 3-3  : FORMA	
ENVOLVENTE_BACK: DB	0		;DB:	BACKUP DE LA FORMA DE LA ENVOLENTE

;; INCLUDE MUSIC DATA (PATTERNS/EFFECTS)

INCLUDE "instrumentos.asm"

;; Songs have to be compressed with aplib
                
SONG_0:			INCBIN "menu-c.mus" 			; happy
SONG_1:			INCBIN "ending-c.mus" 			; nadar


;; INCLUDING SOUND EFFECTS:

				INCLUDE "efectos.asm"

TABLA_SONG:     DW      SONG_0, SONG_1
TABLA_EFECTOS:  DW  	EFECTO0, EFECTO1, EFECTO2, EFECTO3, EFECTO4, EFECTO5, EFECTO6, EFECTO7, EFECTO8, EFECTO9
				DW		EFECTO10, EFECTO11, EFECTO12, EFECTO13, EFECTO14, EFECTO15, EFECTO16, EFECTO17, EFECTO18

;; CURRENT SONG WILL DECOMPOSE HERE:
BUFFER_UNPACK:	DS	9400

BUFFER_DEC: DS $40
