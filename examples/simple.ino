#include <Arduino.h>

#include <dart_weesky.h>

dartWeesky _dartWeesky(D5, D6);

////////////////////////////////////////

void setup(void) {
	Serial.begin(115200);
	while (!Serial) {};
	Serial.println("dartWeesky");

	_dartWeesky.begin();
	_dartWeesky.waitForData(dartWeesky::wakeupTime);
}

////////////////////////////////////////

auto lastRead = millis();

void loop(void) {

	dartWeesky::dartStatus status = _dartWeesky.read();

	switch (status) {
		case dartWeesky::OK:
		{
			Serial.println("_________________");
			auto newRead = millis();
			Serial.print("Wait time ");
			Serial.println(newRead - lastRead);
			lastRead = newRead;
			float multival = 0.001;
			float actHCHOdata = _dartWeesky.readHCHOdata() * multival;
				Serial.print(actHCHOdata,3);
				Serial.print("\t");
				Serial.print("HCHO");
				Serial.print(" [");
				Serial.print("mg/m3");
				Serial.print("]");
				Serial.println();
				Serial.print(_dartWeesky.readZeroVolt());
				Serial.print("\t");
				Serial.print("ZeroVolt");
				Serial.print(" [");
				Serial.print("mV");
				Serial.print("]");
				Serial.println();
				Serial.print(_dartWeesky.readADCVolt());
				Serial.print("\t");
				Serial.print("ADCVolt");
				Serial.print(" [");
				Serial.print("mV");
				Serial.print("]");
				Serial.println();
			if (_dartWeesky.ZeroVolt>_dartWeesky.ADCVolt) {
				_dartWeesky.adjustZeroVolt();
			}
		}
		case dartWeesky::noData:
			break;
		default:
			Serial.println("_________________");
			Serial.println(status);
	};
}
