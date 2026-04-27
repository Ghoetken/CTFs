#BUILD: docker build -t flag_on_sale .
#RUN: docker run -d --rm -p 1447:1447 --name flag_on_sale-container flag_on_sale

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
COPY flag_on_sale .
COPY flag.txt .

# Permissions
#ctf directory
RUN chown root:ctf /home/ctf && \
    chmod 750 /home/ctf
#flag 
RUN chown root:ctf /home/ctf/flag.txt && \
    chmod 750 /home/ctf/flag.txt 
#executable
RUN chown root:ctf /home/ctf/flag_on_sale && \
    chmod 750 /home/ctf/flag_on_sale


EXPOSE 2113

USER ctf

CMD ["socat", "TCP-LISTEN:2113,reuseaddr,fork", "EXEC:./flag_on_sale"]