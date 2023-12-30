/**
*****************************************************************************************
*     Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bt_mesh_user_api.c
  * @brief    Source file for provisioner cmd.
  * @details  User command interfaces.
  * @author   sherman
  * @date     2019-09-16
  * @version  v1.0
  * *************************************************************************************
  */
#include "bt_mesh_user_api.h"
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
#include "bt_mesh_provisioner_api.h"
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
#include "bt_mesh_device_api.h"
#endif

#if defined(CONFIG_BT_MESH_USER_API) && CONFIG_BT_MESH_USER_API
#if ((defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER) || \
    (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE) || \
    (defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) || \
    (defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE))

CMD_MOD_INFO_S              btMeshCmdPriv;
INDICATION_ITEM             btMeshCmdIdPriv;
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
extern struct task_struct	meshProvisionerCmdThread;
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
extern struct task_struct	meshDeviceCmdThread;
#endif

static const mesh_cmd_entry mesh_cmd_table[] = {
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
    PARAM_MESH_CODE("pbadvcon", "\r pbadvcon [dev uuid]", "\r create a pb-adv link with the device uuid\n\r", GEN_MESH_CODE(_pb_adv_con))
    PARAM_MESH_CODE("prov", "\r prov [attn_dur] [manual]", "\r provision a new mesh device\n\r", GEN_MESH_CODE(_prov))
    PARAM_MESH_CODE("aka", "\r aka [dst] [net_key_index] [app_key_index]", "\r app key add\n\r", GEN_MESH_CODE(_app_key_add))
    PARAM_MESH_CODE("mab", "\r mab [dst] [element index] [model_id] [app_key_index]", "\r model app bind\n\r", GEN_MESH_CODE(_model_app_bind))
    PARAM_MESH_CODE("goos", "\r goos [dst] [on/off] [ack] [app_key_index] [steps] [resolution] [delay]", "\r generic on off set\n\r", GEN_MESH_CODE(_generic_on_off_set))
    PARAM_MESH_CODE("goog", "\r goog [dst] [app_key_index]", "\r generic on off get\n\r", GEN_MESH_CODE(_generic_on_off_get))
    PARAM_MESH_CODE("nr", "\r nr [dst]", "\r node reset\n\r", GEN_MESH_CODE(_node_reset))
    PARAM_MESH_CODE("msd", "\r msd [dst] [element index] [model_id] <group addr>", "\r model subsribe delete\n\r", GEN_MESH_CODE(_model_sub_delete))
    PARAM_MESH_CODE("msa", "\r msa [dst] [element index] [model_id] [group addr]", "\r model subsribe add\n\r", GEN_MESH_CODE(_model_sub_add))
    PARAM_MESH_CODE("provdis", "\r provdis [conn id]", "\r Start discovery provisioning service\n\r", GEN_MESH_CODE(_prov_discover))
    PARAM_MESH_CODE("provcmd", "\r provcmd [char CCCD] [command: enable/disable]", "\r Provisioning notify/ind switch command\n\r", GEN_MESH_CODE(_prov_cccd_operate))
    PARAM_MESH_CODE("proxydis", "\r proxydis [conn id]", "\r Start discovery proxy service\n\r", GEN_MESH_CODE(_proxy_discover))
    PARAM_MESH_CODE("proxycmd", "\r proxycmd [char CCCD] [command: enable/disable]", "\r Proxy notify/ind switch command\n\r", GEN_MESH_CODE(_proxy_cccd_operate))
#endif
#if (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
    PARAM_MESH_CODE("nr", "\r nr [mode]\n\r", "\r node reset\n\r", GEN_MESH_CODE(_node_reset))
    PARAM_MESH_CODE("pcs", "\r pcs [public key oob] [static oob] [output size] [output action] [input size] [input action]\n\r", "\r provision capability set\n\r", GEN_MESH_CODE(_prov_capa_set)) 
    PARAM_MESH_CODE("lpninit", "\r lpninit [fn_num]\n\r", "\r low power node init\n\r", GEN_MESH_CODE(_lpn_init)) 
    PARAM_MESH_CODE("lpnreq", "\r lpnreq [fn_index] [net_key_index] [poll int(100ms)] [poll to(100ms)] [rx delay(ms)] [rx widen(ms)]\n\r", "\r LPN request to estabish a friendship\n\r", GEN_MESH_CODE(_lpn_req)) 
    PARAM_MESH_CODE("lpnsub", "\r lpnsub [fn_index] [addr] [add/rm]\n\r", "\r LPN subsript list add or rm\n\r", GEN_MESH_CODE(_lpn_sub)) 
    PARAM_MESH_CODE("lpnclear", "\r lpnclear [fn_index]\n\r", "\r LPN clear\n\r", GEN_MESH_CODE(_lpn_clear)) 
    PARAM_MESH_CODE("dtn", "\r dtn [conn_id] [value...]\n\r", "\r data transmission notify\n\r", GEN_MESH_CODE(_data_transmission_notify))
#endif
    PARAM_MESH_CODE("con", "\r con [bt addr] [addr type]", "\r connect to remote device\n\r", GEN_MESH_CODE(_connect))
    PARAM_MESH_CODE("disc", "\r disc [conn id]", "\r disconnect to remote device\n\r", GEN_MESH_CODE(_disconnect))
    PARAM_MESH_CODE("ls", "\r ls\n\r", "\rlist node state info\n\r", GEN_MESH_CODE(_list))
};

