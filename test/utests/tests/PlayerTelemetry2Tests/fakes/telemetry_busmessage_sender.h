#ifndef FAKE_TELEMETRY_BUSMESSAGE_SENDER_H
#define FAKE_TELEMETRY_BUSMESSAGE_SENDER_H

typedef enum
{
    T2ERROR_SUCCESS,
    T2ERROR_FAILURE
} T2ERROR;

void t2_init(char *component);
T2ERROR t2_event_s(const char *marker, const char *value);
T2ERROR t2_event_d(const char *marker, int value);
void t2_uninit(void);

#endif // FAKE_TELEMETRY_BUSMESSAGE_SENDER_H
