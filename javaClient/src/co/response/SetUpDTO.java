package co.response;

public class SetUpDTO {
    public short session;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static SetUpDTO fromString(String s) throws Exception {
        Deserializer<SetUpDTO> resp = new Deserializer<SetUpDTO>(SetUpDTO.class);
        return resp.fromJson(s);
    }
    public String toString(){
        return String.format("Session Id: %d\n",this.session);
    }
}
