section .data
    err_msg db "Du lieu khong hop le", 0
    hex_chars db "0123456789ABCDEF"

section .text
    global DecToBinAsm
    global BinToDecAsm
    global DecToHexAsm
    global HexToDecAsm
    global BinToHexAsm
    global HexToBinAsm

; ---------------------------------------------------------
; Helper: Write Error Message to Output
; ---------------------------------------------------------
WriteError:
    ; RDX = output buffer
    lea rsi, [rel err_msg]
    mov rdi, rdx
.copy_err:
    mov al, byte [rsi]
    mov byte [rdi], al
    test al, al
    jz .done
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
    xor eax, eax
    xor r8, r8      ; sign flag
    mov r9, rcx     ; current char ptr

    mov bl, byte [r9]
    test bl, bl
    jz .err         ; empty string -> err (though C++ handles this)

    cmp bl, '-'
    jne .parse_loop
    mov r8, 1
    inc r9
    mov bl, byte [r9]
    test bl, bl
    jz .err         ; just "-" -> err

.parse_loop:
    mov bl, byte [r9]
    test bl, bl
    jz .done
    cmp bl, '0'
    jl .err
    cmp bl, '9'
    jg .err

    sub bl, '0'
    mov r10d, 10
    imul eax, r10d  ; EAX = EAX * 10
    movzx ebx, bl
    add eax, ebx    ; EAX = EAX + digit

    inc r9
    jmp .parse_loop

.done:
    test r8, r8
    jz .success
    neg eax         ; two's complement if negative
.success:
    clc
    ret
.err:
    stc
    ret

; ---------------------------------------------------------
; Helper: Parse Binary String (RCX) to EAX
; Returns CF=1 if error, CF=0 if success
; ---------------------------------------------------------
ParseBin:
    xor eax, eax
    mov r9, rcx

    mov bl, byte [r9]
    test bl, bl
    jz .err

.parse_loop:
    mov bl, byte [r9]
    test bl, bl
    jz .done
    cmp bl, '0'
    jl .err
    cmp bl, '1'
    jg .err

    shl eax, 1
    sub bl, '0'
    movzx ebx, bl
    add eax, ebx

    inc r9
    jmp .parse_loop

.done:
    clc
    ret
.err:
    stc
    ret

; ---------------------------------------------------------
; Helper: Parse Hex String (RCX) to EAX
; Returns CF=1 if error, CF=0 if success
; ---------------------------------------------------------
ParseHex:
    xor eax, eax
    mov r9, rcx

    mov bl, byte [r9]
    test bl, bl
    jz .err

.parse_loop:
    mov bl, byte [r9]
    test bl, bl
    jz .done

    cmp bl, 'H'
    je .check_last_h
    cmp bl, 'h'
    je .check_last_h
    jmp .not_h

.check_last_h:
    cmp r9, rcx
    je .err
    cmp byte [r9+1], 0
    je .done

.not_h:
    shl eax, 4

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
    sub bl, '0'
    jmp .add_val
.upper:
    sub bl, 'A'
    add bl, 10
    jmp .add_val
.lower:
    sub bl, 'a'
    add bl, 10

.add_val:
    movzx ebx, bl
    add eax, ebx

    inc r9
    jmp .parse_loop

.done:
    clc
    ret
.err:
    stc
    ret

; ---------------------------------------------------------
; Helper: Format EAX to Binary String in RDX
; ---------------------------------------------------------
FormatBin:
    mov rdi, rdx
    mov r8d, eax    ; save EAX
    test eax, eax
    jnz .not_zero
    mov byte [rdi], '0'
    mov byte [rdi+1], 0
    ret
.not_zero:
    mov rcx, 32     ; 32 bits max
    xor r9, r9      ; leading zeros flag (0 = leading, 1 = started)
.loop:
    dec rcx
    bt r8d, ecx
    jc .bit_one
.bit_zero:
    test r9, r9
    jz .next_bit    ; skip leading zeros
    mov byte [rdi], '0'
    inc rdi
    jmp .next_bit
.bit_one:
    mov r9, 1
    mov byte [rdi], '1'
    inc rdi
.next_bit:
    test rcx, rcx
    jnz .loop
    mov byte [rdi], 0
    ret

; ---------------------------------------------------------
; Helper: Format EAX to Decimal String in RDX
; ---------------------------------------------------------
FormatDec:
    mov rdi, rdx
    mov r8d, eax    ; save value

    ; Handle negative for display if MSB is 1?
    ; Yes, if it's negative, let's print '-' and negate.
    test r8d, r8d
    jns .positive
    mov byte [rdi], '-'
    inc rdi
    neg r8d

.positive:
    mov eax, r8d
    mov rcx, 10     ; divisor
    
    ; We need to push digits to stack to reverse them
    xor r9, r9      ; digit count
.div_loop:
    xor edx, edx
    div ecx
    push rdx
    inc r9
    test eax, eax
    jnz .div_loop

.pop_loop:
    pop rbx
    add bl, '0'
    mov byte [rdi], bl
    inc rdi
    dec r9
    jnz .pop_loop

    mov byte [rdi], 0
    ret

; ---------------------------------------------------------
; Helper: Format EAX to Hex String in RDX
; ---------------------------------------------------------
FormatHex:
    mov rdi, rdx
    mov r8d, eax
    test eax, eax
    jnz .not_zero
    mov byte [rdi], '0'
    mov byte [rdi+1], 0
    ret

.not_zero:
    mov rcx, 8      ; 8 hex digits max
    xor r9, r9      ; leading zeros flag
.loop:
    mov ebx, r8d
    shr ebx, 28     ; get top 4 bits
    and ebx, 0xF
    
    test ebx, ebx
    jnz .digit_nonzero
    test r9, r9
    jz .next_digit  ; skip leading zero
.digit_nonzero:
    mov r9, 1
    lea rsi, [rel hex_chars]
    mov al, byte [rsi + rbx]
    mov byte [rdi], al
    inc rdi
.next_digit:
    shl r8d, 4
    dec rcx
    jnz .loop

    mov byte [rdi], 0
    ret

; ---------------------------------------------------------
; Main API Functions
; ---------------------------------------------------------

DecToBinAsm:
    push rbp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    call ParseDec
    jc .error
    call FormatBin
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

BinToDecAsm:
    push rbp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    call ParseBin
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

BinToHexAsm:
    push rbp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    call ParseBin
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

HexToBinAsm:
    push rbp
    push rbx
    push rsi
    push rdi
    push r12
    push r13
    call ParseHex
    jc .error
    call FormatBin
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
