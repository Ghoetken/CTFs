#BUILD: docker build -t lockbox .
#RUN: docker run -d -rm -p 1448:1448 --name loxkbox-container lockbox

FROM ubuntu:22.04
 
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
COPY LockBox .
COPY flag.txt .

# Permissions
#ctf directory
RUN chown root:ctf /home/ctf && \
    chmod 750 /home/ctf
#flag 
RUN chown root:ctf /home/ctf/flag.txt && \
    chmod 750 /home/ctf/flag.txt 
#executable
RUN chown root:ctf /home/ctf/LockBox && \
    chmod 750 /home/ctf/LockBox

USER ctf

EXPOSE 2333

CMD ["socat", "TCP-LISTEN:2333,reuseaddr,fork", "EXEC:./LockBox"]