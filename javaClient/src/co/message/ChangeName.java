package co.message;

import co.Currency;

public class ChangeName extends Message {

    public String name;
    public int account;
    public String password;
    public String newName;

    public ChangeName(String name, int account, String password, String newName) {
        this.name = name;
        this.account = account;
        this.password = password;
        this.newName = newName;
    }
}
