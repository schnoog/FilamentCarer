#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN D8 //Pin on WeMos D1 Mini
#define RST_PIN D3 //Pin on WeMos D1 Mini





MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

bool bolHasCard = false;

long LastTime = 0;
long rth = 1000;

void mfrc522_fast_Reset(){
		digitalWrite(RST_PIN, HIGH);
		rfid.PCD_Reset();
		rfid.PCD_WriteRegister(rfid.TModeReg, 0x80);			// TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
		rfid.PCD_WriteRegister(rfid.TPrescalerReg, 0x43);		// 10μs.
	//	rfid.PCD_WriteRegister(rfid.TPrescalerReg, 0x20);		// test

		rfid.PCD_WriteRegister(rfid.TReloadRegH, 0x00);		// Reload timer with 0x064 = 30, ie 0.3ms before timeout.
		rfid.PCD_WriteRegister(rfid.TReloadRegL, 0x1E);
		//rfid.PCD_WriteRegister(rfid.TReloadRegL, 0x1E);

		rfid.PCD_WriteRegister(rfid.TxASKReg, 0x40);		// Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
		rfid.PCD_WriteRegister(rfid.ModeReg, 0x3D);		// Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)

		rfid.PCD_AntennaOn();						// Enable the antenna driver pins TX1 and TX2 (they were disabled by the reset)
	}

bool mfrc522_fastDetect3(){
		byte validBits = 7;
		MFRC522::StatusCode status;
		byte command = MFRC522::PICC_CMD_REQA;
		byte waitIRq = 0x30;		// RxIRq and IdleIRq
		byte n;
		uint16_t i;

		rfid.PCD_ClearRegisterBitMask(MFRC522::CollReg, 0x80);		// ValuesAfterColl=1 => Bits received after collision are cleared.

		//rfid.PCD_WriteRegister(MFRC522::CommandReg, MFRC522::PCD_Idle);			// Stop any active command.
		rfid.PCD_WriteRegister(MFRC522::ComIrqReg, 0x7F);					// Clear all seven interrupt request bits
		rfid.PCD_SetRegisterBitMask(MFRC522::FIFOLevelReg, 0x80);			// FlushBuffer = 1, FIFO initialization
		rfid.PCD_WriteRegister(MFRC522::FIFODataReg, 1, &command);			// Write sendData to the FIFO
		rfid.PCD_WriteRegister(MFRC522::BitFramingReg, validBits);			// Bit adjustments
		rfid.PCD_WriteRegister(MFRC522::CommandReg, MFRC522::PCD_Transceive);				// Execute the command
		rfid.PCD_SetRegisterBitMask(MFRC522::BitFramingReg, 0x80);			// StartSend=1, transmission of data starts

		i = 10;
		while (1) {
			n = rfid.PCD_ReadRegister(MFRC522::ComIrqReg);	// ComIrqReg[7..0] bits are: Set1 TxIRq RxIRq IdleIRq HiAlertIRq LoAlertIRq ErrIRq TimerIRq
			if (n & waitIRq) {					// One of the interrupts that signal success has been set.
				break;
			}
			if (n & 0x01) {						// Timer interrupt - nothing received in 25ms
				return false;
			}
			if (--i == 0) {						// The emergency break. If all other conditions fail we will eventually terminate on this one after 35.7ms. Communication with the MFRC522 might be down.
				return false;
			}
		}

		return true;
	}



String printHex(byte *buffer, byte bufferSize) {
  String id = "";
  for (byte i = 0; i < bufferSize; i++) {
    id += buffer[i] < 0x10 ? "0" : "";
    id += String(buffer[i], HEX);
  }
  return id;
}

void handleRFID() {
  Serial.println("try scan");
  int CPL = 10;
  int CL = 0; 
    while (!rfid.PICC_IsNewCardPresent()) {
      if ( CL < CPL){
        CL++;
        mfrc522_fast_Reset();
        delay(50);
      }else{
        return;
      }
    }
    bolHasCard = true;
    Serial.println("New card present");
  
  CL=0;
  while (!rfid.PICC_ReadCardSerial()) {
    if ( CL < CPL){
      CL++;
      delay(50);
    }else{
      bolHasCard = false;
      return;
    }
  }
    Serial.println("Serial read done");
  Serial.print("RFID ");
  Serial.println(printHex(rfid.uid.uidByte, rfid.uid.size));
        snprintf (msg, MSG_BUFFER_SIZE, "RFID-ID: %s", printHex(rfid.uid.uidByte, rfid.uid.size).c_str());
        Serial.println(msg);
        network_publish(msg);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
 
 
void rfid_setup(void){
  SPI.begin();
  rfid.PCD_Init();
  rfid.PCD_SetAntennaGain(rfid.RxGain_max);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Booting Sketch...");
  Serial.println("Up and running!");
  LastTime = millis() - rth;
}
 
void rfid_loop(void){
  long tmp  = millis();

  if(tmp - rth > LastTime){
    LastTime = tmp;
    handleRFID();
  }


 
  
  //things that should only be transmitted all 60 sec
}
 

 

 
 