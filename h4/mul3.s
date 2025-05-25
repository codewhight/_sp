.globl  mul3
        
        .text
mul3:
        mov     %rdi, %rax                
        imul   %rsi, %rax                
        imul   %rdx, %rax               
                     
        ret                               