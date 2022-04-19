# CURLOPT_HEADER 选项

### 用法：

```c++
CURLcode curl_easy_setopt(CURL *handle, CURLOPT_HEADER, long onoff);
```

### 作用：

#### 	如果 onoff 设置为1，libcurl 在调用回调函数写响应数据的时候，会将响应头作为数据流的一部分写入。

### 示例：

#### 初始化部分

```c++
std::ofstream file("test.html"); //将响应数据写入文件
static char errorBuffer[CURL_ERROR_SIZE];

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
    //此处省略若干不重要的选项
    code = curl_easy_setopt(conn, CURLOPT_URL, url);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set URL [%s]\n", errorBuffer);
        return false;
    }
    code = curl_easy_setopt(conn, CURLOPT_WRITEFUNCTION, writer);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set writer [%s]\n", errorBuffer);
        return false;
    }
    //将响应头作为数据一部分
    code = curl_easy_setopt(conn, CURLOPT_HEADER, 1L);
    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to set header [%s]\n", errorBuffer);
        return false;
    }
    return true;
}
```

#### 回调函数

```c++
static int writer(char* data, size_t size, size_t nmemb,
    std::string* writerData)
{
    if (writerData == NULL)
        return 0;

    writerData->append(data, size * nmemb);
    file.write(data, size * nmemb);
    return size * nmemb;
}
```

#### 主程序

```c++
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
    curl_easy_cleanup(conn);

    if (code != CURLE_OK) {
        fprintf(stderr, "Failed to get '%s' [%s]\n", argv[1], errorBuffer);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
```



#### 以百度为例，请求 http://www.baidu.com

#### 可以看到响应头也被写入到文件中

```html
HTTP/1.1 200 OK

Accept-Ranges: bytes

Cache-Control: no-cache

Connection: keep-alive

Content-Length: 9508

Content-Type: text/html

Date: Tue, 19 Apr 2022 16:30:32 GMT

P3p: CP=" OTI DSP COR IVA OUR IND COM "

P3p: CP=" OTI DSP COR IVA OUR IND COM "

Pragma: no-cache

Server: BWS/1.1

Set-Cookie: BAIDUID=24682CE73854B35FE2AF4E510F6BEF50:FG=1; expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/; domain=.baidu.com

Set-Cookie: BIDUPSID=24682CE73854B35FE2AF4E510F6BEF50; expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/; domain=.baidu.com

Set-Cookie: PSTM=1650385832; expires=Thu, 31-Dec-37 23:55:55 GMT; max-age=2147483647; path=/; domain=.baidu.com

Set-Cookie: BAIDUID=24682CE73854B35F42E919CC84B9D9E9:FG=1; max-age=31536000; expires=Wed, 19-Apr-23 16:30:32 GMT; domain=.baidu.com; path=/; version=1; comment=bd

Traceid: 1650385832028336487410266452787264452114

Vary: Accept-Encoding

X-Frame-Options: sameorigin

X-Ua-Compatible: IE=Edge,chrome=1



<!DOCTYPE html><html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1"><meta content="always" name="referrer"><meta name="description" content="全球领先的中文搜索引擎、致力于让网民更便捷地获取信息，找到所求。百度超过千亿的中文网页数据库，可以瞬间找到相关的搜索结果。"><link rel="shortcut icon" href="//www.baidu.com/favicon.ico" type="image/x-icon">
<!--省略剩余内容-->
```

