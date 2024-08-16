/*
 * If not stated otherwise in this file or this component's Licenses.txt file
 * the following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <rbus.h>
#include <rtLog.h>

#define UNUSED1(a)              (void)(a)
#define UNUSED2(a,b)            UNUSED1(a),UNUSED1(b)
#define UNUSED3(a,b,c)          UNUSED1(a),UNUSED2(b,c)
#define UNUSED4(a,b,c,d)        UNUSED1(a),UNUSED3(b,c,d)
#define UNUSED5(a,b,c,d,e)      UNUSED1(a),UNUSED4(b,c,d,e)
#define UNUSED6(a,b,c,d,e,f)    UNUSED1(a),UNUSED5(b,c,d,e,f)

#define TotalParams   14

rbusHandle_t        rbusHandle;
rbusHandle_t        rbusHandle2;
int                 loopFor = 150;
char                componentName[20] = "rbusSampleProvider";

rbusError_t SampleProvider_DeviceGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t SampleProvider_SampleDataSetHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);
rbusError_t SampleProvider_SampleDataGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t SampleProvider_NestedObjectsGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t SampleProvider_BuildResponseDataGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t SampleProvider_allTypesGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts);
rbusError_t SampleProvider_allTypesSetHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts);

rbusDataElement_t dataElements[TotalParams] = {
    {"Device.DeviceInfo.SampleProvider.AssocClients", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_DeviceGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.DeviceInfo.SampleProvider.Manufacturer", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_DeviceGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.DeviceInfo.SampleProvider.ModelName", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_DeviceGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.DeviceInfo.SampleProvider.SoftwareVersion", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_DeviceGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.SampleData.IntData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_SampleDataGetHandler, SampleProvider_SampleDataSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.SampleData.BoolData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_SampleDataGetHandler, SampleProvider_SampleDataSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.SampleData.UIntData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_SampleDataGetHandler, SampleProvider_SampleDataSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.NestedObject1.TestParam", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_NestedObjectsGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.NestedObject1.AnotherTestParam", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_NestedObjectsGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.NestedObject2.TestParam", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_NestedObjectsGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.NestedObject2.AnotherTestParam", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_NestedObjectsGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.TestData.IntData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_BuildResponseDataGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.TestData.BoolData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_BuildResponseDataGetHandler, NULL, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.TestData.UIntData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_BuildResponseDataGetHandler, NULL, NULL, NULL, NULL, NULL}}
};

rbusDataElement_t allTypeDataElements[14] = {
    {"Device.SampleProvider.AllTypes.BoolData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.CharData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.ByteData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.Int16Data", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.UInt16Data", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.Int32Data", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.UInt32Data", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.Int64Data", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.UInt64Data", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.SingleData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.DoubleData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.DateTimeData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.StringData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}},
    {"Device.SampleProvider.AllTypes.BytesData", RBUS_ELEMENT_TYPE_PROPERTY, {SampleProvider_allTypesGetHandler, SampleProvider_allTypesSetHandler, NULL, NULL, NULL, NULL}}
};

typedef struct _rbus_sample_data {
    int m_intData;
    bool m_boolData;
    unsigned int m_unsignedIntData;
} rbus_sample_provider_data_t;

rbus_sample_provider_data_t gTestInfo = {0};

rbusError_t SampleProvider_DeviceGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    rbusValue_t value;
    char const* name;

    rbusValue_Init(&value);

    name = rbusProperty_GetName(property);

    if (strcmp(name, "Device.DeviceInfo.SampleProvider.AssocClients") == 0)
    {
        char *str = "{\"Version\":\"1.0\",\"SubDocName\":\"associatedclients\",\"WiFiAssociatedClients\":[{\"VapName\":\"private_ssid_2g\",\"associatedClients\":[{\"MACAddress\":\"04:f0:21:4d:27:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2172,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:ef:4b:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2172,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":19,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:cc:12:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2174,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:81:8a:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2174,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:ba:20:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2172,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:a3:c9:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2172,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:80:a8:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2172,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":19,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:28:cd:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2308,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":19,\"PacketsReceived\":26,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:e7:2a:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":1,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2174,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":19,\"PacketsReceived\":25,\"ErrorsSent\":0,\"RetransCount\":1,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:74:b8:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":2672,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":32,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:bf:be:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":6920,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":91,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:07:9d:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10738,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:be:47:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10740,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":19,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:6a:36:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10738,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":19,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:85:b2:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10734,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:54:71:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10732,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:ce:b3:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10736,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:8b:c1:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10738,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:98:3c:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10736,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":144,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:e4:7c:e0\",\"WpaKeyMgmt\":\"wpa-psk\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":6,\"LastDataUplinkRate\":6,\"SignalStrength\":-15,\"Retransmissions\":0,\"Active\":true,\"OperatingStandard\":\"n\",\"OperatingChannelBandwidth\":\"20\",\"SNR\":77,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48984,\"BytesReceived\":10890,\"RSSI\":-15,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":20,\"PacketsReceived\":145,\"ErrorsSent\":0,\"RetransCount\":0,\"FailedRetransCount\":0,\"RetryCount\":0,\"MultipleRetryCount\":0}]},{\"VapName\":\"iot_ssid_2g\",\"associatedClients\":[]},{\"VapName\":\"hotspot_open_2g\",\"associatedClients\":[]},{\"VapName\":\"lnf_psk_2g\",\"associatedClients\":[]},{\"VapName\":\"hotspot_secure_2g\",\"associatedClients\":[]},{\"VapName\":\"lnf_radius_2g\",\"associatedClients\":[]},{\"VapName\":\"mesh_backhaul_2g\",\"associatedClients\":[]},{\"VapName\":\"mesh_sta_2g\",\"associatedClients\":[]},{\"VapName\":\"private_ssid_5g\",\"associatedClients\":[{\"MACAddress\":\"04:f0:21:de:3b:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":10,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":3617,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":16,\"ErrorsSent\":0,\"RetransCount\":10,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:14:f0:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":234,\"SignalStrength\":-24,\"Retransmissions\":4,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49236,\"BytesReceived\":4787,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":19,\"ErrorsSent\":0,\"RetransCount\":4,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:1f:52:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":195,\"SignalStrength\":-24,\"Retransmissions\":5,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49236,\"BytesReceived\":3885,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":18,\"ErrorsSent\":0,\"RetransCount\":5,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:ec:4e:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1053,\"LastDataUplinkRate\":195,\"SignalStrength\":-24,\"Retransmissions\":5,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":4217,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":28,\"PacketsReceived\":16,\"ErrorsSent\":0,\"RetransCount\":5,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:b6:28:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":234,\"SignalStrength\":-24,\"Retransmissions\":4,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":5585,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":28,\"PacketsReceived\":22,\"ErrorsSent\":0,\"RetransCount\":4,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:50:7b:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":7,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":3097,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":27,\"PacketsReceived\":12,\"ErrorsSent\":0,\"RetransCount\":7,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:55:9c:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1053,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":8,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49236,\"BytesReceived\":3771,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":15,\"ErrorsSent\":0,\"RetransCount\":8,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:8b:86:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1053,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":7,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":4283,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":28,\"PacketsReceived\":17,\"ErrorsSent\":0,\"RetransCount\":7,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:c2:3a:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1053,\"LastDataUplinkRate\":130,\"SignalStrength\":-24,\"Retransmissions\":11,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49642,\"BytesReceived\":5431,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":31,\"PacketsReceived\":21,\"ErrorsSent\":0,\"RetransCount\":11,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:cd:ce:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":390,\"SignalStrength\":-24,\"Retransmissions\":6,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49642,\"BytesReceived\":4043,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":31,\"PacketsReceived\":15,\"ErrorsSent\":0,\"RetransCount\":6,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:a4:ab:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1053,\"LastDataUplinkRate\":97,\"SignalStrength\":-24,\"Retransmissions\":10,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":4279,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":28,\"PacketsReceived\":19,\"ErrorsSent\":0,\"RetransCount\":10,\"FailedRetransCount\":0,\"RetryCount\":6,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:eb:d5:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":234,\"SignalStrength\":-24,\"Retransmissions\":9,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49236,\"BytesReceived\":4197,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":30,\"PacketsReceived\":16,\"ErrorsSent\":0,\"RetransCount\":9,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:a3:34:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":4,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49236,\"BytesReceived\":3831,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":15,\"ErrorsSent\":0,\"RetransCount\":4,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:0c:e3:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":11,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48830,\"BytesReceived\":4409,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":30,\"PacketsReceived\":18,\"ErrorsSent\":0,\"RetransCount\":11,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:26:d0:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":390,\"SignalStrength\":-24,\"Retransmissions\":7,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49236,\"BytesReceived\":5529,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":30,\"PacketsReceived\":20,\"ErrorsSent\":0,\"RetransCount\":7,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:a9:ff:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":117,\"SignalStrength\":-24,\"Retransmissions\":5,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49924,\"BytesReceived\":4555,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":33,\"PacketsReceived\":17,\"ErrorsSent\":0,\"RetransCount\":5,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:79:c6:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":9,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":49624,\"BytesReceived\":3881,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":33,\"PacketsReceived\":14,\"ErrorsSent\":0,\"RetransCount\":9,\"FailedRetransCount\":0,\"RetryCount\":1,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:ae:b7:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":6,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":34,\"DataFramesSentNoAck\":0,\"BytesSent\":50030,\"BytesReceived\":3503,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":34,\"PacketsReceived\":13,\"ErrorsSent\":0,\"RetransCount\":6,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:86:c8:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":4,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48900,\"BytesReceived\":3369,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":12,\"ErrorsSent\":0,\"RetransCount\":4,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:ba:a8:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1053,\"LastDataUplinkRate\":260,\"SignalStrength\":-24,\"Retransmissions\":5,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":33,\"DataFramesSentNoAck\":0,\"BytesSent\":48900,\"BytesReceived\":3337,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":29,\"PacketsReceived\":14,\"ErrorsSent\":0,\"RetransCount\":5,\"FailedRetransCount\":0,\"RetryCount\":2,\"MultipleRetryCount\":0},{\"MACAddress\":\"04:f0:21:e1:4a:81\",\"WpaKeyMgmt\":\"sae\",\"PairwiseCipher\":\"ccmp-128\",\"AuthenticationState\":true,\"LastDataDownlinkRate\":1170,\"LastDataUplinkRate\":390,\"SignalStrength\":-24,\"Retransmissions\":12,\"Active\":true,\"OperatingStandard\":\"ac\",\"OperatingChannelBandwidth\":\"80\",\"SNR\":68,\"InterferenceSources\":\"\",\"DataFramesSentAck\":35,\"DataFramesSentNoAck\":0,\"BytesSent\":50030,\"BytesReceived\":4993,\"RSSI\":-24,\"MinRSSI\":0,\"MaxRSSI\":0,\"Disassociations\":0,\"AuthenticationFailures\":0,\"PacketsSent\":35,\"PacketsReceived\":16,\"ErrorsSent\":0,\"RetransCount\":12,\"FailedRetransCount\":0,\"RetryCount\":3,\"MultipleRetryCount\":0}]},{\"VapName\":\"iot_ssid_5g\",\"associatedClients\":[]},{\"VapName\":\"hotspot_open_5g\",\"associatedClients\":[]},{\"VapName\":\"lnf_psk_5g\",\"associatedClients\":[]},{\"VapName\":\"hotspot_secure_5g\",\"associatedClients\":[]},{\"VapName\":\"lnf_radius_5g\",\"associatedClients\":[]},{\"VapName\":\"mesh_backhaul_5g\",\"associatedClients\":[]},{\"VapName\":\"mesh_sta_5g\",\"associatedClients\":[]},{\"VapName\":\"private_ssid_6g\",\"associatedClients\":[]},{\"VapName\":\"iot_ssid_6g\",\"associatedClients\":[]},{\"VapName\":\"hotspot_open_6g\",\"associatedClients\":[]},{\"VapName\":\"lnf_psk_6g\",\"associatedClients\":[]},{\"VapName\":\"hotspot_secure_6g\",\"associatedClients\":[]},{\"VapName\":\"mesh_backhaul_6g\",\"associatedClients\":[]},{\"VapName\":\"mesh_sta_6g\",\"associatedClients\":[]}]}";
        rbusValue_SetString(value, str);
    }
    else if(strcmp(name, "Device.DeviceInfo.SampleProvider.Manufacturer") == 0)
    {
        printf("Called get handler for [%s]\n", name);
        rbusValue_SetString(value, "COMCAST");
    }
    else if(strcmp(name, "Device.DeviceInfo.SampleProvider.ModelName") == 0)
    {
        printf("Called get handler for [%s]\n", name);
        rbusValue_SetString(value, "XB3");

    }
    else if (strcmp(name, "Device.DeviceInfo.SampleProvider.SoftwareVersion") == 0)
    {
        printf("Called get handler for [%s]\n", name);
        rbusValue_SetSingle(value, 2.14f);
    }
    else
    {
        printf("Cant Handle [%s]\n", name);
        return RBUS_ERROR_INVALID_INPUT;
    }

    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);

    return RBUS_ERROR_SUCCESS;
}

rbusObject_t gTestObject;
static bool isFirstTime = true;

void _prepare_object_for_future_query(void)
{
    /* This flag is also a sample; as we can always release the object and get a new one.. */
    if (isFirstTime)
    {
        isFirstTime = false;
        rbusObject_Init (&gTestObject, "SampleTestData");

        rbusValue_t val1;
        rbusValue_t val2;
        rbusValue_t val3;
        rbusProperty_t prop1;
        rbusProperty_t prop2;
        rbusProperty_t prop3;

        rbusValue_Init(&val1);
        rbusValue_Init(&val2);
        rbusValue_Init(&val3);

        rbusValue_SetInt32(val1, gTestInfo.m_intData);
        rbusValue_SetBoolean(val2, gTestInfo.m_boolData);
        rbusValue_SetUInt32(val3, gTestInfo.m_unsignedIntData);

        rbusProperty_Init (&prop1, "Device.SampleProvider.TestData.IntData", val1);
        rbusProperty_Init (&prop2, "Device.SampleProvider.TestData.BoolData", val2);
        rbusProperty_Init (&prop3, "Device.SampleProvider.TestData.UIntData", val3);

        rbusObject_SetProperty(gTestObject, prop1);
        rbusObject_SetProperty(gTestObject, prop2);
        rbusObject_SetProperty(gTestObject, prop3);

        rbusValue_Release(val1);
        rbusValue_Release(val2);
        rbusValue_Release(val3);
        rbusProperty_Release(prop1);
        rbusProperty_Release(prop2);
        rbusProperty_Release(prop3);
    }
    else
    {
        rbusValue_t val1;
        rbusValue_t val2;
        rbusValue_t val3;

        rbusValue_Init(&val1);
        rbusValue_Init(&val2);
        rbusValue_Init(&val3);

        rbusValue_SetInt32(val1, gTestInfo.m_intData);
        rbusValue_SetBoolean(val2, gTestInfo.m_boolData);
        rbusValue_SetUInt32(val3, gTestInfo.m_unsignedIntData);

        rbusObject_SetValue(gTestObject, "Device.SampleProvider.TestData.IntData", val1);
        rbusObject_SetValue(gTestObject, "Device.SampleProvider.TestData.BoolData", val2);
        rbusObject_SetValue(gTestObject, "Device.SampleProvider.TestData.UIntData", val3);

        rbusValue_Release(val1);
        rbusValue_Release(val2);
        rbusValue_Release(val3);
    }
    return;
}

