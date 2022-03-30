package co.message;

import co.Currency;

public class CreateAccount extends Message {

    public String name;
    public String password;
    public Currency currency;
    public float amount;

    public CreateAccount(String name, String password, Currency currency, float amount) {
        this.name = name;
        this.password = password;
        this.currency = currency;
        this.amount = amount;
    }
}
