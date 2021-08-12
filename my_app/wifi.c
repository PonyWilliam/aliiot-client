#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_connect.h"
#include <memory.h>
#include "MQTTClient.h"
#include <signal.h>
#include <sys/time.h>
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "base64.h"

#define EXAMPLE_PRODUCT_KEY			"a1rdpdDHgQI"
#define EXAMPLE_DEVICE_NAME			"window1"
#define EXAMPLE_DEVICE_SECRET       "d40b94c9bdfd43fa5eea14ff39062c5c"
extern int aiotMqttSign(const char *productKey, const char *deviceName, const char *deviceSecret, 
                     	char clientId[150], char username[65], char password[65]);

volatile int toStop = 0;
int msgid = 0;
void cfinish(int sig)
{
    sig = sig;
	signal(SIGINT, NULL);
	toStop = 1;
}

void messageArrived(MessageData* md)
{
	MQTTMessage* message = md->message;
    char *res1 = "open";//正转
    char *res2 = "close";//关闭电机
	char *res3 = "reverse";//反转
	printf("%.*s\t", md->topicName->lenstring.len, md->topicName->lenstring.data);
	printf("%.*s\n", (int)message->payloadlen, (char*)message->payload);
	printf("base64:%s\n",my_encode((char*)message->payload));
    if(memcmp(message->payload,res1,(int)message->payloadlen) == 0){
        printf("open\n");
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_2, 1);//打开小灯
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_7, 1);//打开电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_8, 0);//打开电机，并且正转


		MQTTMessage msg = {
			QOS1, 
			0,
			0,
			0,
			"open",
			strlen("open"),
		};
        msg.id = ++msgid;
		int rc = MQTTPublish(&c, pubTopic, &msg);
		printf("MQTTPublish %d, msgid %d\n", rc, msgid);

    }else if(memcmp(message->payload,res2,(int)message->payloadlen) == 0){
        printf("close\n");
        //关发电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_2, 0);
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_7, 0);
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_8, 0);
    }else if(memcmp(message->payload,res3,(int)message->payloadlen) == 0){
		printf("reverse\n");
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_2, 1);//打开小灯
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_7, 0);//打开电机
        GpioSetOutputVal(WIFI_IOT_GPIO_IDX_8, 1);//打开电机，并且反转
	}
}

/* main function */
static void MQTT_DemoTask(void)
{
    GpioInit();
	//说明:GPIO2控制小灯，GPIO7正转，GPIO8反转
    //设置GPIO_2的复用功能为普通GPIO
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_2, WIFI_IOT_IO_FUNC_GPIO_2_GPIO);

    //设置GPIO_2为输出模式
    GpioSetDir(WIFI_IOT_GPIO_IDX_2, WIFI_IOT_GPIO_DIR_OUT);
    //设置GPIO_10的复用功能为普通GPIO
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_8, WIFI_IOT_IO_FUNC_GPIO_8_GPIO);

    //设置GPIO_10为输出模式
    GpioSetDir(WIFI_IOT_GPIO_IDX_8, WIFI_IOT_GPIO_DIR_OUT);
    //设置GPIO_7的复用功能为普通GPIO
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_7, WIFI_IOT_IO_FUNC_GPIO_7_GPIO);

    //设置GPIO_7为输出模式
    GpioSetDir(WIFI_IOT_GPIO_IDX_7, WIFI_IOT_GPIO_DIR_OUT);
    //初始化外设

    GpioSetOutputVal(WIFI_IOT_GPIO_IDX_2, 0);//关闭小灯
    GpioSetOutputVal(WIFI_IOT_GPIO_IDX_10, 0);//关闭电机
    WifiConnect("永州市火车站","abc908803958");//第一次踩坑：没有连接WiFi
	printf("Starting ...\n");
	int rc = 0;

	/* setup the buffer, it must big enough for aliyun IoT platform */
	unsigned char buf[1000];
	unsigned char readbuf[1000];

	Network n;
	MQTTClient c;
	char *host = EXAMPLE_PRODUCT_KEY".iot-as-mqtt.cn-shanghai.aliyuncs.com";
	short port = 443;

	const char *subTopic = "/"EXAMPLE_PRODUCT_KEY"/"EXAMPLE_DEVICE_NAME"/user/get";
	const char *pubTopic = "/"EXAMPLE_PRODUCT_KEY"/"EXAMPLE_DEVICE_NAME"/user/update";

	/* invoke aiotMqttSign to generate mqtt connect parameters */
	char clientId[150] = {0};
	char username[65] = {0};
	char password[65] = {0};
	if ((rc = aiotMqttSign(EXAMPLE_PRODUCT_KEY, EXAMPLE_DEVICE_NAME, EXAMPLE_DEVICE_SECRET, clientId, username, password) < 0)) {
		printf("aiotMqttSign -%0x4x\n", -rc);
		return;
	}
	printf("clientid: %s\n", clientId);
	printf("username: %s\n", username);
	printf("password: %s\n", password);

	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);

	/* network init and establish network to aliyun IoT platform */
	NetworkInit(&n);
	rc = NetworkConnect(&n, host, port);
	printf("NetworkConnect %d\n", rc);

	/* init mqtt client */
	MQTTClientInit(&c, &n, 1000, buf, sizeof(buf), readbuf, sizeof(readbuf));
 
	/* set the default message handler */
	c.defaultMessageHandler = messageArrived;

	/* set mqtt connect parameter */
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
	data.willFlag = 0;
	data.MQTTVersion = 3;
	data.clientID.cstring = clientId;
	data.username.cstring = username;
	data.password.cstring = password;
	data.keepAliveInterval = 60;
	data.cleansession = 1;
	printf("Connecting to %s %d\n", host, port);

	rc = MQTTConnect(&c, &data);
	printf("MQTTConnect %d, Connect aliyun IoT Cloud Success!\n", rc);//0代表成功，其它代表无连接。
    
    printf("Subscribing to %s\n", subTopic);
	rc = MQTTSubscribe(&c, subTopic, 1, messageArrived);
	printf("MQTTSubscribe %d\n", rc);

	int cnt = 0;
    unsigned int msgid = 0;
	while (!toStop)
	{
		MQTTYield(&c, 1000);	

		// if (++cnt % 30 == 0) {
		// 	MQTTMessage msg = {
		// 		QOS1, 
		// 		0,
		// 		0,
		// 		0,
		// 		"Hello world",
		// 		strlen("Hello world"),
		// 	};
        //     msg.id = ++msgid;
		// 	rc = MQTTPublish(&c, pubTopic, &msg);
		// 	printf("MQTTPublish %d, msgid %d\n", rc, msgid);
		// }
	}

	printf("Stopping\n");

	MQTTDisconnect(&c);
	NetworkDisconnect(&n);

	return;
}

static void MQTT_Demo(void)
{
    osThreadAttr_t attr;

    attr.name = "MQTT_DemoTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 10240;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)MQTT_DemoTask, NULL, &attr) == NULL) {
        printf("[MQTT_Demo] Falied to create MQTT_DemoTask!\n");
    }
}

APP_FEATURE_INIT(MQTT_Demo);