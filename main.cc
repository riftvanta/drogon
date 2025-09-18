#include <drogon/drogon.h>

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

    app().setDocumentRoot("../static");
    app().enableSession(1200);

    LOG_INFO << "Server running on http://127.0.0.1:5555";
    app().addListener("0.0.0.0", 5555);
    app().run();
    return 0;
}