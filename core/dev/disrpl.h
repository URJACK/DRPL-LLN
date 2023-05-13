#define ROLE_NULL 0
#define ROLE_DATA 1
#define ROLE_STORE 2

#define MSG_REGISTER 0x31
#define MSG_REGISTER_RESP 0x32
#define MSG_DETECT 0x01
#define MSG_DETECT_RESP 0x02
#define MSG_DETECT_RESP_ACK 0x03
#define MSG_SYNC 0x11
#define MSG_SYNC_RESP 0x12
#define MSG_DATA_READ 0x21
#define MSG_DATA_WRITE 0x22
#define MSG_DATA_RESP 0x23

#define DETECT_INTERVAL 30 * CLOCK_SECOND

#define ACTION_LIVENESS 1
#define ACTION_LIGHT_ON 2
#define ACTION_LIGHT_OFF 3
#define ACTION_BUZZER_ON 4
#define ACTION_BUZZER_OFF 5

#define TRUE 1
#define FALSE 0

/**
 * 数据存储对象（Data Storage Object，DSO）
 */
typedef struct
{
    /* 必要字段 */
    short objId; // DSO的id
    /* 可变字段 */
    short temperature; // DSO记录的温度
    short light;       // DSO记录的光照
} DSO;

/**
 * 数据存储对象表（Data Storage Object Table，DSOT）
 */
typedef struct DSOT_ITEM
{
    short nodeId; // 节点id
    DSO dso;      // 节点对应的DSO
} DSOT_ITEM;

/**
 * 汇集节点所持有，节点存储对象。
 */
typedef struct
{
    short id;          // 节点id
    char role;         // 节点角色
    uip_ipaddr_t addr; // 节点的ip地址
} Node;

void setShort(short *data, short val);
short getShort(short *data);