#include "s1ap_module.h"
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <netinet/ip.h>
#include <iostream>
#include <cstdio>
#include <arpa/inet.h>



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdbool.h>


typedef enum {
    id_HandoverPreparation = 0,
    id_HandoverResourceAllocation = 1,
    id_HandoverNotification = 2,
    id_PathSwitchRequest = 3,
    id_HandoverCancel = 4,
    id_E_RABSetup = 5,
    id_E_RABModify = 6,
    id_E_RABRelease = 7,
    id_E_RABReleaseIndication = 8,
    id_InitialContextSetup = 9,
    id_Paging = 10,
    id_downlinkNASTransport = 11,
    id_initialUEMessage = 12,
    id_uplinkNASTransport = 13,
    id_Reset = 14,
    id_ErrorIndication = 15,
    id_NASNonDeliveryIndication = 16,
    id_S1Setup = 17,
    id_UEContextReleaseRequest = 18,
    id_DownlinkS1cdma2000tunneling = 19,
    id_UplinkS1cdma2000tunneling = 20,
    id_UEContextModification = 21,
    id_UECapabilityInfoIndication = 22,
    id_UEContextRelease = 23,
    id_eNBStatusTransfer = 24,
    id_MMEStatusTransfer = 25,
    id_DeactivateTrace = 26,
    id_TraceStart = 27,
    id_TraceFailureIndication = 28,
    id_ENBConfigurationUpdate = 29,
    id_MMEConfigurationUpdate = 30,
    id_LocationReportingControl = 31,
    id_LocationReportingFailureIndication = 32,
    id_LocationReport = 33,
    id_OverloadStart = 34,
    id_OverloadStop = 35,
    id_WriteReplaceWarning = 36,
    id_eNBDirectInformationTransfer = 37,
    id_MMEDirectInformationTransfer = 38,
    id_PrivateMessage = 39,
    id_eNBConfigurationTransfer = 40,
    id_MMEConfigurationTransfer = 41,
    id_CellTrafficTrace = 42,
    id_Kill = 43,
    id_downlinkUEAssociatedLPPaTransport = 44,
    id_uplinkUEAssociatedLPPaTransport = 45,
    id_downlinkNonUEAssociatedLPPaTransport = 46,
    id_uplinkNonUEAssociatedLPPaTransport = 47,
    id_UERadioCapabilityMatch = 48,
    id_PWSRestartIndication = 49
} ProcedureCode;


