#include "Admin.h"

using namespace drogon_model::webapp;

Json::Value Admin::toJson() const
{
    Json::Value ret;
    ret["id"] = id;
    ret["username"] = username;
    ret["full_name_ar"] = fullNameAr;
    ret["last_login"] = lastLogin;
    ret["login_attempts"] = loginAttempts;
    ret["is_active"] = isActive;
    ret["created_at"] = createdAt;
    ret["updated_at"] = updatedAt;
    ret["timezone"] = timezone;
    return ret;
}

Admin Admin::fromJson(const Json::Value &json)
{
    Admin admin;
    if (json.isMember("id"))
        admin.id = json["id"].asInt();
    if (json.isMember("username"))
        admin.username = json["username"].asString();
    if (json.isMember("password_hash"))
        admin.passwordHash = json["password_hash"].asString();
    if (json.isMember("full_name_ar"))
        admin.fullNameAr = json["full_name_ar"].asString();
    if (json.isMember("last_login"))
        admin.lastLogin = json["last_login"].asString();
    if (json.isMember("login_attempts"))
        admin.loginAttempts = json["login_attempts"].asInt();
    if (json.isMember("is_active"))
        admin.isActive = json["is_active"].asInt();
    if (json.isMember("created_at"))
        admin.createdAt = json["created_at"].asString();
    if (json.isMember("updated_at"))
        admin.updatedAt = json["updated_at"].asString();
    if (json.isMember("timezone"))
        admin.timezone = json["timezone"].asString();
    return admin;
}