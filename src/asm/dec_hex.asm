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
    ; RDX = output buffer
    lea rsi, [rel err_msg] // lea - load effective address of error message, rsi - source index register, rdx - destination index register
    mov rdi, rdx // rdi - destination index register, rdx - return value from caller (output buffer)
.copy_err:
    mov al, byte [rsi] // load byte from source (error message) into al
    mov byte [rdi], al // store byte from al into destination (output buffer)
    test al, al // check if we reached the null terminator of the string
    jz .done // if zero, we are done copying
    inc rsi // inc - increment source index to next byte - kiểm tra ký tự tiếp theo của chuỗi lỗi
    inc rdi // inc - increment destination index to next byte - kiểm tra vị trí tiếp theo trong buffer đầu ra
    jmp .copy_err // jmp - jump back to start of copy loop to copy next byte - tiếp tục sao chép cho đến khi gặp null terminator
    // buffer - được truyền từ caller, sẽ chứa thông báo lỗi sau khi hàm này hoàn thành
.done:
    ret // ret - return from function, control goes back to caller - kết thúc hàm và trả về caller

; ---------------------------------------------------------
; Helper: Parse Decimal String (RCX) to EAX
; Returns CF=1 if error, CF=0 if success (EAX valid)
; ---------------------------------------------------------
ParseDec:
    xor eax, eax
    xor r8, r8      ; sign flag // r8 = 0 for positive, 1 for negative
    mov r9, rcx     ; current char ptr // r9 - register dùng để duyệt chuỗi đầu vào

    mov bl, byte [r9] // load first byte of input string into bl - kiểm tra ký tự đầu tiên của chuỗi đầu vào
    test bl, bl // test - kiểm tra nếu bl = 0 (null terminator), tức là chuỗi rỗng
    jz .err         ; empty string -> err (though C++ handles this) // nếu chuỗi rỗng, nhảy đến nhãn .err để xử lý lỗi

    cmp bl, '-' // compare first char with '-' to check for negative sign
    jne .parse_loop // if not '-', jump to parsing loop
    mov r8, 1 // set sign flag to indicate negative number
    inc r9 // move to next character after '-' for parsing the number
    mov bl, byte [r9] // load next byte after '-' into bl to check if it's not empty
    test bl, bl // test - kiểm tra nếu bl = 0 (null terminator)
    jz .err  

.parse_loop:
    mov bl, byte [r9] // load current byte of input string into bl for parsing
    test bl, bl // test bl = 0, if zero, we are done parsing the number
    jz .done // if we reach the end of the string, jump to .done to finish parsing
    cmp bl, '0' // compare current char with '0' to check if it's a valid digit - nếu ký tự hiện tại nhỏ hơn '0', nó không phải là một chữ số hợp lệ, nhảy đến .err để xử lý lỗi
    jl .err // compare current char with '9' to check if it's a valid digit - nếu ký tự hiện tại lớn hơn '9', nó không phải là một chữ số hợp lệ, nhảy đến .err để xử lý lỗi
    cmp bl, '9' // if bl > '9', it's not a valid digit, jump to .err to handle error
    jg .err // if bl < '0' or bl > '9', it's not a valid digit, jump to .err to handle error

    sub bl, '0' // convert ASCII char to numeric value by substracting '0' from it - chuyển ký tự ASCII thành giá trị số bằng cách trừ '0' khỏi nó
    mov r10d, 10 // load 10 into r10d - compare for multiplication by 10 to shift digits left
    imul eax, r10d  // EAX = EAX * 10, imul - integer multiplication 
    movzx ebx, bl // movzx - move with zero-extend, load numeric value of current digit into ebx
    add eax, ebx // add current digi value to EAX (ebx -> eax)

    inc r9 // increase r9 to move to next character in input string for parsing
    jmp .parse_loop // jump back to start of parsing loop to process next character in input string

.done:
    test r8, r8 // if sign flag is set, negate the result to get the correct negative value
    jz .success // if sign flag is not set, jum to .sucess to return the postive value in EAX
    neg eax // return negative value if sign flag is set