void bt_mesh_io_msg_handler(T_IO_MSG io_msg)
{
    uint8_t ret = 1;
    CMD_ITEM_S *pmeshCmdItem_s = NULL;
    PUSER_ITEM puserItem = NULL;

    pmeshCmdItem_s = (CMD_ITEM_S *)io_msg.u.buf;
    puserItem = (PUSER_ITEM)pmeshCmdItem_s->pmeshCmdItem->userData;
    if ((btMeshCmdPriv.meshMode != BT_MESH_PROVISIONER) && (btMeshCmdPriv.meshMode != BT_MESH_DEVICE)) {
        printf("[BT_MESH] %s(): Error BT MESH mode %d \r\n",__func__);
        goto exit;
    }  
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshMode == BT_MESH_PROVISIONER) {
        if (io_msg.type < MAX_MESH_PROVISIONER_CMD) {
            ret = bt_mesh_user_cmd_hdl(io_msg.type, (CMD_ITEM_S *)io_msg.u.buf);
            if(ret != 0) {
                printf("[BT_MESH] %s(): bt_mesh_io_msg_handler Fail!\r\n",__func__);
            }
            return;
        } else {
            printf("[BT_MESH] %s(): Error mesh code %d \r\n",__func__, io_msg.type);
            goto exit;
        }
    } 
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshMode == BT_MESH_DEVICE) {
        if (io_msg.type < MAX_MESH_DEVICE_CMD) {
            ret = bt_mesh_user_cmd_hdl(io_msg.type, (CMD_ITEM_S *)io_msg.u.buf);
            if(ret != 0) {
                printf("[BT_MESH] %s(): bt_mesh_io_msg_handler Fail!\r\n",__func__);
            }
            return;
        } else {
            printf("[BT_MESH] %s(): Error mesh code %d \r\n",__func__, io_msg.type);
            goto exit;
        }
    }
#endif

exit:
    if (pmeshCmdItem_s->userApiMode == USER_API_ASYNCH) {
        bt_mesh_free_hdl(puserItem);
    }
    bt_mesh_cmdunreg(pmeshCmdItem_s);
    return;
}

int bt_mesh_user_cmd(uint16_t mesh_code, void *pmesh_cmd_item_s)
{
    T_IO_MSG io_msg;
    CMD_ITEM_S *pmeshCmdItem_s = (CMD_ITEM_S *)pmesh_cmd_item_s;

    io_msg.type = mesh_code;
	io_msg.u.buf = (void *)pmeshCmdItem_s;
    pmeshCmdItem_s->msgRecvFlag = 0;
	bt_mesh_send_io_msg(&io_msg);

    return 0;
}

