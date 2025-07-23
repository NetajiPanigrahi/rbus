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

#define PRINT_EVENT(EVENT, SUBSCRIPTION) \
    printf("\n############################################################################\n" \
        " Event received in handler: %s\n" \
        " Subscription:\n" \
        "   eventName=%s\n" \
        "   userData=%s\n" \
        " Event:\n" \
        "   type=%d\n" \
        "   name=%s\n" \
        "   data=\n", \
            __FUNCTION__, \
            (SUBSCRIPTION)->eventName, \
            (char*)(SUBSCRIPTION)->userData, \
            (EVENT)->type, \
            (EVENT)->name); \
    rbusObject_fwrite((EVENT)->data, 8, stdout); \
    printf("\n############################################################################\n");

static void valueChangeHandler(
    rbusHandle_t handle,
    rbusEvent_t const* event,
    rbusEventSubscription_t* subscription)
{
    rbusValue_t newValue = rbusObject_GetValue(event->data, "value");
    rbusValue_t oldValue = rbusObject_GetValue(event->data, "oldValue");

    printf("Consumer receiver ValueChange event for param %s\n", event->name);

    if(newValue)
        printf("  New Value: %s\n", rbusValue_GetString(newValue, NULL));

    if(oldValue)
        printf("  Old Value: %s\n", rbusValue_GetString(oldValue, NULL));

    printf("  My user data: %s\n", (char*)subscription->userData);

    PRINT_EVENT(event, subscription);

    (void)handle;
}
#define Security_ModeEnabled 		"Device.WiFi.AccessPoint.*.Security.ModeEnabled"
#define KeyPassphrase 			"Device.WiFi.AccessPoint.*.Security.X_COMCAST-COM_KeyPassphrase"
#define RadiusServerIPAddr 		"Device.WiFi.AccessPoint.*.Security.RadiusServerIPAddr"
#define SSIDAdvertisementEnabled	"Device.WiFi.AccessPoint.*.SSIDAdvertisementEnabled"
#define MACFilterEnable 		"Device.WiFi.AccessPoint.*.X_CISCO_COM_MACFilter.Enable"
#define FilterAsBlackList 		"Device.WiFi.AccessPoint.*.X_CISCO_COM_MACFilter.FilterAsBlackList"
#define SSID_Enable 			"Device.WiFi.SSID.*.Enable"
#define SSID_Name 			"Device.WiFi.SSID.*.SSID"
#define BSSID 				"Device.WiFi.SSID.*.BSSID"
#define SSID_Status 			"Device.WiFi.SSID.*.Status"
#define Channel 			"Device.WiFi.Radio.*.Channel"
#define OperatingFrequencyBand 		"Device.WiFi.Radio.*.OperatingFrequencyBand"
#define OperatingChannelBandwidth 	"Device.WiFi.Radio.*.OperatingChannelBandwidth"

#define FactoryResetRadioAndAp          "Device.WiFi.X_CISCO_COM_FactoryResetRadioAndAp"
#define LanMode                         "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.*.LanMode"
#define LanIPAddress                    "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.*.LanIPAddress"
#define LanSubnetMask                   "Device.X_CISCO_COM_DeviceControl.LanManagementEntry.*.LanSubnetMask"
#define Bridging_Enable                 "Device.Bridging.Bridge.*.Port.*.Enable"

#define DHCP4_MinAddress                "Device.DHCPv4.Server.Pool.*.MinAddress"
#define DHCP4_MaxAddress                "Device.DHCPv4.Server.Pool.*.MaxAddress"
#define DHCP4_LeaseTime                 "Device.DHCPv4.Server.Pool.*.LeaseTime"

#define Tunnel_PrimaryRemoteEndpoint      "Device.X_COMCAST-COM_GRE.Tunnel.*.PrimaryRemoteEndpoint"
#define Tunnel_SecondaryRemoteEndpoint    "Device.X_COMCAST-COM_GRE.Tunnel.*.SecondaryRemoteEndpoint"
#define Tunnel_VLANID                     "Device.X_COMCAST-COM_GRE.Tunnel.*.Interface.*.VLANID"
#define Tunnel_LocalInterfaces            "Device.X_COMCAST-COM_GRE.Tunnel.*.Interface.*.LocalInterfaces"

