#include "GMServantImp.h"
#include "servant/Application.h"
#include "globe.h"
#include "LogComm.h"
#include "CommonStruct.h"
#include "CommonCode.h"
#include "JFGameHttpProto.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
#include "Define.h"
#include <functional>
#include "./utilityFunction.h"

#include "./player_data_modify/player_data_modify_insert_update.h"
#include "./player_data_modify/player_data_modify_list.h"
#include "./player_data_modify/player_data_modify_white_list.h"
#include "./player_data_modify/player_bag_modify_insert_update.h"

#include "./global_email/global_email_delete.h"
#include "./global_email/global_email_detail.h"
#include "./global_email/global_email_insert_update.h"
#include "./global_email/global_email_list.h"
#include "./global_email/global_message_insert.h"

#include "./personal_email/personal_email_delete.h"
#include "./personal_email/personal_email_detail.h"
#include "./personal_email/personal_email_insert_update.h"
#include "./personal_email/personal_email_list.h"

#include "./marquee/marquee_delete.h"
#include "./marquee/marquee_detail.h"
#include "./marquee/marquee_insert_update.h"
#include "./marquee/marquee_list.h"

#include "./system_info_config/system_info_config_delete.h"
#include "./system_info_config/system_info_config_detail.h"
#include "./system_info_config/system_info_config_insert_update.h"
#include "./system_info_config/system_info_config_list.h"

#include "./rank_board_config/rank_board_config_delete.h"
#include "./rank_board_config/rank_board_config_insert_update.h"
#include "./rank_board_config/rank_board_config_list.h"

#include "./service_charge/service_charge_insert_update.h"
#include "./service_charge/service_charge_list.h"

#include "./match_basic_config/match_basic_config_delete.h"
#include "./match_basic_config/match_basic_config_detail.h"
#include "./match_basic_config/match_basic_config_insert_update.h"
#include "./match_basic_config/match_basic_config_list.h"

#include "./match_rule_config/match_rule_config_delete.h"
#include "./match_rule_config/match_rule_config_insert_update.h"
#include "./match_rule_config/match_rule_config_list.h"

#include "./match_reward_config/match_reward_config_delete.h"
#include "./match_reward_config/match_reward_config_insert_update.h"
#include "./match_reward_config/match_reward_config_list.h"

#include "./certain_victory_config/certain_victory_config_delete.h"
#include "./certain_victory_config/certain_victory_config_detail.h"
#include "./certain_victory_config/certain_victory_config_insert_update.h"
#include "./certain_victory_config/certain_victory_config_list.h"

#include "./props_config/props_config_delete.h"
#include "./props_config/props_config_insert_update.h"
#include "./props_config/props_config_list.h"

#include "./reward_pool_manager/reward_pool_manager_delete.h"
#include "./reward_pool_manager/reward_pool_manager_insert_update.h"
#include "./reward_pool_manager/reward_pool_manager_list.h"

#include "./props_reward_config/props_reward_config_delete.h"
#include "./props_reward_config/props_reward_config_insert_update.h"
#include "./props_reward_config/props_reward_config_list.h"
#include "./arrange_card/arrange_card.h"

#include "./mall/mall_delete.h"
#include "./mall/mall_insert_update.h"
#include "./mall/mall_list.h"

#include "./order/order_create.h"
#include "./order/order_update.h"

#include "./db_config/db_config_reload.h"
#include "./db_config/sysmail_reload.h"

#include "./exchangecode_change/exchangecode_update.h"

#include "./online_info/online_info_user_list.h"

#include "./ads_notify/ads_google.h"
#include "./ads_notify/ads_facebook.h"
#include "./ads_notify/ads_unity.h"

#include "./goods_config/goods_config_insert_update.h"

#include "./server_notify/server_update_notify.h"

#include "user_info_modify/modify_user_reg_time.h"
#include "user_info_modify/modify_user_safe_mail.h"