void _release_object_for_future_query()
{
    rbusObject_Release(gTestObject);
}

rbusError_t SampleProvider_BuildResponseDataGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    rbusValue_t value;
    char const* name;

    name = rbusProperty_GetName(property);
    value = rbusObject_GetValue(gTestObject, name);
    rbusProperty_SetValue(property, value);

    return RBUS_ERROR_SUCCESS;
}

rbusError_t SampleProvider_SampleDataSetHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;

    char const* name = rbusProperty_GetName(prop);
    rbusValue_t value = rbusProperty_GetValue(prop);
    rbusValueType_t type = rbusValue_GetType(value);

    if(strcmp(name, "Device.SampleProvider.SampleData.IntData") == 0)
    {
        if (type != RBUS_INT32)
        {
            printf("%s Called Set handler with wrong data type\n", name);
            return RBUS_ERROR_INVALID_INPUT;
        }
        else
        {
            printf("%s Called Set handler with value = %d\n", name, rbusValue_GetInt32(value));
            gTestInfo.m_intData = rbusValue_GetInt32(value);
        }
    }
    else if(strcmp(name, "Device.SampleProvider.SampleData.BoolData") == 0)
    {
        if (type != RBUS_BOOLEAN)
        {
            printf("%s Called Set handler with wrong data type\n", name);
            return RBUS_ERROR_INVALID_INPUT;
        }
        else
        {
            printf("%s Called Set handler with value = %d\n", name, rbusValue_GetBoolean(value));
            gTestInfo.m_boolData = rbusValue_GetBoolean(value);
        }
    }
    else if (strcmp(name, "Device.SampleProvider.SampleData.UIntData") == 0)
    {
        if (type != RBUS_UINT32)
        {
            printf("%s Called Set handler with wrong data type\n", name);
            return RBUS_ERROR_INVALID_INPUT;
        }
        else
        {
            printf("%s Called Set handler with value = %u\n", name, rbusValue_GetUInt32(value));
            gTestInfo.m_unsignedIntData = rbusValue_GetUInt32(value);
        }
    }

    /* Sample Case for Build Response APIs that are proposed */
    _prepare_object_for_future_query();

    return RBUS_ERROR_SUCCESS;
}

