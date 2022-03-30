package co.message;

import co.message.Message;

public class DeleteAccount extends Message {

    public String name;
    public String password;
    public int account;

    public DeleteAccount(String name, String password, int account) {
        this.name = name;
        this.password = password;
        this.account = account;

    }
}
