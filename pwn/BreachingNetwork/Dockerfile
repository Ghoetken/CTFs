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

# Fix ownership and permissions for flag and vulnerable binary
RUN chown root:ctf /home/ctf/flag.txt && \
	chmod 440 /home/ctf/flag.txt && \
	chown root:ctf /home/ctf/vulnerable && \
	chmod 750 /home/ctf/vulnerable

# Restrict write permissions on the /home/ctf directory
RUN chown root:root /home/ctf && \
	chmod 755 /home/ctf

# Expose the port
EXPOSE 2003

# Switch to the ctf user
USER ctf

CMD ["socat", "TCP-LISTEN:2003,reuseaddr,fork", "EXEC:./vulnerable"]

# build: podman build --network=host -t breaching_network .
# run: podman run --rm -d -p 9001:9001 breaching_network