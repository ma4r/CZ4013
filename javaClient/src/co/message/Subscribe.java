package co.message;

import co.message.Message;

public class Subscribe extends Message {

    public long interval;

    public Subscribe(long interval) {
        this.interval = interval;

    }
}
