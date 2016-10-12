#include <array>
#include <DHT.h>
#include "led_manager.h"

#define GPIO_DHT  12
#define READ_WAIT_TIME  2 * 1000
#define READ_WAIT_LOOP_COUNT  4

template<size_t N>
class DhtData : public DHT
{
    public:
        explicit DhtData(uint8_t pin = GPIO_DHT)
        : DHT(pin, DHT22) {
        }

        ~DhtData() = default;

        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        bool readData() {
            // ignore first values
            readHumidity();
            readTemperature();

            for (auto &pair : values) {
                for (uint8_t i = 0; i < READ_WAIT_LOOP_COUNT; i++) {
                    LedManager.toggleData();
                    delay((READ_WAIT_TIME / READ_WAIT_LOOP_COUNT));
                }

                pair.humidity = readHumidity();
                // Read temperature as Celsius (the default)
                pair.temperature = readTemperature();
                // Check if any reads failed and exit early (to try again).
                if (!pair.isValid())
                    return false;
            }

            return true;
        }

        const String dataAsAscii() {
            float humidity = 0;
            float temperature = 0;

            for (auto &pair : values) {
                humidity += pair.humidity;
                temperature += pair.temperature;
            }
            humidity /= values.size();
            temperature /= values.size();

            String data = "HUMIDITY=";
            data += humidity;
            data += ";";
            data += "TEMPERATURE=";
            data += temperature;
            return data;
        }

    private:
        typedef struct {
            float  humidity;
            float  temperature;

            bool isValid() {
                return !(isnan(humidity) || isnan(temperature));
            }
        } ValuePair;
        std::array<ValuePair, N>  values;
};
