#!/bin/bash
# This script is used to run a container with the current user's UID and GID
# so that the files created in the container are owned by the current user.
# This is useful for development, but not for production.

docker build --build-arg UID=$(id -u) --build-arg GID=$(id -g) -t toolbox:latest -f ./Dockerfile .
