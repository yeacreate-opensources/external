#ifndef __BT_TEST_H__
#define __BT_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

/******************************************/
/*        BT base server init             */
/******************************************/
void bt_test_bluetooth_init(char *data);
void bt_test_bluetooth_deinit(char *data);

void bt_test_set_class(char *data);
void bt_test_enable_reconnect(char *data);
void bt_test_disable_reconnect(char *data);
void bt_test_get_device_name(char *data);
void bt_test_get_device_addr(char *data);
void bt_test_set_device_name(char *data);
void bt_test_pair_by_addr(char *data);
void bt_test_unpair_by_addr(char *data);
void bt_test_get_paired_devices(char *data);
void bt_test_free_paired_devices(char *data);
void bt_test_start_discovery(char *data);
void bt_test_cancel_discovery(char *data);
void bt_test_is_discovering(char *data);
void bt_test_display_devices(char *data);
void bt_test_display_paired_devices(char *data);

/******************************************/
/*               BLE Test                 */
/******************************************/
void bt_test_ble_start(char *data);
void bt_test_ble_write(char *data);
void bt_test_ble_get_status(char *data);
void bt_test_ble_stop(char *data);
void bt_test_ble_setup(char *data);
void bt_test_ble_clean(char *data);
void bt_test_ble_disconnect(char *data);
void bt_test_ble_visibility00(char *data);
void bt_test_ble_visibility11(char *data);

/******************************************/
/*             A2DP SINK Test             */
/******************************************/
void bt_test_sink_open(char *data);
void bt_test_sink_visibility00(char *data);
void bt_test_sink_visibility01(char *data);
void bt_test_sink_visibility10(char *data);
void bt_test_sink_visibility11(char *data);
void bt_test_sink_music_play(char *data);
void bt_test_sink_music_pause(char *data);
void bt_test_sink_music_next(char *data);
void bt_test_sink_music_previous(char *data);
void bt_test_sink_music_stop(char *data);
void bt_test_sink_disconnect(char *data);
void bt_test_sink_close(char *data);
void bt_test_sink_status(char *data);
void bt_test_sink_set_volume(char *data);
void bt_test_sink_connect_by_addr(char *data);
void bt_test_sink_disconnect_by_addr(char *data);
void bt_test_sink_get_play_status(char *data);
void bt_test_sink_get_poschange(char *data);

/******************************************/
/*          A2DP SOURCE Test              */
/******************************************/
void bt_test_source_auto_start(char *data);
void bt_test_source_auto_stop(char *data);
void bt_test_source_connect_status(char *data);
void bt_test_source_open(char *data);
void bt_test_source_close(char *data);
void bt_test_source_connect_by_addr(char *data);
void bt_test_source_disconnect_by_addr(char *data);
void bt_test_source_remove_by_addr(char *data);

/******************************************/
/*              SPP Test                  */
/******************************************/
void bt_test_spp_open(char *data);
void bt_test_spp_write(char *data);
void bt_test_spp_close(char *data);
void bt_test_spp_status(char *data);

/******************************************/
/*              HFP Test                  */
/******************************************/
void bt_test_hfp_sink_open(char *data);
void bt_test_hfp_hp_open(char *data);
void bt_test_hfp_hp_accept(char *data);
void bt_test_hfp_hp_hungup(char *data);
void bt_test_hfp_hp_redial(char *data);
void bt_test_hfp_hp_report_battery(char *data);
void bt_test_hfp_hp_set_volume(char *data);
void bt_test_hfp_hp_close(char *data);
void bt_test_hfp_hp_disconnect(char *data);

//obex pbap
void bt_test_obex_init(char *data);
void bt_test_obex_pbap_connect(char *data);
void bt_test_obex_pbap_get_pb_vcf(char *data);
void bt_test_obex_pbap_get_ich_vcf(char *data);
void bt_test_obex_pbap_get_och_vcf(char *data);
void bt_test_obex_pbap_get_mch_vcf(char *data);
void bt_test_obex_pbap_disconnect(char *data);
void bt_test_obex_close(char *data);

#ifdef __cplusplus
}
#endif

#endif /* __BT_TEST_H__ */
