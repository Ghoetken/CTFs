#BUILD: docker build -t vaults .
#RUN: docker run -d --rm -p 5410 --name vaults vaults

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
COPY vaults_remote .

# Permissions
#ctf directory
RUN chown root:ctf /home/ctf && \
    chmod 750 /home/ctf
#executable
RUN chown root:ctf /home/ctf/vaults_remote && \
    chmod 750 /home/ctf/vaults_remote

EXPOSE 3883

USER ctf

CMD ["socat", "TCP-LISTEN:3883,reuseaddr,fork", "EXEC:./vaults_remote"]