uint8_t bt_mesh_user_cmd_hdl(uint16_t mesh_code, CMD_ITEM_S *pmesh_cmd_item_s)
{
    uint8_t ret = 1;
    user_cmd_parse_result_t (*cmd_hdl)(user_cmd_parse_value_t *pparse_value);
    CMD_ITEM_S *pmeshCmdItem_s = (CMD_ITEM_S *)pmesh_cmd_item_s;
    CMD_ITEM *pmeshCmdItem = NULL;
    PUSER_ITEM puserItem = NULL;
    user_cmd_parse_value_t *pparse_value = NULL;

    pmeshCmdItem = pmeshCmdItem_s->pmeshCmdItem;
    puserItem = (PUSER_ITEM)pmeshCmdItem->userData;
    if ((btMeshCmdPriv.meshMode != BT_MESH_PROVISIONER) && (btMeshCmdPriv.meshMode != BT_MESH_DEVICE)) {
        printf("[BT_MESH] %s(): Error BT MESH mode %d \r\n",__func__);
        goto exit;
    }
    if (pmeshCmdItem_s->msgRecvFlag) {
        printf("[BT_MESH] %s(): mesh code %d send msg fail \r\n", __func__, pmeshCmdItem->meshCmdCode);
        goto exit;
    }
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshMode == BT_MESH_PROVISIONER) {
        cmd_hdl = provisionercmds[mesh_code].mesh_func;
    } 
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshMode == BT_MESH_DEVICE) {
        cmd_hdl = devicecmds[mesh_code].mesh_func;
    }
#endif
    if (pmeshCmdItem_s->userApiMode != USER_API_ASYNCH && pmeshCmdItem_s->semaDownTimeOut) {
        printf("[BT_MESH] %s(): mesh code %d timeout \r\n", __func__, pmeshCmdItem->meshCmdCode);
        goto exit;
    }
    pparse_value = puserItem->pparseValue;
    if (pparse_value == NULL) {
        printf("[BT_MESH] %s(): pparse_value is NULL!\r\n",__func__);
        goto exit;
    }
    ret = cmd_hdl(pparse_value);
    if (pmeshCmdItem_s->userApiMode == USER_API_CMDLINE) {
        if (!pmeshCmdItem_s->semaDownTimeOut) {
            rtw_up_sema(&puserItem->cmdLineSema);
        }
    }
    if (ret != USER_CMD_RESULT_OK) {
        printf("[BT_MESH] %s(): provisioner cmd fail! (%d)\r\n",__func__,ret);
    }

exit:
    pmeshCmdItem_s->msgRecvFlag = 1;
    if (pmeshCmdItem_s->userApiMode == USER_API_CMDLINE) {
        bt_mesh_cmdunreg(pmeshCmdItem_s);
    } else if (pmeshCmdItem_s->userApiMode == USER_API_ASYNCH) {
        bt_mesh_free_hdl(puserItem);
        bt_mesh_cmdunreg(pmeshCmdItem_s);
    }
    
    return ret;    
}

uint8_t bt_mesh_enqueue_cmd(struct list_head *queue, uint8_t head_or_tail)
{
    if (!btMeshCmdPriv.meshCmdEnable) {
        printf("[BT_MESH] %s(): Mesh user command is disabled!\r\n");
		return 1;;
    } 
    rtw_mutex_get(&btMeshCmdPriv.cmdMutex);
    if (head_or_tail) {
        rtw_list_insert_head(queue, &btMeshCmdPriv.meshCmdList);
    } else {
        rtw_list_insert_tail(queue, &btMeshCmdPriv.meshCmdList);
    }
    btMeshCmdPriv.cmdListNum ++;
    rtw_mutex_put(&btMeshCmdPriv.cmdMutex);
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshCmdEnable) {
        rtw_if_wifi_wakeup_task(&meshProvisionerCmdThread);
    }
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshCmdEnable) {
        rtw_if_wifi_wakeup_task(&meshDeviceCmdThread);
    }
