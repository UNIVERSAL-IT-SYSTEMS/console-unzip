#ifndef __UTF8_H__
#define __UTF8_H__ 1

#ifdef __cplusplus
extern "C" {
#endif
    
#ifndef __linux__
#ifndef __APPLE__
    
#include <string>
#include <vector>
#include <windows.h>

int wchar_to_utf8(std::wstring &wstr, std::string &str){
    int error = 0; 
    int len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wstr.c_str(), wstr.length(), NULL, 0, NULL, NULL);
    if(len){
        std::vector<char> buf(len + 1);
        if(WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wstr.c_str(), wstr.length(), (LPSTR)&buf[0], len, NULL, NULL)){
            str = std::string((const char *)&buf[0]);
        }
    }else{
        str = std::string((const char *)"");
        error = -1;
    }
    return error;
}

int utf8_to_wchar(std::string &str, std::wstring &wstr){
    int error = 0; 
    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str.c_str(), str.length(), NULL, 0);
    if(len){
        std::vector<char> buf((len + 1) * sizeof(wchar_t));
        if(MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)str.c_str(), str.length(), (LPWSTR)&buf[0], len)){
            wstr = std::wstring((const wchar_t *)&buf[0]);
        }
    }else{
        wstr = std::wstring((const wchar_t *)L"");
        error = -1;
    }
    return error;
}

#endif
#endif

#ifdef __cplusplus
}
#endif

#endif