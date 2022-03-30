//
// Created by Marvin on 28/03/2022.
//


#ifndef SERVER_REQUEST_H
#define SERVER_REQUEST_H

#include <iostream>
#include "enums.h"
#include "RequestData.h"


using namespace std;

//Request class along with headers, contain additional data in message
//Basically contains the "Message" objects from java client
//Can't be templated because we dont know the class of the message object before deserializing

//Strip header first, and deserialize data object according to action
class Request{

public:
    int session;
    int id;
    SerEnum::Actions_e action;
    string message;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<Request,int>(&Request::session, "session"),
                attribute<Request,int>(&Request::id,"id"),
                attribute<Request,SerEnum::Actions_e>(&Request::action,"action"),
                attribute<Request,string>(&Request::message,"data")

        );
    }


};


#endif //SERVER_REQUEST_H
