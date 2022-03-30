# CZ4013
CZ 4013 Course Project
## Remote Banking Service
Develop a client and server application for banking services. 
### Server
Server is written in C++, compiled using the MSVC toolchain from VS 2017 C++ SDK.</br>
To run:<br/>
```Server.exe {port_number} {success_rec_rate} {total_rec} {success_send_rate} {total_send} {AT_LEAST_ONCE|AT_MOST_ONCE}```

### Client
Client is writen in Java, with org.jetbrain.annotations dependency.</br>
To run:<br/>
```java -jar javaClient.jar {server_ip_dot_notation} {server_port}```





