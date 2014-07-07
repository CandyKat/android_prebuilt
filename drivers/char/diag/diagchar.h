/* Copyright (c) 2008-2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef DIAGCHAR_H
#define DIAGCHAR_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/mempool.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/wakelock.h>
#include <mach/msm_smd.h>
#include <asm/atomic.h>
#include <asm/mach-types.h>

/* Size of the USB buffers used for read and write*/
#define USB_MAX_OUT_BUF 4096
#define APPS_BUF_SIZE	4096
#define IN_BUF_SIZE		16384
#define MAX_IN_BUF_SIZE	32768
#define MAX_SYNC_OBJ_NAME_SIZE	32
/* Size of the buffer used for deframing a packet
  reveived from the PC tool*/
#define HDLC_MAX 4096
#define HDLC_OUT_BUF_SIZE	8192
#define POOL_TYPE_COPY		1
#define POOL_TYPE_HDLC		2
#define POOL_TYPE_USER		3
#define POOL_TYPE_WRITE_STRUCT	4
#define POOL_TYPE_HSIC		5
#define POOL_TYPE_HSIC_2	6
#define POOL_TYPE_HSIC_WRITE	11
#define POOL_TYPE_HSIC_2_WRITE	12
#define POOL_TYPE_ALL		10

#define POOL_COPY_IDX		0
#define POOL_HDLC_IDX		1
#define POOL_USER_IDX		2
#define POOL_WRITE_STRUCT_IDX	3
#define POOL_HSIC_IDX		4
#define POOL_HSIC_2_IDX		5
#define POOL_HSIC_3_IDX		6
#define POOL_HSIC_4_IDX		7
#define POOL_HSIC_WRITE_IDX	8
#define POOL_HSIC_2_WRITE_IDX	9
#define POOL_HSIC_3_WRITE_IDX	10
#define POOL_HSIC_4_WRITE_IDX	11

#ifdef CONFIG_DIAGFWD_BRIDGE_CODE
#define NUM_MEMORY_POOLS	12
#else
#define NUM_MEMORY_POOLS	4
#endif

#define MAX_SSID_PER_RANGE	200

#define MODEM_DATA		0
#define LPASS_DATA		1
#define WCNSS_DATA		2
#define APPS_DATA		3
#define SDIO_DATA		4
#define HSIC_DATA		5
#define HSIC_2_DATA		6
#define SMUX_DATA		10
#define APPS_PROC		1
/*
 * Each row contains First (uint32_t), Last (uint32_t), Actual
 * last (uint32_t) values along with the range of SSIDs
 * (MAX_SSID_PER_RANGE*uint32_t).
 * And there are MSG_MASK_TBL_CNT rows.
 */
#define MSG_MASK_SIZE		((MAX_SSID_PER_RANGE+3) * 4 * MSG_MASK_TBL_CNT)
#define MAX_EQUIP_ID		16
#define MAX_ITEMS_PER_EQUIP_ID	512
#define LOG_MASK_ITEM_SIZE	(5 + MAX_ITEMS_PER_EQUIP_ID)
#define LOG_MASK_SIZE		(MAX_EQUIP_ID * LOG_MASK_ITEM_SIZE)
#define EVENT_MASK_SIZE 1000
#define USER_SPACE_DATA 8192
#define PKT_SIZE 4096

#define DIAG_CTRL_MSG_LOG_MASK	9
#define DIAG_CTRL_MSG_EVENT_MASK	10
#define DIAG_CTRL_MSG_F3_MASK	11
#define CONTROL_CHAR	0x7E

#define DIAG_CON_APSS (0x0001)	/* Bit mask for APSS */
#define DIAG_CON_MPSS (0x0002)	/* Bit mask for MPSS */
#define DIAG_CON_LPASS (0x0004)	/* Bit mask for LPASS */
#define DIAG_CON_WCNSS (0x0008)	/* Bit mask for WCNSS */

#define NUM_STM_PROCESSORS	4

#define DIAG_STM_MODEM	0x01
#define DIAG_STM_LPASS	0x02
#define DIAG_STM_WCNSS	0x04
#define DIAG_STM_APPS	0x08

/*
 * The status bit masks when received in a signal handler are to be
 * used in conjunction with the peripheral list bit mask to determine the
 * status for a peripheral. For instance, 0x00010002 would denote an open
 * status on the MPSS
 */