#endif
    else {
        printf("[BT_MESH] %s(): Mesh user command is disabled\r\n");
        return 2;
    }
    
    return 0;
}

struct list_head *bt_mesh_dequeue_cmd(void)
{
    struct list_head *queue = NULL;
    
    rtw_mutex_get(&btMeshCmdPriv.cmdMutex);
    if (rtw_is_list_empty(&btMeshCmdPriv.meshCmdList)) {
        printf("[BT_MESH] %s(): btMeshCmdPriv.meshCmdList is empty !\r\n", __func__);
        rtw_mutex_put(&btMeshCmdPriv.cmdMutex);
        return NULL;
    } else {
        queue = get_next(&btMeshCmdPriv.meshCmdList);
        rtw_list_delete(queue);
    }
    btMeshCmdPriv.cmdListNum --;
    rtw_mutex_put(&btMeshCmdPriv.cmdMutex); 

    return queue;
}

CMD_ITEM_S* bt_mesh_cmdreg(uint16_t mesh_code, user_cmd_parse_value_t *pparse_value, bt_mesh_func func, user_cmd_cbk cbk, void *user_data)
{
    PUSER_ITEM puserItem = NULL;
    CMD_ITEM   *pmeshCmdItem = NULL;
    CMD_ITEM_S *pmeshCmdItem_s = NULL;

    if (!btMeshCmdPriv.meshCmdEnable) {
        printf("[BT_MESH] %s(): Mesh user command is disabled!\r\n");
		return NULL;
    }
    puserItem = (PUSER_ITEM)user_data;
    pmeshCmdItem = (CMD_ITEM *)rtw_zmalloc(sizeof(CMD_ITEM));
    if (pmeshCmdItem == NULL) {
        printf("[BT_MESH] %s(): alloc pmeshCmdItem for mesh code %d fail !\r\n", __func__, mesh_code);
        return NULL;
    }
    pmeshCmdItem->meshCmdCode = mesh_code;
    pmeshCmdItem->pparseValue = pparse_value;
    pmeshCmdItem->meshFunc = func;
    pmeshCmdItem->userCbk = cbk;
    pmeshCmdItem->userData = user_data;
    pmeshCmdItem_s = (CMD_ITEM_S *)rtw_zmalloc(sizeof(CMD_ITEM_S));
    if (pmeshCmdItem_s == NULL) {
        rtw_mfree((uint8_t *)pmeshCmdItem, sizeof(CMD_ITEM));
        printf("[BT_MESH] %s(): alloc pmeshCmdItem_s for mesh code %d fail !\r\n", __func__, mesh_code);
        return NULL;
    }
    pmeshCmdItem_s->pmeshCmdItem = pmeshCmdItem;
    pmeshCmdItem_s->semaDownTimeOut = 0;
    pmeshCmdItem_s->userApiMode = puserItem->userApiMode;
    if (bt_mesh_enqueue_cmd(&pmeshCmdItem_s->list, 0) == 1) {
        printf("[BT_MESH] %s(): enqueue cmd for mesh code %d fail !\r\n", __func__, mesh_code);
        rtw_mfree((uint8_t *)pmeshCmdItem, sizeof(CMD_ITEM));
        rtw_mfree((uint8_t *)pmeshCmdItem_s, sizeof(CMD_ITEM_S));
        return NULL;
    }

    return pmeshCmdItem_s;
}

uint8_t bt_mesh_cmdunreg(CMD_ITEM_S *pmesh_cmd_item_s)
{
    CMD_ITEM *pmeshCmdItem = NULL;
    CMD_ITEM_S *pmeshCmdItem_s = pmesh_cmd_item_s;

    pmeshCmdItem = pmeshCmdItem_s->pmeshCmdItem;
    rtw_mfree((uint8_t *)pmeshCmdItem, sizeof(CMD_ITEM));
    rtw_mfree((uint8_t *)pmeshCmdItem_s, sizeof(CMD_ITEM_S));

    return 0;
}

