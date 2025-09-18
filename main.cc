#include <drogon/drogon.h>
#include <cstdlib>
#include <string>
#include <filesystem>

using namespace drogon;

int main()
{
    app().registerHandler("/",
                          [](const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) {
                              auto resp = HttpResponse::newRedirectionResponse("/admin/login");
                              callback(resp);
                          },
                          {Get});

    // Set document root - in Docker it's relative to /app
    const char* docRoot = std::getenv("DOCUMENT_ROOT");
    if (docRoot) {
        app().setDocumentRoot(docRoot);
    } else {
        // Check if we're running from build directory (development)
        std::string docRootPath = "./static";
        if (std::filesystem::exists("../static") && !std::filesystem::exists("./static")) {
            docRootPath = "../static";
            LOG_INFO << "Development mode detected - using document root: " << docRootPath;
        }
        app().setDocumentRoot(docRootPath);
    }

    app().enableSession(1200);

    // Check for DATABASE_URL environment variable for PostgreSQL
    const char* dbUrl = std::getenv("DATABASE_URL");
    if (dbUrl) {
        // Parse PostgreSQL URL and configure database
        std::string url(dbUrl);
        LOG_INFO << "Using PostgreSQL database from DATABASE_URL";

        // Parse connection string: postgresql://user:pass@host:port/dbname
        // Example: postgresql://postgres:password@mainline.proxy.rlwy.net:38078/railway
        size_t userStart = url.find("://") + 3;
        size_t passStart = url.find(":", userStart) + 1;
        size_t hostStart = url.find("@") + 1;
        size_t portStart = url.find(":", hostStart) + 1;
        size_t dbStart = url.find("/", portStart) + 1;

        std::string user = url.substr(userStart, url.find(":", userStart) - userStart);
        std::string pass = url.substr(passStart, url.find("@") - passStart);
        std::string host = url.substr(hostStart, url.find(":", hostStart) - hostStart);
        std::string portStr = url.substr(portStart, url.find("/", portStart) - portStart);
        std::string dbname = url.substr(dbStart);

        int port = std::stoi(portStr);

        app().createDbClient("postgresql",
                           host,
                           port,
                           dbname,
                           user,
                           pass,
                           10); // number of connections
    } else {
        // Fall back to SQLite for local development
        LOG_INFO << "Using SQLite database (development mode)";
        app().createDbClient("sqlite3",
                           "database.db",
                           0,    // port (not used for SQLite)
                           "",   // dbname (not used for SQLite)
                           "",   // user (not used for SQLite)
                           "",   // password (not used for SQLite)
                           1);   // number of connections
    }

    // Get port from environment variable or use default
    const char* portEnv = std::getenv("PORT");
    int port = portEnv ? std::atoi(portEnv) : 5555;

    LOG_INFO << "Server running on http://0.0.0.0:" << port;
    app().addListener("0.0.0.0", port);
    app().run();
    return 0;
}