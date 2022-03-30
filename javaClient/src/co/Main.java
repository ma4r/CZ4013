package co;

import co.response.CreateAccountDTO;
import co.response.Deserializer;
import co.response.Response;
import co.response.SetUpDTO;

import java.util.Scanner;

public class Main {
    public static void main(String[] args) throws Exception {

        //Check for args
        if(args.length<2){
            System.out.println("Please enter all 2 command arguments");
            System.out.println("java Client {target_ip_address_dot_notation} {port}");

            return;
        }
        Scanner sc = new Scanner(System.in);

        //Initialize client with server ip address and socket
        JavaClient client = new JavaClient(args[0], Integer.parseInt(args[1]),sc);

        System.out.println(client.serverAddress.toString());

        //Request for session id from server, for at most once semantics
        client.setUp();

        //Main loop
        while(true){
            System.out.println("Choose Actions:\n" +
                    "(1) Create Account\n" +
                    "(2) Close Account\n" +
                    "(3) Deposit/Withdraw Money\n" +
                    "(4) Monitor Updates\n" +
                    "(5) Transfer\n" +
                    "(6) Change Name\n"+
                    "(7) Quit");
            switch(sc.nextLine()){
                case "1":
                    client.createAccount();
                    break;
                case "2":
                    client.closeAccount();
                    break;
                case "3":
                    client.depositWithdraw();
                    break;
                case "4":
                    client.subscribe();
                    break;
                case "5":
                    client.transfer();
                    break;
                case "6":
                    client.changeName();
                    break;
                case "7":
                    return;
                default:
                    break;

            }

        }


//        try{
//            System.out.println(acc.toJson());
//        }catch ( IllegalAccessException e){
//            System.out.println(e.toString());
//        }


    }
}
