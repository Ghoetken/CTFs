#!/bin/bash

IMAGE=maze-ssh
CONTAINER=maze-container
PORT=2553

case "$1" in
  build)
    echo "[*] Building Docker image..."
    docker build -t $IMAGE .
    ;;
  run)
    echo "[*] Running container..."
    docker run --rm --privileged -d \
      --name $CONTAINER \
      -p $PORT:22 \
      $IMAGE
    ;;
  stop)
    echo "[*] Stopping container..."
    docker stop $CONTAINER
    ;;
  clean)
    echo "[*] Removing image..."
    docker rmi $IMAGE
    ;;
  logs)
    docker logs $CONTAINER
    ;;
  status)
    docker ps -a | grep $CONTAINER
    ;;
  exec)
    docker exec -it $CONTAINER /bin/bash
    ;;
  *)
    echo "Usage: $0 {build|run|stop|clean|logs|status|exec}"
    ;;
esac
