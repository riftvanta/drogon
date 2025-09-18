#include "AuthFilter.h"

using namespace app;
using namespace drogon;

void AuthFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb)
{
    auto session = req->getSession();
    if (session->get<bool>("logged_in"))
    {
        fccb();
    }
    else
    {
        auto resp = HttpResponse::newRedirectionResponse("/admin/login");
        fcb(resp);
    }
}