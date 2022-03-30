package co.response;

import java.util.Date;

public class SubscribeDTO {
    public long start;
    public long end;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static SubscribeDTO fromString(String s) throws Exception {
        Deserializer<SubscribeDTO> resp = new Deserializer<SubscribeDTO>(SubscribeDTO.class);
        return resp.fromJson(s);
    }
    public String toString(){

        Date startDate = new Date(this.start*1000);
        Date endDate = new Date(this.end*1000);
        return String.format(
                "Start Time: %s\n" +
                "End Time  :%s\n",
                startDate.toString(),endDate.toString());
    }
}
