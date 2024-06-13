#pragma once

#include "../framework.h"

std::wstring ConvertLPWSTRToString(LPWSTR lpwstr)
{
    if (lpwstr == nullptr)
        return L""; // 널 포인터이면 빈 문자열 반환

    return std::wstring(lpwstr);
}

std::string ConvertWideStringToString(const std::wstring& wstr)
{
    std::string str(wstr.begin(), wstr.end());
    return str;
}

std::string LPWSTRToString(LPWSTR lpwstr)
{
    return ConvertWideStringToString(ConvertLPWSTRToString(lpwstr));
}

std::wstring stringToWString(std::string str) {
    std::wstring wstr(str.begin(), str.end());
    return wstr;
}
