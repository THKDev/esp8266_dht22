class WifiData
{
    public:
        /** WiFi SSID */
        static constexpr const char* SSID() {
            return "";
        }
        /** WiFi password */
        static constexpr const char* WIFI_PWD() {
            return "";
        }
        /** hostname of this IoT ESP8266 device */
        static constexpr const char* HOSTNAME() {
            return "";
        }
        /** name of openhab server */
        static constexpr const char* SERVERNAME() {
            return "";
        }
        /** port of openhab server */
        static constexpr const uint16_t SERVERPORT() {
            return 0;
        }
};
