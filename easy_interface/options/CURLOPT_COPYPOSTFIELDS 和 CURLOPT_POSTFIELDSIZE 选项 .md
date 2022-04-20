# CURLOPT_COPYPOSTFIELDS 和 CURLOPT_POSTFIELDSIZE 选项

### 用法：

```c++
//设置请求体长度
CURLcode curl_easy_setopt(CURL *handle, CURLOPT_POSTFIELDSIZE, long size);
//设置请求体数据
CURLcode curl_easy_setopt(CURL *handle, CURLOPT_COPYPOSTFIELDS, char *data);
```



### 作用： 

#### 	这两个选项需要配合使用，用于设置 post 请求体数据和长度。与 CURLOPT_POSTFIELDS 选项不同的是， CURLOPT_COPYPOSTFIELDS 选项会对传入的 data 进行复制。

### 注意事项： 

#### 	与其他选项不同，这两个选项的设置是有先后顺序的。如果需要手动设置请求体长度，那么一定要在设置 CURLOPT_COPYPOSTFIELDS(或CURLOPT_POSTFIELDS) 选项之前先设置 CURLOPT_POSTFIELDSIZE。如果 data 是以空字符结尾的字符串，也可以不用设置 CURLOPT_POSTFIELDSIZE，因为 libcurl 会自动计算数据长度。但是不要在设置了 CURLOPT_COPYPOSTFIELDS 或者 CURLOPT_POSTFIELDS 之后再用 CURLOPT_POSTFIELDSIZE 修改长度。(在使用 TestServer 进行测试时发现，若两个选项设置顺序颠倒，服务器读取请求体的时候会永久阻塞)



[简单的post请求demo](https://github.com/JustDoIt0910/libcurl_learning_notes/blob/main/easy_interface/demo/post/post.cpp)

