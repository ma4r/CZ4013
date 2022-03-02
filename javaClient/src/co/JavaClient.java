package co;

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

    private static final int RETRY_NUM = 5;
    private static final int TIMEOUT = 5000;
    private String ip;
    private int port;
    private String semantics;
    public DatagramSocket socket;
    private DatagramPacket packet;
    public InetSocketAddress serverAddress;

    private short sessionId = -1;
    private short requestId = 0;


    public JavaClient(String ip, int port, String semantics) throws IOException, IllegalAccessException {
        this.ip = ip;
        this.port = port;
        this.semantics = semantics;

        this.socket = new DatagramSocket();
        this.serverAddress = new InetSocketAddress(InetAddress.getByName(ip),
                port);
        socket.setSoTimeout(TIMEOUT);
//        this.sessionId = this.setUp();


    }

    private short setUp() throws IOException, IllegalAccessException {
        DatagramPacket response = this.send(Actions.SET_UP, null);
        String sessionId_str = new String(response.getData(), StandardCharsets.US_ASCII);

        return Short.parseShort(sessionId_str);


    }

    public void closeAccount(Scanner sc) throws IOException, IllegalAccessException {
        System.out.println("Enter your name: ");
        String name = sc.nextLine();

        System.out.println("Enter your account number: ");
        String acc_str = sc.nextLine();

        int acc_num;
        try {
            acc_num = Integer.parseInt(acc_str);
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return;
        }

        System.out.println("Enter your password: ");
        String pass = sc.nextLine();

        DeleteAccount deleteAcc = new DeleteAccount(name, pass, acc_num, 21.3f);

        this.send(Actions.CLOSE_ACC, deleteAcc);


    }

    public void createAccount(Scanner sc) throws IOException, IllegalAccessException {

        System.out.println("Enter your name: ");
        String name = sc.nextLine();

        System.out.println("Enter your password: ");
        String pass = sc.nextLine();

        System.out.println("Enter your preferred currency: ");
        String curr = sc.nextLine();

        System.out.println("Enter initial account balance: ");
        String amt_str = sc.nextLine();

        long amt;
        try {
            amt = utils.parseMoney(amt_str);
        } catch (NumberFormatException e) {
            System.out.println(e.getMessage());
            return;
        }

        CreateAccount acc = new CreateAccount(name, pass, curr, amt);

        this.send(Actions.CREATE_ACC, acc);
    }

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
        System.out.println(req.length());
        System.out.println(sendBuf.length);
        DatagramPacket request = new DatagramPacket(sendBuf, sendBuf.length, this.serverAddress);

//        this.socket.send(packet);
        byte[] recBuf = new byte[512];
        DatagramPacket response = new DatagramPacket(recBuf, recBuf.length);

        int attempts = 0;
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
            System.out.println("Response received: " + new String(response.getData(), StandardCharsets.US_ASCII));
            break;
        }


        return response;


    }
}