user_api_parse_result_t bt_mesh_set_user_cmd(uint16_t mesh_code, user_cmd_parse_value_t *pparse_value, user_cmd_cbk cbk, void *user_data)
{
    PUSER_ITEM puserItem = NULL;
    CMD_ITEM_S *pmeshCmdItem_s = NULL;
    uint8_t ret = USER_API_RESULT_ERROR;
    uint32_t time_out = 4000;

    puserItem = (PUSER_ITEM)user_data;
    if (!puserItem) {
        printf("[BT_MESH] %s(): puserItem is null!\r\n");
        return USER_API_RESULT_ERROR;
    }
    if (!btMeshCmdPriv.meshCmdEnable) {
        printf("[BT_MESH] %s(): Mesh user command is disabled!\r\n");
        ret = USER_API_RESULT_NOT_ENABLE;
        goto exit;
    }
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshMode == BT_MESH_PROVISIONER) {
        if (mesh_code == MAX_MESH_PROVISIONER_CMD || mesh_code > MAX_MESH_PROVISIONER_CMD ) {
            printf("[BT_MESH] %s(): user cmd %d illegal !\r\n", __func__, mesh_code);
            ret = USER_API_RESULT_NOT_FOUND;
            goto exit;
        }
    } else {
        printf("[BT_MESH] %s(): Error BT MESH mode %d \r\n",__func__);
        ret = USER_API_RESULT_ERROR_MESH_MODE;
        goto exit;
    }
#elif (defined(CONFIG_BT_MESH_DEVICE) && CONFIG_BT_MESH_DEVICE || \
    defined(CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE) && CONFIG_BT_MESH_DEVICE_MULTIPLE_PROFILE)
    if (btMeshCmdPriv.meshMode == BT_MESH_DEVICE) {
        if (mesh_code == MAX_MESH_DEVICE_CMD || mesh_code > MAX_MESH_DEVICE_CMD ) {
            printf("[BT_MESH] %s(): user cmd %d illegal !\r\n", __func__, mesh_code);
            ret = USER_API_RESULT_NOT_FOUND;
            goto exit;
        }
    } else {
        printf("[BT_MESH] %s(): Error BT MESH mode %d \r\n",__func__);
        ret = USER_API_RESULT_ERROR_MESH_MODE;
        goto exit;
    }
#endif
    pmeshCmdItem_s = bt_mesh_cmdreg(mesh_code, pparse_value, bt_mesh_user_cmd, cbk, user_data);
    if (!pmeshCmdItem_s) {
        printf("[BT_MESH] %s(): user cmd %d regiter fail !\r\n", __func__, mesh_code);
        ret = USER_API_RESULT_ERROR;
        goto exit;
    }
#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
    /* proivsioning takes more time */
    if (mesh_code == GEN_MESH_CODE(_prov)) {
        time_out = 20000;
    } else {
        time_out = 4000;
    }
#else
    time_out = 4000;
#endif
    if (puserItem->userApiMode == USER_API_SYNCH) {
        if (rtw_down_timeout_sema(&puserItem->userSema, time_out) == _FAIL) {
            /* if timeout, puserItem will be free by mesh_cmd_thread or bt_mesh_user_cmd_cbk */
            pmeshCmdItem_s->semaDownTimeOut = 1;
            printf("[BT_MESH] %s(): user cmd %d timeout !\r\n", __func__, mesh_code);
            ret = USER_API_RESULT_TIMEOUT;
            goto exit;
        }else {
            if (puserItem->userCmdResult) {
                ret = USER_API_RESULT_OK;
                goto exit;
            } else {
                printf("[BT_MESH] %s():mesh cmd = %d FAIL!\r\n", __func__, mesh_code);
                ret = USER_API_RESULT_ERROR;
                goto exit;                     
            }   
        } 
    } else if (puserItem->userApiMode == USER_API_CMDLINE) {
        if (rtw_down_timeout_sema(&puserItem->cmdLineSema, time_out) == _FAIL) {
            /* if timeout, puserItem will be free by mesh_cmd_thread or bt_mesh_user_cmd_cbk */
            pmeshCmdItem_s->semaDownTimeOut = 1;
            printf("[BT_MESH] %s(): user cmd %d timeout !\r\n", __func__, mesh_code);
            ret = USER_API_RESULT_TIMEOUT;
            goto exit;
        }else {
            ret = USER_API_RESULT_OK;
            goto exit;
        } 
    } else {
        /* if timeout, puserItem will be free by mesh_cmd_thread or bt_mesh_user_cmd_cbk */
        return USER_API_RESULT_OK;
    } 

