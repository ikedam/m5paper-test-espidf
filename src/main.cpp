#include <nvs.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <cstring>
#include <string>
#include <map>

namespace {
  const char* TAG = "app";
}

extern "C" void app_main() {
  if (nvs_flash_init() != ESP_OK) {
    ESP_LOGE(TAG, "Failed to init flash.");
    return;
  }
  ESP_LOGE(TAG, "List nvs entries...");
  nvs_iterator_t iter = nvs_entry_find("nvs", NULL, NVS_TYPE_ANY);
  int records = 0;

  std::map<std::string, nvs_handle_t> handlemap;

  while (iter != NULL) {
    ++records;
    nvs_entry_info_t info;
    nvs_entry_info(iter, &info);
    ESP_LOGE("app", "%s:%s (%02x)", info.namespace_name, info.key, info.type);
    std::string namespace_name(info.namespace_name);
    nvs_handle_t h;
    if (handlemap.find(namespace_name) != handlemap.end()) {
      h = handlemap.at(namespace_name);
    } else {
      if (!nvs_open(info.namespace_name, NVS_READONLY, &h) == ESP_OK) {
        ESP_LOGE(TAG, "Failed to open namespace: %s", info.namespace_name);
        continue;
      }
      handlemap.insert(std::pair<std::string, nvs_handle_t>(namespace_name, h));
    }
    switch(info.type) {
      case NVS_TYPE_U8:
      {
        uint8_t value;
        if (nvs_get_u8(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%u", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_I8:
      {
        int8_t value;
        if (nvs_get_i8(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%d", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_U16:
      {
        uint16_t value;
        if (nvs_get_u16(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%u", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_I16:
      {
        int16_t value;
        if (nvs_get_i16(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%d", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_U32:
      {
        uint32_t value;
        if (nvs_get_u32(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%u", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_I32:
      {
        int32_t value;
        if (nvs_get_i32(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%d", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_U64:
      {
        uint64_t value;
        if (nvs_get_u64(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%llu", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_I64:
      {
        int64_t value;
        if (nvs_get_i64(h, info.key, &value) == ESP_OK) {
          ESP_LOGE(TAG, "%lld", value);
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_STR:
      {
        size_t size;
        if (nvs_get_str(h, info.key, NULL, &size) == ESP_OK) {
          char* buf = new char[size];
          std::memset(buf, 0, size);
          if (nvs_get_str(h, info.key, buf, &size) == ESP_OK) {
            ESP_LOGE(TAG, "%s", buf);
          } else {
            ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
          }
          delete[] buf;
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      case NVS_TYPE_BLOB:
      {
        size_t size;
        if (nvs_get_blob(h, info.key, NULL, &size) == ESP_OK) {
          uint8_t* buf = new uint8_t[size + 1];
          std::memset(buf, 0, size + 1);
          if (nvs_get_blob(h, info.key, buf, &size) == ESP_OK) {
            char* expanded = new char[size * 3 + 1];
            for (int i = 0; i < size; ++i) {
              sprintf(expanded + i * 3, "%02x ", buf[i]);
            }
            expanded[size * 3] = 0;
            ESP_LOGE(TAG, "%s", expanded); // as hex
            ESP_LOGE(TAG, "%s", reinterpret_cast<char*>(buf)); // as string
            delete[] expanded;
          } else {
            ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
          }
          delete[] buf;
        } else {
          ESP_LOGE(TAG, "Failed to read value: %s:%s", info.namespace_name, info.key);
        }
      }
      break;
      default:
      {
        ESP_LOGE(TAG, "Unknown type: %02x", info.type);
      }
      break;
    }
    iter = nvs_entry_next(iter);
  }

  ESP_LOGE(TAG, "Dump %d records", records);

  for (
    std::map<std::string, nvs_handle_t>::const_iterator it = handlemap.begin();
    it != handlemap.end();
    ++it
  ) {
    nvs_close(it->second);
  }
}
