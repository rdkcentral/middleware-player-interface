#include <memory>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>

#include "MockPlayerJsonObject.h"

// ------------------ Global mock object ------------------
std::shared_ptr<MockPlayerJsonObject> g_mockPlayerJsonObject = nullptr;

// ------------------ cJSON stubs ------------------
extern "C" {
    struct cJSON; // forward declaration

    // Creation / Deletion
    cJSON* cJSON_CreateObject() { return (cJSON*)calloc(1, sizeof(cJSON)); }
    cJSON* cJSON_CreateArray() { return (cJSON*)calloc(1, sizeof(cJSON)); }
    cJSON* cJSON_CreateString(const char* str) { 
        cJSON* item = (cJSON*)calloc(1, sizeof(cJSON)); 
        if (str) item->valuestring = strdup(str); 
        return item; 
    }
    cJSON* cJSON_CreateNumber(double num) { 
        cJSON* item = (cJSON*)calloc(1, sizeof(cJSON)); 
        item->valuedouble = num; 
        item->valueint = (int)num; 
        return item; 
    }
    cJSON* cJSON_CreateBool(int b) { 
        cJSON* item = (cJSON*)calloc(1, sizeof(cJSON)); 
        item->valueint = (b != 0); 
        return item; 
    }
    void cJSON_Delete(cJSON* item) {
        if (!item) return;
        if (item->valuestring) free(item->valuestring);
        free(item);
    }
	cJSON * cJSON_GetObjectItem(const cJSON * const object, const char * const string) {
		if (!object || !string) return nullptr;
		// For simplicity, assume object has a child with matching string
		cJSON* child = object->child;
		while (child) {
			if (child->string && strcmp(child->string, string) == 0) {
				return child;
			}
			child = child->next;
		}
		return nullptr;	
	}

	void cJSON_free(void *object){
		free(object);
	}

    // Type checking
    int cJSON_IsArray(const cJSON* item) { (void)item; return 0; }
    int cJSON_IsString(const cJSON* item) { (void)item; return 0; }
    int cJSON_IsNumber(const cJSON* item) { (void)item; return 0; }
    int cJSON_IsObject(const cJSON* item) { (void)item; return 0; }

    // Printing
    char* cJSON_Print(const cJSON* item) { (void)item; char* res = (char*)malloc(16); strcpy(res, "{}"); return res; }
    char* cJSON_PrintUnformatted(const cJSON* item) { (void)item; char* res = (char*)malloc(8); strcpy(res, "{}"); return res; }

    // Get string value
    char* cJSON_GetStringValue(const cJSON* item) { 
        if (!item) return nullptr; 
        return item->valuestring; 
    }

    // Parsing stub
    cJSON* cJSON_Parse(const char* str) { (void)str; return cJSON_CreateObject(); }

	// Object / Array manipulation stubs
    cJSON_bool cJSON_AddItemToObject(cJSON* object, const char* string, cJSON* item) {
        (void)object; (void)string; (void)item;
        return 1; // success
    }

    cJSON_bool cJSON_AddItemToArray(cJSON* array, cJSON* item) {
        (void)array; (void)item;
        return 1; // success
    }
}