exit:
    bt_mesh_free_hdl(puserItem);
    return ret;
}

user_api_parse_result_t bt_mesh_indication(uint16_t mesh_code, uint8_t state, void *pparam)
{
    CMD_ITEM_S *pmeshCmdItem_s = NULL;
    PUSER_ITEM puserItem;
    uint8_t ret;

    if ((btMeshCmdPriv.meshMode != BT_MESH_PROVISIONER) && (btMeshCmdPriv.meshMode != BT_MESH_DEVICE)) {
        printf("[BT_MESH] %s(): BT mesh indication is not eable !\r\n", __func__);
        return USER_API_RESULT_OK;
    }
    //printf("\r\n %s()",__func__);
    if (btMeshCmdIdPriv.userApiMode != USER_API_SYNCH) {
        return USER_API_RESULT_OK;
    }
    if (mesh_code != btMeshCmdIdPriv.meshCmdCode) {
        printf("[BT_MESH] %s(): user cmd %d not found !\r\n", __func__, mesh_code);
        //rtw_up_sema(&btMeshCmdPriv.meshThreadSema);
        return USER_API_RESULT_INCORRECT_CODE;//Cause there are several cmd use the same in prov_cb
    }
    if ((plt_time_read_ms() < btMeshCmdIdPriv.startTime) || ((plt_time_read_ms() - btMeshCmdIdPriv.startTime) > 10000)) {
        printf("[BT_MESH] %s(): BT mesh code start time is not reasonable !\r\n", __func__);
        return USER_API_RESULT_TIMEOUT;
    }
    pmeshCmdItem_s = btMeshCmdIdPriv.pmeshCmdItem_s;
    if (!pmeshCmdItem_s->msgRecvFlag) {
        printf("[BT_MESH] %s(): This indication is not matched !\r\n", __func__, mesh_code);
        return USER_API_RESULT_ERROR;
    }
    if (pmeshCmdItem_s->pmeshCmdItem->userData && pmeshCmdItem_s->pmeshCmdItem->userCbk) {
        /* whatever modification to state is user difined, ex: memcpy state to pmeshCmdItem_s->pmeshCmdItem->userData*/
        puserItem = (USER_ITEM *)pmeshCmdItem_s->pmeshCmdItem->userData;
        puserItem->userCmdResult = state;
        printf("\r\n %s() userCmdResult = %d",__func__,puserItem->userCmdResult);
        puserItem->userParam = pparam;
        ret = pmeshCmdItem_s->pmeshCmdItem->userCbk(mesh_code, (void *)pmeshCmdItem_s);
        if (ret == USER_API_RESULT_INDICATION_NOT_MATCHED) {
            printf("[BT_MESH] %s(): user cmd %d not matched !\r\n", __func__, mesh_code);
            return USER_API_RESULT_ERROR;
        }
    } else {
        printf("[BT_MESH] %s(): user cmd %d pmeshCmdItem_s->pmeshCmdItem->userData is NULL !\r\n", __func__, mesh_code);
    }
    if (!btMeshCmdIdPriv.semaDownTimeOut) {
        rtw_up_sema(&btMeshCmdPriv.meshThreadSema);
    } else {
        printf("[BT_MESH] %s(): user cmd %d timeout !\r\n", __func__, mesh_code);
    }

    return USER_API_RESULT_OK;
}

