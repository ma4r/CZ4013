//
// Created by Marvin on 28/03/2022.
//

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H
#include <string>
#include <tuple>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include <sstream>
#include "enums.h"

using namespace std;

//Template to tag Object properties with their name on json
template<typename Class, typename T>
struct AttrData {
    constexpr AttrData(T Class::*addr, const char* name) : attrAddr{addr}, attrName{name} {}

    using Type = T;

    T Class::*attrAddr;
    const char* attrName;
};

template<typename Class, typename T>
constexpr auto attribute(T Class::* attrAddr, const char* attrName) {
    return AttrData<Class, T>{attrAddr, attrName};
}

//Template for performing iterations over the templated attributes
template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
    using unpack_t = int[];
    (void)unpack_t{(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0};
}

//Template for deserializing string to various primitives and enums
template <typename T>
//When casting to non specialized object, use fromJson template
//Basically the recursive call for child objects
T cast(const string &s){return fromJson<T>(s);}

//Deserialize primitives and enums
template<> inline string cast<string>(const string &s){ return s;};
template<> inline int cast<int>(const string &s){ return stoi(s);};
template<> inline long cast<long>(const string &s){ return stol(s);};
template<> inline float cast<float>(const string &s){return stof(s);};
template<> inline SerEnum::Currency_e cast<SerEnum::Currency_e>(const string &s){return SerEnum::currfroms(s);};
template<> inline SerEnum::Actions_e cast<SerEnum::Actions_e>(const string &s){ return SerEnum::actfroms(s);};
template<> inline double cast<double>(const string &s){ return stod(s);};

//Template for serializing various strings and enums to string
template <typename T>

//Serialize non specialized Objects with toJson call
//Basically the recursive call for child objects
string toString(T obj){return toJson<T>(obj);}

//Serialize primitives and enums
template<> inline string toString<string>(string s){ return s;};
template<> inline string toString<int>(int s){ return to_string(s);};
template<> inline string toString<long>(long s){ return to_string(s);};

template<> inline string toString<SerEnum::Currency_e>(SerEnum::Currency_e s){ return SerEnum::toString(s);};
template<> inline string toString<SerEnum::Actions_e>(SerEnum::Actions_e s){ return SerEnum::toString(s);};

template<> inline string toString<time_t>(time_t s){ return to_string(s);};
template<> inline string toString<Codes>(Codes e){
    switch(e){
        case Codes::BAD_INPUT:return {"BAD_INPUT"};
        case Codes::SUCCESS:return {"SUCCESS"};
        case Codes::REFUSED:return {"REFUSED"};
        case Codes::UNAUTHORIZED:return {"UNAUTHORIZED"};
        case Codes::SERVER_ERROR:return {"SERVER_ERROR"};
        case Codes::BASE:return {"BASE"};

    }
}
template<> inline string toString<float>(float s){
    ostringstream out;
    out.precision(2);
    out<<fixed<<s;
    return out.str();
};
template<> inline string toString<double>(double s){
    ostringstream out;
    out.precision(2);
    out<<fixed<<s;
    return out.str();
};

//Convert json to key value pairs, only for current level
//See java client
unordered_map<string ,string> parseJson(const string &s);

//Serialization to json
template <typename T>
string toJson(const T& object){

    string data ="";
    constexpr auto nAttr = tuple_size<decltype(T::metaData())>::value;

    //Iterate over the metaData attributes that we tag to objects
    for_sequence(std::make_index_sequence<nAttr>{}, [&](auto i) {
        constexpr auto attr = std::get<i>(T::metaData());

        using Type =typename decltype(attr)::Type;

        //Get property name
        data += "\""+string(attr.attrName)+"\": \"";

        //Get property value
        //If object is unspecialized, this is a recursive call
        data+= toString(object.*(attr.attrAddr));
        data +="\",";
    });

    data.pop_back();

    data = "{"+data+"}";

    return data;
}

//Deserialization from json
template <typename T>
T fromJson(string s){

    unordered_map<string,string> data = parseJson(s);
    T obj;
    constexpr auto nAttr = tuple_size<decltype(T::metaData())>::value;

    for_sequence(std::make_index_sequence<nAttr>{}, [&](auto i) {
        //Get the property name
        constexpr auto attr = std::get<i>(T::metaData());

        //Get type of the property
        using Type =typename decltype(attr)::Type;

        //Set the property value,Deserialize with cast
        //If type is un specialized, this is the recursive call

        //Need to have a cast for arrays as well
        //But arrays themselves are already templates
        //Not implemented because we dont have arrays anyways

        //and templates are hard
        obj.*(attr.attrAddr) = cast<Type>(data[attr.attrName]);
    });

    return obj;

};

//End of string in json, see java client
int endofs(string s,int i);

//End of object in json, see java client
int endofobj(string s,int i);

//End of array in json, see java client
int endofarr(string s,int i);

//Float to string with precision
string f_to_string(float f, int n=0);


#endif //SERVER_UTILS_H
