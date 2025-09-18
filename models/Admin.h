#pragma once

#include <string>
#include <memory>
#include <json/json.h>

namespace drogon_model
{
namespace webapp
{

class Admin
{
public:
    Admin() = default;

    int32_t id = 0;
    std::string username;
    std::string passwordHash;
    std::string fullNameAr;
    std::string lastLogin;
    int32_t loginAttempts = 0;
    int8_t isActive = 1;
    std::string createdAt;
    std::string updatedAt;
    std::string timezone = "Asia/Amman";

    Json::Value toJson() const;
    static Admin fromJson(const Json::Value &json);
};

} // namespace webapp
} // namespace drogon_model