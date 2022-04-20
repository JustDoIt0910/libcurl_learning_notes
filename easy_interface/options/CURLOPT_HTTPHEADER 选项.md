# CURLOPT_HTTPHEADER 选项

### 用法：

```c++
CURLcode curl_easy_setopt(CURL *handle, CURLOPT_HTTPHEADER, struct curl_slist *headers);
```



#### 其中 headers 链表要通过 [curl_slist_append](https://curl.se/libcurl/c/curl_slist_append.html)() 函数创建：

```c++
struct curl_slist* header = NULL;
header = curl_slist_append(header, "Content-Type: application/json");
header = curl_slist_append(header, "Accept-Encoding: gzip, deflate, br");
header = curl_slist_append(header, "Connection: Keep-alive");
curl_easy_setopt(conn, CURLOPT_HTTPHEADER, header);
```

#### 并通过  [curl_slist_free_all](https://curl.se/libcurl/c/curl_slist_free_all.html)() 释放：

```c++
curl_slist_free_all(header);
```

### 作用：

#### 设置 http 请求头

### 

### 注意事项：

#### curl_easy_opt() 函数不会对传递给它的 curl_slist 链表进行复制，所以在整个 libcurl 实例的使用过程中要保证改链表不被释放，只有当前 libcurl 实例不再使用时才可以调用 curl_slist_free_all() 进行释放。