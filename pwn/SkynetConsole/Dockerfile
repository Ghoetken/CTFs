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
COPY skynet_console .
COPY ctf_flag.txt ./flag.txt
COPY libc.so.6 .
COPY ld-linux-x86-64.so.2 .
COPY infiltrator_reader .
COPY infiltrator_reader.c .
COPY root_flag.txt /root/flag.txt

# Fix ownership and permissions for challenge files
RUN chown root:root /home/ctf &&chmod 755 /home/ctf && \
  chown root:ctf /home/ctf && chmod 750 /home/ctf  && \
  chown root:ctf /home/ctf/infiltrator_reader && chmod 4755 /home/ctf/infiltrator_reader && \
  chown root:ctf /home/ctf/infiltrator_reader.c && chmod 4755 /home/ctf/infiltrator_reader.c && \
  chown root:ctf /home/ctf/flag.txt && chmod 440 /home/ctf/flag.txt

# Expose the port
EXPOSE 3663

# Switch to the ctf user
USER ctf

CMD ["/home/ctf/ld-linux-x86-64.so.2", "--library-path", "/home/ctf", "/home/ctf/skynet_console"]


# You may need to run these before building and running the container: 
  # sudo firewall-cmd --add-port=3663/tcp --permanent
  # sudo firewall-cmd --reload
# build: podman build --network=host -t skynet-console .
# run: socat TCP-LISTEN:3663,bind=0.0.0.0,reuseaddr,fork SYSTEM:"podman run --rm -i skynet-console"