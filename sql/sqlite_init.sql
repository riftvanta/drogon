-- SQLite schema for admin users
CREATE TABLE IF NOT EXISTS admins (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username VARCHAR(50) NOT NULL UNIQUE,
    password_hash VARCHAR(255) NOT NULL,
    full_name_ar NVARCHAR(100) NOT NULL,
    last_login DATETIME,
    login_attempts INTEGER DEFAULT 0,
    is_active BOOLEAN DEFAULT 1,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    timezone VARCHAR(50) DEFAULT 'Asia/Amman'
);

-- Create index for faster lookups
CREATE INDEX idx_admins_username ON admins(username);
CREATE INDEX idx_admins_is_active ON admins(is_active);

-- Insert default admin user (password: admin123)
-- Note: This uses bcrypt hash - in production, change this immediately
INSERT INTO admins (username, password_hash, full_name_ar)
VALUES ('admin', '$2b$12$LQv3c1yqBWVHxkd0LHAkCOYz6TtxMQJqhN8/LewY5L2iNcnQhJ6MC', 'المسؤول');