#pragma once

#include <drogon/HttpFilter.h>

namespace app
{

class AuthFilter : public drogon::HttpFilter<AuthFilter>
{
public:
    AuthFilter() = default;

    virtual void doFilter(const drogon::HttpRequestPtr &req,
                         drogon::FilterCallback &&fcb,
                         drogon::FilterChainCallback &&fccb) override;
};

}