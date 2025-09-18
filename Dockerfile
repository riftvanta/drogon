# Build stage
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Amman

# Install build dependencies
RUN apt-get update && apt-get install -y \
    git \
    gcc \
    g++ \
    cmake \
    make \
    libjsoncpp-dev \
    uuid-dev \
    openssl \
    libssl-dev \
    zlib1g-dev \
    libsqlite3-dev \
    libpq-dev \
    && rm -rf /var/lib/apt/lists/*

# Clone and build Drogon
WORKDIR /tmp
RUN git clone https://github.com/drogonframework/drogon.git \
    && cd drogon \
    && git submodule update --init \
    && mkdir build \
    && cd build \
    && cmake .. \
    && make -j$(nproc) \
    && make install

# Copy application source
WORKDIR /app
COPY . .

# Build application
RUN mkdir -p build && cd build \
    && cmake .. \
    && make -j$(nproc)

# Generate view files
RUN drogon_ctl create view views/LoginView.csp \
    && drogon_ctl create view views/DashboardView.csp

# Runtime stage
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Amman
ENV DATABASE_URL=postgres://

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
    libjsoncpp25 \
    uuid-runtime \
    openssl \
    libssl3 \
    zlib1g \
    libsqlite3-0 \
    libpq5 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Copy Drogon libraries from builder
COPY --from=builder /usr/local/lib/ /usr/local/lib/
COPY --from=builder /usr/local/include/ /usr/local/include/

# Copy application from builder
WORKDIR /app
COPY --from=builder /app/build/drogon_webapp .
COPY --from=builder /app/static ./static
COPY --from=builder /app/config.json .
COPY --from=builder /app/database.db .
COPY --from=builder /app/views/*.h ./views/
COPY --from=builder /app/views/*.cc ./views/

# Update library cache
RUN ldconfig

# Create non-root user
RUN useradd -m -s /bin/bash appuser && chown -R appuser:appuser /app
USER appuser

# Expose port
EXPOSE 5555

# Run the application
CMD ["./drogon_webapp"]