#define DIAG_STATUS_OPEN (0x00010000)	/* DCI channel open status mask   */
#define DIAG_STATUS_CLOSED (0x00020000)	/* DCI channel closed status mask */

#define MODE_REALTIME 1
#define MODE_NONREALTIME 0

#define NUM_SMD_DATA_CHANNELS 3
#define NUM_SMD_CONTROL_CHANNELS NUM_SMD_DATA_CHANNELS
#define NUM_SMD_DCI_CHANNELS 1
#define NUM_SMD_CMD_CHANNELS 1
#define NUM_SMD_DCI_CMD_CHANNELS 1

#define SMD_DATA_TYPE 0
#define SMD_CNTL_TYPE 1
#define SMD_DCI_TYPE 2
#define SMD_CMD_TYPE 3
#define SMD_DCI_CMD_TYPE 4

#define DIAG_PROC_DCI			1
#define DIAG_PROC_MEMORY_DEVICE		2

/* Flags to vote the DCI or Memory device process up or down
   when it becomes active or inactive */
#define VOTE_DOWN			0
#define VOTE_UP				1

#define DIAG_TS_SIZE	50

/* Maximum number of pkt reg supported at initialization*/
extern int diag_max_reg;
extern int diag_threshold_reg;

#define APPEND_DEBUG(ch) \
do {							\
	diag_debug_buf[diag_debug_buf_idx] = ch; \
	(diag_debug_buf_idx < 1023) ? \
	(diag_debug_buf_idx++) : (diag_debug_buf_idx = 0); \
} while (0)

/* List of remote processor supported */
enum remote_procs {
	MDM = 1,
	MDM2 = 2,
	MDM3 = 3,
	MDM4 = 4,
	QSC = 5,
};

struct diag_master_table {
	uint16_t cmd_code;
	uint16_t subsys_id;
	uint32_t client_id;
	uint16_t cmd_code_lo;
	uint16_t cmd_code_hi;
	int process_id;
};

struct bindpkt_params_per_process {
	/* Name of the synchronization object associated with this proc */
	char sync_obj_name[MAX_SYNC_OBJ_NAME_SIZE];
	uint32_t count;	/* Number of entries in this bind */
	struct bindpkt_params *params; /* first bind params */
};

struct bindpkt_params {
	uint16_t cmd_code;
	uint16_t subsys_id;
	uint16_t cmd_code_lo;
	uint16_t cmd_code_hi;
	/* For Central Routing, used to store Processor number */
	uint16_t proc_id;
	uint32_t event_id;
	uint32_t log_code;
	/* For Central Routing, used to store SMD channel pointer */
	uint32_t client_id;
};

struct diag_write_device {
	void *buf;
	int length;
};

struct diag_client_map {
	char name[20];
	int pid;
};

struct diag_nrt_wake_lock {
	int enabled;
	int ref_count;
	int copy_count;
	struct wake_lock read_lock;
	spinlock_t read_spinlock;
};

struct real_time_vote_t {
	uint16_t proc;
	uint8_t real_time_vote;
};

/* This structure is defined in USB header file */
#ifndef CONFIG_DIAG_OVER_USB
struct diag_request {
	char *buf;
	int length;
	int actual;
	int status;
	void *context;
};
#endif

struct diag_smd_info {
	int peripheral;	/* The peripheral this smd channel communicates with */
	int type;	/* The type of smd channel (data, control, dci) */
	uint16_t peripheral_mask;
	int encode_hdlc; /* Whether data is raw and needs to be hdlc encoded */

	smd_channel_t *ch;
	smd_channel_t *ch_save;

	struct mutex smd_ch_mutex;

	int in_busy_1;
	int in_busy_2;

	unsigned char *buf_in_1;
	unsigned char *buf_in_2;

	unsigned char *buf_in_1_raw;
	unsigned char *buf_in_2_raw;

	unsigned int buf_in_1_size;
	unsigned int buf_in_2_size;

	unsigned int buf_in_1_raw_size;
	unsigned int buf_in_2_raw_size;

	struct diag_request *write_ptr_1;
	struct diag_request *write_ptr_2;

	struct diag_nrt_wake_lock nrt_lock;

	struct workqueue_struct *wq;

	struct work_struct diag_read_smd_work;
	struct work_struct diag_notify_update_smd_work;
	int notify_context;
	struct work_struct diag_general_smd_work;
	int general_context;

	/*
	 * Function ptr for function to call to process the data that
	 * was just read from the smd channel
	 */
	int (*process_smd_read_data)(struct diag_smd_info *smd_info,
						void *buf, int num_bytes);
};

