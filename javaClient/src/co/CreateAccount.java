package co;

public class CreateAccount extends Message {

    public String name;
    public String password;
    public String currency;
    public long amount;

    public CreateAccount(String name, String password, String currency, long amount) {
        this.name = name;
        this.password = password;
        this.currency = currency;
        this.amount = amount;
    }
}
