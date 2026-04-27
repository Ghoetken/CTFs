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
COPY challenge .
COPY flag.txt .

# Fix ownership and permissions for flag and challenge binary
RUN chown root:ctf /home/ctf/flag.txt && \
	chmod 440 /home/ctf/flag.txt && \
	chown root:ctf /home/ctf/challenge && \
	chmod 750 /home/ctf/challenge

# Restrict write permissions on the /home/ctf directory
RUN chown root:root /home/ctf && \
	chmod 755 /home/ctf

# Expose the port
EXPOSE 3553

# Switch to the ctf user
USER ctf

CMD ["socat", "TCP-LISTEN:3553,reuseaddr,fork", "EXEC:/home/ctf/challenge"]

# build: podman build --network=host -t robot-terminal .
# run: podman run --rm -d -p 3553:3553 robot-terminal