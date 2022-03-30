package co;

import co.message.*;
import co.response.*;
import co.response.Response;
import org.jetbrains.annotations.Nullable;

import java.io.IOException;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class JavaClient {
    enum Actions {
        CREATE_ACC,
        CLOSE_ACC,
        DEP_WITH,
        SUBSCRIBE,
        TRANSFER,
        CHANGE_NAME,
        SET_UP
    }

    private static final int RETRY_NUM = 10;
    private static final int TIMEOUT = 1000;
    private String ip;
    private int port;
    public DatagramSocket socket;
    public InetSocketAddress serverAddress;

    private short sessionId = -1;
    private short requestId = 0;

    private Scanner sc;

    //Utility function, read String from byte array/Datagram packets
    private String readResponse(DatagramPacket packet){

        byte temp[]= packet.getData();
        int len = 0;
        for( int i=0;i<temp.length;i++){
            if (temp[i] == '\0'){
                len = i+1;
            }
        }

        return new String(temp,0,len-1);

    }

    //Client info setup and socket binding
    public JavaClient(String ip, int port ,Scanner sc) throws IOException {
        this.sc = sc;

        this.ip = ip;
        this.port = port;

        this.socket = new DatagramSocket();
        this.serverAddress = new InetSocketAddress(InetAddress.getByName(ip),
                port);
        this.socket.setSoTimeout(TIMEOUT);
//        this.sessionId = this.setUp();


    }

    //Set Up request called at client start, to obtain session ID from server
    //Used to identify requests made by this client
    public void setUp() throws Exception {


        DatagramPacket response ;

        try {
            response = this.send(Actions.SET_UP, null);

        }catch (Exception e){
            System.out.println("Failed to send setup msg");
            return;
        }

        String json = readResponse(response);
        SetUpDTO c = new SetUpDTO();

        Deserializer<SetUpDTO> des = new Deserializer<SetUpDTO>((Class<SetUpDTO>) c.getClass());
        Response<SetUpDTO> resp = des.parseResponse(json);

        this.sessionId =  resp.data.session;
        System.out.println(resp.toString());

    }



    public void closeAccount() throws Exception {
        //Get input
        String name = getStr("Enter your name: ");

        int acc_num;
        try {
            acc_num = getInt("Enter your account number:");
        } catch (Exception e) {
            System.out.println("Please enter an integer");
            return;
        }

        String pass = getStr("Enter your password: ");
        //Input to serializable class and serialize
        DeleteAccount deleteAcc = new DeleteAccount(name, pass, acc_num);

        //Send and receive response
        DatagramPacket response ;
        try {
            response = this.send(Actions.CLOSE_ACC, deleteAcc);
            System.out.println(response.toString());

        }catch (Exception e){
            System.out.println("Failed to send close account message");
            return;
        }

        //Deserialize and instantiate Response object containing DeleteAccountDTO
        String json = readResponse(response);
        DeleteAccountDTO c = new DeleteAccountDTO();

        Deserializer<DeleteAccountDTO> des = new Deserializer<DeleteAccountDTO>((Class<DeleteAccountDTO>) c.getClass());
        Response<DeleteAccountDTO> resp = des.parseResponse(json);
        System.out.println(resp.toString());


    }

    public void createAccount() throws Exception {

        //Get input
        String name = getStr("Enter your name: ");



        String pass = getStr("Enter your password: ");

        Currency curr;
        try{
            curr = getCurr("Enter your preferred currency: ");
        }catch (Exception e) {
            System.out.println("Please enter a currency: USD|RMB|YEN|SGD|EUR");
            return;
        }

        float amt;
        try {
            amt = getMoney("Enter initial account balance: ");
        } catch (NumberFormatException e) {
            System.out.println("Please enter a positive float");
            return;
        }
        //Input to serializable class and serialize
        CreateAccount acc = new CreateAccount(name, pass, curr, amt);

        //Send and receive response
        DatagramPacket response ;

        try {
            response = this.send(Actions.CREATE_ACC, acc);
        }catch (Exception e){
            System.out.println("Failed to send create account");
            return;
        }
        String json = readResponse(response);
        CreateAccountDTO c = new CreateAccountDTO();

        //Deserialize and instantiate Response object containing CreateAccountDTO
        Deserializer<CreateAccountDTO> des = new Deserializer<CreateAccountDTO>((Class<CreateAccountDTO>) c.getClass());
        Response<CreateAccountDTO> resp = des.parseResponse(json);
        System.out.println(resp.toString());

    }

    public void depositWithdraw() throws Exception {

        //Get user input
        String choice = getStr("Please select: (1) Deposit (2) Withdraw");
        boolean deposit;
        if (choice.equals("1")){deposit = true;}
        else if (choice.equals("2")){deposit = false;}
        else {
            System.out.println("Please enter \"1\" for Deposit or \"2\" for Withdrawal\n");
            return;
        }


        String name = getStr("Enter your name: ");

        int acc_num;
        try {
            acc_num = getInt("Enter your account number: ");
        } catch (Exception e) {
            System.out.println("Please enter an integer");
            return;
        }

        String pass = getStr("Enter your password: ");

        Currency curr;
        try {
            curr = getCurr("Enter currency type: ");
        }catch (Exception e){
            System.out.println("Please enter a currency: USD|RMB|YEN|SGD|EUR");;
            return;
        }

        float amt;
        try{
            amt = getMoney("Enter amount:");
        }catch (Exception e){
            System.out.println("Please enter a positive float");
            return;
        }


        //Input to serializable class and serialize
        DepositWithdraw depWith = new DepositWithdraw(name,acc_num,pass,curr,amt,deposit);

        //Send and receive response
        DatagramPacket response;
        try {
            response = this.send(Actions.DEP_WITH,depWith);
        }catch (Exception e){
            System.out.println("Failed to send deposit/withdrawal");
            return;
        }

        //Deserialize and instantiate Response object containing DepWithDTO
        String json = readResponse(response);
        DepWithDTO c = new DepWithDTO();

        Deserializer<DepWithDTO> des = new Deserializer<DepWithDTO>((Class<DepWithDTO>) c.getClass());
        Response<DepWithDTO> resp = des.parseResponse(json);
        System.out.println(resp.toString());

    }

    public void subscribe() throws Exception {
        //Get input
        long interval;
        try{
            interval=getLong("Enter monitor interval (seconds):");
        }catch (Exception e){
            System.out.println("Please enter a positive integer");
            return;
        }

        //Input to serializable class and serialize
        Subscribe subRequest = new Subscribe(interval);

        //Send and receive response
        DatagramPacket response;
        try {
            response = this.send(Actions.SUBSCRIBE,subRequest);
        }catch (Exception e){
            System.out.println("Failed to send subscribe");
            return;
        }

        //Deserialize and instantiate Response object containing SubscribeDTO
        String json = readResponse(response);
        SubscribeDTO c = new SubscribeDTO();

        Deserializer<SubscribeDTO> des = new Deserializer<SubscribeDTO>((Class<SubscribeDTO>) c.getClass());
        Response<SubscribeDTO> resp = des.parseResponse(json);
        System.out.println(resp.toString());

        //Receive subscribe end time from server
        //Date time is in unix epoch seconds, convert to milliseconds
        long end = resp.data.end*1000;

        //Set up receiver buffer
        byte[] recBuf = new byte[512];
        DatagramPacket update = new DatagramPacket(recBuf, recBuf.length);

        //Listen until end time
        this.socket.setSoTimeout((int) ((interval+1)*1000));
        while (System.currentTimeMillis()<end){
            System.out.println("Listening for updates");

            //Listen from server
            try {
                this.socket.receive(update);
            }catch(SocketTimeoutException e){
                break;
            }

            //Deserialize and instantiate Update object
            String jsonUpdate = readResponse(update);
            Update cUpdate = new Update();

            Deserializer<Update> desUpdate = new Deserializer<Update>((Class<Update>) cUpdate.getClass());
            Update up = desUpdate.fromJson(jsonUpdate);

            //Log update
            System.out.println(up.toString());
            Thread.sleep(200);
        }
        System.out.println("Done");

        this.socket.setSoTimeout(TIMEOUT);
    }

    public void transfer() throws Exception {
        //Get input
        String name = getStr("Enter your name: ");

        int acc_num;
        try {
            acc_num = getInt("Enter your account number: ");
        } catch (Exception e) {
            System.out.println("Please enter an integer");
            return;
        }

        String pass = getStr("Enter your password: ");

        Currency curr;
        try {
            curr = getCurr("Enter currency type: ");
        }catch (Exception e){
            System.out.println("Please enter a currency: USD|RMB|YEN|SGD|EUR");;
            return;
        }

        float amt;
        try{
            amt = getMoney("Enter amount:");
        }catch (Exception e){
            System.out.println("Please enter a positive float");
            return;
        }

        int targetAcc;
        try{
            targetAcc = getInt("Enter target account number:");
        }catch (Exception e){
            System.out.println("Please enter an integer");
            return;
        }

        //Input to serializable class and serialize
        Transfer trf = new Transfer(name,acc_num,pass,curr,amt,targetAcc);

        //Send and receive response
        DatagramPacket response;
        try {
            response = this.send(Actions.TRANSFER, trf);
        }catch (Exception e){
            System.out.println("Failed to send transfer");
            return;
        }

        //Deserialize and instantiate Response object containing TransferDTO
        String json = readResponse(response);
        TransferDTO c = new TransferDTO();

        Deserializer<TransferDTO> des = new Deserializer<TransferDTO>((Class<TransferDTO>) c.getClass());
        Response<TransferDTO> resp = des.parseResponse(json);
        System.out.println(resp.toString());
    }

    public void changeName() throws Exception {

        //Get input
        String name = getStr("Enter your name: ");

        int acc_num;
        try {
            acc_num = getInt("Enter your account number:");
        } catch (Exception e) {
            System.out.println("Please enter an integer");
            return;
        }

        String pass = getStr("Enter your password: ");
        String newName = getStr("Enter your new name:");

        //Input to serializable class and serialize
        ChangeName chName = new ChangeName(name,acc_num,pass,newName);

        //Send and receive response
        DatagramPacket response;
        try {
            response = this.send(Actions.CHANGE_NAME, chName);
        }catch (Exception e){
            System.out.println("Failed to send change name");
            return;
        }

        //Deserialize and instantiate Response object containing ChangeNameDTO
        String json = readResponse(response);
        ChangeNameDTO c = new ChangeNameDTO();

        Deserializer<ChangeNameDTO> des = new Deserializer<ChangeNameDTO>((Class<ChangeNameDTO>) c.getClass());
        Response<ChangeNameDTO> resp = des.parseResponse(json);
        System.out.println(resp.toString());
    }

    //Wrapper for sending "Message" objects through socket
    //Adds header info such as session id, request id, and requested action type
    //Retries on timeout RETRY_NUM times
    public DatagramPacket send(Actions action, @Nullable Message data) throws IllegalAccessException, IOException {
        this.requestId = (short) ((this.requestId + 1) % 1024);
        String req = "{" +
                "\"session\":\"" + this.sessionId + "\"," +
                "\"id\":\"" + this.requestId + "\"," +
                "\"action\":\"" + action.name() + "\"," +
                "\"data\":";
        if (data == null) {
            req += "{}";
        } else {
            req += data.toJson();
        }
        req += "}";


        byte[] sendBuf = req.getBytes(StandardCharsets.US_ASCII);
        DatagramPacket request = new DatagramPacket(sendBuf, sendBuf.length, this.serverAddress);

//        this.socket.send(packet);
        byte[] recBuf = new byte[512];
        DatagramPacket response = new DatagramPacket(recBuf, recBuf.length);

        //Retry on timeout
        for (int i = 0; i < RETRY_NUM; i++) {

            System.out.println("Sending packet:" + new String(sendBuf, StandardCharsets.US_ASCII));
            this.socket.send(request);

            try {

                this.socket.receive(response);
            } catch (SocketTimeoutException e) {
                System.out.println("Response timeout, retrying...");
                if (i == RETRY_NUM - 1) {
                    throw new SocketTimeoutException("Maximum connection retries exceeded");
                }
                continue;
            }


            System.out.println("Response received: " + readResponse(response));
            break;
        }


        return response;


    }

    private String getStr(String prompt){

        System.out.println(prompt);
        return this.sc.nextLine();
    }

    private float getMoney(String prompt) throws Exception {

        System.out.println(prompt);
        return utils.parseMoney(this.sc.nextLine());
    }

    private int getInt(String prompt){
        System.out.println(prompt);
        return Integer.parseInt(this.sc.nextLine());
    }

    private long getLong(String prompt) throws Exception {
        System.out.println(prompt);

        long tmp = Long.parseLong(this.sc.nextLine());
        if(tmp<0){
            throw new Exception("Interval needs to be bigger than 0");
        }
        return tmp;
    }

    private Currency getCurr(String prompt){
        System.out.println(prompt);
        return Currency.fromString(this.sc.nextLine());
    }
}
