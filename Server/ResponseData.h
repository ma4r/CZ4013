//
// Created by Marvin on 29/03/2022.
//

#ifndef SERVER_RESPONSEDATA_H
#define SERVER_RESPONSEDATA_H
#include "enums.h"
#include "utils.h"
// Response data classes in the response object

class CreateAccountDTO {
public:
    int account;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<CreateAccountDTO,int>(&CreateAccountDTO::account, "account")
        );
    }
};

class DepWithDTO{
public:
    float newBalance;
    SerEnum::Currency_e currency;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<DepWithDTO,float>(&DepWithDTO::newBalance, "newBalance"),
                attribute<DepWithDTO,SerEnum::Currency_e>(&DepWithDTO::currency, "currency")
        );
    }
};

class DeleteAccountDTO{
public:
    int account;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<DeleteAccountDTO,int>(&DeleteAccountDTO::account, "account")
        );
    }
};

class SubscribeDTO{
public:
    time_t start;
    time_t end;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<SubscribeDTO,time_t>(&SubscribeDTO::start, "start"),
                attribute<SubscribeDTO,time_t>(&SubscribeDTO::end, "end")
        );
    }
};


class ChangeNameDTO{
public:
    string oldName;
    string newName;
    constexpr static auto metaData() {
        return make_tuple(
                attribute<ChangeNameDTO,string>(&ChangeNameDTO::oldName, "oldName"),
                attribute<ChangeNameDTO,string>(&ChangeNameDTO::newName, "newName")
        );
    }
};

class TransferDTO{
public:
    int targetAcc;
    string targetName;
    SerEnum::Currency_e currency;
    float amount;
    float balanceLeft;

    constexpr static auto metaData() {
        return make_tuple(
                attribute<TransferDTO,int>(&TransferDTO::targetAcc, "targetAcc"),
                attribute<TransferDTO,string>(&TransferDTO::targetName, "targetName"),
                attribute<TransferDTO,SerEnum::Currency_e>(&TransferDTO::currency, "currency"),
                attribute<TransferDTO,float>(&TransferDTO::amount, "amount"),
                attribute<TransferDTO,float>(&TransferDTO::balanceLeft, "balanceLeft")
        );
    }
};

class SetUpDTO{
public:
    int session;
    constexpr static auto metaData(){
        return make_tuple(
                attribute<SetUpDTO,int>(&SetUpDTO::session, "session")
        );
    }
};
#endif //SERVER_RESPONSEDATA_H
