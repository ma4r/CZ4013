package co.response;

//Object to receive account updates when subscribed to notifications
//Contains account object, which holds new information of modified account
public class Update {

    public String action;
    public Account account;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static Update fromString(String s) throws Exception {
        Deserializer<Update> resp = new Deserializer<>(Update.class);

        return resp.fromJson(s);
    }

    public String toString(){
        return String.format(
                "Action  : %s\n" +
                "-----------Account-----------\n" +
                "%s" +
                "-----------------------------",
                this.action,this.account.toString());
    }


}