PUSER_ITEM bt_mesh_alloc_hdl(uint8_t user_api_mode)
{
    PUSER_ITEM puserItem;

    if (!btMeshCmdPriv.meshCmdEnable) {
        printf("[BT_MESH] Mesh user command is disabled!\r\n");
		return NULL;
    }
    puserItem = (PUSER_ITEM)rtw_zmalloc(sizeof(USER_ITEM));
    if (!puserItem) {
        printf("[BT_MESH] PUSER_ITEM alloc fail!\r\n");
		return NULL;
    }
    puserItem->userApiMode = user_api_mode;
    puserItem->pparseValue = (user_cmd_parse_value_t *)rtw_zmalloc(sizeof(user_cmd_parse_value_t));
    if (!puserItem->pparseValue) {
        printf("[BT_MESH] puserItem->pparse_value alloc fail!\r\n");
        rtw_mfree((uint8_t *)puserItem, sizeof(USER_ITEM));
		return NULL;
    }
    if (user_api_mode == USER_API_SYNCH) {
        rtw_init_sema(&puserItem->userSema, 0);
    	if (puserItem->userSema == NULL) {
    		printf("[BT_MESH] puserItem->user_sema init fail!\r\n");
            rtw_mfree((uint8_t *)puserItem->pparseValue, sizeof(user_cmd_parse_value_t));
            rtw_mfree((uint8_t *)puserItem, sizeof(USER_ITEM));
    		return NULL;
    	}
    }
    if (user_api_mode == USER_API_CMDLINE) {
        rtw_init_sema(&puserItem->cmdLineSema, 0);
    	if (puserItem->cmdLineSema == NULL) {
    		printf("[BT_MESH] puserItem->cmdLineSema init fail!\r\n");
            rtw_mfree((uint8_t *)puserItem->pparseValue, sizeof(user_cmd_parse_value_t));
            rtw_mfree((uint8_t *)puserItem, sizeof(USER_ITEM));
    		return NULL;
    	}
    }

    return puserItem;
}

void bt_mesh_free_hdl(PUSER_ITEM puser_item)
{
    PUSER_ITEM puserItem;
	
	puserItem = puser_item;
    if (puserItem->userApiMode == USER_API_SYNCH) {
        rtw_free_sema(&puserItem->userSema);
    } else if (puserItem->userApiMode == USER_API_CMDLINE) {
        rtw_free_sema(&puserItem->cmdLineSema);
    }
    rtw_mfree((uint8_t *)puserItem->pparseValue, sizeof(user_cmd_parse_value_t));
    rtw_mfree((uint8_t *)puserItem, sizeof(USER_ITEM));
}

static void mesh_user_cmd_list(void)
{
    uint8_t i = 0;

    /* find command in table */
    for (i = 0; i < sizeof(mesh_cmd_table) / sizeof(mesh_cmd_table[0]); i ++)
    {
        data_uart_debug(mesh_cmd_table[i].poption);
        data_uart_debug("  *");
        data_uart_debug(mesh_cmd_table[i].phelp);
    };
    
    return;
}

static user_cmd_parse_result_t mesh_user_cmd_parse(user_cmd_parse_value_t *pparse_value, char *mesh_user_cmd_line)
{
    int32_t              i;
    user_cmd_parse_result_t  Result;
    char            *p, *q;

    /* clear all results */
    Result = USER_CMD_RESULT_OK;
    pparse_value->pcommand            = NULL;
    pparse_value->para_count     = 0;
    for (i = 0 ; i < USER_CMD_MAX_PARAMETERS; i ++)
    {
        pparse_value->pparameter[i]     = NULL;
        pparse_value->dw_parameter[i]    = 0;
    }
    /* Parse line */
    p = mesh_user_cmd_line;
    /*ignore leading spaces */
    p = user_cmd_skip_spaces(p);
    if (*p == '\0')                      /* empty command line ? */
    {
        Result = USER_CMD_RESULT_EMPTY_CMD_LINE;
    }
    else
    {
        /* find end of word */
        q = user_cmd_find_end_of_word(p);
        if (p == q)                        /* empty command line ? */
        {
            Result = USER_CMD_RESULT_EMPTY_CMD_LINE;
        }
        else                                /* command found */
        {
            pparse_value->pcommand = p;
            *q = '\0';                        /* mark end of command */
            p = q + 1;
            /* parse parameters */
            if (*p != '\0')                   /* end of line ? */
            {
                uint8_t j = 0;
                do
                {
                    uint32_t d;
                    /* ignore leading spaces */
                    p = user_cmd_skip_spaces(p);
                    d = user_cmd_string2uint32(p);
                    pparse_value->pparameter[j]    = p;
                    pparse_value->dw_parameter[j++] = d;
                    if (j >= USER_CMD_MAX_PARAMETERS)
                    {
                        break;
                    }
                    /* find next parameter */
                    p  = user_cmd_find_end_of_word(p);
                    *p++ = '\0';                        /* mark end of parameter */
                }
                while (*p != '\0');
                pparse_value->para_count = j;
            }
        }
    }

    return (Result);
}