rbusError_t SampleProvider_SampleDataGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    rbusValue_t value;
    char const* name;

    rbusValue_Init(&value);
    name = rbusProperty_GetName(property);

    if(strcmp(name, "Device.SampleProvider.SampleData.IntData") == 0)
    {
        gTestInfo.m_intData += 101;
        printf("Called get handler for [%s] & value is %d\n", name, gTestInfo.m_intData);
        rbusValue_SetInt32(value, gTestInfo.m_intData);
    }
    else if(strcmp(name, "Device.SampleProvider.SampleData.BoolData") == 0)
    {
        printf("Called get handler for [%s]\n", name);
        rbusValue_SetBoolean(value, gTestInfo.m_boolData);

    }
    else if (strcmp(name, "Device.SampleProvider.SampleData.UIntData") == 0)
    {
        printf("Called get handler for [%s]\n", name);
        rbusValue_SetUInt32(value, gTestInfo.m_unsignedIntData);
    }
    else
    {
        printf("Cant Handle [%s]\n", name);
        return RBUS_ERROR_INVALID_INPUT;
    }

    rbusProperty_SetValue(property, value);
    rbusValue_Release(value);

    return RBUS_ERROR_SUCCESS;
}

rbusError_t SampleProvider_NestedObjectsGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    printf("SampleProvider_NestedObjectsGetHandler called for %s\n", rbusProperty_GetName(property));

    rbusValue_t value;
    rbusValue_Init(&value);

    rbusValue_SetString(value, "Testing Rbus Nested Object");
    rbusProperty_SetValue(property, value);

    rbusValue_Release(value);
    return RBUS_ERROR_SUCCESS;
}

