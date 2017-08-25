#include <dart_weesky.h>

void dartWeesky::setTimeout(const decltype(timeout) timeout) {
	this->_dartSerial->setTimeout(timeout);
	this->timeout = timeout;
};

decltype(dartWeesky::timeout) dartWeesky::getTimeout(void) const {
	return timeout;
};

dartWeesky::dartWeesky(int8_t swsRX, int8_t swsTX) {
	this->_dartSerial = new SoftwareSerial(swsRX, swsTX);
};

dartWeesky::~dartWeesky() {

};

bool dartWeesky::begin(void) {
	this->_dartSerial->setTimeout(dartWeesky::timeoutPassive);
	this->_dartSerial->begin(19200);
	return true;
};

void dartWeesky::end(void) {
	this->_dartSerial->end();
};

size_t dartWeesky::available(void) {
	while (this->_dartSerial->available()) {
		if (this->_dartSerial->peek() != sig_op) {
			this->_dartSerial->read();
		} else {
			break;
		}
	}
	return static_cast<size_t>(this->_dartSerial->available());
}


dartWeesky::dartStatus dartWeesky::read() {

	if (available() < 15) {
		//Serial.println("noDATA");
		return noData;
	}
	//Serial.println("Try read:");
	dartData thisFrame[15];
	if (this->_dartSerial->readBytes(thisFrame, sizeof(thisFrame)) != sizeof(thisFrame)) {
		//Serial.println("sizeof(thisFrame");
		return frameLenMismatch;
	};

	if (thisFrame[0] != dartWeesky::sig_op) {
		//Serial.println("sig_op Error");
		return readError;
	}

	if (thisFrame[4] != dartWeesky::sig_md) {
		//Serial.println("sig_md Error");
		return readError;
	}

	if (thisFrame[13] != dartWeesky::sig_ed1) {
		//Serial.println("sig_ed1 Error");
		return readError;
	}

	if (thisFrame[14] != dartWeesky::sig_ed2) {
		//Serial.println("sig_ed2 Error");
		return readError;
	}

	byte checkbit = ~ (thisFrame[1] + thisFrame[2]) + 1;
	if ( thisFrame[3] != checkbit ) {
		// Serial.print("SumError: Exp=");
		// Serial.print(thisFrame[3]);
		// Serial.print(" Act=");
		// Serial.println(checkbit);
		return sumError;
	}
	
	this->HCHOdata = thisFrame[1] * 256 + thisFrame[2];
	this->ZeroVolt = thisFrame[7] * 256 + thisFrame[8];
	this->ADCVolt = thisFrame[9] * 256 + thisFrame[10];

	return OK;
}

bool dartWeesky::waitForData(const unsigned int maxTime, const size_t nData) {
	const auto t0 = millis();
	if (nData == 0) {
		for (; (millis() - t0) < maxTime; delay(1)) {
			if (this->_dartSerial->available()) {
				return true;
			}
		}
		return this->_dartSerial->available();
	}

	for (; (millis() - t0) < maxTime; delay(1)) {
		if (available() >= nData) {
			return true;
		}
	}
	return available() >= nData;
}

bool dartWeesky::adjustZeroVolt(void) {
	return this->_dartSerial->write("AT+CLEAR\xDA\x0A");
}
