package co.response;


public class CreateAccountDTO {
    public int account;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static CreateAccountDTO fromString(String s) throws Exception {
        Deserializer<CreateAccountDTO> resp = new Deserializer<CreateAccountDTO>(CreateAccountDTO.class);
        return resp.fromJson(s);
    }
    public String toString(){
        return String.format("Account Number: %d\n",this.account);
    }

}
