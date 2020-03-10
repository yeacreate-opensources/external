#ifndef __RK_WIFI_H__
#define __RK_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define RK_WIFI_SAVED_INFO_MAX 10
#define SSID_BUF_LEN 64
#define BSSID_BUF_LEN 20

typedef enum {
	RK_WIFI_State_IDLE = 0,
	RK_WIFI_State_CONNECTING,
	RK_WIFI_State_CONNECTFAILED,
	RK_WIFI_State_CONNECTFAILED_WRONG_KEY,
	RK_WIFI_State_CONNECTED,
	RK_WIFI_State_DISCONNECTED,
	RK_WIFI_State_OPEN,
	RK_WIFI_State_OFF,
	RK_WIFI_State_SCAN_RESULTS,
} RK_WIFI_RUNNING_State_e;

typedef enum {
	NONE = 0,
	WPA,
	WEP
} RK_WIFI_CONNECTION_Encryp_e;

typedef struct {
	int id;
	char bssid[BSSID_BUF_LEN];
	char ssid[SSID_BUF_LEN];
	int freq;
	char mode[20];
	char wpa_state[20];
	char ip_address[20];
	char mac_address[20];
} RK_WIFI_INFO_Connection_s;

typedef struct {
	int id;
	char bssid[20];
	char ssid[64];
	char state[20];
} RK_WIFI_SAVED_INFO_s;

typedef struct {
	int count;
	RK_WIFI_SAVED_INFO_s save_info[RK_WIFI_SAVED_INFO_MAX];
} RK_WIFI_SAVED_INFO;

typedef int(*RK_wifi_state_callback)(RK_WIFI_RUNNING_State_e state, RK_WIFI_INFO_Connection_s *info);

int RK_wifi_register_callback(RK_wifi_state_callback cb);
int RK_wifi_running_getState(RK_WIFI_RUNNING_State_e* pState);
int RK_wifi_running_getConnectionInfo(RK_WIFI_INFO_Connection_s* pInfo);
int RK_wifi_enable_ap(const char* ssid, const char* psk, const char* ip);
int RK_wifi_disable_ap();
int RK_wifi_enable(const int enable);
int RK_wifi_scan(void);
char* RK_wifi_scan_r(void);
char* RK_wifi_scan_r_sec(const unsigned int cols);
int RK_wifi_connect(const char* ssid, const char* psk);
int RK_wifi_connect1(const char* ssid, const char* psk, const RK_WIFI_CONNECTION_Encryp_e encryp, const int hide);
int RK_wifi_disconnect_network(void);
int RK_wifi_restart_network(void);
int RK_wifi_set_hostname(const char* name);
int RK_wifi_get_hostname(char* name, int len);
int RK_wifi_get_mac(char *wifi_mac);
int RK_wifi_has_config(void);
int RK_wifi_ping(char *address);
int RK_wifi_recovery(void);
int RK_wifi_airkiss_start(char *ssid, char *password);
void RK_wifi_airkiss_stop(void);
int RK_wifi_forget_with_bssid(const char *bssid);
int RK_wifi_cancel(void);
int RK_wifi_getSavedInfo(RK_WIFI_SAVED_INFO *pSaveInfo);
int RK_wifi_connect_with_bssid(const char* bssid);

#ifdef __cplusplus
}
#endif

#endif
