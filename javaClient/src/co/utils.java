package co;

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class utils {

    public static float parseMoney(String s) throws Exception {
        float nm = Float.parseFloat(s);

        if(nm<0){
            throw new Exception("Invalid input");
        }
        return nm;


    }

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

    public static boolean isPrimitiveOrWrapper(Class t) {
        return t.isPrimitive() || WRAPPER_TYPES.contains(t);
    }

}


