; SPRITE: Commodore 128 Assembly Language, Listing 16.3
;
; asmsyntax=asm68k  (6502/8502 ASM, but better syntax highlighting in vim)
; 
; The original listing is for the Merlin Assembler. This source has some
; changes for cc65.
.org $1300

COLOR   = $e0

TABLEN  = $800
NEWADR  = $4000         ; Color map

INDFET  = $ff74

CHRBAS  = $d000         ; New screen map

MVSRCE  = $fa
MVDEST  = MVSRCE+2

TABSIZ  = $0c00

HPSN    = TABSIZ+2
VPSN    = HPSN+2
CHAR    = VPSN+1
ROW     = CHAR+1
LINE    = ROW+1
BYTE    = LINE+1
BITT    = BYTE+2

MPRL    = BITT+1
MPRH    = MPRL+1
MPDL    = MPRH+1
MPDH    = MPDL+1
PRODL   = MPDH+1
PRODH   = PRODL+1
FILVAL  = PRODH+1
LENPTR  = FILVAL+1

        jmp START

;
; Subroutine to copy chr set into RAM
;
; Poke chr data into new location
;
COPYCHRS:
        lda #<CHRBAS
        sta MVSRCE
        lda #>CHRBAS
        sta MVSRCE+1

        lda #<NEWADR
        sta MVDEST
        lda #>NEWADR
        sta MVDEST+1
        
        lda #<TABLEN
        sta LENPTR
        lda #>TABLEN
        sta LENPTR+1
        
        ldy #0
        ldx LENPTR+1
        beq MVPART
MVPAGE: jsr GETDATA
        iny
        bne MVPAGE
        inc MVSRCE+1
        inc MVDEST+1
        dex
        bne MVPAGE
MVPART: ldx LENPTR
        beq MVEXIT
MVLAST: jsr GETDATA
        iny
        dex
        BNE MVLAST
MVEXIT: rts
;
; Subroutine to store (MVSRCE),Y IN (MVDEST),Y
;
GETDATA:
        pha             ; Save accumulator
        txa             ; Save X register
        pha
        lda #MVSRCE
        ldx #14         ; Get data in bank 14
        jsr INDFET
        sta (MVDEST),y  ; Store it in bank 15
        pla
        tax             ; Restore X register
        pla             ; Restore accumulator
        rts
;
; Main routine starts here
;
START:
        jsr COPYCHRS    ; Copy chr set into RAM
        lda #$20
        sta $d8         ; Activate high-res graphics


; ...
    rts
