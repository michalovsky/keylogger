#pragma once

#include "Mail.h"
#include "Credentials.h"

namespace keylogger::mail
{
class MailSender
{
public:
    virtual ~MailSender() = default;

    virtual bool sendMail(const Mail&, const Credentials&) = 0;
};
}