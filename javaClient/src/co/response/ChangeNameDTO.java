package co.response;

public class ChangeNameDTO {
    public String oldName;
    public String newName;


    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static ChangeNameDTO fromString(String s) throws Exception {
        Deserializer<ChangeNameDTO> resp = new Deserializer<ChangeNameDTO>(ChangeNameDTO.class);
        return resp.fromJson(s);
    }

    public String toString(){
        return String.format(
                "Old Name: %s\n" +
                "New Name: %s\n",
                oldName,newName);
    }
}
