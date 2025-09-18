#!/bin/bash

# Build script for development

echo "Building Drogon Web Application..."

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Configure with CMake
cmake ..

# Build the project
make -j$(nproc)

# Generate view files
echo "Generating view files..."
drogon_ctl create view ../views/LoginView.csp
drogon_ctl create view ../views/DashboardView.csp

# Initialize database if it doesn't exist
if [ ! -f "../database.db" ]; then
    echo "Initializing SQLite database..."
    sqlite3 ../database.db < ../sql/sqlite_init.sql
fi

echo "Build completed successfully!"
echo "To run the application: cd build && ./drogon_webapp"