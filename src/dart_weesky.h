#include <arduino.h>
#include <SoftwareSerial.h>

// Pin 4: Vcc （5V)
// Pin 3: GND

// Using SoftSerial: （3.3V logical)
//   Pin 2: int8_t swsTX （TX on MCU)
//   Pin 1: int8_t swsRX  (RX on MCU)

class dartWeesky {
public:
    typedef byte dartData; 
    uint16_t HCHOdata = 0;
    uint16_t ZeroVolt = 0;
    uint16_t ADCVolt = 0;
private:
    const byte sig_op = 0xAA;
    const byte sig_md = 0x0A;
    const byte sig_ed1 = 0x0D;
    const byte sig_ed2 = 0x0A;
    unsigned long timeout;    
    SoftwareSerial* _dartSerial;
    static const decltype(timeout) timeoutPassive = 68;
public:
	enum dartStatus : uint8_t {
		OK = 0,
		noData,
		readError,
		frameLenMismatch,
		sumError
    };
        
    void setTimeout(const decltype(timeout) timeout);
	decltype(timeout) getTimeout(void) const;

	static const auto wakeupTime = 2500U;    // Experimentally, time to get ready after reset/wakeup

    dartWeesky(int8_t swsRX, int8_t swsTX);
	~dartWeesky();
	bool begin(void);
	void end(void);
	size_t available(void);
	dartStatus read(void);
	bool adjustZeroVolt(void);
    bool waitForData(const unsigned int maxTime, const size_t nData = 0);
};