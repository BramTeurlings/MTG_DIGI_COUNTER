#include "storage.h"
#include <LittleFS.h>

namespace storage {

namespace {
const char *filename = "/data.json";
}

void begin() {
  LittleFS.begin();
}

void save(Page *const pages[], int pageCount, int currentPage) {
  JsonDocument doc;
  doc["currentPage"] = currentPage;

  JsonArray pageStates = doc["pages"].to<JsonArray>();
  for (int i = 0; i < pageCount; i++) {
    JsonObject state = pageStates.add<JsonObject>();
    state["type"] = pages[i]->type();
    pages[i]->save(state);
  }

  File file = LittleFS.open(filename, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  serializeJson(doc, file);
  file.close();
  Serial.println("State saved");
}

void load(Page *const pages[], int pageCount, int &currentPage) {
  if (!LittleFS.exists(filename)) {
    Serial.println("No save file found, starting fresh.");
    return;
  }

  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse JSON, starting fresh.");
    return;
  }

  currentPage = constrain(doc["currentPage"] | 0, 0, pageCount - 1);

  JsonArrayConst pageStates = doc["pages"];
  int i = 0;
  for (JsonObjectConst state : pageStates) {
    // Skip states that belong to a different kind of page, e.g. after the page order changed
    if (i < pageCount && strcmp(state["type"] | "", pages[i]->type()) == 0) {
      pages[i]->load(state);
    }
    i++;
  }
}

} // namespace storage
