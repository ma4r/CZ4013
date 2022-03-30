//
// Created by Marvin on 29/03/2022.
//

#ifndef SERVER_RESPONSE_H
#define SERVER_RESPONSE_H
#include "enums.h"
#include "ResponseData.h"


//Created response object, containing response data as string
//Contains response header info such a status code and message from server
//Should've been able to be templated , but C++ doesn't like templated type variables since the deserializers and serializers
//themselves are already templated

//e.g. attribute<Response<DTO>,Codes>(&Response<DTO>::code, "code")
//is translated to attribute<T<K>,type>(&T<K>::code,"code")

//Serialize object first, then add header data according to action
class Response{

public:
    Codes code = Codes::BASE;
    string message;
    string data;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<Response,Codes>(&Response::code, "code"),
                attribute<Response,string>(&Response::message,"message")
        );
    }
};

#endif //SERVER_RESPONSE_H