#define FilterAnonymousInternetRequests   "Device.X_CISCO_COM_Security.Firewall.FilterAnonymousInternetRequests"
#define FilterHTTP                        "Device.X_CISCO_COM_Security.Firewall.FilterHTTP"
#define FilterIdent                       "Device.X_CISCO_COM_Security.Firewall.FilterIdent"
#define FilterMulticast                   "Device.X_CISCO_COM_Security.Firewall.FilterMulticast"
#define FilterP2P                         "Device.X_CISCO_COM_Security.Firewall.FilterP2P"
#define FirewallLevel                     "Device.X_CISCO_COM_Security.Firewall.FirewallLevel"
#define FilterAnonymousInternetRequestsV6 "Device.X_CISCO_COM_Security.Firewall.FilterAnonymousInternetRequestsV6"
#define FilterHTTPV6                      "Device.X_CISCO_COM_Security.Firewall.FilterHTTPV6"
#define FilterIdentV6                     "Device.X_CISCO_COM_Security.Firewall.FilterIdentV6"
#define FilterMulticastV6                 "Device.X_CISCO_COM_Security.Firewall.FilterMulticastV6"
#define FilterP2PV6                       "Device.X_CISCO_COM_Security.Firewall.FilterP2PV6"
#define FirewallLevelV6                   "Device.X_CISCO_COM_Security.Firewall.FirewallLevelV6"


#define NAT_Enable             "Device.NAT.X_CISCO_COM_DMZ.Enable"
#define NAT_InternalIP         "Device.NAT.X_CISCO_COM_DMZ.InternalIP"
#define NAT_IPv6Host           "Device.NAT.X_CISCO_COM_DMZ.IPv6Host"
#define NAT_PortMapping        "Device.NAT.X_Comcast_com_EnablePortMapping"



int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);

    int rc = RBUS_ERROR_SUCCESS;
    rbusHandle_t handle;

    rbusEventSubscription_t subscriptions[] = {
                {Security_ModeEnabled, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
                {KeyPassphrase, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
                {RadiusServerIPAddr, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {SSIDAdvertisementEnabled, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {FilterAsBlackList, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {SSID_Enable, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {SSID_Name, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {BSSID, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {SSID_Status, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {Channel, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {OperatingFrequencyBand, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {OperatingChannelBandwidth, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
	        {MACFilterEnable, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterAnonymousInternetRequests, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterHTTP, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterIdent, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterMulticast, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterP2P, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FirewallLevel, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterAnonymousInternetRequestsV6, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterHTTPV6, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterIdentV6, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterMulticastV6, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FilterP2PV6, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FirewallLevelV6, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{NAT_Enable, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{NAT_InternalIP, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{NAT_IPv6Host, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{NAT_PortMapping, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{Tunnel_PrimaryRemoteEndpoint, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{Tunnel_SecondaryRemoteEndpoint, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{Tunnel_VLANID, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{Tunnel_LocalInterfaces, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{DHCP4_MinAddress, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{DHCP4_MaxAddress, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{DHCP4_LeaseTime, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{FactoryResetRadioAndAp, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{LanMode, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{LanIPAddress, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{LanSubnetMask, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{Bridging_Enable, NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{"Device.DeviceInfo.", NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false},
		{"Device.MoCA.Interface.1.Enable", NULL, 0, 0, valueChangeHandler, NULL, NULL, NULL, false}	
    };

    printf("constumer: start\n");

    rc = rbus_open(&handle, "Subscription_Consumer");
    if(rc != RBUS_ERROR_SUCCESS)
    {
        printf("consumer: rbus_open failed: %d\n", rc);
        goto exit4;
    }
    
    rc = rbusEvent_SubscribeEx(handle, subscriptions, 43, 0);

    if(rc != RBUS_ERROR_SUCCESS)
    {
        printf("consumer: rbusEvent_Subscribe 1 failed: %d\n", rc);
        goto exit3;
    }

    while(1)
    {
        if (access("/tmp/Subscription_Consumer", F_OK) == 0) {
            break;
        }
        sleep(20);
    }

    rbusEvent_UnsubscribeEx(handle, subscriptions, 43);

exit3:
    rbus_close(handle);

exit4:
    printf("consumer: exit\n");
    return rc;
}


