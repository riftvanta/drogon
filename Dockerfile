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

# Create config.json for production
RUN echo '{ \
  "listeners": [ \
    { \
      "address": "0.0.0.0", \
      "port": 5555, \
      "https": false \
    } \
  ], \
  "app": { \
    "threads_num": 0, \
    "enable_session": true, \
    "session_timeout": 1200, \
    "document_root": "./static", \
    "upload_path": "./uploads", \
    "max_connections": 100000, \
    "log": { \
      "log_level": "INFO" \
    }, \
    "use_sendfile": true, \
    "use_gzip": true, \
    "static_files_cache_time": 5, \
    "idle_connection_timeout": 60, \
    "enable_server_header": true, \
    "server_header": "drogon/1.9.11" \
  }, \
  "db_clients": [] \
}' > config.json

# Runtime stage
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Amman
ENV DATABASE_URL=postgres://

# Install runtime dependencies including PostgreSQL client
RUN apt-get update && apt-get install -y \
    libjsoncpp25 \
    uuid-runtime \
    openssl \
    libssl3 \
    zlib1g \
    libsqlite3-0 \
    libpq5 \
    postgresql-client \
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
# Note: database.db not needed in production - using PostgreSQL
COPY --from=builder /app/views/*.h ./views/
COPY --from=builder /app/views/*.cc ./views/
COPY --from=builder /app/sql ./sql

# Copy startup script
COPY startup.sh /app/startup.sh
RUN chmod +x /app/startup.sh

# Update library cache
RUN ldconfig

# Create non-root user
RUN useradd -m -s /bin/bash appuser && chown -R appuser:appuser /app
USER appuser

# Expose port
EXPOSE 5555

# Run the application with startup script
CMD ["/app/startup.sh"]