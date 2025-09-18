#include "AdminController.h"
#include "../models/Admin.h"
#include <drogon/HttpViewData.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace app;
using namespace drogon;
using namespace drogon_model::webapp;

void AdminController::login(const HttpRequestPtr &req,
                            std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto session = req->getSession();
    if (session->get<bool>("logged_in"))
    {
        auto resp = HttpResponse::newRedirectionResponse("/admin/dashboard");
        callback(resp);
        return;
    }

    HttpViewData data;
    data["error"] = req->getParameter("error");
    data["current_time"] = getCurrentTimeInAmman();

    auto resp = HttpResponse::newHttpViewResponse("LoginView", data);
    callback(resp);
}

void AdminController::doLogin(const HttpRequestPtr &req,
                              std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto username = req->getParameter("username");
    auto password = req->getParameter("password");

    if (username.empty() || password.empty())
    {
        auto resp = HttpResponse::newRedirectionResponse("/admin/login?error=missing_credentials");
        callback(resp);
        return;
    }

    // TODO: Implement database-backed authentication
    // This is a temporary placeholder - must be replaced with proper authentication
    // Example: Query database for user, verify hashed password
    // auto dbClient = drogon::app().getDbClient();
    // auto result = dbClient->execSqlSync("SELECT id, password_hash, full_name FROM admins WHERE username = ?", username);

    // Temporary authentication check - MUST BE REPLACED IN PRODUCTION
    bool isAuthenticated = false;
    std::string storedHash = "$2b$12$LQv3c1yqBWVHxkd0LHAkCOYz6TtxMQJqhN8/LewY5L2iNcnQhJ6MC";

    // WARNING: This is still insecure - implement proper bcrypt verification
    if (username == "admin" && verifyPassword(password, storedHash))
    {
        isAuthenticated = true;
    }

    if (isAuthenticated)
    {
        auto session = req->getSession();
        session->insert("logged_in", true);
        session->insert("admin_id", 1);
        session->insert("username", username);
        session->insert("full_name_ar", "المسؤول");
        session->insert("login_time", std::chrono::system_clock::now().time_since_epoch().count());

        auto resp = HttpResponse::newRedirectionResponse("/admin/dashboard");
        callback(resp);
    }
    else
    {
        auto resp = HttpResponse::newRedirectionResponse("/admin/login?error=invalid_credentials");
        callback(resp);
    }
}

void AdminController::logout(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback)
{
    req->session()->clear();
    auto resp = HttpResponse::newRedirectionResponse("/admin/login");
    callback(resp);
}

void AdminController::dashboard(const HttpRequestPtr &req,
                                std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto session = req->getSession();

    // Validate session exists and has required data
    if (!session->get<bool>("logged_in"))
    {
        auto resp = HttpResponse::newRedirectionResponse("/admin/login");
        callback(resp);
        return;
    }

    // Check session timeout (30 minutes)
    auto loginTime = session->getOptional<long>("login_time");
    if (loginTime.has_value())
    {
        auto now = std::chrono::system_clock::now().time_since_epoch().count();
        auto elapsed = now - loginTime.value();
        if (elapsed > 1800000000000) // 30 minutes in nanoseconds
        {
            session->clear();
            auto resp = HttpResponse::newRedirectionResponse("/admin/login?error=session_expired");
            callback(resp);
            return;
        }
    }

    HttpViewData data;
    data["username"] = session->getOptional<std::string>("username").value_or("");
    data["full_name_ar"] = session->getOptional<std::string>("full_name_ar").value_or("");
    data["current_time"] = getCurrentTimeInAmman();

    auto resp = HttpResponse::newHttpViewResponse("DashboardView", data);
    callback(resp);
}

bool AdminController::verifyPassword(const std::string &password, const std::string &hash)
{
    // TODO: Implement proper password verification using bcrypt or argon2
    // This is a temporary implementation that should be replaced
    // Example with bcrypt:
    // return BCrypt::validatePassword(password, hash);

    // WARNING: Temporary implementation - MUST BE REPLACED
    // This still checks against a known value for demonstration
    // In production, use a proper password hashing library
    return (password == "admin123" && !hash.empty());
}

std::string AdminController::getCurrentTimeInAmman()
{
    auto now = std::chrono::system_clock::now();
    auto utc_time = std::chrono::system_clock::to_time_t(now);

    // TODO: Use proper timezone library (e.g., date/tz or C++20 chrono)
    // Amman is UTC+3 (or UTC+2 during DST)
    // This is a simplified implementation
    constexpr int AMMAN_UTC_OFFSET = 3;
    auto amman_time = utc_time + (AMMAN_UTC_OFFSET * 3600);

    std::tm* tm_amman = std::gmtime(&amman_time);
    if (!tm_amman)
    {
        return "Time error";
    }

    std::ostringstream oss;
    oss << std::put_time(tm_amman, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}