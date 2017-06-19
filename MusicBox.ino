
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h>


class Mp3Notify
{
public:
  static void OnError(uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }

  static void OnPlayFinished(uint16_t globalTrack)
  {
    Serial.println();
    Serial.print("Play finished for #");
    Serial.println(globalTrack);   
  }

  static void OnCardOnline(uint16_t code)
  {
    Serial.println();
    Serial.print("Card online ");
    Serial.println(code);     
  }

  static void OnCardInserted(uint16_t code)
  {
    Serial.println();
    Serial.print("Card inserted ");
    Serial.println(code); 
  }

  static void OnCardRemoved(uint16_t code)
  {
    Serial.println();
    Serial.print("Card removed ");
    Serial.println(code);  
  }
};


const byte ROWS = 4;
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {9,8, 7, 6};
uint16_t NrTracks;


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
SoftwareSerial secondarySerial(10, 11); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(secondarySerial);

void setup(){
  Serial.begin(9600);

  Serial.println("init Ica's Music Box");
  
  mp3.begin();


  mp3.setVolume(15);
  
  NrTracks = mp3.getTotalTrackCount();
  Serial.print("tracks on SD ");
  Serial.println(NrTracks);
  
  Serial.println("starting...");

  mp3.playMp3FolderTrack(1); // start with the first song
}

int InputTrack;
unsigned long int lastTime;
bool startReading = false;
void loop(){
  
  char key = keypad.getKey();
  unsigned long int currentTime = millis();
  //Serial.print(currentTime);
  //Serial.print("--");
  //Serial.println(mp3.getVolume());

  if( currentTime - lastTime > 4000 && startReading ) {

    if(InputTrack > NrTracks) mp3.playMp3FolderTrack(1);
      else mp3.playMp3FolderTrack(InputTrack);
    Serial.println("Start new song");
     InputTrack = 0 ;
     startReading = false;
  }
  if (key ) {

    
    
    Serial.println(key);
    if(key != '*' && key != '#') {
      if(startReading == false) startReading = true;
      mp3.stop();
      
      lastTime = millis();
      
      int keynumber = key - '0';
      InputTrack = (InputTrack*10) + keynumber;

      Serial.print("Current input track:");
      Serial.println(InputTrack);
      
    }
    else if( key == '#') {
      mp3.setVolume(mp3.getVolume() + 1);
    }
    else if( key == '*') {
      mp3.setVolume(mp3.getVolume() - 1);
    }
  }
  mp3.loop();
}
