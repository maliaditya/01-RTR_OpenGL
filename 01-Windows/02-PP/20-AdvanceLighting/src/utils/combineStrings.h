#pragma once

#include <string>

// Combine the strings
const char* combine(const char* str1, const char* str2) {
       size_t len1 = strlen(str1);
       size_t len2 = strlen(str2);
       char* result = new char[len1 + len2 + 1]; // +1 for the null terminator
       strcpy(result, str1);
       strcat(result, str2);
       return result;
}

