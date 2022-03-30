package co.response;

import co.Currency;

public class DepWithDTO {
    public float newBalance;
    public Currency currency;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static DepWithDTO fromString(String s) throws Exception {
        Deserializer<DepWithDTO> resp = new Deserializer<DepWithDTO>(DepWithDTO.class);
        return resp.fromJson(s);
    }
    public String toString(){
        return String.format(
                "New Balance: %.2f\n" +
                "Currency   : %s\n",
                this.newBalance,this.currency.toString());
    }

}
