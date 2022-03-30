package co.message;

import co.Currency;
import co.message.Message;

public class DepositWithdraw extends Message {
    public String name;
    public int account;
    public String password;
    public Currency currency;
    public float amount;

    public DepositWithdraw(String name, int account,String password, Currency currency, float amount,boolean deposit) {
        this.name = name;
        this.account = account;
        this.password = password;
        this.currency = currency;
        if (deposit){
            this.amount = amount;
        }else{
            this.amount = -amount;
        }
    }
}
