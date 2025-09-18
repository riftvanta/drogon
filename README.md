# Drogon High-Performance Arabic Web Application

A sophisticated web application built with C++ using the Drogon framework, optimized for maximum performance with Arabic language support.

## Features

- **Arabic RTL Support**: Full right-to-left layout for Arabic users
- **Admin Authentication**: Secure login system (username-only for now)
- **Mobile Responsive**: Works perfectly on all devices
- **High Performance**: Built with C++ and Drogon framework
- **Dual Database Support**: SQLite for development, PostgreSQL for production
- **Amman Timezone**: Configured for Asia/Amman (UTC+3)
- **Western Numerals**: Displays 0-9 format
- **Gregorian Calendar**: Standard date format

## Requirements

- Ubuntu 22.04+ or similar Linux distribution
- GCC 11+ or Clang 14+
- CMake 3.5+
- SQLite3 (development)
- PostgreSQL (production)

## Quick Start

### Development Setup

1. **Install Dependencies**:
```bash
sudo apt-get update
sudo apt-get install -y git gcc g++ cmake libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev libsqlite3-dev libpq-dev sqlite3
```

2. **Build the Application**:
```bash
chmod +x build.sh
./build.sh
```

3. **Run the Application**:
```bash
cd build
./drogon_webapp
```

4. **Access the Application**:
Open your browser and navigate to: `http://localhost:5555`

### Default Login Credentials

- **Username**: admin
- **Password**: admin123

⚠️ **Important**: Change these credentials immediately in production!

## Project Structure

```
drogon/
├── controllers/        # HTTP controllers
│   └── AdminController.cc/h
├── filters/           # HTTP filters
│   └── AuthFilter.cc/h
├── models/           # Data models
│   └── Admin.cc/h
├── views/            # CSP view templates
│   ├── LoginView.csp
│   └── DashboardView.csp
├── static/           # Static assets
│   ├── css/
│   │   └── style.css
│   └── js/
│       └── app.js
├── sql/              # Database schemas
│   ├── sqlite_init.sql
│   └── postgres_init.sql
├── config.json       # Application configuration
├── CMakeLists.txt    # Build configuration
├── Dockerfile        # Container configuration
└── railway.json      # Railway deployment config
```

## Deployment

### Using Docker

1. **Build the Docker image**:
```bash
docker build -t drogon-webapp .
```

2. **Run the container**:
```bash
docker run -p 5555:5555 drogon-webapp
```

### Deploy to Railway

1. Push your code to a GitHub repository
2. Connect your repository to Railway
3. Railway will automatically detect the `railway.json` and `Dockerfile`
4. Set environment variables for PostgreSQL connection
5. Deploy!

### Environment Variables

For production deployment, set these environment variables:

- `DATABASE_URL`: PostgreSQL connection string
- `DB_HOST`: PostgreSQL host
- `DB_PORT`: PostgreSQL port (default: 5432)
- `DB_NAME`: Database name
- `DB_USER`: Database username
- `DB_PASSWORD`: Database password

## Configuration

Edit `config.json` to customize:

- Server port and address
- Database connections
- Session timeout
- Static file caching
- Thread pool size
- And more...

## Development

### Adding New Controllers

```bash
drogon_ctl create controller ControllerName
```

### Adding New Views

1. Create a `.csp` file in the `views/` directory
2. Compile it: `drogon_ctl create view views/YourView.csp`

### Database Migrations

- Development: Edit `sql/sqlite_init.sql` and recreate database
- Production: Create migration scripts for PostgreSQL

## Performance Optimization

The application is optimized for high performance:

- **Multi-threading**: Utilizes all CPU cores
- **Connection pooling**: Efficient database connections
- **Static file caching**: Reduces disk I/O
- **Gzip compression**: Reduces bandwidth usage
- **Async I/O**: Non-blocking operations

## Security Features

- Session-based authentication
- Password hashing (to be enhanced with bcrypt)
- CSRF protection (to be added)
- SQL injection prevention
- XSS protection headers

## Browser Support

- Chrome 90+
- Firefox 88+
- Safari 14+
- Edge 90+
- Mobile browsers (iOS Safari, Chrome Mobile)

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to your branch
5. Create a Pull Request

## License

This project is proprietary software. All rights reserved.

## Support

For issues and questions, please create an issue in the repository.