typedef enum {
    id_MME_UE_S1AP_ID = 0,
    id_HandoverType = 1,
    id_Cause = 2,
    id_SourceID = 3,
    id_TargetID = 4,
    id_eNB_UE_S1AP_ID = 8,
    id_E_RABSubjecttoDataForwardingList = 12,
    id_E_RABtoReleaseListHOCmd = 13,
    id_E_RABDataForwardingItem = 14,
    id_E_RABReleaseItemBearerRelComp = 15,
    id_E_RABToBeSetupListBearerSUReq = 16,
    id_E_RABToBeSetupItemBearerSUReq = 17,
    id_E_RABAdmittedList = 18,
    id_E_RABFailedToSetupListHOReqAck = 19,
    id_E_RABAdmittedItem = 20,
    id_E_RABFailedtoSetupItemHOReqAck = 21,
    id_E_RABToBeSwitchedDLList = 22,
    id_E_RABToBeSwitchedDLItem = 23,
    id_E_RABToBeSetupListCtxtSUReq = 24,
    id_TraceActivation = 25,
    id_NAS_PDU = 26,
    id_E_RABToBeSetupItemHOReq = 27,
    id_E_RABSetupListBearerSURes = 28,
    id_E_RABFailedToSetupListBearerSURes = 29,
    id_E_RABToBeModifiedListBearerModReq = 30,
    id_E_RABModifyListBearerModRes = 31,
    id_E_RABFailedToModifyList = 32,
    id_E_RABToBeReleasedList = 33,
    id_E_RABFailedToReleaseList = 34,
    id_E_RABItem = 35,
    id_E_RABToBeModifiedItemBearerModReq = 36,
    id_E_RABModifyItemBearerModRes = 37,
    id_E_RABReleaseItem = 38,
    id_E_RABSetupItemBearerSURes = 39,
    id_SecurityContext = 40,
    id_HandoverRestrictionList = 41,
    id_UEPagingID = 43,
    id_pagingDRX = 44,
    id_TAIList = 46,
    id_TAIItem = 47,
    id_E_RABFailedToSetupListCtxtSURes = 48,
    id_E_RABReleaseItemHOCmd = 49,
    id_E_RABSetupItemCtxtSURes = 50,
    id_E_RABSetupListCtxtSURes = 51,
    id_E_RABToBeSetupItemCtxtSUReq = 52,
    id_E_RABToBeSetupListHOReq = 53,
    id_GERANtoLTEHOInformationRes = 55,
    id_UTRANtoLTEHOInformationRes = 57,
    id_CriticalityDiagnostics = 58,
    id_Global_ENB_ID = 59,
    id_eNBname = 60,
    id_MMEname = 61,
    id_ServedPLMNs = 63,
    id_SupportedTAs = 64,
    id_TimeToWait = 65,
    id_uEaggregateMaximumBitrate = 66,
    id_TAI = 67,
    id_E_RABReleaseListBearerRelComp = 69,
    id_cdma2000PDU = 70,
    id_cdma2000RATType = 71,
    id_cdma2000SectorID = 72,
    id_SecurityKey = 73,
    id_UERadioCapability = 74,
    id_GUMMEI_ID = 75,
    id_E_RABInformationListItem = 78,
    id_Direct_Forwarding_Path_Availability = 79,
    id_UEIdentityIndexValue = 80,
    id_cdma2000HOStatus = 83,
    id_cdma2000HORequiredIndication = 84,
    id_E_UTRAN_Trace_ID = 86,
    id_RelativeMMECapacity = 87,
    id_SourceMME_UE_S1AP_ID = 88,
    id_Bearers_SubjectToStatusTransfer_Item = 89,
    id_eNB_StatusTransfer_TransparentContainer = 90,
    id_UE_associatedLogicalS1_ConnectionItem = 91,
    id_ResetType = 92,
    id_UE_associatedLogicalS1_ConnectionListResAck = 93,
    id_E_RABToBeSwitchedULItem = 94,
    id_E_RABToBeSwitchedULList = 95,
    id_S_TMSI = 96,
    id_cdma2000OneXRAND = 97,
    id_RequestType = 98,
    id_UE_S1AP_IDs = 99,
    id_EUTRAN_CGI = 100,
    id_OverloadResponse = 101,
    id_cdma2000OneXSRVCCInfo = 102,
    id_E_RABFailedToBeReleasedList = 103,
    id_Source_ToTarget_TransparentContainer = 104,
    id_ServedGUMMEIs = 105,
    id_SubscriberProfileIDforRFP = 106,
    id_UESecurityCapabilities = 107,
    id_CSFallbackIndicator = 108,
    id_CNDomain = 109,
    id_E_RABReleasedList = 110,
    id_MessageIdentifier = 111,
    id_SerialNumber = 112,
    id_WarningAreaList = 113,
    id_RepetitionPeriod = 114,
    id_NumberofBroadcastRequest = 115,
    id_WarningType = 116,
    id_WarningSecurityInfo = 117,
    id_DataCodingScheme = 118,
    id_WarningMessageContents = 119,
    id_BroadcastCompletedAreaList = 120,
    id_Inter_SystemInformationTransferTypeEDT = 121,
    id_Inter_SystemInformationTransferTypeMDT = 122,
    id_Target_ToSource_TransparentContainer = 123,
    id_SRVCCOperationPossible = 124,
    id_SRVCCHOIndication = 125,
    id_NAS_DownlinkCount = 126,
    id_CSG_Id = 127,
    id_CSG_IdList = 128,
    id_SONConfigurationTransferECT = 129,
    id_SONConfigurationTransferMCT = 130,
    id_TraceCollectionEntityIPAddress = 131,
    id_MSClassmark2 = 132,
    id_MSClassmark3 = 133,
    id_RRC_Establishment_Cause = 134,
    id_NASSecurityParametersfromE_UTRAN = 135,
    id_NASSecurityParameterstoE_UTRAN = 136,
    id_DefaultPagingDRX = 137,
    id_Source_ToTarget_TransparentContainer_Secondary = 138,
    id_Target_ToSource_TransparentContainer_Secondary = 139,
    id_EUTRANRoundTripDelayEstimationInfo = 140,
    id_BroadcastCancelledAreaList = 141,
    id_ConcurrentWarningMessageIndicator = 142,
    id_Data_Forwarding_Not_Possible = 143,
    id_ExtendedRepetitionPeriod = 144,
    id_CellAccessMode = 145,
    id_CSGMembershipStatus = 146,
    id_LPPa_PDU = 147,
    id_Routing_ID = 148,
    id_Time_Synchronization_Info = 149,
    id_PS_ServiceNotAvailable = 150,
    id_PagingPriority = 151,
    id_x2TNLConfigurationInfo = 152,
    id_eNBX2ExtendedTransportLayerAddresses = 153,
    id_GUMMEIList = 154,
    id_GW_TransportLayerAddress = 155,
    id_Correlation_ID = 156,
    id_SourceMME_GUMMEI = 157,
    id_MME_UE_S1AP_ID_2 = 158,
    id_RegisteredLAI = 159,
    id_RelayNode_Indicator = 160,
    id_TrafficLoadReductionIndication = 161,
    id_MDTConfiguration = 162,
    id_MMERelaySupportIndicator = 163,
    id_GWContextReleaseIndication = 164,
    id_ManagementBasedMDTAllowed = 165,
    id_PrivacyIndicator = 166,
    id_Time_UE_StayedInCell_EnhancedGranularity = 167,
    id_HO_Cause = 168,
    id_VoiceSupportMatchIndicator = 169,
    id_GUMMEIType = 170,
    id_M3Configuration = 171,
    id_M4Configuration = 172,
    id_M5Configuration = 173,
    id_MDT_Location_Info = 174,
    id_MobilityInformation = 175,
    id_Tunnel_Information_for_BBF = 176,
    id_ManagementBasedMDTPLMNList = 177,
    id_SignallingBasedMDTPLMNList = 178,
    id_ULCOUNTValueExtended = 179,
    id_DLCOUNTValueExtended = 180,
    id_ReceiveStatusOfULPDCPSDUsExtended = 181,
    id_ECGIListForRestart = 182,
    id_SIPTO_Correlation_ID = 183,
    id_SIPTO_L_GW_TransportLayerAddress = 184,
    id_TransportInformation = 185,
    id_LHN_ID = 186,
    id_AdditionalCSFallbackIndicator = 187,
    id_TAIListForRestart = 188,
    id_UserLocationInformation = 189,
    id_EmergencyAreaIDListForRestart = 190,
    id_KillAllWarningMessages = 191,
    id_Masked_IMEISV = 192,
    id_eNBIndirectX2TransportLayerAddresses = 193,
    id_uE_HistoryInformationFromTheUE = 194,
    id_ExpectedUEBehaviour = 196,
    id_LoggedMBMSMDT = 197,
    id_UERadioCapabilityForPaging = 198
} ProtocolIE_ID;