.success:
    clc // clear carry flag 
    ret // return result in EAX, control goes back to caller
.err:
    stc // set carry flag to indicate error
    ret // return error status, control goes back to caller

; ---------------------------------------------------------
; Helper: Parse Hex String (RCX) to EAX
; Returns CF=1 if error, CF=0 if success
; ---------------------------------------------------------
ParseHex:
    xor eax, eax // clear eax to store the result of parsing
    mov r9, rcx // rcx - pointer to input string, r9 - register for parsing

    mov bl, byte [r9] // load first byte of input into bl using r9 as pointer
    test bl, bl // test bl = 0, if zero, it returns an error
    jz .err // jump to .err - error

.parse_loop:
    mov bl, byte [r9] // load current byte of input string into bl for parsing
    test bl, bl // if bl = 0, parsing process is done 
    jz .done 

    cmp bl, 'H' // check with 'H' to allow optional suffix for hex numbers
    je .check_last_h // if current char 'H' or 'h' is the last character, it is valid
    cmp bl, 'h' // if current char 'H' or 'h' is the last character, it is valid
    je .check_last_h // check with 'h' to allow optional suffix for hex numbers
    jmp .not_h // if current char is not 'H' or 'h', jump to .not_h to continue parsing as hex digit

.check_last_h:
    cmp r9, rcx // check if 'H' or 'h' is the first character, which is invalid
    je .err // if 'H' or 'h' is the first character, it's an error
    cmp byte [r9+1], 0 // check if 'H' or 'h' is the last character, which is valid
    je .done 

.not_h:
    shl eax, 4 // shift current value in EAX left by 4 bits to make room for next hex digit (multiply by 16)

    cmp bl, '0' // compare current char with '0' to check if it's a valid hex digit
    jl .err
    cmp bl, '9' // compare current char with '9' to check if it's a valid hex digit
    jle .digit
    cmp bl, 'A' // compare current char with 'A' to check if it's a valid hex digit
    jl .err
    cmp bl, 'F' // compare current char with 'F' to check if it's a valid hex digit
    jle .upper
    cmp bl, 'a' // compare current char with 'a' to check if it's a valid hex digit
    jl .err
    cmp bl, 'f' // compare current char with 'f' to check if it's a valid hex digit
    jle .lower // if current char is between 'a' and 'f', it's a valid hex digit, jump to .lower to process it
    jmp .err    
// digit - if current char is between '0' and '9', it's a valid hex digit, jump to .digit to process it
.digit:
    sub bl, '0' // convert ASCII char to numeric value by substracting '0' from it - chuyển ký tự ASCII thành giá trị số bằng cách trừ '0' khỏi nó
    jmp .add_val // jump to .add_val to add the numeric value of the current hex digit to EAX
// upper - if current char is between 'A' and 'F', it's a valid hex digit, jump to .upper to process it
.upper:
    sub bl, 'A' // convert ASCII char to numeric value by substracting 'A' from it - chuyển ký tự ASCII thành giá trị số bằng cách trừ 'A' khỏi nó
    add bl, 10 // add 10 to convert 'A'-'F' to 10-15 - chuyển ký tự 'A'-'F' thành giá trị số 10-15 bằng cách cộng thêm 10
    jmp .add_val // jump to .add_val to add the numeric value of the current hex digit to EAX
// lower - if current char is between 'a' and 'f', it's a valid hex digit, jump to .lower to process it
.lower: 
    sub bl, 'a' // convert ASCII char to numeric value by substracting 'a' from it - chuyển ký tự ASCII thành giá trị số bằng cách trừ 'a' khỏi nó
    add bl, 10 // add 10 to convert 'a'-'f' to 10-15 - chuyển ký tự 'a'-'f' thành giá trị số 10-15 bằng cách cộng thêm 10
// add_val - add the numeric value of the current hex digit to EAX
.add_val: // add the numeric value of the current hex digit to EAX
    movzx ebx, bl // movzx - move with zero-extend, load numeric value of current hex digit into ebx
    add eax, ebx // add current hex digit value to EAX (ebx -> eax)

    inc r9 // increase r9 to move to next character in input string for parsing
    jmp .parse_loop // jump back to start of parsing loop to process next character in input string

