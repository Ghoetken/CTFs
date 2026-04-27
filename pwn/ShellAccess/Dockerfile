FROM debian:bullseye-slim

# Avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
 
RUN echo 'Acquire::AllowInsecureRepositories "true";' > /etc/apt/apt.conf.d/99insecure && \
	echo 'APT::Get::AllowUnauthenticated "true";' >> /etc/apt/apt.conf.d/99insecure && \
	apt-get update && \
	apt-get install -y --allow-unauthenticated socat && \
	rm -rf /var/lib/apt/lists/*

# Create ctf user
RUN useradd -m ctf

# Set working directory
WORKDIR /home/ctf
 
# Copy challenge files
COPY vulnerable .
COPY flag.txt .
COPY libc.so.6 .
COPY ld-linux-x86-64.so.2 .

# Fix ownership and permissions for libc and linker
RUN chown root:root /home/ctf/libc.so.6 /home/ctf/ld-linux-x86-64.so.2 && \
	chmod 755 /home/ctf/ld-linux-x86-64.so.2 /home/ctf/libc.so.6

# Fix ownership and permissions for flag and vulnerable binary
RUN chown root:ctf /home/ctf/flag.txt && \
	chmod 440 /home/ctf/flag.txt && \
	chown root:ctf /home/ctf/vulnerable && \
	chmod 750 /home/ctf/vulnerable

# Restrict write permissions on the /home/ctf directory
RUN chown root:root /home/ctf && \
	chmod 755 /home/ctf

# Expose the port
EXPOSE 2443

# Switch to the ctf user
USER ctf

CMD ["socat", "TCP-LISTEN:2443,reuseaddr,fork", "EXEC:/home/ctf/ld-linux-x86-64.so.2 --library-path /home/ctf /home/ctf/vulnerable"]

# build: podman build --network=host -t shell_access .
# run: podman run --rm -d -p 1447:1447 shell_access