package co.response;

import co.utils;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;


//Deserializer class, input target type as class parameter
public class Deserializer<T> {


    private final Class<T> type;

    public Deserializer(Class<T> type){
        this.type = type;
    }

    public T fromJson(String s) throws Exception {
        if(s ==null){
            return null;
        }



        T obj = this.type.getConstructor().newInstance();

        //Convert current level to key value pairs
        HashMap<String,String> dict = parseJson(s);

        //Iterate over each field of target object type
        for (Field field : this.type.getFields()){

            String val = dict.get(field.getName());
            Class type = field.getType();

            //Handle primitive types, just set field as value after parsing
            if (type.equals(int.class)){
                field.set(obj,Integer.parseInt(val));
            }else if(type.equals(long.class)){
                field.set(obj,Long.parseLong(val));
            }else if(type.equals(float.class)){
                field.set(obj,Float.parseFloat(val));
            }else if(type.equals(double.class)){
                field.set(obj,Double.parseDouble(val));
            }else if(type.equals(short.class)){
                field.set(obj,Short.parseShort(val));
            }else if(type.equals(char.class)){
                field.set(obj,val.charAt(0));
            }else if(type.equals(boolean.class)){
                field.set(obj,Boolean.parseBoolean(val));
            }else if(type.equals(String.class)){
                field.set(obj,val);
            }else if(type.equals(byte.class)){
                field.set(obj,Byte.parseByte(val));
            }else{

                //If field is an object, build the object frm the string found in hashmap;
                // with the object's fromString method

                //The fromString method calls this fromJson method again, but with its own type
                // as type parameter T

                //Basically the recursive call for deserializing nested objects, have to do it this way because
                //java doesn't allow using types as variables
                Method factory = type.getMethod("fromString",String.class);
                if(!val.equals("null")){
                field.set(obj,factory.invoke(null,val));}
                else{
                    field.set(obj,null);
                }
            }

            //Can also build if case for arrays, but since data does not contain array
            //not needed yet, similar implementation, recursive fromString call, but iterated
            //for each element in array

        }

        return obj;
    }

    //Find end of string in a json key or field
    private int endofs(String s,int i){
        i++;
        while(s.charAt(i) != '\"'){
            if(s.charAt(i) == '\\') {
                i += 2;
                continue;
            }
            i++;
        }
        return i;
    }

    //Find end of object in a json field
    private int endofobj(String s,int i){
        i++;

        while(s.charAt(i) != '}'){

            //Skip strings
            if(s.charAt(i)=='\"'){
                i = endofs(s,i)+1;
                continue;
            }

            //Recursive call for child arrays
            if (s.charAt(i) == '['){
                i = endofarr(s,i)+1;
                continue;
            }

            //Recursive call for child objects
            if (s.charAt(i) == '{'){
                i = endofobj(s,i)+1;
                continue;
            }

            i++;

        }
        return i;
    }

    //Find end of array in a json field
    private int endofarr(String s,int i){
        i++;
        while(s.charAt(i) != ']'){

            //Skip strings
            if(s.charAt(i)=='\"'){
                i = endofs(s,i)+1;
                continue;
            }

            //Recursive call for child array
            if (s.charAt(i) == '['){
                i = endofarr(s,i)+1;
                continue;
            }

            //Recursive call for child object
            if (s.charAt(i) == '{'){
                i = endofobj(s,i)+1;
                continue;
            }
            i++;

        }

        return i;

    }

    //Parse json into key value pair, only at top level, child arrays and objects are stored as strings
    private HashMap<String,String> parseJson(String s) throws Exception {


        int len = s.length();

        if(s.charAt(0) != '{' || s.charAt(len-1) != '}' ){
            throw new Exception("Bad JSON format");
        }
        HashMap<String,String>res = new HashMap<>();

        int end =  0;
        int i=1;
        while(i<len-1) {

            //Find Name
            while (s.charAt(i) != '\"') { i++; }

            end = endofs(s, i);
            i++;
            String name = s.substring(i, end);
            i = end + 1;

            //Find  Separator ':'
            while (s.charAt(i) != ':') { i++; }

            //Get Value
            while (s.charAt(i) != '\"' &&
                    s.charAt(i) != '[' &&
                    s.charAt(i) != '{' &&
                    s.charAt(i) != 'n') { i++; }
            String val;
            boolean flag = false;
            boolean isNull=false;

            //Check if value is string,object,array,or null
            if (s.charAt(i) == '\"') {
                //Find length of string
                end = endofs(s, i) - 1;
                i++;
                flag = true;
            } else if (s.charAt(i) == '[') {
                //Find length of array
                end = endofarr(s, i);
            } else if (s.charAt(i) == '{') {
                //Find length of object
                end = endofobj(s, i);
            } else{
                //Length of null
                end = i+3;
                isNull=true;
            }

            //Get json representation of object/string/array as string
            val = s.substring(i, end + 1);
            i = end + 1 + (flag?1:0);

            //Save to hashmap
            if(!isNull)
                res.put(name,val);
            else
                res.put(name,null);

            // Find separator ","
            while (s.charAt(i) != ',' && s.charAt(i)!='}') { i++; }
            if(s.charAt(i)== '}'){
                break;
            }
            i++;
        }

        return res;
    }

    //Parse json array into array of strings, only at top level, child arrays and objects are stored as strings
    private ArrayList<String> parseArray(String s) throws Exception {
        int len = s.length();
        if(s.charAt(0) != '[' || s.charAt(len-1) != ']' ){
            throw new Exception("Bad Array format");
        }
        ArrayList<String> res = new ArrayList<>();

        int end =  0;
        int i=1;
        while(i<len-1) {

            //Get Value
            while (s.charAt(i) != '\"' && s.charAt(i) != '[' && s.charAt(i) != '{') { i++; }
            String val;
            boolean flag = false;

            if (s.charAt(i) == '\"') {
                //Handle primitive types
                end = endofs(s, i) - 1;
                i++;
                flag = true;
            } else if (s.charAt(i) == '[') {
                end = endofarr(s, i);
            } else if (s.charAt(i) == '{') {
                end = endofobj(s, i);
            } else if (s.charAt(i) == 'n'){
                end = i+3;
            }
            val = s.substring(i, end + 1);
            i = end + 1 + (flag?1:0);

            res.add(val);

            while (s.charAt(i) != ',' && s.charAt(i)!=']') { i++; }
            if(s.charAt(i)== ']'){
                break;
            }
            i++;
        }

        return res;
    }



    public Response<T> parseResponse(String res) throws Exception {
        HashMap<String,String> pairs = parseJson(res);

        if(pairs.get("data") == "null"){

        }
        T temp = fromJson(pairs.get("data"));

        return new Response<>(pairs.get("code"),pairs.get("message"),temp);
    }
}
