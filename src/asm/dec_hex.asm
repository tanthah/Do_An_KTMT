section .data
    err_msg db "Du lieu khong hop le", 0
    hex_chars db "0123456789ABCDEF"

section .text
    global DecToHexAsm
    global HexToDecAsm

; ---------------------------------------------------------
; Helper: Write Error Message to Output
; ---------------------------------------------------------
WriteError:
    lea rsi, [rel err_msg]  ; load error message address
    mov rdi, rdx            ; rdi = output buffer
.copy_err:
    mov al, byte [rsi]      ; load byte from message
    mov byte [rdi], al      ; write to output
    test al, al             ; check null terminator
    jz .done                ; if done, return
    inc rsi
    inc rdi
    jmp .copy_err
.done:
    ret 

; ---------------------------------------------------------
; Helper: Parse Decimal String (RCX) to EAX
; Returns CF=1 if error, CF=0 if success (EAX valid)
; ---------------------------------------------------------
ParseDec:
    xor eax, eax            ; result = 0
    xor r8, r8              ; r8 = sign flag
    mov r9, rcx             ; r9 = input string pointer

    mov bl, byte [r9]
    test bl, bl
    jz .err                 ; empty string -> error

    cmp bl, '-'
    jne .parse_loop
    mov r8, 1               ; negative flag
    inc r9
    mov bl, byte [r9]
    test bl, bl
    jz .err                 ; just "-" -> error

.parse_loop:
    mov bl, byte [r9]
    test bl, bl
    jz .done
    cmp bl, '0'
    jl .err
    cmp bl, '9'
    jg .err

    sub bl, '0'             ; convert ASCII to digit value
    mov r10d, 10
    imul eax, r10d          ; EAX *= 10
    movzx ebx, bl           ; zero-extend digit
    add eax, ebx            ; EAX += digit

    inc r9
    jmp .parse_loop

.done:
    test r8, r8
    jz .success
    neg eax                 ; negate if negative
.success:
    clc                     ; clear carry flag
    ret
.err:
    stc                     ; set carry flag for error
    ret

; ---------------------------------------------------------
; Helper: Parse Hex String (RCX) to EAX
; Returns CF=1 if error, CF=0 if success
; ---------------------------------------------------------
ParseHex:
    xor eax, eax            ; result = 0
    mov r9, rcx             ; r9 = input pointer

    mov bl, byte [r9]
    test bl, bl
    jz .err                 ; empty string -> error

.parse_loop:
    mov bl, byte [r9]
    test bl, bl
    jz .done

    cmp bl, 'H'             ; check for optional 'H'/'h' suffix
    je .check_last_h
    cmp bl, 'h'
    je .check_last_h
    jmp .not_h

.check_last_h:
    cmp r9, rcx             ; first char can't be 'H'
    je .err
    cmp byte [r9+1], 0      ; 'H' must be last
    je .done

.not_h:
    shl eax, 4              ; EAX *= 16 (make room for next hex digit)

    cmp bl, '0'
    jl .err
    cmp bl, '9'
    jle .digit
    cmp bl, 'A'
    jl .err
    cmp bl, 'F'
    jle .upper
    cmp bl, 'a'
    jl .err
    cmp bl, 'f'
    jle .lower
    jmp .err

.digit:
    sub bl, '0'             ; convert '0'-'9' to value
    jmp .add_val
.upper:
    sub bl, 'A'             ; convert 'A'-'F' to 0-5
    add bl, 10              ; then to 10-15
    jmp .add_val
.lower:
    sub bl, 'a'             ; convert 'a'-'f' to 0-5
    add bl, 10              ; then to 10-15
.add_val:
    movzx ebx, bl           ; zero-extend to 32-bit
    add eax, ebx            ; EAX += hex digit

    inc r9
    jmp .parse_loop

.done:
    clc                     ; clear carry for success
    ret
.err:
    stc                     ; set carry for error
    ret

; ---------------------------------------------------------
; Helper: Format EAX to Decimal String in RDX
; ---------------------------------------------------------
FormatDec: ;RDX = output buffer, EAX = value to format
    mov rdi, rdx            ; rdi = output buffer
    mov r8d, eax            ; r8d = value to format
    test r8d, r8d
    jns .positive
    mov byte [rdi], '-'     ; write minus sign if negative
    inc rdi
    neg r8d

.positive:
    mov eax, r8d
    mov rcx, 10
    xor r9, r9              ; digit counter

.div_loop:
    xor edx, edx            ; clear edx for division
    div ecx                 ; EAX / 10, remainder in EDX
    push rdx                ; push digit
    inc r9
    test eax, eax
    jnz .div_loop

.pop_loop:
    pop rbx
    add bl, '0'             ; convert to ASCII
    mov byte [rdi], bl
    inc rdi
    dec r9
    jnz .pop_loop

    mov byte [rdi], 0       ; null-terminate
    ret

; ---------------------------------------------------------
; Helper: Format EAX to Hex String in RDX
; ---------------------------------------------------------
FormatHex:
    mov rdi, rdx
    mov r8d, eax
    test eax, eax
    jnz .not_zero
    mov byte [rdi], '0'     ; special case: zero
    mov byte [rdi+1], 0
    ret

.not_zero:
    mov rcx, 8              ; 8 hex digits max
    xor r9, r9              ; leading zeros flag
.loop:
    mov ebx, r8d
    shr ebx, 28             ; extract high 4 bits
    and ebx, 0xF

    test ebx, ebx
    jnz .digit_nonzero
    test r9, r9
    jz .next_digit          ; skip leading zeros

.digit_nonzero:
    mov r9, 1               ; set flag: non-zero digit found
    lea rsi, [rel hex_chars]
    mov al, byte [rsi + rbx] ; get hex char
    mov byte [rdi], al
    inc rdi

.next_digit:
    shl r8d, 4              ; shift to next digit
    dec rcx
    jnz .loop

    mov byte [rdi], 0       ; null-terminate
    ret

; ---------------------------------------------------------
; Main API Functions
; ---------------------------------------------------------

DecToHexAsm:
    push rbp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    call ParseDec
    jc .error
    call FormatHex
    jmp .end
.error:
    call WriteError
.end:
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    pop rbp
    ret

HexToDecAsm:
    push rbp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    call ParseHex
    jc .error
    call FormatDec
    jmp .end
.error:
    call WriteError
.end:
    pop r13
    pop r12
    pop rdi
    pop rsi
    pop rbx
    pop rbp
    ret