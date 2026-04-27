FROM ubuntu:22.04

# Allow unauthenticated repos (if needed)
RUN apt-get update && \
    apt-get install -y openssh-server login procps libpam-modules libpam-runtime

RUN useradd -m -s /bin/bash ctf && passwd -d ctf

# Set up SSH host keys
COPY ssh_host_rsa_key /etc/ssh/ssh_host_rsa_key
COPY ssh_host_rsa_key.pub /etc/ssh/ssh_host_rsa_key.pub
RUN chmod 600 /etc/ssh/ssh_host_rsa_key && chown root:root /etc/ssh/ssh_host_rsa_key

# Configure SSHD to allow empty password login without keys
RUN echo "HostKey /etc/ssh/ssh_host_rsa_key" > /etc/ssh/sshd_config && \
    echo "PasswordAuthentication yes" >> /etc/ssh/sshd_config && \
    echo "PermitEmptyPasswords yes" >> /etc/ssh/sshd_config && \
    echo "PermitRootLogin no" >> /etc/ssh/sshd_config && \
    echo "UsePAM no" >> /etc/ssh/sshd_config

# Make sure sshd runtime dir exists
RUN mkdir -p /var/run/sshd

# Copy your maze program
COPY flag.txt /home/ctf/flag.txt
COPY maze /home/ctf/maze
RUN chown root:root /home/ctf/flag.txt && \
    chmod 600 /home/ctf/flag.txt && \
    chown root:ctf /home/ctf/maze && \
    chmod 750 /home/ctf/maze

RUN chown root:root /home/ctf && \
	chmod 755 /home/ctf

RUN chmod u+s /home/ctf/maze

# Auto-run maze on login
RUN echo './maze; exit' >> /home/ctf/.bashrc

EXPOSE 2553
CMD ["/usr/sbin/sshd", "-D"]