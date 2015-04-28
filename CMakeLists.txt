set(surbasic_SRC
 AESHelper.cpp
 Base64Helper.cpp
 ByteArray.cpp
 CJsonParse.cpp
 CompressHelper.cpp
 CurlHeaders.cpp
 CurlUtils.cpp
 Digestor.cpp
 EciesHelper.cpp
 File.cpp
 FileDownloaderEnc.cpp
 FileDownloaderFactory.cpp
 FileInfo.cpp
 FileUploaderEnc.cpp
 FileUploaderFactory.cpp
 SDContext.cpp
 SDCurl.cpp
 surbasic.cpp
 timestamp.cpp
 TokenManager.cpp
 TreeEntity.cpp
 XmlHelper.cpp
 tinyxml/tinyxml2.cpp
 )


set_source_files_properties(${surbasic_SRC}
    PROPERTIES COMPILE_FLAGS "-Wno-error")

add_library(surbasic STATIC ${surbasic_SRC})
add_subdirectory(tests)