std::vector<trace_packet_pcap_t> s_trace_packet_pcap;
// Global connection states
std::map<std::string, sctp_connection_state> connection_states;
// packet_data_pcap_t m_packet_data_pcap[13];

typedef struct {
    uint8_t procedure_code;
    uint8_t criticality;
    uint32_t length;
    // Downlink NAS Transport specific fields
    uint16_t mme_ue_s1ap_id;
    uint16_t enb_ue_s1ap_id;
    uint8_t nas_pdu_length;
    uint8_t* nas_pdu; // Pointer to NAS PDU data
    uint8_t nas_pdu_buffer[256]; // Buffer for NAS PDU (adjust size as needed)
} s1ap_downlink_nas_transport_t;


static int parse_downlink_nas_transport(uint8_t* data, size_t len, s1ap_downlink_nas_transport_t* result) {
    size_t offset = 0;

    // Parse Procedure Code
    result->procedure_code = data[offset++];

    // Verify this is actually a DownlinkNASTransport message
    if (result->procedure_code != id_downlinkNASTransport) {
        return -1;
    }

    // Parse Criticality
    result->criticality = data[offset++];

    // Parse Length (2 bytes, big-endian)
    result->length = (data[offset] << 8) | data[offset + 1];
    offset += 2;

    // Verify the total length matches
    if (len != (offset + result->length)) {
        return -2;
    }

    // Parse Downlink NAS Transport specific fields
    // Adjust these offsets based on your actual S1AP message structure

    // Parse MME UE S1AP ID (2 bytes)
    if (offset + 2 <= len) {
        result->mme_ue_s1ap_id = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }

    // Parse eNB UE S1AP ID (2 bytes)
    if (offset + 2 <= len) {
        result->enb_ue_s1ap_id = (data[offset] << 8) | data[offset + 1];
        offset += 2;
    }

    // Parse NAS PDU length and data
    if (offset < len) {
        result->nas_pdu_length = data[offset++];

        // Copy NAS PDU data to buffer
        size_t nas_bytes_to_copy = (result->nas_pdu_length < sizeof(result->nas_pdu_buffer)) ?
                    result->nas_pdu_length : sizeof(result->nas_pdu_buffer) - 1;

        if (offset + nas_bytes_to_copy <= len) {
            memcpy(result->nas_pdu_buffer, &data[offset], nas_bytes_to_copy);
            result->nas_pdu_buffer[nas_bytes_to_copy] = '\0'; // Null-terminate for safety
            result->nas_pdu = result->nas_pdu_buffer;
            offset += nas_bytes_to_copy;
        } else {
            result->nas_pdu_length = 0;
            result->nas_pdu = nullptr;
        }
    }

    return 0;
}


std::string imsi_to_string(const unsigned char* packet_bytes) {
    imsi_structure_t* imsi = (imsi_structure_t*)packet_bytes;

    std::string imsi_str;
    int digit_count = 15; // Start with max digits

    // Handle odd/even length
    if (!imsi->odd_even) {
        digit_count = 14;
    }

    // Build the string digit by digit
    for (int i = 0; i < digit_count; i++) {
        uint8_t digit;
        switch (i) {
        case 0: digit = imsi->digit1; break;
        case 1: digit = imsi->digit2; break;
        case 2: digit = imsi->digit3; break;
        case 3: digit = imsi->digit4; break;
        case 4: digit = imsi->digit5; break;
        case 5: digit = imsi->digit6; break;
        case 6: digit = imsi->digit7; break;
        case 7: digit = imsi->digit8; break;
        case 8: digit = imsi->digit9; break;
        case 9: digit = imsi->digit10; break;
        case 10: digit = imsi->digit11; break;
        case 11: digit = imsi->digit12; break;
        case 12: digit = imsi->digit13; break;
        case 13: digit = imsi->digit14; break;
        case 14: digit = imsi->digit15; break;
        default: digit = 0;
        }
        imsi_str += std::to_string(digit);
    }

    return imsi_str;
}

long long imsi_direct_to_long_long(const unsigned char* packet_bytes) {
    imsi_structure_t* imsi = (imsi_structure_t*)packet_bytes;

    long long result = 0;
    int digit_count = 15; // Start with max digits

    // Handle odd/even length
    if (!imsi->odd_even) {
        digit_count = 14;
    }

    // std::string imsi_str("999999999999999"); int itr = 15;

    // Build the number digit by digit
    long long multiplier = 1;
    for (int i = digit_count - 1; i >= 0; i--) {
        uint8_t digit;
        switch (i) {
        case 0: digit = imsi->digit1; break;
        case 1: digit = imsi->digit2; break;
        case 2: digit = imsi->digit3; break;
        case 3: digit = imsi->digit4; break;
        case 4: digit = imsi->digit5; break;
        case 5: digit = imsi->digit6; break;
        case 6: digit = imsi->digit7; break;
        case 7: digit = imsi->digit8; break;
        case 8: digit = imsi->digit9; break;
        case 9: digit = imsi->digit10; break;
        case 10: digit = imsi->digit11; break;
        case 11: digit = imsi->digit12; break;
        case 12: digit = imsi->digit13; break;
        case 13: digit = imsi->digit14; break;
        case 14: digit = imsi->digit15; break;
        default: digit = 0;
        }
        // imsi_str.at(itr--) = digit;
        // printf("%d", digit);
        result += digit * multiplier;
        multiplier *= 10;
    }
    // printf("\n\n");
    // printf("%s", imsi_str.data());

    return result;
}

