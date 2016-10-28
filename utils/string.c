//
// Created by Simone Mattogno on 28/10/2016.
//

#include "string.h"

int intlen(int value)
{
    return ceil(log10(abs(value))) + 1;
}

char *str(int value)
{
    int nDigits = intlen(value);
    char *buffer = malloc(nDigits * sizeof(char*));
    snprintf(buffer, nDigits, "%d", value);
    return buffer;
}

ssize_t fd_getline(int fd, char **out)
{
    int buf_size = 128;
    int bytesloaded = 0;
    ssize_t ret;
    char buf;
    char *buffer = malloc(buf_size);
    char *newbuf;
    
    if (NULL == buffer)
        return -1;
    do
    {
        if ((ret = read(fd, &buf, 1)) < 1)
        {
            free(buffer);
            return -1;
        }
        
        buffer[bytesloaded] = buf;
        bytesloaded++;
        
        if (buf == '\n')
            break;
        
        if (bytesloaded >= buf_size)
        {
            buf_size += buf_size;
            newbuf = realloc(buffer, buf_size);
            
            if (NULL == newbuf)
            {
                free(buffer);
                return -1;
            }
            
            buffer = newbuf;
        }
    } while (1);
    
    buffer[bytesloaded-1] = '\0'; //terminate the string
    
    *out = buffer;
    
    return bytesloaded;
}

int index_of_char(const char *string, const char character)
{
    char *ptr;
    
    ptr = strchr(string, character);
    if (ptr == NULL) return -1;
    
    return (int)(ptr - string);
}

char *substr(const char *string, const int start_index, const int end_index)
{
    uint64_t sub_string_length = end_index - start_index;
    char *sub_string = malloc(sub_string_length + 1);
    memcpy(sub_string, string + start_index, end_index);
    sub_string[sub_string_length] = '\0';
    return sub_string;
}



char *string_by_appending_string(const char *base_string, const char *string_to_append)
{
    uint64_t base_string_length = strlen(base_string);
    uint64_t string_to_append_length = strlen(string_to_append);
    uint64_t final_string_length = base_string_length + string_to_append_length;
    
    char *final_string = malloc(final_string_length + 1);
    
    memcpy(final_string, base_string, base_string_length);
    memcpy(final_string + base_string_length, string_to_append, string_to_append_length);
    
    final_string[final_string_length] = '\0';
    
    return final_string;
}

char *string_by_adding_path_component(const char *base_path, const char *path_component)
{
    uint64_t base_path_length = strlen(base_path);
    char *truncated_base_path = base_path[base_path_length - 1] == '/' ? substr(base_path, 0, (int)base_path_length - 1) : (char *)base_path;
    char *truncated_path_component = path_component[0] == '/' ? (char *)path_component++ : (char *)path_component;
    
    uint64_t truncated_base_path_length = strlen(truncated_base_path);
    uint64_t truncated_path_component_length = strlen(truncated_path_component);
    
    uint64_t final_path_length = truncated_base_path_length + 1 + truncated_path_component_length;
    
    char *final_path = malloc(final_path_length + 1);
    
    memcpy(final_path, truncated_base_path, truncated_base_path_length);
    final_path[truncated_base_path_length] = '/';
    memcpy(final_path + truncated_base_path_length + 1, truncated_path_component, truncated_path_component_length);
    
    final_path[final_path_length] = '\0';
    
    if (base_path[base_path_length - 1] == '/') free(truncated_base_path);
    
    return final_path;
}

char *file_path_extension(const char *file_path)
{
    uint64_t path_length = strlen(file_path);
    uint64_t extension_length = 0;
    for (char *pivot = (char *)file_path + path_length; extension_length < path_length; pivot--, extension_length++) {
        if (*pivot == '.')
        {
            extension_length--;
            pivot++;
            break;
        }
    }
    if (extension_length == 0) return "";
    
    char *buffer = malloc(extension_length + 1);
    memcpy(buffer, &file_path[path_length-extension_length], extension_length);
    buffer[extension_length] = '\0';
    return buffer;
}

char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof(charset) - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

int streq(const char *a, const char *b)
{
    return a != NULL && b != NULL && strcmp(a, b) == 0;
}

void urldecode2(char *dst, const char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a')
                a -= 'a'-'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a'-'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16*a+b;
            src+=3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}

const char *string_copy(const char *string)
{
    unsigned long string_length = strlen(string);
    char *buffer = malloc(string_length + 1);
    memcpy(buffer, string, string_length);
    buffer[string_length] = '\0';
    return buffer;
}

void formatted_time(char *buffer, size_t size, const char *format)
{
    struct tm *sTm;
    
    time_t now = time (0);
    sTm = localtime(&now);
    
    strftime (buffer, size, format, sTm);
}
