

class Relay {
  private: 
    bool isOn; 
    
  public: 
    Relay::Relay(bool val) 
    {
      isOn = val;
    }
    void Relay::toggleRelay(bool val)
    {
      isOn = !isOn;
    }
}
