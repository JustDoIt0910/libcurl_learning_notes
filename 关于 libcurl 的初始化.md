# 关于 libcurl 的初始化

#### 	libcurl 的初始化由 [curl_global_init](https://curl.se/libcurl/c/curl_global_init.html)() 函数完成。该函数负责为 libcurl 分配必要资源，所以必须与 [curl_global_cleanup](https://curl.se/libcurl/c/curl_global_cleanup.html)() 函数成对使用。后者负责在 libcurl 使用过后进行资源的释放。

#### 		需要特别注意的是，curl_global_init() 函数不是线程安全的，所以必须在程序启动之初，仅有主线程一个线程运行的时候调用该函数。同理，也必须在程序即将退出之前，仅剩主线程一个线程存在的时候调用 curl_global_cleanup() 函数。

#### 	此外，在使用 libcurl 的 easy interface 时，需要首先调用 [curl_easy_init](https://curl.se/libcurl/c/curl_easy_init.html)() 函数进行初始化，如果在调用 curl_easy_init() 函数之前没有调用过 curl_global_init(), 后者会被自动调用，但是为了线程安全，永远要在调用 curl_easy_init() 之前手动调用 curl_global_init()

