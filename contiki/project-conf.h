/*---------------------------------------------------------------------------*/
#ifndef PROJECT_CONF_H_
#define PROJECT_CONF_H_
/*---------------------------------------------------------------------------*/
/* Change to match your configuration */
#define IEEE802154_CONF_PANID            0xABCD
#define RF_CHANNEL                 18
/*---------------------------------------------------------------------------*/
#define RIME_UNICAST_CHANNEL 146
#define SINK_SHORT_ADDR 105
#define SINK_NET_ADDR 0
#define SOURCE_SHORT_ADDR 112
#define SOURCE_NET_ADDR 0
#define MSG_COUNT 100
#define MSG_LEN 1
#define MSG_SLEEP CLOCK_SECOND
/*---------------------------------------------------------------------------*/
#undef NETSTACK_CONF_MAC
#define NETSTACK_CONF_MAC     nullmac_driver
/*---------------------------------------------------------------------------*/
#undef NETSTACK_CONF_RDC
#define NETSTACK_CONF_RDC     nullrdc_driver
/*---------------------------------------------------------------------------*/
#endif /* PROJECT_CONF_H_ */
/*---------------------------------------------------------------------------*/
