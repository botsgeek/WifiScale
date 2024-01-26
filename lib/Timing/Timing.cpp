 #include "Timing.h"

Timing::Timing() {
    // Constructor implementation, if needed.
}

 std::string Timing::sendLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return "";
  }

  static char buffer[20];  // Buffer to store the formatted time
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

  std::string local_time = std::string(buffer);
  Serial.println(local_time.c_str());
  return local_time;
}

void Timing::timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  sendLocalTime();
}

