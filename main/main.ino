

unsigned int FLAG; 

void setup() {
  // little delay to get voltage (~20 ms) to charge cap. !!!
  //delay(30);
  FLAG = 1;
}

//**********************************************************
//  Loop()
//
//
//***********************************************************

void loop() {

  // state 1 
  // normal operation for the main panel 
        // turn on relays 1,2,3,4,5 (double check this)
        // check voltage from ulitility
        // if voltage from ulit. is present 
            // turn off battery relay
            // and THEN turn on utility relay 
        // if not present 
            // turn off util relay 
            // and THEN turn on battery relay AND start timer 
            // if (util power is seen)
                // wait 30 seconds
                // if (util remains seen)
                      // turn battery relay OFF 
                      // THEN turn util. relay back on 
            // else if (NOT SEEN)
                // keep battery relay on
  
  // state 2 
  // something happened: 
        // turn off specific load 
        // raise alarm for THAT specific load


  // state 3 (button pressed set FLAG to '3'
  // else turn on specific load and raise alarm

}
