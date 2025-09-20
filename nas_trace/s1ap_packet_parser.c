#include "s1ap_packet_parser.h"

static uint32_t previous_tsn = 0;
static bool first_packet = true;

#define TARGET_CLIENT_IP "127.0.0.2"
#define TARGET_SERVER_IP "127.0.1.1"


void handle_handover_preparation(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_handover_resource_allocation(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_handover_notification(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_path_switch_request(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_handover_cancel(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_e_rab_setup(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_e_rab_modify(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_e_rab_release(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_e_rab_release_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_initial_context_setup(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_paging(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_downlink_nas_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_initial_ue_message(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_uplink_nas_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_reset(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_error_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_nas_non_delivery_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_s1_setup(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_ue_context_release_request(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_downlink_s1cdma2000tunneling(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_uplink_s1cdma2000tunneling(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_ue_context_modification(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_ue_capability_info_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_ue_context_release(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_enb_status_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_mme_status_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_deactivate_trace(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_trace_start(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_trace_failure_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_enb_configuration_update(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_mme_configuration_update(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_location_reporting_control(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_location_reporting_failure_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_location_report(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_overload_start(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_overload_stop(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_write_replace_warning(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_enb_direct_information_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_mme_direct_information_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_private_message(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_enb_configuration_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_mme_configuration_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_cell_traffic_trace(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_kill(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_downlink_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_uplink_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_downlink_non_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_uplink_non_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_ue_radio_capability_match(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);
void handle_pws_restart_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this);

/* Function implementations */
void handle_handover_preparation(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing HandoverPreparation\n");
    /* Add your HandoverPreparation handling logic here */
}

void handle_handover_resource_allocation(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing HandoverResourceAllocation\n");
    /* Add your HandoverResourceAllocation handling logic here */
}

void handle_handover_notification(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing HandoverNotification\n");
    /* Add your HandoverNotification handling logic here */
}

void handle_path_switch_request(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing PathSwitchRequest\n");
    /* Add your PathSwitchRequest handling logic here */
}

void handle_handover_cancel(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing HandoverCancel\n");
    /* Add your HandoverCancel handling logic here */
}

void handle_e_rab_setup(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing E_RABSetup\n");
    /* Add your E_RABSetup handling logic here */
}

void handle_e_rab_modify(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing E_RABModify\n");
    /* Add your E_RABModify handling logic here */
}

void handle_e_rab_release(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing E_RABRelease\n");
    /* Add your E_RABRelease handling logic here */
}

void handle_e_rab_release_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing E_RABReleaseIndication\n");
    /* Add your E_RABReleaseIndication handling logic here */
}

void handle_initial_context_setup(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing InitialContextSetup\n");
    /* Add your InitialContextSetup handling logic here */
}

void handle_paging(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing Paging\n");
    /* Add your Paging handling logic here */
}

void handle_downlink_nas_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing downlinkNASTransport\n");
    /* Add your downlinkNASTransport handling logic here */
}

void handle_initial_ue_message(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{

    printf("\n\n");

    int idx = 2;
    if (s1ap_data[idx] != 0x40) {
        printf("initialUEMessage:: Criticality is not ignore\n");
    }

    idx++;
    uint8_t payload_len = s1ap_data[3];

    idx += 4; /* Skip IE Items NO. */
    uint16_t protocol_field = htons( *(uint16_t*)&s1ap_data[idx] );
    printf("%d --> %d", payload_len, protocol_field);

    printf("\n\n");

    if (id_eNB_UE_S1AP_ID == protocol_field) {
        uint32_t enb_ue_s1ap_id = 0x00;
        const int len_idx = idx + (int)(sizeof (uint16_t));
        uint16_t enb_ue_s1ap_id_len = htons( *(uint16_t*)&s1ap_data[len_idx] );
        const int enb_idx = len_idx + (int)(sizeof (uint16_t));
        memcpy(&enb_ue_s1ap_id, &s1ap_data[enb_idx], enb_ue_s1ap_id_len);
        if (enb_ue_s1ap_id_len == 2) {
            enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
        } else if (enb_ue_s1ap_id_len == 4) {
            enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
        }
        printf("ENB UE S1AP ID: %d\n\n", enb_ue_s1ap_id);
    }

    for (int i = 0; i < s1ap_length - 3; i++) {
        if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x1a && s1ap_data[i+2] == 0x00) {
            idx = i;
            break;
        }
    }


    typedef struct {
        uint8_t msb:4;
        uint8_t lsb:4;
    } data_t;


    data_t d = {0};
    memcpy(&d, &s1ap_data[idx + 5], sizeof(data_t));
    if (d.lsb == 1) {
        idx += 6;
    } else {}


    uint8_t nas_eps_mobility_management_message_type = *(uint8_t*)&s1ap_data[idx + 6];
    memcpy(&nas_eps_mobility_management_message_type, &s1ap_data[idx + 6], sizeof(uint8_t));
    if (nas_eps_mobility_management_message_type == 0x41) {
        printf("nas_eps_mobility_management_message_type 0x%02X\n", nas_eps_mobility_management_message_type);
    }else if (nas_eps_mobility_management_message_type == 0x48) {
        printf("nas_eps_mobility_management_message_type 0x%02X\n", nas_eps_mobility_management_message_type);
    }

    /*7*/

    uint8_t guti_type = *(uint8_t*)&s1ap_data[idx + 9];
    uint8_t type = (guti_type  & 0x07);

    if (type == 6) {
        uint32_t tmsi = 0x00;
        memcpy(&tmsi, &s1ap_data[idx + 16], sizeof(uint32_t));
        tmsi = htonl(tmsi);
        printf("guti: %d", tmsi);
    } else if (type == 1) {
        long long imsi = imsi_direct_to_long_long(&s1ap_data[idx + 9]);
        printf("imsi: %llu\n", imsi);
    }

    printf("\n\n");

    /* Add your initialUEMessage handling logic here */
}

void handle_uplink_nas_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing uplinkNASTransport\n");
    /* Add your uplinkNASTransport handling logic here */
}

void handle_reset(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing Reset\n");
    /* Add your Reset handling logic here */
}

void handle_error_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing ErrorIndication\n");
    /* Add your ErrorIndication handling logic here */
}

void handle_nas_non_delivery_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing NASNonDeliveryIndication\n");
    /* Add your NASNonDeliveryIndication handling logic here */
}

void handle_s1_setup(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing S1Setup\n");
    /* Add your S1Setup handling logic here */
}

void handle_ue_context_release_request(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing UEContextReleaseRequest\n");
    /* Add your UEContextReleaseRequest handling logic here */
}

void handle_downlink_s1cdma2000tunneling(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing DownlinkS1cdma2000tunneling\n");
    /* Add your DownlinkS1cdma2000tunneling handling logic here */
}

void handle_uplink_s1cdma2000tunneling(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing UplinkS1cdma2000tunneling\n");
    /* Add your UplinkS1cdma2000tunneling handling logic here */
}

void handle_ue_context_modification(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing UEContextModification\n");
    /* Add your UEContextModification handling logic here */
}

void handle_ue_capability_info_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing UECapabilityInfoIndication\n");
    /* Add your UECapabilityInfoIndication handling logic here */
}

void handle_ue_context_release(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing UEContextRelease\n");
    /* Add your UEContextRelease handling logic here */
}

void handle_enb_status_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing eNBStatusTransfer\n");
    /* Add your eNBStatusTransfer handling logic here */
}

void handle_mme_status_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing MMEStatusTransfer\n");
    /* Add your MMEStatusTransfer handling logic here */
}

void handle_deactivate_trace(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing DeactivateTrace\n");
    /* Add your DeactivateTrace handling logic here */
}

void handle_trace_start(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing TraceStart\n");
    /* Add your TraceStart handling logic here */
}

void handle_trace_failure_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing TraceFailureIndication\n");
    /* Add your TraceFailureIndication handling logic here */
}

void handle_enb_configuration_update(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing ENBConfigurationUpdate\n");
    /* Add your ENBConfigurationUpdate handling logic here */
}

void handle_mme_configuration_update(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing MMEConfigurationUpdate\n");
    /* Add your MMEConfigurationUpdate handling logic here */
}

void handle_location_reporting_control(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing LocationReportingControl\n");
    /* Add your LocationReportingControl handling logic here */
}

void handle_location_reporting_failure_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing LocationReportingFailureIndication\n");
    /* Add your LocationReportingFailureIndication handling logic here */
}

void handle_location_report(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing LocationReport\n");
    /* Add your LocationReport handling logic here */
}

void handle_overload_start(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing OverloadStart\n");
    /* Add your OverloadStart handling logic here */
}

void handle_overload_stop(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing OverloadStop\n");
    /* Add your OverloadStop handling logic here */
}

void handle_write_replace_warning(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing WriteReplaceWarning\n");
    /* Add your WriteReplaceWarning handling logic here */
}

void handle_enb_direct_information_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing eNBDirectInformationTransfer\n");
    /* Add your eNBDirectInformationTransfer handling logic here */
}

void handle_mme_direct_information_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing MMEDirectInformationTransfer\n");
    /* Add your MMEDirectInformationTransfer handling logic here */
}

void handle_private_message(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing PrivateMessage\n");
    /* Add your PrivateMessage handling logic here */
}

void handle_enb_configuration_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing eNBConfigurationTransfer\n");
    /* Add your eNBConfigurationTransfer handling logic here */
}

void handle_mme_configuration_transfer(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing MMEConfigurationTransfer\n");
    /* Add your MMEConfigurationTransfer handling logic here */
}

void handle_cell_traffic_trace(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing CellTrafficTrace\n");
    /* Add your CellTrafficTrace handling logic here */
}

void handle_kill(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing Kill\n");
    /* Add your Kill handling logic here */
}

void handle_downlink_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing downlinkUEAssociatedLPPaTransport\n");
    /* Add your downlinkUEAssociatedLPPaTransport handling logic here */
}

void handle_uplink_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing uplinkUEAssociatedLPPaTransport\n");
    /* Add your uplinkUEAssociatedLPPaTransport handling logic here */
}

void handle_downlink_non_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing downlinkNonUEAssociatedLPPaTransport\n");
    /* Add your downlinkNonUEAssociatedLPPaTransport handling logic here */
}

void handle_uplink_non_ue_associated_lppa_transport(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing uplinkNonUEAssociatedLPPaTransport\n");
    /* Add your uplinkNonUEAssociatedLPPaTransport handling logic here */
}

void handle_ue_radio_capability_match(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing UERadioCapabilityMatch\n");
    /* Add your UERadioCapabilityMatch handling logic here */
}

void handle_pws_restart_indication(const uint8_t *s1ap_data, int s1ap_length, bool *capture_this)
{
    printf("Processing PWSRestartIndication\n");
    /* Add your PWSRestartIndication handling logic here */
}

char* message_type_str(uint8_t code) {
    switch (code) {
    case id_HandoverPreparation: return "HandoverPreparation";
    case id_HandoverResourceAllocation: return "HandoverResourceAllocation";
    case id_HandoverNotification: return "HandoverNotification";
    case id_PathSwitchRequest: return "PathSwitchRequest";
    case id_HandoverCancel: return "HandoverCancel";
    case id_E_RABSetup: return "E_RABSetup";
    case id_E_RABModify: return "E_RABModify";
    case id_E_RABRelease: return "E_RABRelease";
    case id_E_RABReleaseIndication: return "E_RABReleaseIndication";
    case id_InitialContextSetup: return "InitialContextSetup";
    case id_Paging: return "Paging";
    case id_downlinkNASTransport: return "downlinkNASTransport";
    case id_initialUEMessage: return "initialUEMessage";
    case id_uplinkNASTransport: return "uplinkNASTransport";
    case id_Reset: return "Reset";
    case id_ErrorIndication: return "ErrorIndication";
    case id_NASNonDeliveryIndication: return "NASNonDeliveryIndication";
    case id_S1Setup: return "S1Setup";
    case id_UEContextReleaseRequest: return "UEContextReleaseRequest";
    case id_DownlinkS1cdma2000tunneling: return "DownlinkS1cdma2000tunneling";
    case id_UplinkS1cdma2000tunneling: return "UplinkS1cdma2000tunneling";
    case id_UEContextModification: return "UEContextModification";
    case id_UECapabilityInfoIndication: return "UECapabilityInfoIndication";
    case id_UEContextRelease: return "UEContextRelease";
    case id_eNBStatusTransfer: return "eNBStatusTransfer";
    case id_MMEStatusTransfer: return "MMEStatusTransfer";
    case id_DeactivateTrace: return "DeactivateTrace";
    case id_TraceStart: return "TraceStart";
    case id_TraceFailureIndication: return "TraceFailureIndication";
    case id_ENBConfigurationUpdate: return "ENBConfigurationUpdate";
    case id_MMEConfigurationUpdate: return "MMEConfigurationUpdate";
    case id_LocationReportingControl: return "LocationReportingControl";
    case id_LocationReportingFailureIndication: return "LocationReportingFailureIndication";
    case id_LocationReport: return "LocationReport";
    case id_OverloadStart: return "OverloadStart";
    case id_OverloadStop: return "OverloadStop";
    case id_WriteReplaceWarning: return "WriteReplaceWarning";
    case id_eNBDirectInformationTransfer: return "eNBDirectInformationTransfer";
    case id_MMEDirectInformationTransfer: return "MMEDirectInformationTransfer";
    case id_PrivateMessage: return "PrivateMessage";
    case id_eNBConfigurationTransfer: return "eNBConfigurationTransfer";
    case id_MMEConfigurationTransfer: return "MMEConfigurationTransfer";
    case id_CellTrafficTrace: return "CellTrafficTrace";
    case id_Kill: return "Kill";
    case id_downlinkUEAssociatedLPPaTransport: return "downlinkUEAssociatedLPPaTransport";
    case id_uplinkUEAssociatedLPPaTransport: return "uplinkUEAssociatedLPPaTransport";
    case id_downlinkNonUEAssociatedLPPaTransport: return "downlinkNonUEAssociatedLPPaTransport";
    case id_uplinkNonUEAssociatedLPPaTransport: return "uplinkNonUEAssociatedLPPaTransport";
    case id_UERadioCapabilityMatch: return "UERadioCapabilityMatch";
    case id_PWSRestartIndication: return "PWSRestartIndication";
    default: return "Unknown";
    }


}

static void parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length)
{
    if (s1ap_length < 2) return;

    uint8_t procedure_code = s1ap_data[1];

    bool capture_this = false;

    switch (procedure_code) {
    case id_HandoverPreparation:
        handle_handover_preparation(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_HandoverResourceAllocation:
        handle_handover_resource_allocation(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_HandoverNotification:
        handle_handover_notification(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_PathSwitchRequest:
        handle_path_switch_request(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_HandoverCancel:
        handle_handover_cancel(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_E_RABSetup:
        handle_e_rab_setup(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_E_RABModify:
        handle_e_rab_modify(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_E_RABRelease:
        handle_e_rab_release(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_E_RABReleaseIndication:
        handle_e_rab_release_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_InitialContextSetup:
        handle_initial_context_setup(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_Paging:
        handle_paging(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_downlinkNASTransport:
        handle_downlink_nas_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_initialUEMessage:
        handle_initial_ue_message(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_uplinkNASTransport:
        handle_uplink_nas_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_Reset:
        handle_reset(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_ErrorIndication:
        handle_error_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_NASNonDeliveryIndication:
        handle_nas_non_delivery_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_S1Setup:
        handle_s1_setup(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_UEContextReleaseRequest:
        handle_ue_context_release_request(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_DownlinkS1cdma2000tunneling:
        handle_downlink_s1cdma2000tunneling(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_UplinkS1cdma2000tunneling:
        handle_uplink_s1cdma2000tunneling(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_UEContextModification:
        handle_ue_context_modification(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_UECapabilityInfoIndication:
        handle_ue_capability_info_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_UEContextRelease:
        handle_ue_context_release(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_eNBStatusTransfer:
        handle_enb_status_transfer(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_MMEStatusTransfer:
        handle_mme_status_transfer(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_DeactivateTrace:
        handle_deactivate_trace(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_TraceStart:
        handle_trace_start(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_TraceFailureIndication:
        handle_trace_failure_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_ENBConfigurationUpdate:
        handle_enb_configuration_update(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_MMEConfigurationUpdate:
        handle_mme_configuration_update(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_LocationReportingControl:
        handle_location_reporting_control(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_LocationReportingFailureIndication:
        handle_location_reporting_failure_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_LocationReport:
        handle_location_report(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_OverloadStart:
        handle_overload_start(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_OverloadStop:
        handle_overload_stop(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_WriteReplaceWarning:
        handle_write_replace_warning(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_eNBDirectInformationTransfer:
        handle_enb_direct_information_transfer(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_MMEDirectInformationTransfer:
        handle_mme_direct_information_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_PrivateMessage:
        handle_private_message(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_eNBConfigurationTransfer:
        handle_enb_configuration_transfer(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_MMEConfigurationTransfer:
        handle_mme_configuration_transfer(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_CellTrafficTrace:
        handle_cell_traffic_trace(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_Kill:
        handle_kill(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_downlinkUEAssociatedLPPaTransport:
        handle_downlink_ue_associated_lppa_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_uplinkUEAssociatedLPPaTransport:
        handle_uplink_ue_associated_lppa_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_downlinkNonUEAssociatedLPPaTransport:
        handle_downlink_non_ue_associated_lppa_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_uplinkNonUEAssociatedLPPaTransport:
        handle_uplink_non_ue_associated_lppa_transport(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_UERadioCapabilityMatch:
        handle_ue_radio_capability_match(s1ap_data, s1ap_length, &capture_this);
        break;
    case id_PWSRestartIndication:
        handle_pws_restart_indication(s1ap_data, s1ap_length, &capture_this);
        break;
    default:
        break;
    }


}

int is_s1ap_packet(const unsigned char *packet, int length, bool* is_retrans)
{
    *is_retrans = false;

    /* Check minimum size for Ethernet + IP + SCTP headers */
    if (length < (int)(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(sctp_common_header_t))) {
        return 0;
    }

    /* Check Ethernet type (IP) */
    const struct ethhdr *eth = (const struct ethhdr *)packet;
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return 0;
    }

    /* Check IP protocol (SCTP - protocol number 132) */
    const struct iphdr *ip = (const struct iphdr *)(packet + sizeof(struct ethhdr));
    if (ip->protocol != SCTP_PROTOCOL_NO) {  // SCTP protocol number
        return 0;
    }

    /* Check IP header length */
    int ip_header_len = ip->ihl * 4;
    if (length < (int)(sizeof(struct ethhdr) + ip_header_len + sizeof(sctp_common_header_t))) {
        return 0;
    }

    /* Convert IPs to strings for verification */
    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);

    // Check if this is our target S1AP traffic
    int is_target_traffic = 0;
    if ((strcmp(src_ip, TARGET_CLIENT_IP) == 0 && strcmp(dst_ip, TARGET_SERVER_IP) == 0) ||
            (strcmp(src_ip, TARGET_SERVER_IP) == 0 && strcmp(dst_ip, TARGET_CLIENT_IP) == 0)) {
        is_target_traffic = 1;
    }

    if (!is_target_traffic) {
        return 0;
    }

    /* Now parse SCTP chunks to find S1AP data */
    const unsigned char *sctp_start = packet + sizeof(struct ethhdr) + ip_header_len;
    unsigned long long sctp_length = length - (sizeof(struct ethhdr) + ip_header_len);

    if (sctp_length < (int)sizeof(sctp_common_header_t)) {
        return 0;
    }

    /* Get SCTP common header */
    //    const sctp_common_header_t *sctp_common = (const sctp_common_header_t*)sctp_start;
    //    uint16_t src_port = ntohs(sctp_common->source_port);
    //    uint16_t dst_port = ntohs(sctp_common->dest_port);


    /* Pointer to first chunk after common header */
    const unsigned char *chunk_ptr = sctp_start + sizeof(sctp_common_header_t);
    unsigned long long remaining_length = sctp_length - sizeof(sctp_common_header_t);

    bool has_s1ap = false;
    //    bool is_client_to_server = (strcmp(src_ip, TARGET_CLIENT_IP) == 0);
    //    const char* direction = is_client_to_server ? "Client->Server" : "Server->Client";

    while (remaining_length >= (int)sizeof(sctp_chunk_header_t)) {
        const sctp_chunk_header_t *chunk = (const sctp_chunk_header_t*)chunk_ptr;
        uint16_t chunk_length = ntohs(chunk->length);

        if (chunk_length == 0 || chunk_length > (uint16_t)remaining_length) {
            break;  /* Invalid chunk length */
        }

        if (chunk->type == 0) {
            if (chunk_length >= sizeof(sctp_data_chunk_t)) {
                const sctp_data_chunk_t *data_chunk = (const sctp_data_chunk_t*)chunk_ptr;
                uint32_t ppid = ntohl(data_chunk->protocol_id);
                uint32_t tsn = ntohl(data_chunk->tsn);

                if (ppid == SCTP_PPID_S1AP) {
                    has_s1ap = true;

                    /* Check for retransmission */
                    if (!first_packet && tsn == previous_tsn) {
                        *is_retrans = true;
                    }

                    previous_tsn = tsn;
                    first_packet = false;

                    /* Calculate S1AP payload position and length */
                    int s1ap_payload_offset = sizeof(sctp_data_chunk_t);
                    int s1ap_payload_length = chunk_length - s1ap_payload_offset;

                    if (s1ap_payload_length > 0) {
                        const uint8_t *s1ap_data = chunk_ptr + s1ap_payload_offset;

                        if (!(*is_retrans)) {
                            parse_s1ap_message(s1ap_data, s1ap_payload_length);
                        }
                    }
                }
            }
        }

        /* Move to next chunk (chunk length is padded to 4 bytes) */
        int padded_length = (chunk_length + 3) & ~3;
        chunk_ptr += padded_length;
        remaining_length -= padded_length;

        if (remaining_length <= 0) {
            break;
        }
    }

    return has_s1ap ? 1 : 0;
}
