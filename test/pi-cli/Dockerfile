# Use a base image with git and bash
##################################################################################################################################
# Docker Build Command : docker build -t pi-cli-docker1 . --build-arg EMAIL="<email>" --build-arg PASSWORD="<password>"
# Here <email> is comcast email address and <password> can be generated from https://code.rdkcentral.com/r/settings/ 
##################################################################################################################################

FROM ubuntu:latest

ARG EMAIL
ARG PASSWORD

# Install required packages
RUN apt-get update && \
    apt-get install -y git curl bash && \
    apt-get install -y sudo && \
    apt-get install -y python3 python3-pip ninja-build bash cmake && \
    pip3 install --break-system-packages meson && \
    apt-get clean

# Set environment variables to disable Git prompts
ENV GIT_TERMINAL_PROMPT=0
ENV GIT_ASKPASS=echo

# Create /root/.netrc with credentials
RUN mkdir -p /root && \
    echo "machine code.rdkcentral.com login $EMAIL password $PASSWORD" > /root/.netrc && \
    chmod 600 /root/.netrc

# Set the working directory inside the container
WORKDIR /app

# Clone the middleware-player-interface repo
RUN git clone https://github.com/rdkcentral/middleware-player-interface.git

# Set working directory to the cloned repo
WORKDIR /app/middleware-player-interface


# Run the script
RUN ./install-middleware.sh

# Copy & Apply patch for Ubuntu
WORKDIR /app/middleware-player-interface
RUN git apply ./test/pi-cli/pi-cli-ubuntu.patch

# to find libcjson, your path may vary
RUN PKG_CONFIG_PATH="/app/middleware-player-interface/.libs/lib/pkgconfig"

# Create Build directory for pi-cli
RUN mkdir -p /app/middleware-player-interface/build

# Set working directory to test/pi-cli/build
WORKDIR /app/middleware-player-interface/build

# Enable pi-cli
RUN cmake -DBUILD_PICLI=ON ..

# Build pi-cli
RUN cmake --build .

# RUN ./test/pi-cli/pi-cli

# Keep Container Running
CMD ["tail -f /dev/null"]