typedef struct sample_all_types_t {
    bool m_bool;
    char m_char;
    unsigned char m_byte;
    int16_t m_int16;
    uint16_t m_uint16;
    int32_t m_int32;
    uint32_t m_uint32;
    int64_t m_int64;
    uint64_t m_uint64;
    float m_float;
    double m_double;
    rbusDateTime_t m_timeval;
    char m_string[251];
    unsigned char m_bytes[8*1024];
} sampleDataTypes_t;

sampleDataTypes_t gTestSampleVal = {
            true,
            'k',
            0xd,
            0xFFFF,
            0xFFFF,
            0xFFFFFFFF,
            0xFFFFFFFF,
            0xFFFFFFFFFFFFFFFF,
            0xFFFFFFFFFFFFFFFF,
            3.141592653589793f,
            3.141592653589793,
            {{0},{0}},
            "AllTypes",
            {1,2,3,4,5,6,7,8,9,10,11,12}
            };

rbusError_t SampleProvider_allTypesSetHandler(rbusHandle_t handle, rbusProperty_t prop, rbusSetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    char const* name = rbusProperty_GetName(prop);
    rbusValue_t value = rbusProperty_GetValue(prop);
    rbusValueType_t type = rbusValue_GetType(value);
    if ((strcmp(name, "Device.SampleProvider.AllTypes.BoolData") == 0) && (type == RBUS_BOOLEAN))
        gTestSampleVal.m_bool = rbusValue_GetBoolean(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.CharData") == 0) && (type == RBUS_CHAR))
        gTestSampleVal.m_char = rbusValue_GetChar(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.BytesData") == 0) && (type == RBUS_BYTES)){
        int bytes_len=0;
        uint8_t const* ptr = rbusValue_GetBytes(value, &bytes_len);
        memset(&gTestSampleVal.m_bytes, 0, sizeof(gTestSampleVal.m_bytes));
        if((size_t)bytes_len > sizeof(gTestSampleVal.m_bytes))
            bytes_len = sizeof(gTestSampleVal.m_bytes);
        memcpy(&gTestSampleVal.m_bytes, ptr, bytes_len);
    }
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.ByteData") == 0) && (type == RBUS_BYTE))
        gTestSampleVal.m_byte = rbusValue_GetByte(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.Int16Data") == 0) && (type == RBUS_INT16))
        gTestSampleVal.m_int16 = rbusValue_GetInt16(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.UInt16Data") == 0) && (type == RBUS_UINT16))
        gTestSampleVal.m_uint16 = rbusValue_GetUInt16(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.Int32Data") == 0) && (type == RBUS_INT32))
        gTestSampleVal.m_int32 = rbusValue_GetInt32(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.UInt32Data") == 0) && (type == RBUS_UINT32))
        gTestSampleVal.m_uint32 = rbusValue_GetUInt32(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.Int64Data") == 0) && (type == RBUS_INT64))
        gTestSampleVal.m_int64 = rbusValue_GetInt64(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.UInt64Data") == 0) && (type == RBUS_UINT64))
        gTestSampleVal.m_uint64 = rbusValue_GetUInt64(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.SingleData") == 0) && (type == RBUS_SINGLE))
        gTestSampleVal.m_float = rbusValue_GetSingle(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.DoubleData") == 0) && (type == RBUS_DOUBLE))
        gTestSampleVal.m_double = rbusValue_GetDouble(value);
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.StringData") == 0) && (type == RBUS_STRING))
    {
        int len = 0;
        char const* pTmp = NULL;
        pTmp = rbusValue_GetString(value, &len);
        strncpy (gTestSampleVal.m_string, pTmp, 250);
    }
    else if ((strcmp(name, "Device.SampleProvider.AllTypes.DateTimeData") == 0) && (type == RBUS_DATETIME))
    {
        gTestSampleVal.m_timeval = *rbusValue_GetTime(value);
    }
    else
        return RBUS_ERROR_INVALID_INPUT;

    return RBUS_ERROR_SUCCESS;
}

rbusError_t SampleProvider_allTypesGetHandler(rbusHandle_t handle, rbusProperty_t property, rbusGetHandlerOptions_t* opts)
{
    (void)handle;
    (void)opts;
    char const* name;
    rbusValue_t value;
    name = rbusProperty_GetName(property);
    if (!name)
        return RBUS_ERROR_INVALID_INPUT;

    printf("SampleProvider_allTypesGetHandler called for %s\n", name);

    rbusValue_Init(&value);
    if (strcmp(name, "Device.SampleProvider.AllTypes.BoolData") == 0)
        rbusValue_SetBoolean(value, gTestSampleVal.m_bool);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.CharData") == 0)
        rbusValue_SetChar(value, gTestSampleVal.m_char);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.ByteData") == 0)
        rbusValue_SetByte(value, gTestSampleVal.m_byte);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.Int16Data") == 0)
        rbusValue_SetInt16(value, gTestSampleVal.m_int16);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.UInt16Data") == 0)
        rbusValue_SetUInt16(value, gTestSampleVal.m_uint16);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.Int32Data") == 0)
        rbusValue_SetInt32(value, gTestSampleVal.m_int32);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.UInt32Data") == 0)
        rbusValue_SetUInt32(value, gTestSampleVal.m_uint32);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.Int64Data") == 0)
        rbusValue_SetInt64(value, gTestSampleVal.m_int64);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.UInt64Data") == 0)
        rbusValue_SetUInt64(value, gTestSampleVal.m_uint64);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.SingleData") == 0)
    {
        gTestSampleVal.m_float /= 0.03;
        rbusValue_SetSingle(value, gTestSampleVal.m_float);
    }
    else if (strcmp(name, "Device.SampleProvider.AllTypes.DoubleData") == 0)
        rbusValue_SetDouble(value, gTestSampleVal.m_double);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.DateTimeData") == 0)
        rbusValue_SetTime(value, &(gTestSampleVal.m_timeval));
    else if (strcmp(name, "Device.SampleProvider.AllTypes.StringData") == 0)
        rbusValue_SetString(value, gTestSampleVal.m_string);
    else if (strcmp(name, "Device.SampleProvider.AllTypes.BytesData") == 0)
        rbusValue_SetBytes(value, gTestSampleVal.m_bytes, sizeof(gTestSampleVal.m_bytes));

    rbusProperty_SetValue(property, value);

    rbusValue_Release(value);
    return RBUS_ERROR_SUCCESS;
}

int main(int argc, char *argv[])
{
    rbusError_t rc;

    (void)argc;
    (void)argv;
    int retryCount = 25;
    printf("provider: start\n");

    if(argc == 2)
    {
        loopFor = atoi(argv[1]);
    }

    rc = rbus_open(&rbusHandle, componentName);
    if(rc != RBUS_ERROR_SUCCESS)
    {
        printf("provider: rbus_open failed: %d\n", rc);
        goto exit2;
    }

    /* Sample Case for Build Response APIs that are proposed */
    _prepare_object_for_future_query();

    rtLog_SetOption(RT_USE_RTLOGGER);

    //having the retryCount in loop is to verify the duplicate data model registraion thro rbuscli..
    while (retryCount--)
    {
        rc = rbus_regDataElements(rbusHandle, TotalParams, dataElements);
        if(rc == RBUS_ERROR_SUCCESS)
        {
            printf("provider: rbus_regDataElements Successful:\n");
            break;
        }
        sleep (1);
    }

    retryCount = 25;
    while (retryCount--)
    {
        rc = rbus_open(&rbusHandle2, "Second-Provider-for-Sample");
        rc = rbus_regDataElements(rbusHandle2, 14, allTypeDataElements);
        if(rc == RBUS_ERROR_SUCCESS)
        {
            printf("provider: rbus_regDataElements Successful:\n");
            break;
        }
        sleep (1);
    }

    if(rc != RBUS_ERROR_SUCCESS)
    {
        printf("provider: rbus_regDataElements failed: %d\n", rc);
        goto exit1;
    }

    while(loopFor--)
    {
        printf("provider: exiting in %d seconds\n", loopFor);
        sleep(1);
    }

    rbus_unregDataElements(rbusHandle, TotalParams, dataElements);
    rbus_unregDataElements(rbusHandle2, 14, allTypeDataElements);

exit1:
    /* Sample Case for freeing the Response that was prebuilt */
    _release_object_for_future_query();

    rbus_close(rbusHandle);
    rbus_close(rbusHandle2);

exit2:
    printf("provider: exit\n");
    return 0;
}
