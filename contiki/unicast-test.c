/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Best-effort single-hop unicast example
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime/rime.h"

#include "dev/button-sensor.h"

#include "dev/leds.h"
#include "dev/watchdog.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(example_unicast_process, "Example unicast");
AUTOSTART_PROCESSES(&example_unicast_process);
/*---------------------------------------------------------------------------*/
static struct ctimer ct;
static void ctback(void *ptr){
  P6OUT &= ~(BIT2);
}
/*---------------------------------------------------------------------------*/
static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
  P6OUT |= (BIT2);
  ctimer_set(&ct, CLOCK_SECOND>4, ctback, NULL);
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

static char msg[PACKETBUF_SIZE]={0};
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_unicast_process, ev, data)
{
  PROCESS_EXITHANDLER(unicast_close(&uc);)
    
  static linkaddr_t source_addr;
  static linkaddr_t sink_addr;
  static struct etimer et;
  static uint16_t sleep=MSG_SLEEP;
  static uint16_t counter=0;
  PROCESS_BEGIN();

  //ADC2
  P6SEL &= ~(BIT2);
  P6DIR |= BIT2;
  P6OUT &= ~(BIT2);

  sink_addr.u8[0] = SINK_SHORT_ADDR;
  sink_addr.u8[1] = SINK_NET_ADDR;

  source_addr.u8[0] = SOURCE_SHORT_ADDR;
  source_addr.u8[1] = SOURCE_NET_ADDR;

  unicast_open(&uc, RIME_UNICAST_CHANNEL, &unicast_callbacks);

  printf("Running experiment: sink=%d.%d, source=%d.%d, sleep=%d, count=%d\n",
		  SINK_SHORT_ADDR, SINK_NET_ADDR, SOURCE_SHORT_ADDR, SOURCE_NET_ADDR, MSG_COUNT, MSG_COUNT);

  printf("checking if i am the sink\n");
  if(linkaddr_cmp(&source_addr, &linkaddr_node_addr)) {
    printf("i am a source, gotta sleep now\n");
    etimer_set(&et, 5*CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    printf("source will start now\n");
    while(1){
      etimer_set(&et, sleep);
      PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  
      if(counter<=MSG_COUNT) {
        packetbuf_copyfrom(msg, MSG_LEN);
        unicast_send(&uc, &sink_addr);
        P6OUT |= (BIT2);
	ctimer_set(&ct, CLOCK_SECOND>4, ctback, NULL);
      } else {
      sleep=CLOCK_SECOND*10;
      }
    }
  }
  else if(linkaddr_cmp(&sink_addr, &linkaddr_node_addr)){
    printf("i am the sink!\n");
    while(1) {
        PROCESS_WAIT_EVENT();
    }
  }
  printf("bystander\n");

  while(1) {
    PROCESS_WAIT_EVENT(); 
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
