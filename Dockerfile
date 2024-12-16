# Base image
FROM ubuntu:22.04

# Set environment variables to avoid interactive prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages, including zsh
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gdb \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev \
    git \
    zsh && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Create a new non-root user
ARG USERNAME=devuser
ARG USER_UID=1000
ARG USER_GID=1000
RUN groupadd --gid $USER_GID $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME && \
    usermod -aG sudo $USERNAME && \
    chsh -s /usr/bin/zsh $USERNAME

# Set the new user as the default
USER $USERNAME

# Set working directory inside the container
WORKDIR /usr/src/app

# Copy project files into the container
COPY . .

# Build the project
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    cmake --build .

# Default shell to zsh
CMD ["zsh"]
