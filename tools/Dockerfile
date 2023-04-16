FROM ubuntu:jammy

ENV DEBIAN_FRONTEND noninteractive
RUN apt update && apt install --no-install-recommends -y \
    dbus init systemd build-essential clang-format cmake gcovr \
    git libglib2.0-dev meson ninja-build libsystemd-dev wget sudo \
    libdbus-glib-1-dev vim daemonize valgrind software-properties-common

RUN mkdir -p /run/dbus; mkdir -p /data/config; ln -s /data/config /root/.config;  \
    rm -f /etc/localtime; ln -s /data/localtime_relay_link /etc/localtime;  \
    ln -s /usr/share/zoneinfo/UTC /data/localtime_relay_link

ARG UNAME=bob
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o $UNAME && useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME && usermod -aG sudo $UNAME
RUN echo '%sudo ALL=(ALL) NOPASSWD:ALL' > /etc/sudoers.d/NOPASSWD

COPY *.sh /

WORKDIR /builds/WV/toolbox


#ENTRYPOINT [ "/entrypoint.sh" ]

USER $UNAME

# docker run --rm -it --mount type=bind,source=$(pwd),target=/builds/WV/toolbox toolbox:latest 