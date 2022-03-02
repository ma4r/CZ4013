package co;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

public abstract class Message {


    static final Set<Class> WRAPPER_TYPES = new HashSet<>(Arrays.asList(
            Byte.class,
            Short.class,
            Integer.class,
            Long.class,
            Float.class,
            Double.class,
            Character.class,
            Boolean.class,
            String.class));

    private boolean isPrimitiveOrWrapper(Class t) {
        return t.isPrimitive() || WRAPPER_TYPES.contains(t);
    }

    private String jsonSerializer(Object o) throws IllegalAccessException {
        StringBuilder str = new StringBuilder();

        if (isPrimitiveOrWrapper(o.getClass())) {
            str.append("\"").append(o).append("\"");
            return str.toString();
        }

        if (o.getClass().isArray()) {
            str.append("[");

            for (int i = 0; i < Array.getLength(o); i++) {
                Object child = Array.get(o, i);
                str.append(jsonSerializer(child));
                str.append(",");
            }

            str.deleteCharAt(str.length() - 1);
            str.append("]");

            return str.toString();
        }


        str.append("{");
        for (Field field : o.getClass().getFields()) {

            str.append("\"").append(field.getName()).append("\"");
            str.append(":");

            str.append(jsonSerializer(field.get(o)));
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