.done: // parsing is done, return result in EAX, control goes back to caller
    clc
    ret
.err: // parsing error, set carry flag and return to caller
    stc
    ret

; ---------------------------------------------------------
; Helper: Format EAX to Decimal String in RDX
; ---------------------------------------------------------
FormatDec: // RDX = output buffer, EAX = value to format
    mov rdi, rdx
    mov r8d, eax // save value - add to r8d from EAX to preserve it for formatting

    ; Handle negative for display if MSB is 1?
    ; Yes, if it's negative, let's print '-' and negate.
    test r8d, r8d
    jns .positive
    mov byte [rdi], '-'
    inc rdi
    neg r8d

.positive:
    mov eax, r8d // move the value to EAX for division, r8d is the original value we want to format, we will divide it by 10 to extract digits
    mov rcx, 10 // divisor for decimal, we will use it to get each digit by dividing EAX by 10
    
    ; We need to push digits to stack to reverse them
    xor r9, r9 // r9 will count the number of digits we push to stack, we will use it later to pop them back in correct order
.div_loop: // loop to divide the number by 10 and push the remainder (digit) to stack until EAX becomes 0
    xor edx, edx // clear edx before division, because div instruction uses edx:eax as dividend, we need to ensure edx is 0 for correct division
    div ecx // divide EAX by 10, quotient goes to EAX, remainder (digit) goes to EDX
    push rdx // push the digit (remainder) to stack, we will pop it later to get the digits in correct order
    inc r9 // increment digit count in r9
    test eax, eax // check if EAX is 0 after division, if it is, we are done extracting digits
    jnz .div_loop // if EAX is not 0, jump back to start of division loop to extract next digit

.pop_loop: // loop to pop digits from stack and write them to output buffer in correct order
    pop rbx // pop the last digit from stack into rbx, this is the current digit we want to write to output buffer
    add bl, '0' // convert numeric digit to ASCII character by adding '0' to it, now bl contains the ASCII character of the digit
    mov byte [rdi], bl // write the ASCII character of the digit to output buffer at rdi
    inc rdi // move to next position in output buffer for next digit
    dec r9 // decrement digit count in r9, when r9 reaches 0, we are done writing digits to output buffer
    jnz .pop_loop // if r9 is not 0, jump back to start of pop loop to write next digit to output buffer

    mov byte [rdi], 0 // null-terminate the output string after writing all digits, rdi is currently at the position after the last digit, we write a null terminator to mark the end of the string
    ret // return from function, control goes back to caller, output buffer now contains the formatted decimal string of the original value in EAX

; ---------------------------------------------------------
; Helper: Format EAX to Hex String in RDX
; ---------------------------------------------------------
FormatHex:
    mov rdi, rdx // rdi - pointer to output buffer, rdx - output buffer passed from caller
    mov r8d, eax // save value to r8d for formatting, we will use r8d to extract hex digits by shifting and masking
    test eax, eax // check if value is zero, if it is, we should just print '0' and return, because the loop for extracting digits won't run for zero
    jnz .not_zero // if value is not zero, jump to .not_zero to extract hex digits, if it is zero, we will handle it here by printing '0' and returning
    mov byte [rdi], '0' // write '0' to output buffer if value is zero, since zero in hex is just '0'
    mov byte [rdi+1], 0 // null-terminate the string after '0', rdi+1 is the position after '0' where we write the null terminator
    ret 

.not_zero:
    mov rcx, 8 // 8 hex digits max
    xor r9, r9 // leading zeros flag
