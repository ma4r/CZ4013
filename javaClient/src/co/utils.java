package co;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public final class utils {
    public static long parseMoney(String s) throws NumberFormatException {
        Pattern pattern = Pattern.compile("^(\\d+)(?:\\.(\\d{0,2}))?$");
        Matcher matcher = pattern.matcher(s);
        if (!matcher.find()) {
            throw new NumberFormatException("Invalid format");
        }


        long res = Long.parseLong(matcher.group(1));


        res = res * 100;

        if (matcher.group(2) != null && matcher.group(2).length() > 0) {
            int cents = Integer.parseInt(matcher.group(2));

            if (matcher.group(2).length() == 1) {
                cents = cents * 10;
            }
            res = res + cents;
        }
        if (res < 0) {
            throw new NumberFormatException("Overflow");
        }

        return res;


    }


}
