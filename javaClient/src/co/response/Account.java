package co.response;

import co.Currency;
//Holds new information of modified account on updates
public class Account {
    public String name;
    public int account;
    public Currency currency;
    public float amount;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static Account fromString(String s) throws Exception {
        Deserializer<Account> resp = new Deserializer<>(Account.class);
        return resp.fromJson(s);
    }
    public String toString(){
        return String.format(
                "Account Name   : %s\n"+
                "Account Number : %d\n"+
                "Currency       : %s\n"+
                "Balance        : %.2f\n",
                this.name,this.account,this.currency.toString(),this.amount);
    }


}