struct diagchar_dev {

	/* State for the char driver */
	unsigned int major;
	unsigned int minor_start;
	int num;
	struct cdev *cdev;
	char *name;
	int dropped_count;
	struct class *diagchar_class;
	int ref_count;
	struct mutex diagchar_mutex;
	wait_queue_head_t wait_q;
	wait_queue_head_t smd_wait_q;
	struct diag_client_map *client_map;
	int *data_ready;
	int num_clients;
	int polling_reg_flag;
	struct diag_write_device *buf_tbl;
	unsigned int buf_tbl_size;
	int use_device_tree;
	int supports_separate_cmdrsp;
	int supports_apps_hdlc_encoding;
	/* The state requested in the STM command */
	int stm_state_requested[NUM_STM_PROCESSORS];
	/* The current STM state */
	int stm_state[NUM_STM_PROCESSORS];
	/* Whether or not the peripheral supports STM */
	int peripheral_supports_stm[NUM_SMD_CONTROL_CHANNELS];
	/* DCI related variables */
	struct list_head dci_req_list;
	struct diag_dci_client_tbl *dci_client_tbl;
	int dci_tag;
	int dci_client_id;
	struct mutex dci_mutex;
	int num_dci_client;
	unsigned char *apps_dci_buf;
	int dci_state;
	struct workqueue_struct *diag_dci_wq;
	/* Memory pool parameters */
	unsigned int itemsize;
	unsigned int poolsize;
	unsigned int itemsize_hdlc;
	unsigned int poolsize_hdlc;
	unsigned int itemsize_user;
	unsigned int poolsize_user;
	unsigned int itemsize_write_struct;
	unsigned int poolsize_write_struct;
	unsigned int debug_flag;
	/* State for the mempool for the char driver */
	mempool_t *diagpool;
	mempool_t *diag_hdlc_pool;
	mempool_t *diag_user_pool;
	mempool_t *diag_write_struct_pool;
	spinlock_t diag_mem_lock;
	int count;
	int count_hdlc_pool;
	int count_user_pool;
	int count_write_struct_pool;
	int used;
	/* Buffers for masks */
	struct mutex diag_cntl_mutex;
	struct diag_ctrl_event_mask *event_mask;
	struct diag_ctrl_log_mask *log_mask;
	struct diag_ctrl_msg_mask *msg_mask;
	struct diag_ctrl_feature_mask *feature_mask;
	struct mutex log_mask_mutex;
	/* State for diag forwarding */
	struct diag_smd_info smd_data[NUM_SMD_DATA_CHANNELS];
	struct diag_smd_info smd_cntl[NUM_SMD_CONTROL_CHANNELS];
	struct diag_smd_info smd_dci[NUM_SMD_DCI_CHANNELS];
	struct diag_smd_info smd_cmd[NUM_SMD_CMD_CHANNELS];
	struct diag_smd_info smd_dci_cmd[NUM_SMD_DCI_CMD_CHANNELS];
	int rcvd_feature_mask[NUM_SMD_CONTROL_CHANNELS];
	int separate_cmdrsp[NUM_SMD_CONTROL_CHANNELS];
	unsigned char *usb_buf_out;
	unsigned char *apps_rsp_buf;
	unsigned char *user_space_data_buf;
	/* buffer for updating mask to peripherals */
	unsigned char *buf_msg_mask_update;
	unsigned char *buf_log_mask_update;
	unsigned char *buf_event_mask_update;
	unsigned char *buf_feature_mask_update;
	int read_len_legacy;
	struct mutex diag_hdlc_mutex;
	unsigned char *hdlc_buf;
	unsigned hdlc_count;
	unsigned hdlc_escape;
	int in_busy_pktdata;
	struct device *dci_device;
	struct device *dci_cmd_device;
	/* Variables for non real time mode */
	int real_time_mode;
	int real_time_update_busy;
	uint16_t proc_active_mask;
	uint16_t proc_rt_vote_mask;
	struct mutex real_time_mutex;
	struct work_struct diag_real_time_work;
	struct workqueue_struct *diag_real_time_wq;
#ifdef CONFIG_DIAG_OVER_USB
	int usb_connected;
	struct usb_diag_ch *legacy_ch;
	int usb_req_allocated;
	struct work_struct diag_proc_hdlc_work;
	struct work_struct diag_read_work;
	struct work_struct diag_usb_connect_work;
	struct work_struct diag_usb_disconnect_work;
#endif
	struct workqueue_struct *diag_wq;
	struct work_struct diag_drain_work;
	struct workqueue_struct *diag_cntl_wq;
	uint8_t *msg_masks;
	uint8_t msg_status;
	uint8_t *log_masks;
	uint8_t log_status;
	uint8_t *event_masks;
	uint8_t event_status;
	uint8_t log_on_demand_support;
	struct diag_master_table *table;
	uint8_t *pkt_buf;
	int pkt_length;
	struct diag_request *usb_read_ptr;
	struct diag_request *write_ptr_svc;
	int logging_mode;
	int mask_check;
	int logging_process_id;
	struct task_struct *socket_process;
	struct task_struct *callback_process;
#ifdef CONFIG_DIAG_SDIO_PIPE
	unsigned char *buf_in_sdio;
	unsigned char *usb_buf_mdm_out;
	struct sdio_channel *sdio_ch;
	int read_len_mdm;
	int in_busy_sdio;
	struct usb_diag_ch *mdm_ch;
	struct work_struct diag_read_mdm_work;
	struct workqueue_struct *diag_sdio_wq;
	struct work_struct diag_read_sdio_work;
	struct work_struct diag_close_sdio_work;
	struct diag_request *usb_read_mdm_ptr;
	struct diag_request *write_ptr_mdm;
#endif
#ifdef CONFIG_DIAGFWD_BRIDGE_CODE
	/* common for all bridges */
	struct work_struct diag_connect_work;
	struct work_struct diag_disconnect_work;
	/* SGLTE variables */
	int lcid;
	unsigned char *buf_in_smux;
	int in_busy_smux;
	int diag_smux_enabled;
	int smux_connected;
	struct diag_request *write_ptr_mdm;
#endif
#ifdef CONFIG_DIAG_EXTENSION
	struct list_head addon_list;
#endif
};

