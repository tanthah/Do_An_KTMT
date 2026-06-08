#include "InputValidator.h"
#include <cstring>
#include <cctype>

bool IsDecimal(const char* text)
{
    if (!text || *text == '\0')
        return false;

    // Skip leading/trailing whitespaces if any (standard strip)
    while (*text == ' ' || *text == '\t')
        text++;

    if (*text == '\0')
        return false;

    // Handle optional minus sign
    if (*text == '-')
        text++;

    if (*text == '\0')
        return false; // Just "-" is invalid

    // Check all remaining characters are digits
    while (*text != '\0')
    {
        // Handle trailing whitespaces
        if (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
        {
            // Ensure only whitespace characters remain
            while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
                text++;
            return (*text == '\0');
        }

        if (!isdigit((unsigned char)*text))
            return false;
        text++;
    }

    return true;
}

bool IsBinary(const char* text)
{
    if (!text || *text == '\0')
        return false;

    while (*text == ' ' || *text == '\t')
        text++;

    if (*text == '\0')
        return false;

    while (*text != '\0')
    {
        if (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
        {
            while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n')
                text++;
            return (*text == '\0');
        }

        if (*text != '0' && *text != '1')
            return false;
        text++;
    }

    return true;
}

bool IsHex(const char* text)
{
    if (!text || *text == '\0')
        return false;

    while (*text == ' ' || *text == '\t')
        text++;

    if (*text == '\0')
        return false;

    // Count length of hex part
    const char* start = text;
    const char* end = text;
    while (*end != '\0' && *end != ' ' && *end != '\t' && *end != '\r' && *end != '\n')
    {
        end++;
    }

    if (start == end)
        return false;

    const char* orig_end = end;
    const char* last_char = end - 1;
    if (last_char >= start && (*last_char == 'H' || *last_char == 'h'))
    {
        end--; // exclude the H/h from hex digit validation
    }

    if (start == end)
        return false; // Just "H" or "h" is invalid

    // Check characters are valid hex digits
    for (const char* p = start; p < end; p++)
    {
        char c = *p;
        if (!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')))
            return false;
    }

    // Check remaining characters are only whitespace
    const char* p_rem = orig_end;
    while (*p_rem != '\0')
    {
        if (*p_rem != ' ' && *p_rem != '\t' && *p_rem != '\r' && *p_rem != '\n')
            return false;
        p_rem++;
    }

    return true;
}