#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace app
{

class AdminController : public drogon::HttpController<AdminController>
{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(AdminController::login, "/admin/login", Get);
    ADD_METHOD_TO(AdminController::doLogin, "/admin/login", Post);
    ADD_METHOD_TO(AdminController::logout, "/admin/logout", Get);
    ADD_METHOD_TO(AdminController::dashboard, "/admin/dashboard", Get, "app::AuthFilter");
    METHOD_LIST_END

    void login(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback);
    void doLogin(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback);
    void logout(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
    void dashboard(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback);

private:
    bool verifyPassword(const std::string &password, const std::string &hash);
    std::string getCurrentTimeInAmman();
};

} // namespace app