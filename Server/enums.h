//
// Created by Marvin on 28/03/2022.
//

#ifndef SERVER_ENUMS_H
#define SERVER_ENUMS_H

#include <string>
using namespace std;


enum Codes{
    BASE,
    SUCCESS,
    BAD_INPUT,
    SERVER_ERROR,
    UNAUTHORIZED,
    REFUSED
};


//Serializable enums, with toString and fromString methods
struct SerEnum{
    //Currency types
    enum Currency_e {USD,
        SGD,
        RMB,
        YEN,
        EUR};

    static string toString(Currency_e e){
        switch(e){
            case Currency_e::USD:return {"usd"};
            case Currency_e::SGD:return {"sgd"};
            case Currency_e::RMB:return {"rmb"};
            case Currency_e::YEN:return {"yen"};
            case Currency_e::EUR:return {"eur"};
        }
    }
    static Currency_e currfroms(string k){
        string::iterator it;
        for(it = k.begin();it!=k.end();it++){
            *it = (char)tolower(*it);
        }
        if (k == "usd"){return  Currency_e::USD;}
        else if (k == "sgd"){return  Currency_e::SGD;}
        else if (k == "rmb"){return  Currency_e::RMB;}
        else if (k == "eur"){return  Currency_e::EUR;}
        else if (k == "yen"){return  Currency_e::YEN;}
        else{throw exception("Enum not found");}
    }

    //Possible request actions
    enum Actions_e {
        CREATE_ACC,
        CLOSE_ACC,
        DEP_WITH,
        SUBSCRIBE,
        TRANSFER,
        CHANGE_NAME,
        SET_UP
    };
    static Actions_e actfroms(string k){
        string::iterator it;
        for(it = k.begin();it!=k.end();it++){
            *it = (char)tolower(*it);
        }
        if (k == "set_up"){return  Actions_e::SET_UP;}
        else if (k == "create_acc"){return  Actions_e::CREATE_ACC;}
        else if (k == "close_acc"){return  Actions_e::CLOSE_ACC;}
        else if (k == "dep_with"){return  Actions_e::DEP_WITH;}
        else if (k == "subscribe"){return  Actions_e::SUBSCRIBE;}
        else if (k == "transfer"){return  Actions_e::TRANSFER;}
        else if (k == "change_name"){return  Actions_e::CHANGE_NAME;}
        else{throw exception("Enum not found");}

    }
    static string toString(Actions_e e){
        switch(e){
            case Actions_e::SET_UP:return {"set_up"};
            case Actions_e::CREATE_ACC:return {"create_acc"};
            case Actions_e::CLOSE_ACC:return {"close_acc"};
            case Actions_e::DEP_WITH:return {"dep_with"};
            case Actions_e::SUBSCRIBE:return {"subscribe"};
            case Actions_e::TRANSFER:return {"transfer"};
            case Actions_e::CHANGE_NAME:return {"change_name"};

        }
    }


};



#endif //SERVER_ENUMS_H
