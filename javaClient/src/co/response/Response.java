package co.response;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;


//Response object from server, can contain various DTO types, which is the generic parameter
public class Response<T> {

    public String code;
    public String message;
    public T data;

    public Response (String code, String message,T dat) throws Exception {
        this.code = code;
        this.message = message;
        this.data = dat;

    }

    public String toString(){

        if(data!=null) {
            return String.format(
                    "STATUS  : %s\n" +
                            "Message : %s\n" +
                            "-----------Data-----------\n" +
                            "%s" +
                            "--------------------------",
                    this.code, this.message, this.data.toString());
        }else{
            return String.format(
                    "STATUS  : %s\n" +
                            "Message : %s\n" +
                            "-----------Data-----------\n" +
                            "--------------------------",
                    this.code,this.message);
        }
    }

}