#include "user_club_modify/create_club.h"
#include "user_club_modify/modify_club.h"
#include "user_club_modify/audit_club.h"
#include "user_club_modify/change_position_club.h"
#include "user_club_modify/kick_member_club.h"
#include "user_club_modify/check_cut_club.h"
#include "user_club_modify/take_cut_club.h"
#include "user_club_modify/dissolve_club.h"
#include "user_club_modify/create_table_club.h"
#include "user_club_modify/change_club_balance.h"
#include "user_club_modify/change_club_fund.h"
#include "user_club_modify/change_club_merchant_level.h"

#include "user_union/union_create.h"
#include "user_union/union_change_name.h"
#include "user_union/union_upgrade.h"
#include "user_union/union_dissolve.h"
#include "user_union/union_audit.h"
#include "user_union/union_kick.h"

#include "system_activity/create_activity.h"
#include "system_activity/update_activity.h"


using namespace std;
using namespace JFGame;
using namespace DaqiGame;
using namespace JFGameHttpProto;
using namespace rapidjson;

//////////////////////////////////////////////////////
void GMServantImp::initialize()
{
    //initialize servant here:
    //...
    initHandlers();
}

//////////////////////////////////////////////////////
void GMServantImp::destroy()
{
    //destroy servant here:
    //...
}

//http请求处理接口 TODO handler里面出错直接抛出异常,这里打印异常信息
tars::Int32 GMServantImp::doRequest(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf, tars::TarsCurrentPtr current)
{
    FUNC_ENTRY("");
    int iRet = 0;

    try
    {
        string postData(reqBuf.begin(), reqBuf.end());
        ROLLLOG_DEBUG << "req json: " << postData << endl;

        GMRequest req;
        req.Deserialize(postData);

        vector<tars::Char> params;
        req.getParams(params);

        int code = req.getCode();
        ROLLLOG_DEBUG << "code: " << code << endl;
        auto it = this->handlers.find(code);
        ROLLLOG_DEBUG << "size: " << this->handlers.size() << endl;
        if (it == this->handlers.end())
            ROLLLOG_DEBUG << "----end: " << endl;
            //throw logic_error("code not exsist: " + I2S(code));

        (it->second)(reqBuf, extraInfo, rspBuf);
        ROLLLOG_DEBUG << "code : " << code << " rspBuf: " << string(rspBuf.begin(), rspBuf.end()) << endl;
    }
    catch(std::exception &e)
    {
        GMResponse rsp(0, e.what(), "", 0, 0);
        std::string resultJson;
        rsp.toString(resultJson);
        rspBuf.assign(resultJson.begin(), resultJson.end());
    }

    FUNC_EXIT("", iRet);
    return iRet;
}

tars::Int32 GMServantImp::getCards(gm::GetCardsResp &resp, tars::TarsCurrentPtr current)
{
    return ArrageCard::getCards(resp);
}

int GMServantImp::onJsonRequest()
{
    FUNC_ENTRY("");

    int iRet = 0;

    //__TRY__

    //const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
    //   //printf("Original JSON:\n %s\n", json);

    //Document document;

    //if (document.Parse(json).HasParseError())
    //{
    //  return 1;
    //}

    ////printf("\nAccess values in document:\n");
    //ROLLLOG_DEBUG << "Access values in document : " << endl;
    ////assert(document.IsObject());

    ////assert(document.HasMember("hello"));
    //   //assert(document["hello"].IsString());E_RANBOARD_INSRET_UPDATE

    //ROLLLOG_DEBUG << "assert, " << document.IsObject() << ", " << document.HasMember("hello") << ", " << document["hello"].IsString() << endl;
    //
    ////printf("hello = %s\n", document["hello"].GetString());
    //ROLLLOG_DEBUG << "hello = " << document["hello"].GetString() << endl;

    //__CATCH__

    //FUNC_EXIT("", iRet);

    return iRet;
}

