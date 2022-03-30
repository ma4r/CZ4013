package co;

import java.util.HashMap;
import java.util.Map;

//Currency enum, with toString and fromString conversion
public enum Currency  {
    USD("USD"),
    SGD("SGD"),
    RMB("RMB"),
    YEN("YEN"),
    EUR("EUR");
    private String name;
    private static final Map<String,Currency> CURRENCY_MAP;
    Currency (String name){
        this.name = name;
    }
    public String toString(){
        return this.name.toLowerCase();
    }

    static {
        Map<String,Currency> map = new HashMap<String,Currency>();
        for (Currency curr: Currency.values()){
            map.put(curr.toString(),curr);
        }
        CURRENCY_MAP = map;
    }

    public static Currency fromString(String name){
        return CURRENCY_MAP.get(name.toLowerCase());
    }

}