std::string get_connection_key(uint32_t src_ip, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port) {
    char key[64];
    snprintf(key, sizeof(key), "%u:%u->%u:%u", src_ip, src_port, dst_ip, dst_port);
    return std::string(key);
}

bool is_retransmission(const std::set<uint32_t>& seen_tsns, uint32_t tsn) {
    return seen_tsns.find(tsn) != seen_tsns.end();
}

void parse_nas_eps_message(const uint8_t *nas_data, int nas_length, const char* direction) {
    if (nas_length < 1) return;

    uint8_t message_type = nas_data[0];

    printf("  NAS-EPS Message Type: 0x%02X", message_type);

    switch (message_type) {
    case NAS_EPS_MSG_ATTACH_REQUEST:
        printf(" (Attach Request)\n");
        break;
    case NAS_EPS_MSG_IDENTITY_RESPONSE:
        printf(" (Identity Response)\n");
        break;
    default:
        printf(" (Unknown)\n");
        break;
    }

    // Parse NAS information elements
    int offset = 1; // Skip message type

    while (offset < nas_length) {
        uint8_t iei = nas_data[offset++];

        // printf("xxxxxxxxxxxxxxxxxxxx %d", iei);
        if (iei == 0x00) break; // No more IEs

        // For IMSI (IEI = 0x02)
        if (iei == NAS_EPS_IE_IMSI && offset + 8 <= nas_length) {
            printf("  IMSI found in %s direction\n", direction);

            // Extract IMSI bytes (typically 8 bytes for 15-digit IMSI)
            const unsigned char* imsi_bytes = &nas_data[offset];
            long long imsi_value = imsi_direct_to_long_long(imsi_bytes);

            printf("  IMSI: %lld\n", imsi_value);

            offset += 8; // Move past IMSI field
        } else {
            // Skip unknown IEs (advance offset)
            offset++;
        }
    }
}

void parse_plmn_id(const uint8_t *data, int length) {
    if (length < 3) {
        printf("  PLMN ID: Invalid length (%d bytes)\n", length);
        return;
    }

    const struct plmn_id *plmn = (const struct plmn_id *)data;

    // Extract MCC digits
    uint8_t mcc1 = plmn->mcc_digit1;
    uint8_t mcc2 = plmn->mcc_digit2;
    uint8_t mcc3 = plmn->mcc_digit3;

    // Extract MNC digits
    uint8_t mnc1 = plmn->mnc_digit1;
    uint8_t mnc2 = plmn->mnc_digit2;
    uint8_t mnc3 = plmn->mnc_digit3;

    printf("  PLMN ID: MCC=%d%d%d, MNC=%d%d%d\n",
           mcc1, mcc2, mcc3,
           mnc1, mnc2, (mnc3 == 0xF) ? 0 : mnc3);  // 0xF indicates 2-digit MNC
}

// std::map<std::string, uint32_t> map_imsi_tmsi;

std::string stored_imsi = "999990123456780";
uint32_t stored_tmsi = 0;
#include <bitset>

//struct eps_mobile_identity {
//    uint8_t __pad__ : 4;
//    uint8_t even_num_identity : 1;
//    uint8_t typeof_identity : 3;
//};

struct data_t {
    uint8_t msb:4;
    uint8_t lsb:4;
};


struct eps_type_t {
    uint8_t attach_type : 3;
    uint8_t spare_bit : 1;
    uint8_t nas_key_set_identifier : 3;
    uint8_t tsc : 1;
};


uint32_t should_trace_enb_ue_s1ap_id = 0;


