FROM ubuntu:22.04
 
# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
 
RUN apt-get update && \
    apt-get install -y socat && \
    rm -rf /var/lib/apt/lists/*

# Create ctf user
RUN useradd -m ctf

# Set working directory
WORKDIR /home/ctf

# Copy challenge files
COPY stickynotes .
COPY flag.txt .
COPY ld-linux-x86-64.so.2 .
COPY libc.so.6 . 

# Permissions
RUN chown root:root /home/ctf/libc.so.6 /home/ctf/ld-linux-x86-64.so.2 && \
	chmod 755 /home/ctf/ld-linux-x86-64.so.2 /home/ctf/libc.so.6
#ctf directory
RUN chown root:ctf /home/ctf && \
    chmod 750 /home/ctf
#executable
RUN chown root:ctf /home/ctf/stickynotes && \
    chmod 750 /home/ctf/stickynotes
#flag
RUN chown root:ctf /home/ctf/flag.txt && \
    chmod 750 /home/ctf/flag.txt

EXPOSE 3773

USER ctf

CMD ["socat", "TCP-LISTEN:3773,reuseaddr,fork", "EXEC:/home/ctf/ld-linux-x86-64.so.2 --library-path /home/ctf /home/ctf/stickynotes"]