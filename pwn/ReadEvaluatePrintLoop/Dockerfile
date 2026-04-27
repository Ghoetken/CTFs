# Use the official Python image as the base image
FROM python:3.10-slim

# Set the working directory inside the container
WORKDIR /app

# Copy the Python script and any other necessary files into the container
COPY repl.py .

# Expose the port your server will listen on
EXPOSE 3443

# Command to run your Python script
CMD ["python", "repl.py"]

# build: podman build -t read-eval .
# run: podman run --rm -d -p 3443:3443 read-eval