void bt_mesh_param_user_cmd(unsigned int argc, char **argv)
{
    uint8_t i ,j ,k = 0;
    uint8_t found = 0;
    char meshUserCmdLine[USER_CMD_MAX_COMMAND_LINE + 2];
    PUSER_ITEM puserItem = NULL;
    
    if (strcmp(argv[0], "pro") == 0) {
        if (btMeshCmdPriv.meshMode != BT_MESH_PROVISIONER) {
            printf("[BT_MESH] Currently mode is not provisioner\r\n");
            return;
        }
    } else if (strcmp(argv[0], "dev") == 0) {
        if (btMeshCmdPriv.meshMode != BT_MESH_DEVICE) {
            printf("[BT_MESH] Currently mode is not device\r\n");
            return;
        }
    }
    if (strcmp((const char *)argv[1], (const char *)"?") == 0) {
        mesh_user_cmd_list();
        return;
    }
    for (i = 0; i < sizeof(mesh_cmd_table) / sizeof(mesh_cmd_table[0]); i ++) {
		if (strcmp((const char *)argv[1], (const char *)(mesh_cmd_table[i].pcommand)) == 0) {
            if (argc > 2) {
                if (strcmp(argv[2], "?") == 0) {
                    data_uart_debug(mesh_cmd_table[i].poption);
                    data_uart_debug("  *");
                    data_uart_debug(mesh_cmd_table[i].phelp);
                    return;
                }
            }
            k = 0;
            memset(meshUserCmdLine, 0, sizeof(meshUserCmdLine));
            for (j = 1; j < argc; j ++) {
                if (strlen(argv[j]) < (USER_CMD_MAX_COMMAND_LINE - k)) {
                    memcpy(&meshUserCmdLine[k], argv[j], strlen(argv[j]));
                    k += strlen(argv[j]);
                    strcpy(&meshUserCmdLine[k++], " ");
                } else {
                    printf("[BT_MESH] No Enough buffer for user cmd\r\n");
                    return;
                }
            }
            puserItem = bt_mesh_alloc_hdl(USER_API_CMDLINE);
            if (!puserItem) {
                printf("[BT_MESH_DEMO] bt_mesh_alloc_hdl fail!\r\n");
        		return;
            }
            mesh_user_cmd_parse(puserItem->pparseValue, meshUserCmdLine);
            bt_mesh_set_user_cmd(mesh_cmd_table[i].meshCode, puserItem->pparseValue, NULL, puserItem);
			found = 1;
			break;
		}
	}
    if (!found) {
        printf("\r\n [BT_MESH] unknown command %s", argv[1]);
    }

    return;
}

void user_cmd_array2string(uint8_t *buf, unsigned int buflen, char *out)
{
    /* larger than 32 for "/0" */
    char strBuf[40] = {0};
	char pbuf[2];
	uint8_t i;

    if (buflen >16) {
        printf("\r\n [BT_MESH] input pbuf buflen %d is to large", buflen);
        return;
    }
	for(i = 0; i < buflen; i++)
	{
	    sprintf(pbuf, "%02X", buf[i]);
	    strncat(strBuf, pbuf, 2);
	}
	strncpy(out, strBuf, buflen * 2);
}

#endif
#endif
