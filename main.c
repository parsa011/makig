#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curl/curl.h>

const char *options[] = {
    "C",
    "C++"
};

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) 
{
    s->len = 0;
    s->ptr = malloc(s->len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "malloc() failed\n");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
    size_t new_len = s->len + size * nmemb;
    s->ptr = realloc(s->ptr, new_len+1);
    if (s->ptr == NULL) {
        fprintf(stderr, "realloc() failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, ptr, size * nmemb);
    s->ptr[new_len] = '\0';
    s->len = new_len;
    return size * nmemb;
}

struct string make_url(const char *type)
{
    struct string s;
    init_string(&s);
    s.ptr = malloc(128);
    sprintf(s.ptr, "https://raw.githubusercontent.com/github/gitignore/main/%s.gitignore", type);
    return s;
}

void make_curl_request(char *url, struct string *s)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
}

void write_options()
{
    printf("Available Options : \n");
    for (int i = 0; i < sizeof(options) / sizeof(options[0]); i++) {
        printf("[%d] : %s\n", i, options[i]);
    }
}

const char *get_option()
{
    printf("Select (Write Number And press enter) :\n");
    int opt_size = sizeof(options) / sizeof(options[0]);
    int ch;
again :
    do {
        ch = getchar();
    } while (!isdigit(ch));
    ch -= '0';
    if (ch < 0 || ch > opt_size - 1) {
        printf("Please Enter valid Number\n");
        goto again;
    }
    return options[ch];
}

int main(int argc, char *argv[])
{
    write_options();
    const char *opt = get_option();
    struct string data, url;
    init_string(&data);
    init_string(&url);
    url = make_url(opt);
    printf("%s\n", url.ptr);
    make_curl_request(url.ptr, &data);
    printf("%s\n", data.ptr);
    return 0;
}
