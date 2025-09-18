#!/bin/bash

# Run script for the Drogon web application

echo "Starting Drogon Web Application..."
echo "================================================"
echo "Server: http://localhost:5555"
echo "Admin Login: http://localhost:5555/admin/login"
echo ""
echo "Default credentials:"
echo "  Username: admin"
echo "  Password: admin123"
echo "================================================"
echo ""
echo "Press Ctrl+C to stop the server"
echo ""

cd build
./drogon_webapp