package co;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class mockServer {
    public static void main(String[] args) throws IOException {

        DatagramSocket serverSock = new DatagramSocket(3333);
        byte[] recBuf = new byte[1024];
        DatagramPacket rec = new DatagramPacket(recBuf, recBuf.length);
        System.out.println("SERVER WAITING FOR REC");
        serverSock.receive(rec);
        String res = new String(recBuf, StandardCharsets.US_ASCII);
        System.out.println("SERVER RECEIVE:" + res);

        byte[] sendBuf = "SERVER RESPONSE1".getBytes(StandardCharsets.US_ASCII);
        SocketAddress clientAddr = rec.getSocketAddress();
        Scanner sc = new Scanner(System.in);
        sc.nextLine();
        DatagramPacket resp = new DatagramPacket(sendBuf, sendBuf.length, clientAddr);
        serverSock.send(resp);

        serverSock.receive(rec);
        res = new String(recBuf, StandardCharsets.US_ASCII);
        System.out.println("SERVER RECEIVE:" + res);
        System.out.println();

        sendBuf = "SERVER RESPONSE2".getBytes(StandardCharsets.US_ASCII);
        clientAddr = rec.getSocketAddress();
        resp = new DatagramPacket(sendBuf, sendBuf.length, clientAddr);
        serverSock.send(resp);
    }
}
