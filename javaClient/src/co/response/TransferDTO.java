package co.response;

import co.Currency;

public class TransferDTO {
    public int targetAcc;
    public String targetName;
    public Currency currency;
    public float amount;
    public float balanceLeft;

    //Recursive call to fromJson to construct object, all class with this method are "Deserializeable"
    public static TransferDTO fromString(String s) throws Exception {
        Deserializer<TransferDTO> resp = new Deserializer<TransferDTO>(TransferDTO.class);
        return resp.fromJson(s);
    }
    public String toString(){
        return String.format(
                "Target Account : %d\n"+
                "Target Name    : %s\n"+
                "Currency       : %s\n"+
                "Amount         : %.2f\n"+
                "New Balance    : %.2f\n" ,
                this.targetAcc,this.targetName,this.currency.toString(),this.amount,this.balanceLeft);
    }

}
