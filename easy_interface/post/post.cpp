#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <curl.h>
#include <iostream>

static char errorBuffer[CURL_ERROR_SIZE];
static std::string buffer;
struct curl_slist* header = NULL;

static int writer(char* data, size_t size, size_t nmemb,
    std::string* writerData)
{
    if (writerData == NULL)
        return 0;
    writerData->append(data, size * nmemb);
    return size * nmemb;
}

static bool init(CURL*& conn, char* url)
{
    CURLcode code;

    conn = curl_easy_init();

    if (conn == NULL) {
        fprintf(stderr, "Failed to create CURL connection\n");
        exit(EXIT_FAILURE);
    }
    code = curl_easy_setopt(conn, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set error buffer [%d]\n", code);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_URL, url);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
        return false;
    }
    
    header = curl_slist_append(header, "Content-Type: application/json");
    header = curl_slist_append(header, "Accept-Encoding: gzip, deflate, br");
    header = curl_slist_append(header, "Connection: Keep-alive");
    curl_easy_setopt(conn, CURLOPT_HTTPHEADER, header);
    code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_WRITEDATA, &buffer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set write data [%s]\n", errorBuffer);
        return false;
    }
    char postData[1024] = "data to post";
    /*code = curl_easy_setopt(conn, CURLOPT_POSTFIELDSIZE, 12L);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set set post data size [%s]\n", errorBuffer);
        return false;
    }*/
    code = curl_easy_setopt(conn, CURLOPT_COPYPOSTFIELDS, postData);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set set post data [%s]\n", errorBuffer);
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    CURL* conn = NULL;
    CURLcode code;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <url>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    curl_global_init(CURL_GLOBAL_DEFAULT);
    if (!init(conn, argv[1])) {
        fprintf(stderr, "Connection initializion failed\n");
        exit(EXIT_FAILURE);
    }

    code = curl_easy_perform(conn);
    curl_slist_free_all(header);
    curl_easy_cleanup(conn);

    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to get '%s' [%s]\n", argv[1], errorBuffer);
        exit(EXIT_FAILURE);
    }
    std::cout << buffer << std::endl;
    return EXIT_SUCCESS;
}
