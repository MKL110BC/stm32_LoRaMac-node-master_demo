#ifndef __LORAWAN_COMMON_H__
#define __LORAWAN_COMMON_H__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "LoRaMac.h"
#include "LoRaMacTest.h"
#include "rtc-board.h"
#include "sx126x-board.h"
#include "board-config.h"


enum
{
	LORAWAN_UNCONFIRM_MSG	=	0,
	LORAWAN_CONFIRM_MSG		=	1,
};

enum
{
	LORAWAN_ADR_OFF		=	0,
	LORAWAN_ADR_ON		=	1,
};



#define LORAWAN_CERTIF_PORT			224
#define LORAWAN_CERTIF_DELAY		5000
#define LORAWAN_APP_DATA_MAX_SIZE	242
#define LORAWAN_MAX_APP_DATA_LEN		242


#define LORAWAN_MIN_APP_PORT			1
#define LORAWAN_MAX_APP_PORT			223

#define LORAWAN_MIN_CONFIRM_NBTRIALS	1
#define LORAWAN_MAX_CONFIRM_NBTRIALS	8



#define LORA_DEVEUI_LEN				8
#define LORA_APPEUI_LEN				8
#define LORA_APPKEY_LEN				16
#define	LORA_APPSKEY_LEN			16
#define	LORA_NWKSKEY_LEN			16
#define LORA_MC_APPSKEY_LEN			16
#define	LORA_MC_NWKSKEY_LEN			16



//Э��ջ���մ�����ָ��
//bufΪ�����������ָ�룬buflenΪ���ݳ��ȣ�RSSIΪ���������ź�ǿ�ȣ�SNRΪ����ȣ�portΪ�������ݶ˿�
typedef  void(*LORAWAN_RECV_DEAL)(uint8_t* /*buf*/,uint8_t  /*buflen*/,int16_t /*rssi*/,int8_t  /*snr*/,uint8_t  /*port*/);

#pragma pack(1)


struct  lorawan_param
{
	LoRaMacRegion_t		region;
	DeviceClass_t   	class_type;
	ActivationType_t 	connect_type;

	//otaa param
	uint8_t  	deveui[LORA_DEVEUI_LEN];
	uint8_t  	appeui[LORA_APPEUI_LEN];
	uint8_t  	appkey[LORA_APPKEY_LEN];
	
	//ABP param
	uint32_t 	devaddr;
	uint8_t		appskey[LORA_APPSKEY_LEN];
	uint8_t 	nwkskey[LORA_NWKSKEY_LEN];
	
	//Ƶ����ز���
	uint32_t 	join_delay1;
	uint32_t 	join_delay2;
	uint32_t	rx1_delay;   //rx2delay=rx1delay+1000ms
	uint16_t 	channels_mask[6];
	int8_t		tx_power;
	uint8_t 	dr;  //join dr 
	uint32_t 	rx2_freq;

	float		antenna_gain;  //��������
};

#pragma pack()
extern const char* MacStatusStrings[24];
extern const char* EventInfoStatusStrings[17] ;



void lorawan_init(LoRaMacRegion_t	region);
LoRaMacStatus_t lorawan_deinit(void);
LoRaMacStatus_t lorawan_join_network(uint8_t dr);

/*
FUNC:ע��lorawan���մ�����
PARAM:recvָ��һ������ָ��	
*/
void lora_app_register_lorawan_recv_func(LORAWAN_RECV_DEAL recv);


/*
FUNC:	lorawan ����Ӧ������֡ 
PARAM:	
		buf �������ݵ�ָ��
		buflen�������ݳ���
		port �������ݶ˿ڣ�Ӧ�����ݶ˿�Ϊ1-223
		is_confirm	�Ƿ���ȷ��֡
		dr		��ADR��������ݷ��͵�DR
		confirm_nbtrials    ȷ��֡�ط�����

RETURN:	
		LORAMAC_STATUS_OK���ͳɹ�
		LORAMAC_STATUS_DUTYCYCLE_RESTRICTED ��������dutycycle������lora_app_get_dutycycle_remain��ѯ��Ҫ�ȴ���ʱ��
		LORAMAC_STATUS_LENGTH_ERROR  ���ͳ��������ƣ�����lora_app_get_tx_possible_len��ѯ�����͵ĳ���
*/
LoRaMacStatus_t lora_app_send_frame( uint8_t *buf,uint8_t buflen,uint8_t port,uint8_t is_confirm,uint8_t dr ,uint8_t confirm_nbtrials);



