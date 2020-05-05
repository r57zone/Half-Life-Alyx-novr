#ifndef INIREADER_H
#define INIREADER_H
#include "ini_parser.hpp"
#include <string>
#include <string_view>
#include <Windows.h>

/*
*  String comparision functions, with case sensitive option
*/

using std::strcmp;

inline int strcmp(const char* str1, const char* str2, bool csensitive)
{
    return (csensitive ? ::strcmp(str1, str2) : ::_stricmp(str1, str2));
}

inline int strcmp(const char* str1, const char* str2, size_t num, bool csensitive)
{
    return (csensitive ? ::strncmp(str1, str2, num) : ::_strnicmp(str1, str2, num));
}

inline int compare(const std::string& str1, const std::string& str2, bool case_sensitive)
{
    if (str1.length() == str2.length())
        return strcmp(str1.c_str(), str2.c_str(), case_sensitive);
    return (str1.length() < str2.length() ? -1 : 1);
}

inline int compare(const std::string& str1, const std::string& str2, size_t num, bool case_sensitive)
{
    if (str1.length() == str2.length())
        return strcmp(str1.c_str(), str2.c_str(), num, case_sensitive);
    return (str1.length() < str2.length() ? -1 : 1);
}

inline int compare(const char* str1, const char* str2, bool case_sensitive)
{
    return strcmp(str1, str2, case_sensitive);
}

inline int compare(const char* str1, const char* str2, size_t num, bool case_sensitive)
{
    return strcmp(str1, str2, num, case_sensitive);
}

inline bool starts_with(const char* str, const char* prefix, bool case_sensitive)
{
    while (*prefix)
    {
        bool equal;
        if (case_sensitive)
            equal = (*str++ == *prefix++);
        else
            equal = (::tolower(*str++) == ::tolower(*prefix++));

        if (!equal) return false;
    }
    return true;
}

inline bool ends_with(const char* str, const char* prefix, bool case_sensitive)
{
    auto str2 = &str[strlen(str) - 1];
    auto prefix2 = &prefix[strlen(prefix) - 1];

    while (prefix2 >= prefix)
    {
        bool equal;
        if (case_sensitive)
            equal = (*str2-- == *prefix2--);
        else
            equal = (::tolower(*str2--) == ::tolower(*prefix2--));

        if (!equal) return false;
    }
    return true;
}

class CIniReader
{
private:
    std::string m_szFileName;

public:
    linb::ini data;

    CIniReader()
    {
        SetIniPath("");
    }

    CIniReader(std::string_view szFileName)
    {
        SetIniPath(szFileName);
    }

    CIniReader(std::stringstream& ini_mem)
    {
        data.load_file(ini_mem);
    }

    bool operator==(CIniReader& ir)
    {
        if (data.size() != ir.data.size())
            return false;

        for (auto& section : data)
        {
            for (auto& key : data.at(section.first))
            {
                if (key.second != ir.data.at(section.first)[key.first])
                    return false;
            }
        }
        return true;
    }

    bool operator!=(CIniReader& ir)
    {
        return !(*this == ir);
    }

    bool CompareBySections(CIniReader& ir)
    {
        if (data.size() != ir.data.size())
            return false;

        for (auto& section : data)
        {
            if (ir.data.find(section.first) == ir.data.end())
                return false;

            if (section.second.size() != ir.data.find(section.first)->second.size())
                return false;

            if (section.first != ir.data.find(section.first)->first)
                return false;
        }
        return true;
    }

    bool CompareByValues(CIniReader& ir)
    {
        return *this == ir;
    }

    const std::string& GetIniPath()
    {
        return m_szFileName;
    }

    void SetIniPath()
    {
        SetIniPath("");
    }

    void SetIniPath(std::string_view szFileName)
    {
        char buffer[MAX_PATH];
        HMODULE hm = NULL;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)&ends_with, &hm);
        GetModuleFileNameA(hm, buffer, sizeof(buffer));
        std::string modulePath = buffer;

        if (szFileName.find(':') != std::string_view::npos)
        {
            m_szFileName = szFileName;
        }
        else if (szFileName.length() == 0)
        {
            m_szFileName = modulePath.substr(0, modulePath.find_last_of('.')) + ".ini";
        }
        else
        {
            m_szFileName = modulePath.substr(0, modulePath.rfind('\\') + 1) + szFileName.data();
        }

        data.load_file(m_szFileName);
    }

    int ReadInteger(std::string_view szSection, std::string_view szKey, int iDefaultValue)
    {
        auto str = data.get(szSection.data(), szKey.data(), std::to_string(iDefaultValue));
        return std::stoi(str, nullptr, starts_with(str.c_str(), "0x", false) ? 16 : 10);
    }

    float ReadFloat(std::string_view szSection, std::string_view szKey, float fltDefaultValue)
    {
        return (float)atof(data.get(szSection.data(), szKey.data(), std::to_string(fltDefaultValue)).c_str());
    }

    bool ReadBoolean(std::string_view szSection, std::string_view szKey, bool bolDefaultValue)
    {
        auto val = data.get(szSection.data(), szKey.data(), "");
        if (!val.empty())
        {
            if (val.size() == 1)
                return val.front() != '0';
            else
                return compare(val, "false", false);
        }
        return bolDefaultValue;
    }

    std::string ReadString(std::string_view szSection, std::string_view szKey, std::string_view szDefaultValue)
    {
        auto s = data.get(szSection.data(), szKey.data(), szDefaultValue.data());

        if (!s.empty())
        {
            if (s.at(0) == '\"' || s.at(0) == '\'')
                s.erase(0, 1);

            if (s.at(s.size() - 1) == '\"' || s.at(s.size() - 1) == '\'')
                s.erase(s.size() - 1);
        }

        return s;
    }

    void WriteInteger(std::string_view szSection, std::string_view szKey, int iValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection.data(), szKey.data(), std::to_string(iValue));
            data.write_file(m_szFileName);
        }
        else
        {
            char szValue[255];
            _snprintf_s(szValue, 255, "%s%d", " ", iValue);
            WritePrivateProfileStringA(szSection.data(), szKey.data(), szValue, m_szFileName.c_str());
        }
    }

    void WriteFloat(std::string_view szSection, std::string_view szKey, float fltValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection.data(), szKey.data(), std::to_string(fltValue));
            data.write_file(m_szFileName);
        }
        else
        {
            char szValue[255];
            _snprintf_s(szValue, 255, "%s%f", " ", fltValue);
            WritePrivateProfileStringA(szSection.data(), szKey.data(), szValue, m_szFileName.c_str());
        }
    }

    void WriteBoolean(std::string_view szSection, std::string_view szKey, bool bolValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection.data(), szKey.data(), std::to_string(bolValue));
            data.write_file(m_szFileName);
        }
        else
        {
            char szValue[255];
            _snprintf_s(szValue, 255, "%s%s", " ", bolValue ? "True" : "False");
            WritePrivateProfileStringA(szSection.data(), szKey.data(), szValue, m_szFileName.c_str());
        }
    }

    void WriteString(std::string_view szSection, std::string_view szKey, std::string_view szValue, bool useparser = false)
    {
        if (useparser)
        {
            data.set(szSection.data(), szKey.data(), szValue.data());
            data.write_file(m_szFileName);
        }
        else
        {
            WritePrivateProfileStringA(szSection.data(), szKey.data(), szValue.data(), m_szFileName.c_str());
        }
    }
};

#endif //INIREADER_H
