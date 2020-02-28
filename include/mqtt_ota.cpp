#include <StreamString.h>


#define debugging 1
#if debugging
  #define init_debug() {  }
  #define debug(msg)   { Serial.print(msg);              }
  #define debugln(msg) { Serial.println(msg);            }
#else
  #define init_debug()
  #define debug(msg)
  #define debugln(msg)
#endif


class OTAConsumer : public Stream
{
private:
  size_t length;
  bool running;
  bool completed;

public:
  bool begin(size_t size) {
    debugln("begin()");
    length    = size;
    completed = false;
    running   = Update.begin(size);
    if (!running) showError();
    return running;
  }

  size_t write(uint8_t b) {
    if (running) {
      if (--length < 0) running = false;
      else {
        Update.write(&b, 1);
        return 1;
      }
    }
    return 0;
  }

  bool end() {
    debugln("end()");
    running = false;
    if (Update.end()) {
      completed = Update.isFinished();
      if (!completed) showError();
    } else showError();
    return completed;
  }

  OTAConsumer()   { running = false; }

  int available() { return length;   }
  int      read() { return 0;        } // not used
  int      peek() { return 0;        } // not used

  bool isCompleted() { return completed;         }
  bool   isRunning() { return running;           }
  int     getError() { return Update.getError(); }
  void   showError() { 
      StreamString error; Update.printError(error); debug(error); 
  }
} cons;




void callbackOTA(const char *topic, uint8_t * data, int len){
  if (memcmp("SIZE=", data, 5) == 0) {
    long size = atol((const char *) &data[5]);
    debug("Receive size: ");
    debugln(size);

    if (cons.begin(size)) {
      client.setStream(cons);
    }
  }
  else if (cons.isRunning()) {
    if (cons.end()) {
      if (cons.isCompleted()) {
        debugln("Upload Completed. Rebooting....");
        ESP.restart();
        delay(10000);
      }
      else {
        debugln("ERROR: Upload not completed!");
      }
    }
    else {
      debugln("ERROR: Upload not complete!");
    }
  }
}