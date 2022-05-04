extern lpRemain:QWORD
extern dwNumber:QWORD

.code 
hkGetValue proc
	mov rax,dwNumber
	jmp lpRemain
hkGetValue endp
end