void lorawan_task(void);
LoRaMacStatus_t  lorawan_send_linkcheck(uint8_t dr);
uint8_t  lorawan_get_linkcheck_result(uint8_t *margin,uint8_t *gw_cnt);
LoRaMacStatus_t  lorawan_send_devtime(uint8_t dr);
uint8_t lorawan_get_devtime_result(void);
void lorawan_get_sys_time(uint8_t *time);
uint8_t lora_mac_set_channel( uint16_t *channel_mask);
uint8_t lora_mac_set_dr(uint8_t dr);
uint8_t lora_mac_get_dr(void);
uint8_t lora_mac_set_deveui( uint8_t *deveui);
uint8_t lora_mac_set_appeui( uint8_t *appeui);
uint8_t lora_mac_set_appkey( uint8_t *appkey);
uint8_t lora_mac_set_devaddr( uint32_t devaddr);
uint32_t lora_mac_get_devaddr(void);
uint8_t lora_mac_set_appskey( uint8_t *appskey);
uint8_t lora_mac_set_nwkskey( uint8_t *nwkskey);


/*
FUNC:�����������մ���1�ӳ�ʱ��//��lora_mac_set_join_delay����һ����Ϊ�˼��ݲ�ɾ��
PARAM:��λms
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_join_delay1(uint32_t join_delay1);

/*
FUNC:�����������մ���2�ӳ�ʱ��//��lora_mac_set_join_delay����һ����Ϊ�˼��ݲ�ɾ��
PARAM:��λms
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_join_delay2(uint32_t join_delay2);


/*
FUNC:��ȡ�������մ����ӳ�
PARAM:ָ��delay1 delay2��ָ��
*/
void lora_mac_get_join_delay(uint32_t *join_delay1,uint32_t *join_delay2);


/*
FUNC:����Ӧ�ý��մ���1�ӳ�ʱ��    ��RX2=RX1+1000
PARAM:��λms
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_rx1_delay(uint32_t rx1_delay);


/*
FUNC:��ȡ��ǰЭ��ջRX1delay��RX2=RX1+1000MS
RETURN:
*/
uint32_t lora_mac_get_rx1_delay(void);


/*
FUNC:����RX2���յ�Ƶ���Լ�dr
PARAM:
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_rx2_channel(uint32_t freq,uint8_t dr);

/*
FUNC:���÷��书��
PARAM:��ǰƵ���������õķ�Χ,��ͬƵ������ķ�Χ��һ��
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_tx_power(int8_t tx_power);

/*
FUNC:��ȡ��ǰЭ��ջ��class����
RETURN:��ǰ��class����
*/
DeviceClass_t lora_mac_get_class_mode(void);

/*
FUNC:����class����, 
PARAM: CLASS_A/CLASS_C
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_class_mode(DeviceClass_t class_mode);


/*
FUNC:����rx�����ݴ�ʱ��
PARAM: default is10ms
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_rx_error_time(uint32_t error_time);


/*
FUNC:�����������ͣ���Ҫ��������ABPģʽ
PARAM: 
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_net_type(ActivationType_t type);


/*
FUNC:��ȡ��ǰ�������ͣ�����ͨ���˺����ж������ɹ����
RETURN:ACTIVATION_TYPE_NONE��ʾOTAAģʽδ������ACTIVATION_TYPE_OTAA��ʾOTAAģʽ�����ɹ�
*/
ActivationType_t lora_mac_get_net_type(void);

/*
FUNC:������������
PARAM: ���õ���������ֵ
RETURN:0����ʧ�ܣ�1���óɹ�
*/
uint8_t lora_mac_set_antenna_gain(float antenna_gain);

void set_region_default(struct  lorawan_param *param);
#endif
