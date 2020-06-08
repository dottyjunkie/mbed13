#include "mbed.h"
#include "bbcar.h"

#include "bbcar_rpc.h"


Ticker servo_ticker;

PwmOut pin9(D9), pin8(D8);


BBCar car(pin8, pin9, servo_ticker);

Serial pc(USBTX, USBRX);
Serial xbee(D12, D11);

void reply_messange(char *xbee_reply, char *messange){
  xbee_reply[0] = xbee.getc();
  xbee_reply[1] = xbee.getc();
  xbee_reply[2] = xbee.getc();
  if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){
   pc.printf("%s\r\n", messange);
   xbee_reply[0] = '\0';
   xbee_reply[1] = '\0';
   xbee_reply[2] = '\0';
  }
}
void check_addr(char *xbee_reply, char *messenger)
{
	xbee_reply[0] = xbee.getc();
	xbee_reply[1] = xbee.getc();
	xbee_reply[2] = xbee.getc();
	xbee_reply[3] = xbee.getc();
	pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);
	xbee_reply[0] = '\0';
	xbee_reply[1] = '\0';
	xbee_reply[2] = '\0';
	xbee_reply[3] = '\0';
}
int main(){
  pc.baud(9600);

  char xbee_reply[3];

  xbee.baud(9600);
	xbee.printf("+++");
	xbee_reply[0] = xbee.getc();
	xbee_reply[1] = xbee.getc();
	if (xbee_reply[0] == 'O' && xbee_reply[1] == 'K')
	{
		pc.printf("enter AT mode.\r\n");
		xbee_reply[0] = '\0';
		xbee_reply[1] = '\0';
	}
	xbee.printf("ATMY 0x234\r\n");
	reply_messange(xbee_reply, "setting MY : 0x234");

	xbee.printf("ATDL 0x123\r\n");
	reply_messange(xbee_reply, "setting DL : 0x123");

	xbee.printf("ATID 0x17\r\n");
	reply_messange(xbee_reply, "setting PAN ID : 0x17");

	xbee.printf("ATWR\r\n");
	reply_messange(xbee_reply, "write config");

	xbee.printf("ATMY\r\n");
	check_addr(xbee_reply, "MY");

	xbee.printf("ATDL\r\n");
	check_addr(xbee_reply, "DL");

	xbee.printf("ATCN\r\n");
	reply_messange(xbee_reply, "exit AT mode");
	xbee.getc();


  // start
  pc.printf("start\r\n");

    char buf[256], outbuf[256];

    while (1) {

        for( int i=0; ;i++ ) {

            buf[i] = xbee.getc();

            if(buf[i] == '\n') break;

        }


        RPC::call(buf, outbuf);

        xbee.printf("%s\r\n", outbuf);

    }
}