.loop:
    mov ebx, r8d // move the value to ebx for processing, r8d is the original value we want to format, we will shift it right to get each hex digit
    shr ebx, 28 // shift right by 28 bits to get the most significant hex digit in the lower 4 bits of ebx, since each hex digit represents 4 bits, shifting by 28 bits will leave us with the most significant hex digit in the lower 4 bits of ebx
    and ebx, 0xF // mask with 0xF to get only the lower 4 bits of ebx, which is the current hex digit we want to process
    
    test ebx, ebx // check if the current hex digit is zero, if it is, we may want to skip it if it's a leading zero, but if we have already encountered a non-zero digit, we should print it
    jnz .digit_nonzero // if the current hex digit is not zero, jump to .digit_nonzero to print it, if it is zero, we will check if it's a leading zero and skip it if it is
    test r9, r9 // check if we have already encountered a non-zero digit, if r9 is 0, it means we haven't encountered a non-zero digit yet, so this zero is a leading zero and we should skip it, if r9 is 1, it means we have already encountered a non-zero digit, so this zero is not a leading zero and we should print it
    jz .next_digit // if we haven't encountered a non-zero digit yet, jump to .next_digit to skip this leading zero, if we have already encountered a non-zero digit, we will print this zero as well
.digit_nonzero:
    mov r9, 1 // set leading zeros flag to indicate we have encountered a non-zero digit, from now on we will print all digits including zeros
    lea rsi, [rel hex_chars] // load address of hex_chars string into rsi, rsi will be used to get the ASCII character for the current hex digit, since ebx contains the value of the current hex digit (0-15), we can use it as an index into the hex_chars string to get the corresponding ASCII character
    mov al, byte [rsi + rbx] // load the ASCII character for the current hex digit from hex_chars string into al, rsi is the base address of hex_chars, rbx is the index of the current hex digit, so rsi + rbx gives us the address of the ASCII character for that digit, we load it into al to write it to output buffer
    mov byte [rdi], al // write the ASCII character of the current hex digit to output buffer at rdi
    inc rdi // move to next position in output buffer for next digit
.next_digit: // label for processing the next hex digit, we will shift the value left to bring the next hex digit into position for processing
    shl r8d, 4 // shift the value left by 4 bits to bring the next hex digit into the most significant position for the next iteration, since we are processing from the most significant digit to the least significant digit, we need to shift left to move the next digit into position for processing
    dec rcx // decrement the hex digit count in rcx, when rcx reaches 0, we have processed all 8 hex digits
    jnz .loop // if rcx is not 0, jump back to start of loop to process the next hex digit, if it is 0, we are done processing all hex digits and we will null-terminate the string

    mov byte [rdi], 0 // null-terminate the output string after writing all hex digits, rdi is currently at the position after the last digit, we write a null terminator to mark the end of the string
    ret 

; ---------------------------------------------------------
; Main API Functions
; ---------------------------------------------------------

DecToHexAsm:
    push rbp // push - save base pointer of caller, we will restore it before returning to caller
    push rbx // push - save rbx register, we will restore it before returning to caller, rbx is a callee-saved register, so we need to save and restore it if we use it in this function
    push rsi // push - save rsi register, we will restore it before returning to caller, rsi is a callee-saved register, so we need to save and restore it if we use it in this function
    push rdi // push - save rdi register, we will restore it before returning to caller, rdi is a callee-saved register, so we need to save and restore it if we use it in this function
    push r12 // push - save r12 register, we will restore it before returning to caller, r12 is a callee-saved register, so we need to save and restore it if we use it in this function
    push r13 // push - save r13 register, we will restore it before returning to caller, r13 is a callee-saved register, so we need to save and restore it if we use it in this function
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
    push rbp // push - save base pointer of caller, we will restore it before returning to caller
    push rbx // push - save rbx register, we will restore it before returning to caller, rbx is a callee-saved register, so we need to save and restore it if we use it in this function
    push rsi // push - save rsi register, we will restore it before returning to caller, rsi is a callee-saved register, so we need to save and restore it if we use it in this function
    push rdi // push - save rdi register, we will restore it before returning to caller, rdi is a callee-saved register, so we need to save and restore it if we use it in this function
    push r12 // push - save r12 register, we will restore it before returning to caller, r12 is a callee-saved register, so we need to save and restore it if we use it in this function
    push r13 // push - save r13 register, we will restore it before returning to caller, r13 is a callee-saved register, so we need to save and restore it if we use it in this function
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