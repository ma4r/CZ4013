package co.message;

import co.Currency;

public class Transfer extends Message {

    public String name;
    public String password;
    public int account;
    public Currency currency;
    public float amount;
    public int targetAcc;

    public Transfer(String name, int account,String password, Currency currency, float amount,int targetAcc) {
        this.name = name;
        this.account = account;
        this.password = password;
        this.currency = currency;
        this.amount = amount;
        this.targetAcc = targetAcc;
    }
}
