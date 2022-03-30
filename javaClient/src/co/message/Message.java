package co.message;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import co.utils;

//Serializable class
public abstract class Message {




    private String jsonSerializer(Object o) throws IllegalAccessException {
        StringBuilder str = new StringBuilder();

        //Parse primitive fields as text
        if (utils.isPrimitiveOrWrapper(o.getClass())) {
            str.append("\"").append(o).append("\"");
            return str.toString();
        }

        //Build array if object type is array
        if (o.getClass().isArray()) {
            str.append("[");

            for (int i = 0; i < Array.getLength(o); i++) {
                Object child = Array.get(o, i);

                //Serialize objects in array
                str.append(jsonSerializer(child));
                str.append(",");
            }

            str.deleteCharAt(str.length() - 1);
            str.append("]");

            return str.toString();
        }


        //Serialize object
        str.append("{");
        for (Field field : o.getClass().getFields()) {
            //Insert field name
            str.append("\"").append(field.getName()).append("\"");
            str.append(":");
            //Inert field value
            if (field.get(o).getClass().isEnum()){
                //Special case for enums
                str.append("\"").append(field.get(o).toString()).append("\"");
            }else {
                //Serialize child object
                str.append(jsonSerializer(field.get(o)));
            }
            str.append(",");
        }
        str.deleteCharAt(str.length() - 1);
        str.append("}");
        return str.toString();
    }

    ;

    public String toJson() throws IllegalAccessException {
        return jsonSerializer(this);
    }

    ;


};

