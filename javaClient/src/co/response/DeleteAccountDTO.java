package co.response;

public class DeleteAccountDTO {
    public int account;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static DeleteAccountDTO fromString(String s) throws Exception {
        Deserializer<DeleteAccountDTO> resp = new Deserializer<DeleteAccountDTO>(DeleteAccountDTO.class);
        return resp.fromJson(s);
    }
    public String toString(){
        return String.format("Account Number: %d\n",this.account);
    }

}
