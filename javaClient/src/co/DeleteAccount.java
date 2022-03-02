package co;

public class DeleteAccount extends Message {

    public String name;
    public String password;
    public int account;
    public float l;

    public DeleteAccount(String name, String password, int account, float l) {
        this.name = name;
        this.password = password;
        this.account = account;
        this.l = l;
    }
}
