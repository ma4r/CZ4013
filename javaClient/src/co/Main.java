package co;

import java.io.IOException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws IOException, IllegalAccessException {
        JavaClient client = new JavaClient("192.168.1.111", 8888, "Once");

        System.out.println(client.serverAddress.toString());


        Scanner sc = new Scanner(System.in);
        client.createAccount(sc);


//        try{
//            System.out.println(acc.toJson());
//        }catch ( IllegalAccessException e){
//            System.out.println(e.toString());
//        }


    }
}
