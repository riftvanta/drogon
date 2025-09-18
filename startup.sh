#!/bin/bash

# Production startup script
echo "Starting Drogon Web Application..."

# Initialize PostgreSQL database if DATABASE_URL is set
if [ ! -z "$DATABASE_URL" ]; then
    echo "Detected PostgreSQL configuration..."

    # Parse DATABASE_URL
    # Format: postgresql://user:pass@host:port/dbname
    DB_URL=$DATABASE_URL

    # Extract components using sed
    DB_USER=$(echo $DB_URL | sed -n 's/.*:\/\/\([^:]*\):.*/\1/p')
    DB_PASS=$(echo $DB_URL | sed -n 's/.*:\/\/[^:]*:\([^@]*\)@.*/\1/p')
    DB_HOST=$(echo $DB_URL | sed -n 's/.*@\([^:]*\):.*/\1/p')
    DB_PORT=$(echo $DB_URL | sed -n 's/.*:\([0-9]*\)\/.*/\1/p')
    DB_NAME=$(echo $DB_URL | sed -n 's/.*\/\([^?]*\).*/\1/p')

    echo "Checking PostgreSQL database initialization..."

    # Check if admins table exists
    export PGPASSWORD=$DB_PASS
    TABLE_EXISTS=$(psql -h $DB_HOST -U $DB_USER -p $DB_PORT -d $DB_NAME -t -c "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = 'admins');" 2>/dev/null | tr -d '[:space:]')

    if [ "$TABLE_EXISTS" != "t" ]; then
        echo "Initializing database schema..."
        psql -h $DB_HOST -U $DB_USER -p $DB_PORT -d $DB_NAME < /app/sql/postgres_init.sql
        if [ $? -eq 0 ]; then
            echo "Database initialized successfully!"
        else
            echo "Warning: Database initialization failed, but continuing..."
        fi
    else
        echo "Database already initialized."
    fi
fi

# Start the application
echo "Starting server..."
exec ./drogon_webapp