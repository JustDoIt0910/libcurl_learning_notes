package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
)

type respStruct struct {
	Code int         `json:"code"`
	Data interface{} `json:"data"`
	Msg  string      `json:"msg"`
}

func main() {
	http.HandleFunc("/", handler)
	http.ListenAndServe("127.0.0.1:9999", nil)
}

func handler(writer http.ResponseWriter, r *http.Request) {
	fmt.Println(r.Method)
	fmt.Println(r.URL)
	for key, value := range r.Header {
		fmt.Printf("%s: %s\n", key, value)
	}
	body, err := ioutil.ReadAll(r.Body)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println("request body: " + string(body))
	resp := respStruct{
		200,
		"data",
		"OK",
	}
	respBody, err := json.Marshal(resp)
	if err != nil {
		fmt.Println(err)
	}
	writer.WriteHeader(http.StatusOK)
	writer.Write(respBody)
}
