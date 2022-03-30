//
// Created by Marvin on 28/03/2022.
//

#ifndef SERVER_REQUESTDATA_H
#define SERVER_REQUESTDATA_H
#include <string>
#include "enums.h"
#include "utils.h"
using namespace std;

//Classes of the data from client requests, there is a separate Class for each different requests
class Req{

public:
    virtual void text(){
        cout<<"BASE"<<endl;
    }
};

class CreateAccReq: public Req{
public:
    string name;
    string password;
    SerEnum::Currency_e currency;
    float amount;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<CreateAccReq,string>(&CreateAccReq::name, "name"),
                attribute<CreateAccReq,string>(&CreateAccReq::password, "password"),
                attribute<CreateAccReq,SerEnum::Currency_e>(&CreateAccReq::currency, "currency"),
                attribute<CreateAccReq,float>(&CreateAccReq::amount, "amount")

        );
    }

    void text(){
        cout<<this->name<<endl;
        cout<<this->password<<endl;
        cout<<SerEnum::toString(this->currency)<<endl;
        cout<<this->amount<<endl;
    }
};

class DeleteAccountReq: public Req{
public:
    string name;
    int account;
    string password;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<DeleteAccountReq,string>(&DeleteAccountReq::name, "name"),
                attribute<DeleteAccountReq,int>(&DeleteAccountReq::account, "account"),
                attribute<DeleteAccountReq,string>(&DeleteAccountReq::password, "password")
        );
    }



};

class DepWithReq: public Req{
public:
    string name;
    int account;
    string password;
    SerEnum::Currency_e currency;
    float amount;

    constexpr static auto metaData() {
        return make_tuple(
                attribute<DepWithReq,string>(&DepWithReq::name, "name"),
                attribute<DepWithReq,int>(&DepWithReq::account, "account"),
                attribute<DepWithReq,string>(&DepWithReq::password, "password"),
                attribute<DepWithReq,SerEnum::Currency_e>(&DepWithReq::currency, "currency"),
                attribute<DepWithReq,float>(&DepWithReq::amount, "amount")

        );
    }

};

class SubscribeReq: public Req{
public:
    long interval;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<SubscribeReq,long>(&SubscribeReq::interval, "interval")
        );
    }

};

class ChangeNameReq: public Req{
public:
    string name;
    int account;
    string password;
    string newName;

    constexpr static auto metaData() {
        return make_tuple(
                attribute<ChangeNameReq,string>(&ChangeNameReq::name, "name"),
                attribute<ChangeNameReq,int>(&ChangeNameReq::account, "account"),
                attribute<ChangeNameReq,string>(&ChangeNameReq::password, "password"),
                attribute<ChangeNameReq,string>(&ChangeNameReq::newName, "newName")

        );
    }
};

class TransferReq: public Req{
public:
    string name;
    int account;
    string password;
    SerEnum::Currency_e currency;
    float amount;
    int targetAcc;

    constexpr static auto metaData() {
        return make_tuple(
                attribute<TransferReq,string>(&TransferReq::name, "name"),
                attribute<TransferReq,int>(&TransferReq::account, "account"),
                attribute<TransferReq,string>(&TransferReq::password, "password"),
                attribute<TransferReq,SerEnum::Currency_e>(&TransferReq::currency, "currency"),
                attribute<TransferReq,float>(&TransferReq::amount, "amount"),
                attribute<TransferReq,int>(&TransferReq::targetAcc, "targetAcc")

        );
    }
};









#endif //SERVER_REQUESTDATA_H