void parse_s1ap_message(const uint8_t *s1ap_data, int s1ap_length, const char* direction, bool& is_cap, const unsigned char *packet, int packet_len) {
    if (s1ap_length < 2) return;

    static bool repeater = false;
    static bool capture_tmsi = false;
    static bool valid_imsi = true;
    repeater = !repeater;
    if (!repeater) return;

    struct timeval tv;
    gettimeofday(&tv, NULL);

    static int packet_id = 0;
    uint32_t enb_ue_s1ap_id = 0x00;

    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0c && s1ap_data[2] == 0x40) {



        int idx = 0;
        for (int i = 0; i < s1ap_length - 2; i++) {
            if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x08) {
                idx = i;
                break;
            }
        }

        uint16_t protocol_field = htons( *(uint16_t*)&s1ap_data[idx] );
        //        std::cout << "protocol_field: " << protocol_field << std::endl;

        if (protocol_field == 0x0008) {
            const int len_idx = idx + static_cast<int>(sizeof (uint16_t));
            uint16_t enb_ue_s1ap_id_len = htons( *(uint16_t*)&s1ap_data[len_idx] );

            const int enb_idx = len_idx + static_cast<int>(sizeof (uint16_t));


            memcpy(&enb_ue_s1ap_id, &s1ap_data[enb_idx], enb_ue_s1ap_id_len);


            if (enb_ue_s1ap_id_len == 2) {
                enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
            } else if (enb_ue_s1ap_id_len == 4) {
                enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
            }


            std::cout << "\n *********** ENB UE S1AP ID: " << enb_ue_s1ap_id  << '\n' << std::endl;

        }



        int idx2 = 0;
        for (int i = 0; i < s1ap_length - 3; i++) {
            if (s1ap_data[i] == 0x00 && s1ap_data[i+1] == 0x1a && s1ap_data[i+2] == 0x00) {
                idx2 = i;
                break;
            }
        }

        data_t d;
        memset(&d, 0, sizeof (data_t));
        memcpy(&d, &s1ap_data[idx2 + 5], sizeof(data_t));
        if (d.lsb == 0) {
            //            std::cout << "ok " << std::bitset<4>(d.msb) << ' ' << std::bitset<4>(d.lsb) << std::endl;
        } else if (d.lsb == 1) {
            //            std::cout << "should shift 6 byte " << std::bitset<4>(d.lsb) << std::endl;
            idx2 += 6;
        }


        uint8_t nas_eps_mobility_management_message_type = *(uint8_t*)&s1ap_data[idx2 + 6];
        memcpy(&nas_eps_mobility_management_message_type, &s1ap_data[idx2 + 6], sizeof(uint8_t));
        if (nas_eps_mobility_management_message_type == 0x41) {
            std::cout << "nas_eps_mobility_management_message_type 0x41" << nas_eps_mobility_management_message_type << std::endl;
        }else if (nas_eps_mobility_management_message_type == 0x48) {
            std::cout << "nas_eps_mobility_management_message_type 0x48" << nas_eps_mobility_management_message_type << std::endl;
        }

        //7

        uint8_t guti_type = *(uint8_t*)&s1ap_data[idx2 + 9];
        uint8_t type = (guti_type  & 0x07);

        if (type == 6) {
            uint32_t tmsi = 0x00;
            memcpy(&tmsi, &s1ap_data[idx2 + 16], sizeof(uint32_t));
            tmsi = htonl(tmsi);
            std::cout << "guti: " << tmsi << std::endl;

            if (0x00 == tmsi) {
                should_trace_enb_ue_s1ap_id = enb_ue_s1ap_id;
                is_cap = true;
            }

        } else if (type == 1) {
            std::string imsi_str = imsi_to_string(&s1ap_data[idx2 + 9]);
            std::cout << "imsi: " << imsi_str << std::endl;

            if ("999990123456780" == imsi_str) {
                should_trace_enb_ue_s1ap_id = enb_ue_s1ap_id;
                is_cap = true;
            }

        }

    } else {

        //                auto type = *(uint16_t*)&s1ap_data[7];
        ////                int idx = 0;
        //                if (type == 0) {
        //                    auto enb_ue_s1ap_id_len = *(uint8_t*)&s1ap_data[10];
        //                    uint32_t enb_ue_s1ap_id = 0x00;
        //                    memcpy(&enb_ue_s1ap_id, &s1ap_data[11], enb_ue_s1ap_id_len);
        //                    if (enb_ue_s1ap_id_len == 2) {
        //                        enb_ue_s1ap_id = htons(enb_ue_s1ap_id);
        //                    } else if (enb_ue_s1ap_id_len == 4) {
        //                        enb_ue_s1ap_id = htonl(enb_ue_s1ap_id);
        //                    }
        //                    std::cout << "Catch: " << enb_ue_s1ap_id << std::endl;
        ////                    idx = 11 + enb_ue_s1ap_id_len;
        //                }

        //                type = *(uint16_t*)&s1ap_data[idx];
        //                if (type == 8) {
        //                    idx++;
        //                    auto mme_ue_s1ap_id_len = *(uint8_t*)&s1ap_data[idx++];
        //                    uint32_t mme_ue_s1ap_id = 0x00;
        //                    memcpy(&mme_ue_s1ap_id, &s1ap_data[idx], mme_ue_s1ap_id_len);
        //                    if (mme_ue_s1ap_id_len == 2) {
        //                        mme_ue_s1ap_id = htons(mme_ue_s1ap_id);
        //                    } else if (mme_ue_s1ap_id_len == 4) {
        //                        mme_ue_s1ap_id = htonl(mme_ue_s1ap_id);
        //                    }
        //                } //test konammmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm

        uint8_t len = *(uint8_t*)&s1ap_data[10];
        uint32_t enb = 0x00;
        memcpy(&enb, &s1ap_data[11], len);
        if (len == 2) {
            enb = htons(enb);
        } else if (len == 4) {
            enb = htonl(enb);
        }
        uint16_t xxx =  *(uint16_t*)&s1ap_data[11] ;
        xxx = htons(xxx);

        uint16_t xxx2 =  *(uint16_t*)&s1ap_data[17] ;
        xxx2 = htons(xxx2);
        //        std::cout << "Catch: " << enb << ' ' << xxx << ' ' << xxx2 << std::endl;

        uint32_t message_type = 0;//;*(uint32_t*)&s1ap_data[0];

        auto capture = [&]() { if (should_trace_enb_ue_s1ap_id == xxx2) is_cap = true; };

        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x17) {
            printf("DownlinkNASTransport, Identity request: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[1], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
            printf("UplinkNASTransport, Identity response: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x38) {
            printf("DownlinkNASTransport, Authentication request: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
            printf("UplinkNASTransport, Authentication response: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x2c) {
            printf("DownlinkNASTransport, Security mode command: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3d) {
            printf("UplinkNASTransport, Security mode complete: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x1d) {
            printf("DownlinkNASTransport, ESM information request: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3e) {
            printf("UplinkNASTransport, ESM information response: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x80) {
            printf("InitialContextSetupRequest, Attach accept, Activate default EPS bearer context request: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x16 && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x24) {
            printf("UECapabilityInfoIndication, UECapabilityInformation: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x20 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x22) {
            printf("InitialContextSetupResponse, UplinkNASTransport, Attach complete, Activate default EPS bearer context accept: %d - %d - %d\n", message_type, xxx, xxx2);
            capture();
        }
        if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x45) {
            printf("DownlinkNASTransport, EMM information: %d - %d - %d\n", message_type, xxx, xxx2);
            s1ap_downlink_nas_transport_t dnt;
            int result_code = parse_downlink_nas_transport((uint8_t*)&s1ap_data[0], s1ap_length, &dnt);
            std::cout << "************ * //// " <<dnt.mme_ue_s1ap_id << ' ' << dnt.enb_ue_s1ap_id << ' ' << dnt.procedure_code << std::endl;
            capture();
        }

    }


    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x17) {
    //        printf("DownlinkNASTransport, Identity request: %d\n", message_type);


    //        int found_at = 0;
    //            for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //                if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                    found_at = i;
    //                    break;
    //                }
    //            }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
    //        printf("UplinkNASTransport, Identity response: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        auto imsi_str = imsi_to_string(&s1ap_data[27]);
    //        if (valid_imsi) {
    //            s_trace_packet_pcap.at(found_at).imsi = imsi_str;
    //        }

    //        valid_imsi  = false;
    //        capture_tmsi = true;

    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x38) {
    //        printf("DownlinkNASTransport, Authentication request: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x35) {
    //        printf("UplinkNASTransport, Authentication response: %d\n", message_type);
    //        int index = 4;
    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;


    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x2c) {
    //        int index = 5;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        printf("DownlinkNASTransport, Security mode command: %d\n", message_type);
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3d) {
    //        printf("UplinkNASTransport, Security mode complete: %d\n", message_type);
    //        int index = 6;
    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x1d) {
    //        int index = 7;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        printf("DownlinkNASTransport, ESM information request: %d\n", message_type);
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3e) {
    //        printf("UplinkNASTransport, ESM information response: %d\n", message_type);
    //        int index = 8;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x80) {
    //        printf("InitialContextSetupRequest, Attach accept, Activate default EPS bearer context request: %d\n", message_type);
    //        int index = 9;


    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;


    ////        uint32_t TMSI = ;
    ////        TMSI = htonl(TMSI);
    //        if (capture_tmsi) {
    //            s_trace_packet_pcap.at(found_at).valid_tmsi = htonl(*(uint32_t*)&s1ap_data[115]);
    ////            printf("++++++++++++++++ TMSI %d : %u -> %x %x %x %x\n", capture_tmsi, TMSI, s1ap_data[115], s1ap_data[116], s1ap_data[117], s1ap_data[118]);
    ////            stored_tmsi = TMSI;
    //        }
    //        capture_tmsi = false;
    //        valid_imsi = true;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x16 && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x24) {
    //        printf("UECapabilityInfoIndication, UECapabilityInformation: %d\n", message_type);
    //        int index = 10;


    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;



    //    }

    //    if (s1ap_data[0] == 0x20 && s1ap_data[1] == 0x09 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x22) {
    //        printf("InitialContextSetupResponse, UplinkNASTransport, Attach complete, Activate default EPS bearer context accept: %d\n", message_type);
    //        int index = 11;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0b && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x45) {
    //        printf("DownlinkNASTransport, EMM information: %d\n", message_type);
    //        int index = 12;

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //        is_cap = true;

    //    }

    //    //////////////////////////////////////////////////////////////////////////////////////

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x0d && s1ap_data[2] == 0x40 && s1ap_data[3] == 0x3f) {
    //        printf("UplinkNASTransport, Detach request (EPS detach / switch-off): %d\n", message_type);
    //        uint32_t TMSI = *(uint32_t*)&s1ap_data[41];
    //        // TMSI = htonl(TMSI);
    //        printf("++++++++++++++++ TMSI detach %d : \n", TMSI);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;
    //    }

    //    if (s1ap_data[0] == 0x00 && s1ap_data[1] == 0x17 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x10) {
    //        printf("UEContextReleaseCommand [NAS-cause=detach]: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;

    //    }

    //    if (s1ap_data[0] == 0x20 && s1ap_data[1] == 0x17 && s1ap_data[2] == 0x00 && s1ap_data[3] == 0x0f) {
    //        printf("UEContextReleaseComplete: %d\n", message_type);

    //        uint16_t ue_id = *(uint16_t*)&s1ap_data[11];
    //        int found_at = 0;
    //        for (int i = 0; i < s_trace_packet_pcap.size(); i++) {
    //            if (s_trace_packet_pcap.at(i).ue_id == ue_id) {
    //                found_at = i;
    //                break;
    //            }
    //        }

    //        s_trace_packet_pcap.at(found_at).ue_id = ue_id;
    //        int count = s_trace_packet_pcap.at(found_at).count;
    //        s_trace_packet_pcap.at(found_at).packet[count].packet_type = message_type;
    //        s_trace_packet_pcap.at(found_at).packet[count].size = packet_len;
    //        memcpy(&s_trace_packet_pcap.at(found_at).packet[count].packet[0], packet, packet_len);;
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_sec = static_cast<uint32_t>(tv.tv_sec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.ts_usec = static_cast<uint32_t>(tv.tv_usec);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.incl_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).packet[count].rec_header.orig_len = static_cast<uint32_t>(s1ap_length);
    //        s_trace_packet_pcap.at(found_at).count++;



    ////        std::cout << "should write" << std::endl;

    ////        std::erase(s_trace_packet_pcap, std::distance(std::begin(s_trace_packet_pcap), std::begin(s_trace_packet_pcap) + found_at));

    ////        auto* iter = s_trace_packet_pcap.at(found_at);

    ////        s_trace_packet_pcap.erase(  )




    //    }


}



int is_s1ap_packet(const unsigned char *packet, int length, bool& is_retrans, bool& is_cap) {
    is_retrans = false;

    // Check minimum size for Ethernet + IP + SCTP headers
    if (length < (int)(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct sctp_common_header))) {
        return 0;
    }

    // Check Ethernet type (IP)
    const struct ethhdr *eth = (const struct ethhdr *)packet;
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return 0;
    }

    // Check IP protocol (SCTP - protocol number 132)
    const struct iphdr *ip = (const struct iphdr *)(packet + sizeof(struct ethhdr));
    if (ip->protocol != 132) {  // SCTP protocol number
        return 0;
    }

    // Check IP header length
    int ip_header_len = ip->ihl * 4;
    if (length < (int)(sizeof(struct ethhdr) + ip_header_len + sizeof(struct sctp_common_header))) {
        return 0;
    }

    // Convert IPs to strings for verification
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

    // Now parse SCTP chunks to find S1AP data
    const unsigned char *sctp_start = packet + sizeof(struct ethhdr) + ip_header_len;
    int sctp_length = length - (sizeof(struct ethhdr) + ip_header_len);

    if (sctp_length < (int)sizeof(struct sctp_common_header)) {
        return 0;
    }

    // Get SCTP common header
    const struct sctp_common_header *sctp_common = (const struct sctp_common_header *)sctp_start;
    uint16_t src_port = ntohs(sctp_common->source_port);
    uint16_t dst_port = ntohs(sctp_common->dest_port);

    // Create connection key
    std::string conn_key = get_connection_key(ip->saddr, ip->daddr, src_port, dst_port);

    // Initialize connection state if it doesn't exist
    if (connection_states.find(conn_key) == connection_states.end()) {
        connection_states[conn_key] = sctp_connection_state();
    }
    sctp_connection_state& conn_state = connection_states[conn_key];

    // Pointer to first chunk after common header
    const unsigned char *chunk_ptr = sctp_start + sizeof(struct sctp_common_header);
    int remaining_length = sctp_length - sizeof(struct sctp_common_header);

    bool has_s1ap = false;
    bool is_client_to_server = (strcmp(src_ip, TARGET_CLIENT_IP) == 0);
    const char* direction = is_client_to_server ? "Client->Server" : "Server->Client";

    while (remaining_length >= (int)sizeof(struct sctp_chunk_header)) {
        const struct sctp_chunk_header *chunk = (const struct sctp_chunk_header *)chunk_ptr;
        uint16_t chunk_length = ntohs(chunk->length);

        if (chunk_length == 0 || chunk_length > (uint16_t)remaining_length) {
            break;  // Invalid chunk length
        }

        // Check for DATA chunk (type 0)
        if (chunk->type == 0) {
            if (chunk_length >= sizeof(struct sctp_data_chunk)) {
                const struct sctp_data_chunk *data_chunk = (const struct sctp_data_chunk *)chunk_ptr;
                uint32_t ppid = ntohl(data_chunk->protocol_id);
                uint32_t tsn = ntohl(data_chunk->tsn);

                // Check if this is S1AP traffic (PPID = 18)
                if (ppid == SCTP_PPID_S1AP) {
                    has_s1ap = true;

                    // Calculate S1AP payload position and length
                    int s1ap_payload_offset = sizeof(struct sctp_data_chunk);
                    int s1ap_payload_length = chunk_length - s1ap_payload_offset;

                    if (s1ap_payload_length > 0) {
                        const uint8_t *s1ap_data = chunk_ptr + s1ap_payload_offset;

                        // printf("\n=== S1AP Packet Detected ===\n");
                        // printf("Direction: %s -> %s\n", src_ip, dst_ip);
                        // printf("TSN: %u\n", tsn);
                        parse_s1ap_message(s1ap_data, s1ap_payload_length, direction, is_cap, packet, length);
                        // printf("============================\n\n");
                    }

                    // Check for retransmission
                    if (is_client_to_server) {
                        if (is_retransmission(conn_state.seen_tsn_client, tsn)) {
                            is_retrans = true;
                        } else {
                            conn_state.seen_tsn_client.insert(tsn);
                            conn_state.last_client_tsn = tsn;
                        }
                    } else {
                        if (is_retransmission(conn_state.seen_tsn_server, tsn)) {
                            is_retrans = true;
                        } else {
                            conn_state.seen_tsn_server.insert(tsn);
                            conn_state.last_server_tsn = tsn;
                        }
                    }
                }
            }
        }

        // Move to next chunk (chunk length is padded to 4 bytes)
        int padded_length = (chunk_length + 3) & ~3;
        chunk_ptr += padded_length;
        remaining_length -= padded_length;

        if (remaining_length <= 0) {
            break;
        }
    }

    return has_s1ap ? 1 : 0;
}


typedef struct diameter_header {
    uint8_t version;
    uint32_t length : 24;
    uint8_t flags;
    uint32_t command_code : 24;
    uint32_t application_id;
    uint32_t hop_by_hop_id;
    uint32_t end_to_end_id;
} __attribute__((packed)) diameter_header_t;


typedef struct {
    uint32_t expected_client_seq;
    uint32_t expected_server_seq;
    uint32_t last_client_ack;
    uint32_t last_server_ack;
    int connection_established;
} tcp_connection_state;
tcp_connection_state conn_state = {0};

int is_diameter_packet(const unsigned char *packet, int length, bool &is_retrans, bool &is_cap) {

    is_retrans = false;

    // Check minimum size for Ethernet + IP + SCTP headers
    if (length < (int)(sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct sctp_common_header))) {
        return 0;
    }

    // Check Ethernet type (IP)
    const struct ethhdr *eth = (const struct ethhdr *)packet;
    if (ntohs(eth->h_proto) != ETH_P_IP) {
        return 0;
    }

    // Check IP protocol (SCTP - protocol number 132)
    const struct iphdr *ip = (const struct iphdr *)(packet + sizeof(struct ethhdr));
    if (ip->protocol != 132) {  // SCTP protocol number
        return 0;
    }

    // Check IP header length
    int ip_header_len = ip->ihl * 4;
    if (length < (int)(sizeof(struct ethhdr) + ip_header_len + sizeof(struct sctp_common_header))) {
        return 0;
    }

    // Get TCP header
    const struct tcphdr *tcp = (const struct tcphdr *)(packet + sizeof(struct ethhdr) + ip_header_len);
    int tcp_header_len = tcp->doff * 4;
    int payload_length = length - (sizeof(struct ethhdr) + ip_header_len + tcp_header_len);

    // Convert IPs to strings for verification
    char src_ip[INET_ADDRSTRLEN], dst_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ip->saddr), src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(ip->daddr), dst_ip, INET_ADDRSTRLEN);

    // Check if this is our target S1AP traffic
    int is_target_traffic = 0;
    int is_client_to_server = 0;


        if (((strcmp(src_ip, TARGET_DIAMETER_CLIENT_IP) == 0) && (strcmp(dst_ip, TARGET_DIAMETER_SERVER_IP) == 0) && (htons(tcp->source) == TARGET_DIAMETER_PORT) )) {
            is_target_traffic = 1;
            is_client_to_server = 1;
        } else if ((strcmp(src_ip, TARGET_DIAMETER_SERVER_IP) == 0) && (strcmp(dst_ip, TARGET_DIAMETER_CLIENT_IP) == 0) && (htons(tcp->dest) == TARGET_DIAMETER_PORT)) {
            is_target_traffic = 1;
            is_client_to_server = 0;
        }

    if (!is_target_traffic) {
        return 0;
    }


    const unsigned char *sctp_start = packet + sizeof(struct ethhdr) + ip_header_len;
    int sctp_length = length - (sizeof(struct ethhdr) + ip_header_len);

    if (sctp_length < (int)sizeof(struct sctp_common_header)) {
        return 0;
    }

    // Get SCTP common header
    const struct sctp_common_header *sctp_common = (const struct sctp_common_header *)sctp_start;
    uint16_t src_port = ntohs(sctp_common->source_port);
    uint16_t dst_port = ntohs(sctp_common->dest_port);

//    std::cout << "src port: " << src_port << " dest port : " << dst_port << std::endl;

    std::string conn_key = get_connection_key(ip->saddr, ip->daddr, src_port, dst_port);

    // Initialize connection state if it doesn't exist
    if (connection_states.find(conn_key) == connection_states.end()) {
        connection_states[conn_key] = sctp_connection_state();
    }
    sctp_connection_state& conn_state = connection_states[conn_key];

    const unsigned char *chunk_ptr = sctp_start + sizeof(struct sctp_common_header);
    int remaining_length = sctp_length - sizeof(struct sctp_common_header);


        bool has_diam = false;
    while (remaining_length >= (int)sizeof(struct sctp_chunk_header)) {
        const struct sctp_chunk_header *chunk = (const struct sctp_chunk_header *)chunk_ptr;
        uint16_t chunk_length = ntohs(chunk->length);

        if (chunk_length == 0 || chunk_length > (uint16_t)remaining_length) {
            break;  // Invalid chunk length
        }

        // Check for DATA chunk (type 0)
        if (chunk->type == 0) {
            if (chunk_length >= sizeof(struct sctp_data_chunk)) {
                const struct sctp_data_chunk *data_chunk = (const struct sctp_data_chunk *)chunk_ptr;
                uint32_t ppid = ntohl(data_chunk->protocol_id);
                uint32_t tsn = ntohl(data_chunk->tsn);

                // Check if this is S1AP traffic (PPID = 46)
                if (ppid == SCTP_PPID_DIAM) {
                    has_diam = true;

                    int diam_payload_offset = sizeof(struct sctp_data_chunk);
                    int diam_payload_length = chunk_length - diam_payload_offset;

                    if (diam_payload_length > 0) {
                        const uint8_t *diam_data = chunk_ptr + diam_payload_offset;



                    }

                    // Check for retransmission
                    if (is_client_to_server) {
                        if (is_retransmission(conn_state.seen_tsn_client, tsn)) {
                            is_retrans = true;
                        } else {
                            conn_state.seen_tsn_client.insert(tsn);
                            conn_state.last_client_tsn = tsn;
                        }
                    } else {
                        if (is_retransmission(conn_state.seen_tsn_server, tsn)) {
                            is_retrans = true;
                        } else {
                            conn_state.seen_tsn_server.insert(tsn);
                            conn_state.last_server_tsn = tsn;
                        }
                    }
                }
            }
        }

        // Move to next chunk (chunk length is padded to 4 bytes)
        int padded_length = (chunk_length + 3) & ~3;
        chunk_ptr += padded_length;
        remaining_length -= padded_length;

        if (remaining_length <= 0) {
            break;
        }
    }


    return has_diam ? 1 : 0;

}



std::vector<trace_packet_pcap_t>& get_packet()
{
    return s_trace_packet_pcap;
}
