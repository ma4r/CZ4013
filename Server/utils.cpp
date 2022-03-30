//
// Created by Marvin on 29/03/2022.
//

#include "utils.h"

int endofs(string s,int i){
    i++;
    while(s[i] != '\"'){
        if(s[i] == '\\') {
            i += 2;
            continue;
        }
        i++;
    }
    return i;
}

int endofobj(string s,int i){
    i++;

    while(s[i] != '}'){
        if(s[i]=='\"'){
            i = endofs(s,i)+1;
            continue;
        }

        if (s[i] == '['){
            i = endofarr(s,i)+1;
            continue;
        }

        if (s[i] == '{'){
            i = endofobj(s,i)+1;
            continue;
        }

        i++;

    }
    return i;
}

int endofarr(string s,int i){
        i++;
        while(s[i] != ']'){
            if(s[i]=='\"'){
                i = endofs(s,i)+1;
                continue;
            }

            if (s[i] == '['){
                i = endofarr(s,i)+1;
                continue;
            }

            if (s[i] == '{'){
                i = endofobj(s,i)+1;
                continue;
            }
            i++;

        }

        return i;

}

unordered_map<string,string> parseJson(const string &s){

    auto res = unordered_map<string,string>();

    if(s[0] != '{' && s[s.length()-1] != '}'){
    //throw exception("Invalid Json Format");
    cout<<"INVALID JSON"<<endl;
    }
    int end =  0;
    int i=1;
    while(i<s.length()-1) {

        //Find Name
        while (s[i] != '\"') { i++; }

        end = endofs(s, i);
        i++;
        string name = s.substr(i, end - i);
        i = end + 1;

        //Find Separator
        while (s[i] != ':') { i++; }

        //Get Value
        while (s[i] != '\"' && s[i] != '[' && s[i] != '{') { i++; }
        string val;
        bool flag = false;

        if (s[i] == '\"') {
            //Handle primitive types
            end = endofs(s, i) - 1;
            i++;
            flag = true;
        } else if (s[i] == '[') {
            end = endofarr(s, i);
        } else if (s[i] == '{') {
            end = endofobj(s, i);
        }
        val = s.substr(i, end - i + 1);
        i = end + 1 + flag;
        res[name] = val;

        while (s[i] != ',' && s[i]!='}') { i++; }
        if(s[i] == '}'){
            break;
        }
        i++;
    }
    return res;
};

string f_to_string(float f, int n){
    ostringstream out;
    out.precision(n);
    out<<fixed<<f;

    return out.str();
};