extern struct diag_bridge_dev *diag_bridge;
extern struct diag_hsic_dev *diag_hsic;
extern struct diagchar_dev *driver;

extern int wrap_enabled;
extern uint16_t wrap_count;

void diag_get_timestamp(char *time_str);
int diag_find_polling_reg(int i);

#ifdef CONFIG_DIAG_EXTENSION
/* This structure is for addon. It is used by slate feature */
struct diag_addon {
	struct list_head list;

	/* function list of addon
	return-value of the functions decide
	whether the callback-function of next-addon is called or not.
	refer to DIAGADDON_BASE below.
	*/
	int (*ioctl)(struct file *filp, unsigned int iocmd,
					unsigned long ioarg, int *retval);
	int (*force_returntype)(int pkt_type, int *retval);
	int (*addon_channel_diag_write)(struct diag_request *write_ptr,
								int *retval);
	int (*channel_diag_write)(struct usb_diag_ch *ch,
						struct diag_request *d_req);
	void *private;

	/* function list of diag-driver to use addon */
	int (*diag_process_apps_pkt)(unsigned char *buf, int len);
};

#define DIAGADDON_BASE(func, retval, ...)		\
	do {						\
		struct diag_addon *addon;		 \
		int next_addon_call;			\
		list_for_each_entry(addon, &driver->addon_list, list) {	\
			if (addon->func) {		\
				next_addon_call =	\
					addon->func(__VA_ARGS__, retval);\
				if (next_addon_call == false)	\
					break;			\
			}				\
		}					\
	} while (0)

#define DIAGADDON_EXIST() (!list_empty(&driver->addon_list))
#define DIAGADDON_ioctl(retval, ...)\
		DIAGADDON_BASE(ioctl, retval, ##__VA_ARGS__)
#define DIAGADDON_force_returntype(retval, ...)\
		DIAGADDON_BASE(force_returntype, retval, ##__VA_ARGS__)
#define DIAGADDON_channel_diag_write(retval, ...)\
		DIAGADDON_BASE(addon_channel_diag_write, retval, ##__VA_ARGS__)

int diag_addon_register(struct diag_addon *addon);
int diag_addon_unregister(struct diag_addon *addon);
#else
#define DIAGADDON_EXIST() 0
#define DIAGADDON_ioctl(retval, ...) do {} while (0)
#define DIAGADDON_force_returntype(retval, ...) do {} while (0)
#define DIAGADDON_channel_diag_write(retval, i...) do {} while (0)
#endif /* endif of '#ifdef CONFIG_DIAG_EXTENSION' */

#endif