// 这部分的代码是用Python脚本生成的
void GMServantImp::initHandlers()
{
    this->handlers[gm::E_PLAYER_DATA_MODIFY_LIST] = std::bind(&PlayerDataModifyListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PLAYER_DATA_MODIFY_INSERT_UPDATE] = std::bind(&PlayerDataModifyInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PLAYER_DATA_MODIFY_WHITE_LIST] = std::bind(&PlayerDataModifyWhiteListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PLAYER_BAG_MODIFY_INSERT_UPDATE] = std::bind(&PlayerBagModifyInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GLOBAL_EMAIL_LIST] = std::bind(&GlobalEmailListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GLOBAL_EMAIL_INSERT_UPDATE] = std::bind(&GlobalEmailInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GLOBAL_EMAIL_DETAIL] = std::bind(&GlobalEmailDetailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GLOBAL_EMAIL_DELETE] = std::bind(&GlobalEmailDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GLOBAL_EMAIL_CONFIG_RELOAD] = std::bind(&GlobalMailReloadConfigResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GLOBAL_MESSAGE_INSERT] = std::bind(&GlobalMessageInsertResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PERSONAL_EMAIL_LIST] = std::bind(&PersonalEmailListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PERSONAL_EMAIL_INSERT_UPDATE] = std::bind(&PersonalEmailInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PERSONAL_EMAIL_DETAIL] = std::bind(&PersonalEmailDetailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PERSONAL_EMAIL_DELETE] = std::bind(&PersonalEmailDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MARQUEE_LIST] = std::bind(&MarqueeListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MARQUEE_INSERT_UPDATE] = std::bind(&MarqueeInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MARQUEE_DETAIL] = std::bind(&MarqueeDetailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MARQUEE_DELETE] = std::bind(&MarqueeDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_SYS_INFO_CONFIG_LIST] = std::bind(&SysInfoConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_SYS_INFO_CONFIG_INSERT_UPDATE] = std::bind(&SystemInfoConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_SYS_INFO_CONFIG_DETAIL] = std::bind(&SysInfoConfigDetailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_SYS_INFO_CONFIG_DELETE] = std::bind(&SysInfoConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_RANBOARD_LIST] = std::bind(&RankBoardConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_RANBOARD_INSRET_UPDATE] = std::bind(&RankBoardConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_RANBOARD_DELETE] = std::bind(&RanboardDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_SERVICE_CHARGE_LIST] = std::bind(&ServiceChargeListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_SERVICE_CHARGE_INSERT_UPDATE] = std::bind(&ServiceChargeInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_BASIC_CONFIG_LIST] = std::bind(&MatchBasicConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_BASIC_CONFIG_INSERT_UPDATE] = std::bind(&MatchBasicConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_BASIC_CONFIG_DETAIL] = std::bind(&MatchBasicConfigDetailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_BASIC_CONFIG_DELETE] = std::bind(&MatchBasicConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_RULE_CONFIG_LIST] = std::bind(&MatchRuleConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_RULE_CONFIG_INSERT_UPDATE] = std::bind(&MatchRuleConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_RULE_CONFIG_DELETE] = std::bind(&MatchRuleConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_REWARD_CONFIG_LIST] = std::bind(&MatchRewardConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_REWARD_CONFIG_INSERT_UPDATE] = std::bind(&MatchRewardConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MATCH_REWARD_CONFIG_DELETE] = std::bind(&MatchRewardConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_CERTAIN_VICTORY_CONFIG_LIST] = std::bind(&CertainVictoryConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_CERTAIN_VICTORY_CONFIG_INSERT_UPDATE] = std::bind(&CertainVictoryConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_CERTAIN_VICTORY_CONFIG_DETAIL] = std::bind(&CertainVictoryConfigDetailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_CERTAIN_VICTORY_CONFIG_DELETE] = std::bind(&CertainVictoryConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MALL_LIST] = std::bind(&MallListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MALL_INSERT_UPDATE] = std::bind(&MallInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MALL_DELETE] = std::bind(&MallDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ORDER_CREATE] = std::bind(&OrderCreateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ORDER_UPDATE] = std::bind(&OrderUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PROPS_CONFIG_LIST] = std::bind(&PropsConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PROPS_CONFIG_INSERT_UPDATE] = std::bind(&PropsConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PROPS_CONFIG_DELETE] = std::bind(&PropsConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_AI_CONFIG_LIST] = std::bind(&AiConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_AI_CONFIG_INSERT_UPDATE] = std::bind(&AiConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    //this->handlers[gm::E_AI_CONFIG_DELETE] = std::bind(&AiConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_REWARD_POOL_MANAGER_LIST] = std::bind(&RewardPoolManagerListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_REWARD_POOL_MANAGER_INSERT_UPDATE] = std::bind(&RewardPoolManagerInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_REWARD_POOL_MANAGER_DELETE] = std::bind(&RewardPoolManagerDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PROPS_REWARD_CONFIG_LIST] = std::bind(&PropsRewardConfigListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PROPS_REWARD_CONFIG_INSERT_UPDATE] = std::bind(&PropsRewardConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_PROPS_REWARD_CONFIG_DELETE] = std::bind(&PropsRewardConfigDeleteResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_GOODS_CONFIG_INSERT_UPDATE] = std::bind(&GoodsConfigInsertUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_DB_CONFIG_RELOAD] = std::bind(&DBConfigReloadResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ONLINE_INFO_LIST] = std::bind(&OnlineInfoUserListResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ARRANGE_CARD] = std::bind(&ArrageCard::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ADS_GOOGLE_NOTIFY] = std::bind(&AdsGoogleNotify::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ADS_FACEBOOK_NOTIFY] = std::bind(&AdsFaceBookNotify::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_ADS_UNITY_NOTIFY] = std::bind(&AdsUnityNotify::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_SERVER_UPDATE_NOTIFY] = std::bind(&ServerUpdateNotifyResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_EXCHANGECODE_UPDATE] = std::bind(&ExchangeCodeUpdateResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MODIFY_USER_INFO_REGISTER_TIME] = std::bind(&ModifyUserRegTimeResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_MODIFY_USER_INFO_SAFE_MAIL] = std::bind(&ModifySafeMailResponse::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_MODIFY_CREATE_CLUB] = std::bind(&CreateClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_MODIFY_CLUB] = std::bind(&ModifyClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_AUDIT_CLUB] = std::bind(&AuditClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_CHANGE_POSITION_CLUB] = std::bind(&ChangePositionClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_KICK_MEMBER_CLUB] = std::bind(&KickMemberClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_CHECK_CUT_CLUB] = std::bind(&CheckCutClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_TAKE_CUT_CLUB] = std::bind(&TakeCutClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_DISSOLVE_CLUB] = std::bind(&DissolveClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_CREATE_TABLE_CLUB] = std::bind(&CreateTableClubRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_CHANGE_BALANCE] = std::bind(&ChangeClubBalanceRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_CHANGE_FUND] = std::bind(&ChangeClubFundRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_CLUB_CHANGE_MERCHANT_LEVEL] = std::bind(&ChangeClubMerchantLevelRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_UNION_CREATE] = std::bind(&UnionCreateRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_UNION_CHANGE_NAME] = std::bind(&UnionChangeNameRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_UNION_UPGRADE] = std::bind(&UnionUpgradeRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_UNION_DISSOLVE] = std::bind(&UnionDissolveRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_UNION_AUDIT] = std::bind(&UnionAuditRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_USER_UNION_KICK] = std::bind(&UnionKickRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_SYS_ACTIVITY_INSERT] = std::bind(&CreateSystemActivityRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    this->handlers[gm::E_SYS_ACTIVITY_UPDATE] = std::bind(&UpdateSystemActivityRespons::handler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

