#pragma once

#include "pages/page.h"

// Saves and loads the state of all pages to/from a JSON file in flash (LittleFS).
// Format: {"currentPage": 0, "pages": [{"type": "counter", "value": 20, "icon": 1}, ...]}
namespace storage {

// Mounts the flash file system
void begin();
void save(Page *const pages[], int pageCount, int currentPage);
// Leaves the pages and currentPage untouched if there's no valid save file
void load(Page *const pages[], int pageCount, int &currentPage);

} // namespace storage
