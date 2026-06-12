/********************************************************************************************************
 * @file    uuid16bit.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

/**
 *  @brief Definition for GATT Services UUID
 */
#define BLE_HOST_SERVICE_UUIDS                                                                                                                   \
    X(SERVICE_UUID_GENERIC_ACCESS, serviceGenericAccessAttUuid, 0x1800, "Generic Access Service")                                                \
    X(SERVICE_UUID_GENERIC_ATTRIBUTE, serviceGenericAttributeAttUuid, 0x1801, "Generic Attribute Service")                                       \
    X(SERVICE_UUID_IMMEDIATE_ALERT, serviceImmediateAlertAttUuid, 0x1802, "Immediate Alert Service")                                             \
    X(SERVICE_UUID_LINK_LOSS, serviceLinkLossAttUuid, 0x1803, "Link Loss Service")                                                               \
    X(SERVICE_UUID_TX_POWER, serviceTxPowerAttUuid, 0x1804, "Tx Power Service")                                                                  \
    X(SERVICE_UUID_CURRENT_TIME, serviceCurrentTimeAttUuid, 0x1805, "Current Time Service")                                                      \
    X(SERVICE_UUID_REFERENCE_TIME_UPDATE, serviceReferenceTimeUpdateAttUuid, 0x1806, "Reference Time Update Service")                            \
    X(SERVICE_UUID_NEXT_DST_CHANGE, serviceNextDstChangeAttUuid, 0x1807, "Next DST Change Service")                                              \
    X(SERVICE_UUID_GLUCOSE, serviceGlucoseAttUuid, 0x1808, "Glucose Service")                                                                    \
    X(SERVICE_UUID_HEALTH_THERMOMETER, serviceHealthThermometerAttUuid, 0x1809, "Health Thermometer Service")                                    \
    X(SERVICE_UUID_DEVICE_INFORMATION, serviceDeviceInformationAttUuid, 0x180A, "Device Information Service")                                    \
    X(SERVICE_UUID_HEART_RATE, serviceHeartRateAttUuid, 0x180D, "Heart Rate Service")                                                            \
    X(SERVICE_UUID_PHONE_ALERT_STATUS, servicePhoneAlertStatusAttUuid, 0x180E, "Phone Alert Status Service")                                     \
    X(SERVICE_UUID_BATTERY, serviceBatteryAttUuid, 0x180F, "Battery Service")                                                                    \
    X(SERVICE_UUID_BLOOD_PRESSURE, serviceBloodPressureAttUuid, 0x1810, "Blood Pressure Service")                                                \
    X(SERVICE_UUID_ALERT_NOTIFICATION, serviceAlertNotificationAttUuid, 0x1811, "Alert Notification Service")                                    \
    X(SERVICE_UUID_HUMAN_INTERFACE_DEVICE, serviceHumanInterfaceDeviceAttUuid, 0x1812, "Human Interface Device Service")                         \
    X(SERVICE_UUID_SCAN_PARAMETERS, serviceScanParametersAttUuid, 0x1813, "Scan Parameters Service")                                             \
    X(SERVICE_UUID_RUNNING_SPEED_AND_CADENCE, serviceRunningSpeedAndCadenceAttUuid, 0x1814, "Running Speed and Cadence Service")                 \
    X(SERVICE_UUID_AUTOMATION_IO, serviceAutomationIoAttUuid, 0x1815, "Automation IO Service")                                                   \
    X(SERVICE_UUID_CYCLING_SPEED_AND_CADENCE, serviceCyclingSpeedAndCadenceAttUuid, 0x1816, "Cycling Speed and Cadence Service")                 \
    X(SERVICE_UUID_CYCLING_POWER, serviceCyclingPowerAttUuid, 0x1818, "Cycling Power Service")                                                   \
    X(SERVICE_UUID_LOCATION_AND_NAVIGATION, serviceLocationAndNavigationAttUuid, 0x1819, "Location and Navigation Service")                      \
    X(SERVICE_UUID_ENVIRONMENTAL_SENSING, serviceEnvironmentalSensingAttUuid, 0x181A, "Environmental Sensing Service")                           \
    X(SERVICE_UUID_BODY_COMPOSITION, serviceBodyCompositionAttUuid, 0x181B, "Body Composition Service")                                          \
    X(SERVICE_UUID_USER_DATA, serviceUserDataAttUuid, 0x181C, "User Data Service")                                                               \
    X(SERVICE_UUID_WEIGHT_SCALE, serviceWeightScaleAttUuid, 0x181D, "Weight Scale Service")                                                      \
    X(SERVICE_UUID_BOND_MANAGEMENT, serviceBondManagementAttUuid, 0x181E, "Bond Management Service")                                             \
    X(SERVICE_UUID_CONTINUOUS_GLUCOSE_MONITORING, serviceContinuousGlucoseMonitoringAttUuid, 0x181F, "Continuous Glucose Monitoring Service")    \
    X(SERVICE_UUID_INTERNET_PROTOCOL_SUPPORT, serviceInternetProtocolSupportAttUuid, 0x1820, "Internet Protocol Support Service")                \
    X(SERVICE_UUID_INDOOR_POSITIONING, serviceIndoorPositioningAttUuid, 0x1821, "Indoor Positioning Service")                                    \
    X(SERVICE_UUID_PULSE_OXIMETER, servicePulseOximeterAttUuid, 0x1822, "Pulse Oximeter Service")                                                \
    X(SERVICE_UUID_HTTP_PROXY, serviceHttpProxyAttUuid, 0x1823, "HTTP Proxy Service")                                                            \
    X(SERVICE_UUID_TRANSPORT_DISCOVERY, serviceTransportDiscoveryAttUuid, 0x1824, "Transport Discovery Service")                                 \
    X(SERVICE_UUID_OBJECT_TRANSFER, serviceObjectTransferAttUuid, 0x1825, "Object Transfer Service")                                             \
    X(SERVICE_UUID_FITNESS_MACHINE, serviceFitnessMachineAttUuid, 0x1826, "Fitness Machine Service")                                             \
    X(SERVICE_UUID_MESH_PROVISIONING, serviceMeshProvisioningAttUuid, 0x1827, "Mesh Provisioning Service")                                       \
    X(SERVICE_UUID_MESH_PROXY, serviceMeshProxyAttUuid, 0x1828, "Mesh Proxy Service")                                                            \
    X(SERVICE_UUID_RECONNECTION_CONFIGURATION, serviceReconnectionConfigurationAttUuid, 0x1829, "Reconnection Configuration Service")            \
    X(SERVICE_UUID_INSULIN_DELIVERY, serviceInsulinDeliveryAttUuid, 0x183A, "Insulin Delivery Service")                                          \
    X(SERVICE_UUID_BINARY_SENSOR, serviceBinarySensorAttUuid, 0x183B, "Binary Sensor Service")                                                   \
    X(SERVICE_UUID_EMERGENCY_CONFIGURATION, serviceEmergencyConfigurationAttUuid, 0x183C, "Emergency Configuration Service")                     \
    X(SERVICE_UUID_AUTHORIZATION_CONTROL, serviceAuthorizationControlAttUuid, 0x183D, "Authorization Control Service")                           \
    X(SERVICE_UUID_PHYSICAL_ACTIVITY_MONITOR, servicePhysicalActivityMonitorAttUuid, 0x183E, "Physical Activity Monitor Service")                \
    X(SERVICE_UUID_ELAPSED_TIME, serviceElapsedTimeAttUuid, 0x183F, "Elapsed Time Service")                                                      \
    X(SERVICE_UUID_GENERIC_HEALTH_SENSOR, serviceGenericHealthSensorAttUuid, 0x1840, "Generic Health Sensor Service")                            \
    X(SERVICE_UUID_AUDIO_INPUT_CONTROL, serviceAudioInputControlAttUuid, 0x1843, "Audio Input Control Service")                                  \
    X(SERVICE_UUID_VOLUME_CONTROL, serviceVolumeControlAttUuid, 0x1844, "Volume Control Service")                                                \
    X(SERVICE_UUID_VOLUME_OFFSET_CONTROL, serviceVolumeOffsetControlAttUuid, 0x1845, "Volume Offset Control Service")                            \
    X(SERVICE_UUID_COORDINATED_SET_IDENTIFICATION, serviceCoordinatedSetIdentificationAttUuid, 0x1846, "Coordinated Set Identification Service") \
    X(SERVICE_UUID_DEVICE_TIME, serviceDeviceTimeAttUuid, 0x1847, "Device Time Service")                                                         \
    X(SERVICE_UUID_MEDIA_CONTROL, serviceMediaControlAttUuid, 0x1848, "Media Control Service")                                                   \
    X(SERVICE_UUID_GENERIC_MEDIA_CONTROL, serviceGenericMediaControlAttUuid, 0x1849, "Generic Media Control Service")                            \
    X(SERVICE_UUID_CONSTANT_TONE_EXTENSION, serviceConstantToneExtensionAttUuid, 0x184A, "Constant Tone Extension Service")                      \
    X(SERVICE_UUID_TELEPHONE_BEARER, serviceTelephoneBearerAttUuid, 0x184B, "Telephone Bearer Service")                                          \
    X(SERVICE_UUID_GENERIC_TELEPHONE_BEARER, serviceGenericTelephoneBearerAttUuid, 0x184C, "Generic Telephone Bearer Service")                   \
    X(SERVICE_UUID_MICROPHONE_CONTROL, serviceMicrophoneControlAttUuid, 0x184D, "Microphone Control Service")                                    \
    X(SERVICE_UUID_AUDIO_STREAM_CONTROL, serviceAudioStreamControlAttUuid, 0x184E, "Audio Stream Control Service")                               \
    X(SERVICE_UUID_BROADCAST_AUDIO_SCAN, serviceBroadcastAudioScanAttUuid, 0x184F, "Broadcast Audio Scan Service")                               \
    X(SERVICE_UUID_PUBLISHED_AUDIO_CAPABILITIES, servicePublishedAudioCapabilitiesAttUuid, 0x1850, "Published Audio Capabilities Service")       \
    X(SERVICE_UUID_BASIC_AUDIO_ANNOUNCEMENT, serviceBasicAudioAnnouncementAttUuid, 0x1851, "Basic Audio Announcement Service")                   \
    X(SERVICE_UUID_BROADCAST_AUDIO_ANNOUNCEMENT, serviceBroadcastAudioAnnouncementAttUuid, 0x1852, "Broadcast Audio Announcement Service")       \
    X(SERVICE_UUID_COMMON_AUDIO, serviceCommonAudioAttUuid, 0x1853, "Common Audio Service")                                                      \
    X(SERVICE_UUID_HEARING_ACCESS, serviceHearingAccessAttUuid, 0x1854, "Hearing Access Service")                                                \
    X(SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO, serviceTelephonyAndMediaAudioAttUuid, 0x1855, "Telephony and Media Audio Service")                 \
    X(SERVICE_UUID_PUBLIC_BROADCAST_ANNOUNCEMENT, servicePublicBroadcastAnnouncementAttUuid, 0x1856, "Public Broadcast Announcement Service")    \
    X(SERVICE_UUID_ELECTRONIC_SHELF_LABEL, serviceElectronicShelfLabelAttUuid, 0x1857, "Electronic Shelf Label Service")                         \
    X(SERVICE_UUID_GAMING_AUDIO, serviceGamingAudioAttUuid, 0x1858, "Gaming Audio Service")                                                      \
    X(SERVICE_UUID_MESH_PROXY_SOLICITATION, serviceMeshProxySolicitationAttUuid, 0x1859, "Mesh Proxy Solicitation Service")                      \
    X(SERVICE_UUID_RANGING, serviceRangingAttUuid, 0x185A, "Ranging Service")                                                                    \
    X(SERVICE_UUID_HID_OVER_ISO, serviceHidOverIsoAttUuid, 0x185C, "Ultra Low Latency HID Service")

/**
 *  @brief Definition for Declaration UUID
 */
#define BLE_HOST_DECLARATIONS_UUIDS                                                                          \
    X(DECLARATIONS_UUID_PRIMARY_SERVICE, declarationsPrimaryServiceAttUuid, 0x2800, "Primary Service")       \
    X(DECLARATIONS_UUID_SECONDARY_SERVICE, declarationsSecondaryServiceAttUuid, 0x2801, "Secondary Service") \
    X(DECLARATIONS_UUID_INCLUDE, declarationsIncludeAttUuid, 0x2802, "Include")                              \
    X(DECLARATIONS_UUID_CHARACTERISTIC, declarationsCharacteristicAttUuid, 0x2803, "Characteristic")

/**
 * @brief Definition for Descriptor UUID
 */
#define BLE_HOST_DESCRIPTOR_UUIDS                                                                                                                                  \
    X(DESCRIPTOR_UUID_CHARACTERISTIC_EXTENDED_PROPERTIES, descriptorCharacteristicExtendedPropertiesAttUuid, 0x2900, "Characteristic Extended Properties")         \
    X(DESCRIPTOR_UUID_CHARACTERISTIC_USER_DESCRIPTION, descriptorCharacteristicUserDescriptionAttUuid, 0x2901, "Characteristic User Description")                  \
    X(DESCRIPTOR_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION, descriptorClientCharacteristicConfigurationAttUuid, 0x2902, "Client Characteristic Configuration")      \
    X(DESCRIPTOR_UUID_SERVER_CHARACTERISTIC_CONFIGURATION, descriptorServerCharacteristicConfigurationAttUuid, 0x2903, "Server Characteristic Configuration")      \
    X(DESCRIPTOR_UUID_CHARACTERISTIC_PRESENTATION_FORMAT, descriptorCharacteristicPresentationFormatAttUuid, 0x2904, "Characteristic Presentation Format")         \
    X(DESCRIPTOR_UUID_CHARACTERISTIC_AGGREGATE_FORMAT, descriptorCharacteristicAggregateFormatAttUuid, 0x2905, "Characteristic Aggregate Format")                  \
    X(DESCRIPTOR_UUID_VALID_RANGE, descriptorValidRangeAttUuid, 0x2906, "Valid Range")                                                                             \
    X(DESCRIPTOR_UUID_EXTERNAL_REPORT_REFERENCE, descriptorExternalReportReferenceAttUuid, 0x2907, "External Report Reference")                                    \
    X(DESCRIPTOR_UUID_REPORT_REFERENCE, descriptorReportReferenceAttUuid, 0x2908, "Report Reference")                                                              \
    X(DESCRIPTOR_UUID_NUMBER_OF_DIGITALS, descriptorNumberOfDigitalsAttUuid, 0x2909, "Number of Digitals")                                                         \
    X(DESCRIPTOR_UUID_VALUE_TRIGGER_SETTING, descriptorValueTriggerSettingAttUuid, 0x290A, "Value Trigger Setting")                                                \
    X(DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_CONFIGURATION, descriptorEnvironmentalSensingConfigurationAttUuid, 0x290B, "Environmental Sensing Configuration")      \
    X(DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_MEASUREMENT, descriptorEnvironmentalSensingMeasurementAttUuid, 0x290C, "Environmental Sensing Measurement")            \
    X(DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_TRIGGER_SETTING, descriptorEnvironmentalSensingTriggerSettingAttUuid, 0x290D, "Environmental Sensing Trigger Setting") \
    X(DESCRIPTOR_UUID_TIME_TRIGGER_SETTING, descriptorTimeTriggerSettingAttUuid, 0x290E, "Time Trigger Setting")                                                   \
    X(DESCRIPTOR_UUID_COMPLETE_BR_EDR_TRANSPORT_BLOCK_DATA, descriptorCompleteBrEdrTransportBlockDataAttUuid, 0x290F, "Complete BR-EDR Transport Block Data")      \
    X(DESCRIPTOR_UUID_OBSERVATION_SCHEDULE, descriptorObservationScheduleAttUuid, 0x2910, "Observation Schedule")                                                  \
    X(DESCRIPTOR_UUID_VALID_RANGE_AND_ACCURACY, descriptorValidRangeAndAccuracyAttUuid, 0x2911, "Valid Range and Accuracy")

/**
 * @brief Definition for Characteristic UUID
 */
#define BLE_HOST_CHARACTERISTIC_UUIDS                                                                                                                                      \
    X(CHARACTERISTIC_UUID_DEVICE_NAME, characteristicDeviceNameAttUuid, 0x2A00, "Device Name")                                                                             \
    X(CHARACTERISTIC_UUID_APPEARANCE, characteristicAppearanceAttUuid, 0x2A01, "Appearance")                                                                               \
    X(CHARACTERISTIC_UUID_PERIPHERAL_PRIVACY_FLAG, characteristicPeripheralPrivacyFlagAttUuid, 0x2A02, "Peripheral Privacy Flag")                                          \
    X(CHARACTERISTIC_UUID_RECONNECTION_ADDRESS, characteristicReconnectionAddressAttUuid, 0x2A03, "Reconnection Address")                                                  \
    X(CHARACTERISTIC_UUID_PERIPHERAL_PREFERRED_CONN_PARAM, characteristicPeripheralPreferredConnParamAttUuid, 0x2A04, "Peripheral Preferred Connection Parameters")        \
    X(CHARACTERISTIC_UUID_SERVICE_CHANGED, characteristicServiceChangedAttUuid, 0x2A05, "Service Changed")                                                                 \
    X(CHARACTERISTIC_UUID_ALERT_LEVEL, characteristicAlertLevelAttUuid, 0x2A06, "Alert Level")                                                                             \
    X(CHARACTERISTIC_UUID_TX_POWER_LEVEL, characteristicTxPowerLevelAttUuid, 0x2A07, "Tx Power Level")                                                                     \
    X(CHARACTERISTIC_UUID_DATE_TIME, characteristicDateTimeAttUuid, 0x2A08, "Date Time")                                                                                   \
    X(CHARACTERISTIC_UUID_DAYOF_WEEK, characteristicDayofWeekAttUuid, 0x2A09, "Dayof Week")                                                                                \
    X(CHARACTERISTIC_UUID_DAY_DATE_TIME, characteristicDayDateTimeAttUuid, 0x2A0A, "Day Date Time")                                                                        \
    X(CHARACTERISTIC_UUID_EXACT_TIME_256, characteristicExactTime256AttUuid, 0x2A0C, "Exact Time 256")                                                                     \
    X(CHARACTERISTIC_UUID_DST_OFFSET, characteristicDstOffsetAttUuid, 0x2A0D, "DST Offset")                                                                                \
    X(CHARACTERISTIC_UUID_TIME_ZONE, characteristicTimeZoneAttUuid, 0x2A0E, "Time Zone")                                                                                   \
    X(CHARACTERISTIC_UUID_LOCAL_TIME_INFORMATION, characteristicLocalTimeInformationAttUuid, 0x2A0F, "Local Time Information")                                             \
    X(CHARACTERISTIC_UUID_TIME_WITH_DST, characteristicTimeWithDstAttUuid, 0x2A11, "Time with DST")                                                                        \
    X(CHARACTERISTIC_UUID_TIME_ACCURACY, characteristicTimeAccuracyAttUuid, 0x2A12, "Time Accuracy")                                                                       \
    X(CHARACTERISTIC_UUID_TIME_SOURCE, characteristicTimeSourceAttUuid, 0x2A13, "Time Source")                                                                             \
    X(CHARACTERISTIC_UUID_REFERENCE_TIME_INFORMATION, characteristicReferenceTimeInformationAttUuid, 0x2A14, "Reference Time Information")                                 \
    X(CHARACTERISTIC_UUID_TIME_UPDATE_CONTROL_POINT, characteristicTimeUpdateControlPointAttUuid, 0x2A16, "Time Update Control Point")                                     \
    X(CHARACTERISTIC_UUID_TIME_UPDATE_STATE, characteristicTimeUpdateStateAttUuid, 0x2A17, "Time Update State")                                                            \
    X(CHARACTERISTIC_UUID_GLUCOSE_MEASUREMENT, characteristicGlucoseMeasurementAttUuid, 0x2A18, "Glucose Measurement")                                                     \
    X(CHARACTERISTIC_UUID_BATTERY_LEVEL, characteristicBatteryLevelAttUuid, 0x2A19, "Battery Level")                                                                       \
    X(CHARACTERISTIC_UUID_BATTERY_POWER_STATE, characteristicBatteryPowerStateAttUuid, 0x2A1A, "Battery Power state")                                                      \
    X(CHARACTERISTIC_UUID_TEMPERATURE_MEASUREMENT, characteristicTemperatureMeasurementAttUuid, 0x2A1C, "Temperature Measurement")                                         \
    X(CHARACTERISTIC_UUID_TEMPERATURE_TYPE, characteristicTemperatureTypeAttUuid, 0x2A1D, "Temperature Type")                                                              \
    X(CHARACTERISTIC_UUID_INTERMEDIATE_TEMPERATURE, characteristicIntermediateTemperatureAttUuid, 0x2A1E, "Intermediate Temperature")                                      \
    X(CHARACTERISTIC_UUID_MEASUREMENT_INTERVAL, characteristicMeasurementIntervalAttUuid, 0x2A21, "Measurement Interval")                                                  \
    X(CHARACTERISTIC_UUID_BOOT_KEYBOARD_INPUT_REPORT, characteristicBootKeyboardInputReportAttUuid, 0x2A22, "Boot Keyboard Input Report")                                  \
    X(CHARACTERISTIC_UUID_SYSTEM_ID, characteristicSystemIdAttUuid, 0x2A23, "System ID")                                                                                   \
    X(CHARACTERISTIC_UUID_MODEL_NUMBER_STRING, characteristicModelNumberStringAttUuid, 0x2A24, "Model Number String")                                                      \
    X(CHARACTERISTIC_UUID_SERIAL_NUMBER_STRING, characteristicSerialNumberStringAttUuid, 0x2A25, "Serial Number String")                                                   \
    X(CHARACTERISTIC_UUID_FIRMWARE_REVISION_STRING, characteristicFirmwareRevisionStringAttUuid, 0x2A26, "Firmware Revision String")                                       \
    X(CHARACTERISTIC_UUID_HARDWARE_REVISION_STRING, characteristicHardwareRevisionStringAttUuid, 0x2A27, "Hardware Revision String")                                       \
    X(CHARACTERISTIC_UUID_SOFTWARE_REVISION_STRING, characteristicSoftwareRevisionStringAttUuid, 0x2A28, "Software Revision String")                                       \
    X(CHARACTERISTIC_UUID_MANUFACTURER_NAME_STRING, characteristicManufacturerNameStringAttUuid, 0x2A29, "Manufacturer Name String")                                       \
    X(CHARACTERISTIC_UUID_IEEE11073_20601_DATA_LIST, characteristicIeee1107320601DataListAttUuid, 0x2A2A, "IEEE11073-20601 Regulatory Certification Data List")            \
    X(CHARACTERISTIC_UUID_CURRENT_TIME, characteristicCurrentTimeAttUuid, 0x2A2B, "Current Time")                                                                          \
    X(CHARACTERISTIC_UUID_MAGNETIC_DECLINATION, characteristicMagneticDeclinationAttUuid, 0x2A2C, "Magnetic Declination")                                                  \
    X(CHARACTERISTIC_UUID_SCAN_REFRESH, characteristicScanRefreshAttUuid, 0x2A31, "Scan Refresh")                                                                          \
    X(CHARACTERISTIC_UUID_BOOT_KEYBOARD_OUTPUT_REPORT, characteristicBootKeyboardOutputReportAttUuid, 0x2A32, "Boot Keyboard Output Report")                               \
    X(CHARACTERISTIC_UUID_BOOT_MOUSE_INPUT_REPORT, characteristicBootMouseInputReportAttUuid, 0x2A33, "Boot Mouse Input Report")                                           \
    X(CHARACTERISTIC_UUID_GLUCOSE_MEASUREMENT_CONTEXT, characteristicGlucoseMeasurementContextAttUuid, 0x2A34, "Glucose Measurement Context")                              \
    X(CHARACTERISTIC_UUID_BLOOD_PRESSURE_MEASUREMENT, characteristicBloodPressureMeasurementAttUuid, 0x2A35, "Blood Pressure Measurement")                                 \
    X(CHARACTERISTIC_UUID_INTERMEDIATE_CUFF_PRESSURE, characteristicIntermediateCuffPressureAttUuid, 0x2A36, "Intermediate Cuff Pressure")                                 \
    X(CHARACTERISTIC_UUID_HEART_RATE_MEASUREMENT, characteristicHeartRateMeasurementAttUuid, 0x2A37, "Heart Rate Measurement")                                             \
    X(CHARACTERISTIC_UUID_BODY_SENSOR_LOCATION, characteristicBodySensorLocationAttUuid, 0x2A38, "Body Sensor Location")                                                   \
    X(CHARACTERISTIC_UUID_HEART_RATE_CONTROL_POINT, characteristicHeartRateControlPointAttUuid, 0x2A39, "Heart Rate Control Point")                                        \
    X(CHARACTERISTIC_UUID_ALERT_STATUS, characteristicAlertStatusAttUuid, 0x2A3F, "Alert Status")                                                                          \
    X(CHARACTERISTIC_UUID_RINGER_CONTROL_POINT, characteristicRingerControlPointAttUuid, 0x2A40, "Ringer Control Point")                                                   \
    X(CHARACTERISTIC_UUID_RINGER_SETTING, characteristicRingerSettingAttUuid, 0x2A41, "Ringer Setting")                                                                    \
    X(CHARACTERISTIC_UUID_ALERT_CATEGORY_ID_BIT_MASK, characteristicAlertCategoryIdBitMaskAttUuid, 0x2A42, "Alert Category ID Bit Mask")                                   \
    X(CHARACTERISTIC_UUID_ALERT_CATEGORY_ID, characteristicAlertCategoryIdAttUuid, 0x2A43, "Alert Category ID")                                                            \
    X(CHARACTERISTIC_UUID_ALERT_NOTIFICATION_CONTROL_POINT, characteristicAlertNotificationControlPointAttUuid, 0x2A44, "Alert Notification Control Point")                \
    X(CHARACTERISTIC_UUID_UNREAD_ALERT_STATUS, characteristicUnreadAlertStatusAttUuid, 0x2A45, "Unread Alert Status")                                                      \
    X(CHARACTERISTIC_UUID_NEW_ALERT, characteristicNewAlertAttUuid, 0x2A46, "New Alert")                                                                                   \
    X(CHARACTERISTIC_UUID_SUPPORTED_NEW_ALERT_CATEGORY, characteristicSupportedNewAlertCategoryAttUuid, 0x2A47, "Supported New Alert Category")                            \
    X(CHARACTERISTIC_UUID_SUPPORTED_UNREAD_ALERT_CATEGORY, characteristicSupportedUnreadAlertCategoryAttUuid, 0x2A48, "Supported Unread Alert Category")                   \
    X(CHARACTERISTIC_UUID_BLOOD_PRESSURE_FEATURE, characteristicBloodPressureFeatureAttUuid, 0x2A49, "Blood Pressure Feature")                                             \
    X(CHARACTERISTIC_UUID_HID_INFORMATION, characteristicHidInformationAttUuid, 0x2A4A, "HID Information")                                                                 \
    X(CHARACTERISTIC_UUID_REPORT_MAP, characteristicReportMapAttUuid, 0x2A4B, "Report Map")                                                                                \
    X(CHARACTERISTIC_UUID_HID_CONTROL_POINT, characteristicHidControlPointAttUuid, 0x2A4C, "HID Control Point")                                                            \
    X(CHARACTERISTIC_UUID_REPORT, characteristicReportAttUuid, 0x2A4D, "Report")                                                                                           \
    X(CHARACTERISTIC_UUID_PROTOCOL_MODE, characteristicProtocolModeAttUuid, 0x2A4E, "Protocol Mode")                                                                       \
    X(CHARACTERISTIC_UUID_SCAN_INTERVAL_WINDOW, characteristicScanIntervalWindowAttUuid, 0x2A4F, "Scan Interval Window")                                                   \
    X(CHARACTERISTIC_UUID_PNP_ID, characteristicPnpIdAttUuid, 0x2A50, "PnP ID")                                                                                            \
    X(CHARACTERISTIC_UUID_GLUCOSE_FEATURE, characteristicGlucoseFeatureAttUuid, 0x2A51, "Glucose Feature")                                                                 \
    X(CHARACTERISTIC_UUID_RECORD_ACCESS_CONTROL_POINT, characteristicRecordAccessControlPointAttUuid, 0x2A52, "Record Access Control Point")                               \
    X(CHARACTERISTIC_UUID_RSC_MEASUREMENT, characteristicRscMeasurementAttUuid, 0x2A53, "RSC Measurement")                                                                 \
    X(CHARACTERISTIC_UUID_RSC_FEATURE, characteristicRscFeatureAttUuid, 0x2A54, "RSC Feature")                                                                             \
    X(CHARACTERISTIC_UUID_SC_CONTROL_POINT, characteristicScControlPointAttUuid, 0x2A55, "SC Control Point")                                                               \
    X(CHARACTERISTIC_UUID_AGGREGATE, characteristicAggregateAttUuid, 0x2A5A, "Aggregate")                                                                                  \
    X(CHARACTERISTIC_UUID_CSC_MEASUREMENT, characteristicCscMeasurementAttUuid, 0x2A5B, "CSC Measurement")                                                                 \
    X(CHARACTERISTIC_UUID_CSC_FEATURE, characteristicCscFeatureAttUuid, 0x2A5C, "CSC Feature")                                                                             \
    X(CHARACTERISTIC_UUID_SENSOR_LOCATION, characteristicSensorLocationAttUuid, 0x2A5D, "Sensor Location")                                                                 \
    X(CHARACTERISTIC_UUID_PLX_SPOT_CHECK_MEASUREMENT, characteristicPlxSpotCheckMeasurementAttUuid, 0x2A5E, "PLX Spot-Check Measurement")                                  \
    X(CHARACTERISTIC_UUID_PLX_CONTINUOUS_MEASUREMENT, characteristicPlxContinuousMeasurementAttUuid, 0x2A5F, "PLX Continuous Measurement")                                 \
    X(CHARACTERISTIC_UUID_PLX_FEATURES, characteristicPlxFeaturesAttUuid, 0x2A60, "PLX Features")                                                                          \
    X(CHARACTERISTIC_UUID_CYCLING_POWER_MEASUREMENT, characteristicCyclingPowerMeasurementAttUuid, 0x2A63, "Cycling Power Measurement")                                    \
    X(CHARACTERISTIC_UUID_CYCLING_POWER_VECTOR, characteristicCyclingPowerVectorAttUuid, 0x2A64, "Cycling Power Vector")                                                   \
    X(CHARACTERISTIC_UUID_CYCLING_POWER_FEATURE, characteristicCyclingPowerFeatureAttUuid, 0x2A65, "Cycling Power Feature")                                                \
    X(CHARACTERISTIC_UUID_CYCLING_POWER_CONTROL_POINT, characteristicCyclingPowerControlPointAttUuid, 0x2A66, "Cycling Power Control Point")                               \
    X(CHARACTERISTIC_UUID_LOCATION_AND_SPEED, characteristicLocationAndSpeedAttUuid, 0x2A67, "Location and Speed")                                                         \
    X(CHARACTERISTIC_UUID_NAVIGATION, characteristicNavigationAttUuid, 0x2A68, "Navigation")                                                                               \
    X(CHARACTERISTIC_UUID_POSITION_QUALITY, characteristicPositionQualityAttUuid, 0x2A69, "Position Quality")                                                              \
    X(CHARACTERISTIC_UUID_LN_FEATURE, characteristicLnFeatureAttUuid, 0x2A6A, "LN Feature")                                                                                \
    X(CHARACTERISTIC_UUID_LN_CONTROL_POINT, characteristicLnControlPointAttUuid, 0x2A6B, "LN Control Point")                                                               \
    X(CHARACTERISTIC_UUID_ELEVATION, characteristicElevationAttUuid, 0x2A6C, "Elevation")                                                                                  \
    X(CHARACTERISTIC_UUID_PRESSURE, characteristicPressureAttUuid, 0x2A6D, "Pressure")                                                                                     \
    X(CHARACTERISTIC_UUID_TEMPERATURE, characteristicTemperatureAttUuid, 0x2A6E, "Temperature")                                                                            \
    X(CHARACTERISTIC_UUID_HUMIDITY, characteristicHumidityAttUuid, 0x2A6F, "Humidity")                                                                                     \
    X(CHARACTERISTIC_UUID_TRUE_WIND_SPEED, characteristicTrueWindSpeedAttUuid, 0x2A70, "True Wind Speed")                                                                  \
    X(CHARACTERISTIC_UUID_TRUE_WIND_DIRECTION, characteristicTrueWindDirectionAttUuid, 0x2A71, "True Wind Direction")                                                      \
    X(CHARACTERISTIC_UUID_APPARENT_WIND_SPEED, characteristicApparentWindSpeedAttUuid, 0x2A72, "Apparent Wind Speed")                                                      \
    X(CHARACTERISTIC_UUID_APPARENT_WIND_DIRECTION, characteristicApparentWindDirectionAttUuid, 0x2A73, "Apparent Wind Direction")                                          \
    X(CHARACTERISTIC_UUID_GUST_FACTOR, characteristicGustFactorAttUuid, 0x2A74, "Gust Factor")                                                                             \
    X(CHARACTERISTIC_UUID_POLLEN_CONC, characteristicPollenConcAttUuid, 0x2A75, "Pollen Concentration")                                                                    \
    X(CHARACTERISTIC_UUID_UV_INDEX, characteristicUvIndexAttUuid, 0x2A76, "UV Index")                                                                                      \
    X(CHARACTERISTIC_UUID_IRRADIANCE, characteristicIrradianceAttUuid, 0x2A77, "Irradiance")                                                                               \
    X(CHARACTERISTIC_UUID_RAINFALL, characteristicRainfallAttUuid, 0x2A78, "Rainfall")                                                                                     \
    X(CHARACTERISTIC_UUID_WIND_CHILL, characteristicWindChillAttUuid, 0x2A79, "Wind Chill")                                                                                \
    X(CHARACTERISTIC_UUID_HEAT_INDEX, characteristicHeatIndexAttUuid, 0x2A7A, "Heat Index")                                                                                \
    X(CHARACTERISTIC_UUID_DEW_POINT, characteristicDewPointAttUuid, 0x2A7B, "Dew Point")                                                                                   \
    X(CHARACTERISTIC_UUID_DESCRIPTOR_VALUE_CHANGED, characteristicDescriptorValueChangedAttUuid, 0x2A7D, "Descriptor Value Changed")                                       \
    X(CHARACTERISTIC_UUID_AEROBIC_HEART_RATE_LOWER_LIMIT, characteristicAerobicHeartRateLowerLimitAttUuid, 0x2A7E, "Aerobic Heart Rate Lower Limit")                       \
    X(CHARACTERISTIC_UUID_AEROBIC_THRESHOLD, characteristicAerobicThresholdAttUuid, 0x2A7F, "Aerobic Threshold")                                                           \
    X(CHARACTERISTIC_UUID_AGE, characteristicAgeAttUuid, 0x2A80, "Age")                                                                                                    \
    X(CHARACTERISTIC_UUID_ANAEROBIC_HEART_RATE_LOWER_LIMIT, characteristicAnaerobicHeartRateLowerLimitAttUuid, 0x2A81, "Anaerobic Heart Rate Lower Limit")                 \
    X(CHARACTERISTIC_UUID_ANAEROBIC_HEART_RATE_UPPER_LIMIT, characteristicAnaerobicHeartRateUpperLimitAttUuid, 0x2A82, "Anaerobic Heart Rate Upper Limit")                 \
    X(CHARACTERISTIC_UUID_ANAEROBIC_THRESHOLD, characteristicAnaerobicThresholdAttUuid, 0x2A83, "Anaerobic Threshold")                                                     \
    X(CHARACTERISTIC_UUID_AEROBIC_HEART_RATE_UPPER_LIMIT, characteristicAerobicHeartRateUpperLimitAttUuid, 0x2A84, "Aerobic Heart Rate Upper Limit")                       \
    X(CHARACTERISTIC_UUID_DATE_OF_BIRTH, characteristicDateOfBirthAttUuid, 0x2A85, "Date of Birth")                                                                        \
    X(CHARACTERISTIC_UUID_DATE_OF_THRESHOLD_ASSESSMENT, characteristicDateOfThresholdAssessmentAttUuid, 0x2A86, "Date of Threshold Assessment")                            \
    X(CHARACTERISTIC_UUID_EMAIL_ADDRESS, characteristicEmailAddressAttUuid, 0x2A87, "Email Address")                                                                       \
    X(CHARACTERISTIC_UUID_FAT_BURN_HEART_RATE_LOWER_LIMIT, characteristicFatBurnHeartRateLowerLimitAttUuid, 0x2A88, "Fat Burn Heart Rate Lower Limit")                     \
    X(CHARACTERISTIC_UUID_FAT_BURN_HEART_RATE_UPPER_LIMIT, characteristicFatBurnHeartRateUpperLimitAttUuid, 0x2A89, "Fat Burn Heart Rate Upper Limit")                     \
    X(CHARACTERISTIC_UUID_FIRST_NAME, characteristicFirstNameAttUuid, 0x2A8A, "First Name")                                                                                \
    X(CHARACTERISTIC_UUID_FIVE_ZONE_HEART_RATE_LIMITS, characteristicFiveZoneHeartRateLimitsAttUuid, 0x2A8B, "Five Zone Heart Rate Limits")                                \
    X(CHARACTERISTIC_UUID_GENDER, characteristicGenderAttUuid, 0x2A8C, "Gender")                                                                                           \
    X(CHARACTERISTIC_UUID_HEART_RATE_MAX, characteristicHeartRateMaxAttUuid, 0x2A8D, "Heart Rate Max")                                                                     \
    X(CHARACTERISTIC_UUID_HEIGHT, characteristicHeightAttUuid, 0x2A8E, "Height")                                                                                           \
    X(CHARACTERISTIC_UUID_HIP_CIRCUMFERENCE, characteristicHipCircumferenceAttUuid, 0x2A8F, "Hip Circumference")                                                           \
    X(CHARACTERISTIC_UUID_LAST_NAME, characteristicLastNameAttUuid, 0x2A90, "Last Name")                                                                                   \
    X(CHARACTERISTIC_UUID_MAXIMUM_RECOMMENDED_HEART_RATE, characteristicMaximumRecommendedHeartRateAttUuid, 0x2A91, "Maximum Recommended Heart Rate")                      \
    X(CHARACTERISTIC_UUID_RESTING_HEART_RATE, characteristicRestingHeartRateAttUuid, 0x2A92, "Resting Heart Rate")                                                         \
    X(CHARACTERISTIC_UUID_SPORT_TYPE, characteristicSportTypeAttUuid, 0x2A93, "Sport Type for Aerobic and Anaerobic Thresholds")                                           \
    X(CHARACTERISTIC_UUID_THREE_ZONE_HEART_RATE_LIMITS, characteristicThreeZoneHeartRateLimitsAttUuid, 0x2A94, "Three Zone Heart Rate Limits")                             \
    X(CHARACTERISTIC_UUID_TWO_ZONE_HEART_RATE_LIMITS, characteristicTwoZoneHeartRateLimitsAttUuid, 0x2A95, "Two Zone Heart Rate Limits")                                   \
    X(CHARACTERISTIC_UUID_VO2_MAX, characteristicVo2MaxAttUuid, 0x2A96, "VO2 Max")                                                                                         \
    X(CHARACTERISTIC_UUID_WAIST_CIRCUMFERENCE, characteristicWaistCircumferenceAttUuid, 0x2A97, "Waist Circumference")                                                     \
    X(CHARACTERISTIC_UUID_WEIGHT, characteristicWeightAttUuid, 0x2A98, "Weight")                                                                                           \
    X(CHARACTERISTIC_UUID_DATABASE_CHANGE_INCREMENT, characteristicDatabaseChangeIncrementAttUuid, 0x2A99, "Database Change Increment")                                    \
    X(CHARACTERISTIC_UUID_USER_INDEX, characteristicUserIndexAttUuid, 0x2A9A, "User Index")                                                                                \
    X(CHARACTERISTIC_UUID_BODY_COMPOSITION_FEATURE, characteristicBodyCompositionFeatureAttUuid, 0x2A9B, "Body Composition Feature")                                       \
    X(CHARACTERISTIC_UUID_BODY_COMPOSITION_MEASUREMENT, characteristicBodyCompositionMeasurementAttUuid, 0x2A9C, "Body Composition Measurement")                           \
    X(CHARACTERISTIC_UUID_WEIGHT_MEASUREMENT, characteristicWeightMeasurementAttUuid, 0x2A9D, "Weight Measurement")                                                        \
    X(CHARACTERISTIC_UUID_WEIGHT_SCALE_FEATURE, characteristicWeightScaleFeatureAttUuid, 0x2A9E, "Weight Scale Feature")                                                   \
    X(CHARACTERISTIC_UUID_USER_CONTROL_POINT, characteristicUserControlPointAttUuid, 0x2A9F, "User Control Point")                                                         \
    X(CHARACTERISTIC_UUID_MAGNETIC_FLUX_DENSITY_2D, characteristicMagneticFluxDensity2dAttUuid, 0x2AA0, "Magnetic Flux Density-2D")                                        \
    X(CHARACTERISTIC_UUID_MAGNETIC_FLUX_DENSITY_3D, characteristicMagneticFluxDensity3dAttUuid, 0x2AA1, "Magnetic Flux Density-3D")                                        \
    X(CHARACTERISTIC_UUID_LANGUAGE, characteristicLanguageAttUuid, 0x2AA2, "Language")                                                                                     \
    X(CHARACTERISTIC_UUID_BAROMETRIC_PRESSURE_TREND, characteristicBarometricPressureTrendAttUuid, 0x2AA3, "Barometric Pressure Trend")                                    \
    X(CHARACTERISTIC_UUID_BOND_MANAGEMENT_CONTROL_POINT, characteristicBondManagementControlPointAttUuid, 0x2AA4, "Bond Management Control Point")                         \
    X(CHARACTERISTIC_UUID_BOND_MANAGEMENT_FEATURE, characteristicBondManagementFeatureAttUuid, 0x2AA5, "Bond Management Feature")                                          \
    X(CHARACTERISTIC_UUID_CENTRAL_ADDRESS_RESOLUTION, characteristicCentralAddressResolutionAttUuid, 0x2AA6, "Central Address Resolution")                                 \
    X(CHARACTERISTIC_UUID_CGM_MEASUREMENT, characteristicCgmMeasurementAttUuid, 0x2AA7, "CGM	Measurement")                                                              \
    X(CHARACTERISTIC_UUID_CGM_FEATURE, characteristicCgmFeatureAttUuid, 0x2AA8, "CGM	Feature")                                                                          \
    X(CHARACTERISTIC_UUID_CGM_STATUS, characteristicCgmStatusAttUuid, 0x2AA9, "CGM	Status")                                                                               \
    X(CHARACTERISTIC_UUID_CGM_SESSION_START_TIME, characteristicCgmSessionStartTimeAttUuid, 0x2AAA, "CGM	Session Start Time")                                           \
    X(CHARACTERISTIC_UUID_CGM_SESSION_RUN_TIME, characteristicCgmSessionRunTimeAttUuid, 0x2AAB, "CGM	Session Run Time")                                                 \
    X(CHARACTERISTIC_UUID_CGM_SPECIFIC_OPS_CONTROL_POINT, characteristicCgmSpecificOpsControlPointAttUuid, 0x2AAC, "CGM	Specific Ops Control Point")                       \
    X(CHARACTERISTIC_UUID_INDOOR_POSITIONING_CONFIGURATION, characteristicIndoorPositioningConfigurationAttUuid, 0x2AAD, "Indoor Positioning Configuration")               \
    X(CHARACTERISTIC_UUID_LATITUDE, characteristicLatitudeAttUuid, 0x2AAE, "Latitude")                                                                                     \
    X(CHARACTERISTIC_UUID_LONGITUDE, characteristicLongitudeAttUuid, 0x2AAF, "Longitude")                                                                                  \
    X(CHARACTERISTIC_UUID_LOCAL_NORTH_COORDINATE, characteristicLocalNorthCoordinateAttUuid, 0x2AB0, "Local North Coordinate")                                             \
    X(CHARACTERISTIC_UUID_LOCAL_EAST_COORDINATE, characteristicLocalEastCoordinateAttUuid, 0x2AB1, "Local East Coordinate")                                                \
    X(CHARACTERISTIC_UUID_FLOOR_NUMBER, characteristicFloorNumberAttUuid, 0x2AB2, "Floor Number")                                                                          \
    X(CHARACTERISTIC_UUID_ALTITUDE, characteristicAltitudeAttUuid, 0x2AB3, "Altitude")                                                                                     \
    X(CHARACTERISTIC_UUID_UNCERTAINTY, characteristicUncertaintyAttUuid, 0x2AB4, "Uncertainty")                                                                            \
    X(CHARACTERISTIC_UUID_LOCATION_NAME, characteristicLocationNameAttUuid, 0x2AB5, "Location Name")                                                                       \
    X(CHARACTERISTIC_UUID_URI, characteristicUriAttUuid, 0x2AB6, "URI")                                                                                                    \
    X(CHARACTERISTIC_UUID_HTTP_HEADERS, characteristicHttpHeadersAttUuid, 0x2AB7, "HTTP Headers")                                                                          \
    X(CHARACTERISTIC_UUID_HTTP_STATUS_CODE, characteristicHttpStatusCodeAttUuid, 0x2AB8, "HTTP Status Code")                                                               \
    X(CHARACTERISTIC_UUID_HTTP_ENTITY_BODY, characteristicHttpEntityBodyAttUuid, 0x2AB9, "HTTP Entity Body")                                                               \
    X(CHARACTERISTIC_UUID_HTTP_CONTROL_POINT, characteristicHttpControlPointAttUuid, 0x2ABA, "HTTP Control Point")                                                         \
    X(CHARACTERISTIC_UUID_HTTPS_SECURITY, characteristicHttpsSecurityAttUuid, 0x2ABB, "HTTPS Security")                                                                    \
    X(CHARACTERISTIC_UUID_TDS_CONTROL_POINT, characteristicTdsControlPointAttUuid, 0x2ABC, "TDS Control Point")                                                            \
    X(CHARACTERISTIC_UUID_OTS_FEATURE, characteristicOtsFeatureAttUuid, 0x2ABD, "OTS Feature")                                                                             \
    X(CHARACTERISTIC_UUID_OBJECT_NAME, characteristicObjectNameAttUuid, 0x2ABE, "Object Name")                                                                             \
    X(CHARACTERISTIC_UUID_OBJECT_TYPE, characteristicObjectTypeAttUuid, 0x2ABF, "Object Type")                                                                             \
    X(CHARACTERISTIC_UUID_OBJECT_SIZE, characteristicObjectSizeAttUuid, 0x2AC0, "Object Size")                                                                             \
    X(CHARACTERISTIC_UUID_OBJECT_FIRST_CREATED, characteristicObjectFirstCreatedAttUuid, 0x2AC1, "Object First-Created")                                                   \
    X(CHARACTERISTIC_UUID_OBJECT_LAST_MODIFIED, characteristicObjectLastModifiedAttUuid, 0x2AC2, "Object Last-Modified")                                                   \
    X(CHARACTERISTIC_UUID_OBJECT_ID, characteristicObjectIdAttUuid, 0x2AC3, "Object ID")                                                                                   \
    X(CHARACTERISTIC_UUID_OBJECT_PROPERTIES, characteristicObjectPropertiesAttUuid, 0x2AC4, "Object Properties")                                                           \
    X(CHARACTERISTIC_UUID_OBJECT_ACTION_CONTROL_POINT, characteristicObjectActionControlPointAttUuid, 0x2AC5, "Object Action Control Point")                               \
    X(CHARACTERISTIC_UUID_OBJECT_LIST_CONTROL_POINT, characteristicObjectListControlPointAttUuid, 0x2AC6, "Object List Control Point")                                     \
    X(CHARACTERISTIC_UUID_OBJECT_LIST_FILTER, characteristicObjectListFilterAttUuid, 0x2AC7, "Object List Filter")                                                         \
    X(CHARACTERISTIC_UUID_OBJECT_CHANGED, characteristicObjectChangedAttUuid, 0x2AC8, "Object Changed")                                                                    \
    X(CHARACTERISTIC_UUID_RESOLVABLE_PRIVATE_ADDRESS_ONLY, characteristicResolvablePrivateAddressOnlyAttUuid, 0x2AC9, "Resolvable Private Address Only")                   \
    X(CHARACTERISTIC_UUID_FITNESS_MACHINE_FEATURE, characteristicFitnessMachineFeatureAttUuid, 0x2ACC, "Fitness Machine Feature")                                          \
    X(CHARACTERISTIC_UUID_TREADMILL_DATA, characteristicTreadmillDataAttUuid, 0x2ACD, "Treadmill Data")                                                                    \
    X(CHARACTERISTIC_UUID_CROSS_TRAINER_DATA, characteristicCrossTrainerDataAttUuid, 0x2ACE, "Cross Trainer Data")                                                         \
    X(CHARACTERISTIC_UUID_STEP_CLIMBER_DATA, characteristicStepClimberDataAttUuid, 0x2ACF, "Step Climber Data")                                                            \
    X(CHARACTERISTIC_UUID_STAIR_CLIMBER_DATA, characteristicStairClimberDataAttUuid, 0x2AD0, "Stair Climber Data")                                                         \
    X(CHARACTERISTIC_UUID_ROWER_DATA, characteristicRowerDataAttUuid, 0x2AD1, "Rower Data")                                                                                \
    X(CHARACTERISTIC_UUID_INDOOR_BIKE_DATA, characteristicIndoorBikeDataAttUuid, 0x2AD2, "Indoor Bike Data")                                                               \
    X(CHARACTERISTIC_UUID_TRAINING_STATUS, characteristicTrainingStatusAttUuid, 0x2AD3, "Training Status")                                                                 \
    X(CHARACTERISTIC_UUID_SUPPORTED_SPEED_RANGE, characteristicSupportedSpeedRangeAttUuid, 0x2AD4, "Supported Speed Range")                                                \
    X(CHARACTERISTIC_UUID_SUPPORTED_INCLINATION_RANGE, characteristicSupportedInclinationRangeAttUuid, 0x2AD5, "Supported Inclination Range")                              \
    X(CHARACTERISTIC_UUID_SUPPORTED_RESISTANCE_LEVEL_RANGE, characteristicSupportedResistanceLevelRangeAttUuid, 0x2AD6, "Supported Resistance Level Range")                \
    X(CHARACTERISTIC_UUID_SUPPORTED_HEART_RATE_RANGE, characteristicSupportedHeartRateRangeAttUuid, 0x2AD7, "Supported Heart Rate Range")                                  \
    X(CHARACTERISTIC_UUID_SUPPORTED_POWER_RANGE, characteristicSupportedPowerRangeAttUuid, 0x2AD8, "Supported Power Range")                                                \
    X(CHARACTERISTIC_UUID_FITNESS_MACHINE_CONTROL_POINT, characteristicFitnessMachineControlPointAttUuid, 0x2AD9, "Fitness Machine Control Point")                         \
    X(CHARACTERISTIC_UUID_FITNESS_MACHINE_STATUS, characteristicFitnessMachineStatusAttUuid, 0x2ADA, "Fitness Machine Status")                                             \
    X(CHARACTERISTIC_UUID_MESH_PROVISIONING_DATA_IN, characteristicMeshProvisioningDataInAttUuid, 0x2ADB, "Mesh Provisioning Data In")                                     \
    X(CHARACTERISTIC_UUID_MESH_PROVISIONING_DATA_OUT, characteristicMeshProvisioningDataOutAttUuid, 0x2ADC, "Mesh Provisioning Data Out")                                  \
    X(CHARACTERISTIC_UUID_MESH_PROXY_DATA_IN, characteristicMeshProxyDataInAttUuid, 0x2ADD, "Mesh Proxy Data In")                                                          \
    X(CHARACTERISTIC_UUID_MESH_PROXY_DATA_OUT, characteristicMeshProxyDataOutAttUuid, 0x2ADE, "Mesh Proxy Data Out")                                                       \
    X(CHARACTERISTIC_UUID_AVERAGE_CURRENT, characteristicAverageCurrentAttUuid, 0x2AE0, "Average Current")                                                                 \
    X(CHARACTERISTIC_UUID_AVERAGE_VOLTAGE, characteristicAverageVoltageAttUuid, 0x2AE1, "Average Voltage")                                                                 \
    X(CHARACTERISTIC_UUID_BOOLEAN, characteristicBooleanAttUuid, 0x2AE2, "Boolean")                                                                                        \
    X(CHARACTERISTIC_UUID_CHROMATIC_DIST_FROM_PLANCKIAN, characteristicChromaticDistFromPlanckianAttUuid, 0x2AE3, "Chromatic Distance from Planckian")                     \
    X(CHARACTERISTIC_UUID_CHROMATICITY_COORDINATES, characteristicChromaticityCoordinatesAttUuid, 0x2AE4, "Chromaticity Coordinates")                                      \
    X(CHARACTERISTIC_UUID_CHROMATICITY_IN_CCT_DUV_VALUES, characteristicChromaticityInCctDuvValuesAttUuid, 0x2AE5, "Chromaticity in CCT and Duv Values")                   \
    X(CHARACTERISTIC_UUID_CHROMATICITY_TOLERANCE, characteristicChromaticityToleranceAttUuid, 0x2AE6, "Chromaticity Tolerance")                                            \
    X(CHARACTERISTIC_UUID_CIE13_3_1995_COLOR_INDEX, characteristicCie1331995ColorIndexAttUuid, 0x2AE7, "CIE13.3-1995 Color Rendering Index")                               \
    X(CHARACTERISTIC_UUID_COEFFICIENT, characteristicCoefficientAttUuid, 0x2AE8, "Coefficient")                                                                            \
    X(CHARACTERISTIC_UUID_CORRELATED_COLOR_TEMPERATURE, characteristicCorrelatedColorTemperatureAttUuid, 0x2AE9, "Correlated Color Temperature")                           \
    X(CHARACTERISTIC_UUID_COUNT_16, characteristicCount16AttUuid, 0x2AEA, "Count 16")                                                                                      \
    X(CHARACTERISTIC_UUID_COUNT_24, characteristicCount24AttUuid, 0x2AEB, "Count 24")                                                                                      \
    X(CHARACTERISTIC_UUID_COUNTRY_CODE, characteristicCountryCodeAttUuid, 0x2AEC, "Country Code")                                                                          \
    X(CHARACTERISTIC_UUID_DATE_UTC, characteristicDateUtcAttUuid, 0x2AED, "Date UTC")                                                                                      \
    X(CHARACTERISTIC_UUID_ELECTRIC_CURRENT, characteristicElectricCurrentAttUuid, 0x2AEE, "Electric Current")                                                              \
    X(CHARACTERISTIC_UUID_ELECTRIC_CURRENT_RANGE, characteristicElectricCurrentRangeAttUuid, 0x2AEF, "Electric Current Range")                                             \
    X(CHARACTERISTIC_UUID_ELECTRIC_CURRENT_SPECIFICATION, characteristicElectricCurrentSpecificationAttUuid, 0x2AF0, "Electric Current Specification")                     \
    X(CHARACTERISTIC_UUID_ELECTRIC_CURRENT_STATISTICS, characteristicElectricCurrentStatisticsAttUuid, 0x2AF1, "Electric Current Statistics")                              \
    X(CHARACTERISTIC_UUID_ENERGY, characteristicEnergyAttUuid, 0x2AF2, "Energy")                                                                                           \
    X(CHARACTERISTIC_UUID_ENERGY_IN_A_PERIOD_OF_DAY, characteristicEnergyInAPeriodOfDayAttUuid, 0x2AF3, "Energy in a Period of Day")                                       \
    X(CHARACTERISTIC_UUID_EVENT_STATISTICS, characteristicEventStatisticsAttUuid, 0x2AF4, "Event Statistics")                                                              \
    X(CHARACTERISTIC_UUID_FIXED_STRING_16, characteristicFixedString16AttUuid, 0x2AF5, "Fixed String 16")                                                                  \
    X(CHARACTERISTIC_UUID_FIXED_STRING_24, characteristicFixedString24AttUuid, 0x2AF6, "Fixed String 24")                                                                  \
    X(CHARACTERISTIC_UUID_FIXED_STRING_36, characteristicFixedString36AttUuid, 0x2AF7, "Fixed String 36")                                                                  \
    X(CHARACTERISTIC_UUID_FIXED_STRING_8, characteristicFixedString8AttUuid, 0x2AF8, "Fixed String 8")                                                                     \
    X(CHARACTERISTIC_UUID_GENERIC_LEVEL, characteristicGenericLevelAttUuid, 0x2AF9, "Generic Level")                                                                       \
    X(CHARACTERISTIC_UUID_GLOBAL_TRADE_ITEM_NUMBER, characteristicGlobalTradeItemNumberAttUuid, 0x2AFA, "Global Trade Item Number")                                        \
    X(CHARACTERISTIC_UUID_ILLUMINANCE, characteristicIlluminanceAttUuid, 0x2AFB, "Illuminance")                                                                            \
    X(CHARACTERISTIC_UUID_LUMINOUS_EFFICACY, characteristicLuminousEfficacyAttUuid, 0x2AFC, "Luminous Efficacy")                                                           \
    X(CHARACTERISTIC_UUID_LUMINOUS_ENERGY, characteristicLuminousEnergyAttUuid, 0x2AFD, "Luminous Energy")                                                                 \
    X(CHARACTERISTIC_UUID_LUMINOUS_EXPOSURE, characteristicLuminousExposureAttUuid, 0x2AFE, "Luminous Exposure")                                                           \
    X(CHARACTERISTIC_UUID_LUMINOUS_FLUX, characteristicLuminousFluxAttUuid, 0x2AFF, "Luminous Flux")                                                                       \
    X(CHARACTERISTIC_UUID_LUMINOUS_FLUX_RANGE, characteristicLuminousFluxRangeAttUuid, 0x2B00, "Luminous Flux Range")                                                      \
    X(CHARACTERISTIC_UUID_LUMINOUS_INTENSITY, characteristicLuminousIntensityAttUuid, 0x2B01, "Luminous Intensity")                                                        \
    X(CHARACTERISTIC_UUID_MASS_FLOW, characteristicMassFlowAttUuid, 0x2B02, "Mass Flow")                                                                                   \
    X(CHARACTERISTIC_UUID_PERCEIVED_LIGHTNESS, characteristicPerceivedLightnessAttUuid, 0x2B03, "Perceived Lightness")                                                     \
    X(CHARACTERISTIC_UUID_PERCENTAGE_8, characteristicPercentage8AttUuid, 0x2B04, "Percentage_8")                                                                          \
    X(CHARACTERISTIC_UUID_POWER, characteristicPowerAttUuid, 0x2B05, "Power")                                                                                              \
    X(CHARACTERISTIC_UUID_POWER_SPECIFICATION, characteristicPowerSpecificationAttUuid, 0x2B06, "Power Specification")                                                     \
    X(CHARACTERISTIC_UUID_RELATIVE_RUNTIME_CURRENT_RANGE, characteristicRelativeRuntimeCurrentRangeAttUuid, 0x2B07, "Relative Runtime in a Current Range")                 \
    X(CHARACTERISTIC_UUID_RELATIVE_RUNTIME_GENERIC_LEVEL, characteristicRelativeRuntimeGenericLevelAttUuid, 0x2B08, "Relative Runtime in a Generic Level Range")           \
    X(CHARACTERISTIC_UUID_RELATIVE_VALUE_VOLTAGE_RANGE, characteristicRelativeValueVoltageRangeAttUuid, 0x2B09, "Relative Value in a Voltage Range")                       \
    X(CHARACTERISTIC_UUID_RELATIVE_VALUE_ILLUMINANCE, characteristicRelativeValueIlluminanceAttUuid, 0x2B0A, "Relative Value in an Illuminance Range")                     \
    X(CHARACTERISTIC_UUID_RELATIVE_VALUE_PERIOD_OF_DAY, characteristicRelativeValuePeriodOfDayAttUuid, 0x2B0B, "Relative Value in a Period of Day")                        \
    X(CHARACTERISTIC_UUID_RELATIVE_VALUE_TEMPERATURE_RANGE, characteristicRelativeValueTemperatureRangeAttUuid, 0x2B0C, "Relative Value in a Temperature Range")           \
    X(CHARACTERISTIC_UUID_TEMPERATURE_8, characteristicTemperature8AttUuid, 0x2B0D, "Temperature 8")                                                                       \
    X(CHARACTERISTIC_UUID_TEMPERATURE_8_IN_A_PERIOD_OF_DAY, characteristicTemperature8InAPeriodOfDayAttUuid, 0x2B0E, "Temperature 8 in a Period of Day")                   \
    X(CHARACTERISTIC_UUID_TEMPERATURE_8_STATISTICS, characteristicTemperature8StatisticsAttUuid, 0x2B0F, "Temperature 8 Statistics")                                       \
    X(CHARACTERISTIC_UUID_TEMPERATURE_RANGE, characteristicTemperatureRangeAttUuid, 0x2B10, "Temperature Range")                                                           \
    X(CHARACTERISTIC_UUID_TEMPERATURE_STATISTICS, characteristicTemperatureStatisticsAttUuid, 0x2B11, "Temperature Statistics")                                            \
    X(CHARACTERISTIC_UUID_TIME_DECIHOUR_8, characteristicTimeDecihour8AttUuid, 0x2B12, "Time Decihour 8")                                                                  \
    X(CHARACTERISTIC_UUID_TIME_EXPONENTIAL_8, characteristicTimeExponential8AttUuid, 0x2B13, "Time Exponential 8")                                                         \
    X(CHARACTERISTIC_UUID_TIME_HOUR_24, characteristicTimeHour24AttUuid, 0x2B14, "Time Hour 24")                                                                           \
    X(CHARACTERISTIC_UUID_TIME_MILLISECOND_24, characteristicTimeMillisecond24AttUuid, 0x2B15, "Time Millisecond 24")                                                      \
    X(CHARACTERISTIC_UUID_TIME_SECOND_16, characteristicTimeSecond16AttUuid, 0x2B16, "Time Second1 6")                                                                     \
    X(CHARACTERISTIC_UUID_TIME_SECOND_8, characteristicTimeSecond8AttUuid, 0x2B17, "Time Second 8")                                                                        \
    X(CHARACTERISTIC_UUID_VOLTAGE, characteristicVoltageAttUuid, 0x2B18, "Voltage")                                                                                        \
    X(CHARACTERISTIC_UUID_VOLTAGE_SPECIFICATION, characteristicVoltageSpecificationAttUuid, 0x2B19, "Voltage Specification")                                               \
    X(CHARACTERISTIC_UUID_VOLTAGE_STATISTICS, characteristicVoltageStatisticsAttUuid, 0x2B1A, "Voltage Statistics")                                                        \
    X(CHARACTERISTIC_UUID_VOLUME_FLOW, characteristicVolumeFlowAttUuid, 0x2B1B, "Volume Flow")                                                                             \
    X(CHARACTERISTIC_UUID_CHROMATICITY_COORDINATE, characteristicChromaticityCoordinateAttUuid, 0x2B1C, "Chromaticity Coordinate")                                         \
    X(CHARACTERISTIC_UUID_RC_FEATURE, characteristicRcFeatureAttUuid, 0x2B1D, "RC Feature")                                                                                \
    X(CHARACTERISTIC_UUID_RC_SETTINGS, characteristicRcSettingsAttUuid, 0x2B1E, "RC Settings")                                                                             \
    X(CHARACTERISTIC_UUID_RECONN_CFG_CONTROL_POINT, characteristicReconnCfgControlPointAttUuid, 0x2B1F, "Reconnection Configuration Control Point")                        \
    X(CHARACTERISTIC_UUID_IDD_STATUS_CHANGED, characteristicIddStatusChangedAttUuid, 0x2B20, "IDD Status Changed")                                                         \
    X(CHARACTERISTIC_UUID_IDD_STATUS, characteristicIddStatusAttUuid, 0x2B21, "IDD Status")                                                                                \
    X(CHARACTERISTIC_UUID_IDD_ANNUNCIATION_STATUS, characteristicIddAnnunciationStatusAttUuid, 0x2B22, "IDD Annunciation Status")                                          \
    X(CHARACTERISTIC_UUID_IDD_FEATURES, characteristicIddFeaturesAttUuid, 0x2B23, "IDD Features")                                                                          \
    X(CHARACTERISTIC_UUID_IDD_STATUS_READER_CONTROL_POINT, characteristicIddStatusReaderControlPointAttUuid, 0x2B24, "IDD Status Reader Control Point")                    \
    X(CHARACTERISTIC_UUID_IDD_COMMAND_CONTROL_POINT, characteristicIddCommandControlPointAttUuid, 0x2B25, "IDD Command Control Point")                                     \
    X(CHARACTERISTIC_UUID_IDD_COMMAND_DATA, characteristicIddCommandDataAttUuid, 0x2B26, "IDD Command Data")                                                               \
    X(CHARACTERISTIC_UUID_IDD_RECORD_ACCESS_CONTROL_POINT, characteristicIddRecordAccessControlPointAttUuid, 0x2B27, "IDD Record Access Control Point")                    \
    X(CHARACTERISTIC_UUID_IDD_HISTORY_DATA, characteristicIddHistoryDataAttUuid, 0x2B28, "IDD History Data")                                                               \
    X(CHARACTERISTIC_UUID_CLIENT_SUPPORTED_FEATURES, characteristicClientSupportedFeaturesAttUuid, 0x2B29, "Client Supported Features")                                    \
    X(CHARACTERISTIC_UUID_DATABASE_HASH, characteristicDatabaseHashAttUuid, 0x2B2A, "Database Hash")                                                                       \
    X(CHARACTERISTIC_UUID_BSS_CONTROL_POINT, characteristicBssControlPointAttUuid, 0x2B2B, "BSS Control Point")                                                            \
    X(CHARACTERISTIC_UUID_BSS_RESPONSE, characteristicBssResponseAttUuid, 0x2B2C, "BSS Response")                                                                          \
    X(CHARACTERISTIC_UUID_EMERGENCY_ID, characteristicEmergencyIdAttUuid, 0x2B2D, "Emergency ID")                                                                          \
    X(CHARACTERISTIC_UUID_EMERGENCY_TEXT, characteristicEmergencyTextAttUuid, 0x2B2E, "Emergency Text")                                                                    \
    X(CHARACTERISTIC_UUID_ACS_STATUS, characteristicAcsStatusAttUuid, 0x2B2F, "ACS Status")                                                                                \
    X(CHARACTERISTIC_UUID_ACS_DATA_IN, characteristicAcsDataInAttUuid, 0x2B30, "ACS Data In")                                                                              \
    X(CHARACTERISTIC_UUID_ACS_DATA_OUT_NOTIFY, characteristicAcsDataOutNotifyAttUuid, 0x2B31, "ACS Data Out Notify")                                                       \
    X(CHARACTERISTIC_UUID_ACS_DATA_OUT_INDICATE, characteristicAcsDataOutIndicateAttUuid, 0x2B32, "ACS Data Out Indicate")                                                 \
    X(CHARACTERISTIC_UUID_ACS_CONTROL_POINT, characteristicAcsControlPointAttUuid, 0x2B33, "ACS Control Point")                                                            \
    X(CHARACTERISTIC_UUID_ENHANCED_BP_MEASUREMENT, characteristicEnhancedBpMeasurementAttUuid, 0x2B34, "Enhanced Blood Pressure Measurement")                              \
    X(CHARACTERISTIC_UUID_ENHANCED_INTERM_CUFF_PRESSURE, characteristicEnhancedIntermCuffPressureAttUuid, 0x2B35, "Enhanced Intermediate Cuff Pressure")                   \
    X(CHARACTERISTIC_UUID_BLOOD_PRESSURE_RECORD, characteristicBloodPressureRecordAttUuid, 0x2B36, "Blood Pressure Record")                                                \
    X(CHARACTERISTIC_UUID_REGISTERED_USER, characteristicRegisteredUserAttUuid, 0x2B37, "Registered User")                                                                 \
    X(CHARACTERISTIC_UUID_BR_EDR_HANDOVER_DATA, characteristicBrEdrHandoverDataAttUuid, 0x2B38, "BR-EDR Handover Data")                                                    \
    X(CHARACTERISTIC_UUID_BLUETOOTH_SIG_DATA, characteristicBluetoothSigDataAttUuid, 0x2B39, "Bluetooth SIG Data")                                                         \
    X(CHARACTERISTIC_UUID_SERVER_SUPPORTED_FEATURES, characteristicServerSupportedFeaturesAttUuid, 0x2B3A, "Server Supported Features")                                    \
    X(CHARACTERISTIC_UUID_PHYSICAL_AM_FEATURES, characteristicPhysicalAmFeaturesAttUuid, 0x2B3B, "Physical Activity Monitor Features")                                     \
    X(CHARACTERISTIC_UUID_GENERAL_ACTIVITY_INST_DATA, characteristicGeneralActivityInstDataAttUuid, 0x2B3C, "General Activity Instantaneous Data")                         \
    X(CHARACTERISTIC_UUID_GENERAL_ACTIVITY_SUMMARY_DATA, characteristicGeneralActivitySummaryDataAttUuid, 0x2B3D, "General Activity Summary Data")                         \
    X(CHARACTERISTIC_UUID_CR_ACTIVITY_INSTANTANEOUS_DATA, characteristicCrActivityInstantaneousDataAttUuid, 0x2B3E, "Cardio Respiratory Activity Instantaneous Data")      \
    X(CHARACTERISTIC_UUID_CR_ACTIVITY_SUMMARY_DATA, characteristicCrActivitySummaryDataAttUuid, 0x2B3F, "Cardio Respiratory Activity Summary Data")                        \
    X(CHARACTERISTIC_UUID_SC_ACTIVITY_SUMMARY_DATA, characteristicScActivitySummaryDataAttUuid, 0x2B40, "Step Counter Activity Summary Data")                              \
    X(CHARACTERISTIC_UUID_SLEEP_ACTIVITY_INST_DATA, characteristicSleepActivityInstDataAttUuid, 0x2B41, "Sleep Activity Instantaneous Data")                               \
    X(CHARACTERISTIC_UUID_SLEEP_ACTIVITY_SUMMARY_DATA, characteristicSleepActivitySummaryDataAttUuid, 0x2B42, "Sleep Activity Summary Data")                               \
    X(CHARACTERISTIC_UUID_PHYSICAL_AM_CONTROL_POINT, characteristicPhysicalAmControlPointAttUuid, 0x2B43, "Physical Activity Monitor Control Point")                       \
    X(CHARACTERISTIC_UUID_ACTIVITY_CURRENT_SESSION, characteristicActivityCurrentSessionAttUuid, 0x2B44, "Activity Current Session")                                       \
    X(CHARACTERISTIC_UUID_PHYSICAL_AS_DESCRIPTOR, characteristicPhysicalAsDescriptorAttUuid, 0x2B45, "Physical Activity Session Descriptor")                               \
    X(CHARACTERISTIC_UUID_PREFERRED_UNITS, characteristicPreferredUnitsAttUuid, 0x2B46, "Preferred Units")                                                                 \
    X(CHARACTERISTIC_UUID_HIGH_RESOLUTION_HEIGHT, characteristicHighResolutionHeightAttUuid, 0x2B47, "High Resolution Height")                                             \
    X(CHARACTERISTIC_UUID_MIDDLE_NAME, characteristicMiddleNameAttUuid, 0x2B48, "Middle Name")                                                                             \
    X(CHARACTERISTIC_UUID_STRIDE_LENGTH, characteristicStrideLengthAttUuid, 0x2B49, "Stride Length")                                                                       \
    X(CHARACTERISTIC_UUID_HANDEDNESS, characteristicHandednessAttUuid, 0x2B4A, "Handedness")                                                                               \
    X(CHARACTERISTIC_UUID_DEVICE_WEARING_POSITION, characteristicDeviceWearingPositionAttUuid, 0x2B4B, "Device Wearing Position")                                          \
    X(CHARACTERISTIC_UUID_FOUR_ZONE_HEART_RATE_LIMITS, characteristicFourZoneHeartRateLimitsAttUuid, 0x2B4C, "Four Zone Heart Rate Limits")                                \
    X(CHARACTERISTIC_UUID_HIGH_INTENSITY_EXERCISE_TH, characteristicHighIntensityExerciseThAttUuid, 0x2B4D, "High Intensity Exercise Threshold")                           \
    X(CHARACTERISTIC_UUID_ACTIVITY_GOAL, characteristicActivityGoalAttUuid, 0x2B4E, "Activity Goal")                                                                       \
    X(CHARACTERISTIC_UUID_SEDENTARY_INTERVAL_NOTIFICATION, characteristicSedentaryIntervalNotificationAttUuid, 0x2B4F, "Sedentary Interval Notification")                  \
    X(CHARACTERISTIC_UUID_CALORIC_INTAKE, characteristicCaloricIntakeAttUuid, 0x2B50, "Caloric Intake")                                                                    \
    X(CHARACTERISTIC_UUID_TMAP_ROLE, characteristicTmapRoleAttUuid, 0x2B51, "TMAP Role")                                                                                   \
    X(CHARACTERISTIC_UUID_AUDIO_INPUT_STATE, characteristicAudioInputStateAttUuid, 0x2B77, "Audio Input State")                                                            \
    X(CHARACTERISTIC_UUID_GAIN_SETTINGS_ATTRIBUTE, characteristicGainSettingsAttributeAttUuid, 0x2B78, "Gain Settings Attribute")                                          \
    X(CHARACTERISTIC_UUID_AUDIO_INPUT_TYPE, characteristicAudioInputTypeAttUuid, 0x2B79, "Audio Input Type")                                                               \
    X(CHARACTERISTIC_UUID_AICS_INPUT_STATUS, characteristicAicsInputStatusAttUuid, 0x2B7A, "Audio Input Status")                                                           \
    X(CHARACTERISTIC_UUID_AUDIO_INPUT_CONTROL_POINT, characteristicAudioInputControlPointAttUuid, 0x2B7B, "Audio Input Control Point")                                     \
    X(CHARACTERISTIC_UUID_AUDIO_INPUT_DESCRIPTION, characteristicAudioInputDescriptionAttUuid, 0x2B7C, "Audio Input Description")                                          \
    X(CHARACTERISTIC_UUID_VOLUME_STATE, characteristicVolumeStateAttUuid, 0x2B7D, "Volume State")                                                                          \
    X(CHARACTERISTIC_UUID_VOLUME_CONTROL_POINT, characteristicVolumeControlPointAttUuid, 0x2B7E, "Volume Control Point")                                                   \
    X(CHARACTERISTIC_UUID_VOLUME_FLAGS, characteristicVolumeFlagsAttUuid, 0x2B7F, "Volume Flags")                                                                          \
    X(CHARACTERISTIC_UUID_VOLUME_OFFSET_STATE, characteristicVolumeOffsetStateAttUuid, 0x2B80, "Volume Offset State")                                                      \
    X(CHARACTERISTIC_UUID_AUDIO_LOCATION, characteristicAudioLocationAttUuid, 0x2B81, "Audio Location")                                                                    \
    X(CHARACTERISTIC_UUID_VOLUME_OFFSET_CONTROL_POINT, characteristicVolumeOffsetControlPointAttUuid, 0x2B82, "Volume Offset Control Point")                               \
    X(CHARACTERISTIC_UUID_AUDIO_OUTPUT_DESCRIPTION, characteristicAudioOutputDescriptionAttUuid, 0x2B83, "Audio Output Description")                                       \
    X(CHARACTERISTIC_UUID_SET_IDENTITY_RESOLVING_KEY, characteristicSetIdentityResolvingKeyAttUuid, 0x2B84, "Set Identity Resolving Key")                                  \
    X(CHARACTERISTIC_UUID_COORDINATED_SET_SIZE, characteristicCoordinatedSetSizeAttUuid, 0x2B85, "Coordinated Set Size")                                                   \
    X(CHARACTERISTIC_UUID_SET_MEMBER_LOCK, characteristicSetMemberLockAttUuid, 0x2B86, "Set Member Lock")                                                                  \
    X(CHARACTERISTIC_UUID_SET_MEMBER_RANK, characteristicSetMemberRankAttUuid, 0x2B87, "Set Member Rank")                                                                  \
    X(CHARACTERISTIC_UUID_ENCRYPTED_DATA_KEY_MATERIAL, characteristicEncryptedDataKeyMaterialAttUuid, 0x2B88, "Encrypted Data Key Material")                               \
    X(CHARACTERISTIC_UUID_APPARENT_ENERGY_32, characteristicApparentEnergy32AttUuid, 0x2B89, "Apparent Energy 32")                                                         \
    X(CHARACTERISTIC_UUID_APPARENT_POWER, characteristicApparentPowerAttUuid, 0x2B8A, "Apparent Power")                                                                    \
    X(CHARACTERISTIC_UUID_LIVE_HEALTH_OBSERVATIONS, characteristicLiveHealthObservationsAttUuid, 0x2B8B, "Live Health Observations")                                       \
    X(CHARACTERISTIC_UUID_CO_CONC, characteristicCoConcAttUuid, 0x2B8C, "CO₂ Concentration")                                                                               \
    X(CHARACTERISTIC_UUID_COSINE_OF_THE_ANGLE, characteristicCosineOfTheAngleAttUuid, 0x2B8D, "Cosine of the Angle")                                                       \
    X(CHARACTERISTIC_UUID_DEVICE_TIME_FEATURE, characteristicDeviceTimeFeatureAttUuid, 0x2B8E, "Device Time Feature")                                                      \
    X(CHARACTERISTIC_UUID_DEVICE_TIME_PARAMETERS, characteristicDeviceTimeParametersAttUuid, 0x2B8F, "Device Time Parameters")                                             \
    X(CHARACTERISTIC_UUID_DEVICE_TIME, characteristicDeviceTimeAttUuid, 0x2B90, "Device Time")                                                                             \
    X(CHARACTERISTIC_UUID_DEVICE_TIME_CONTROL_POINT, characteristicDeviceTimeControlPointAttUuid, 0x2B91, "Device Time Control Point")                                     \
    X(CHARACTERISTIC_UUID_TIME_CHANGE_LOG_DATA, characteristicTimeChangeLogDataAttUuid, 0x2B92, "Time Change Log Data")                                                    \
    X(CHARACTERISTIC_UUID_MEDIA_PLAYER_NAME, characteristicMediaPlayerNameAttUuid, 0x2B93, "Media Player Name")                                                            \
    X(CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_OBJECT_ID, characteristicMediaPlayerIconObjectIdAttUuid, 0x2B94, "Media Player Icon Object ID")                                \
    X(CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_URL, characteristicMediaPlayerIconUrlAttUuid, 0x2B95, "Media Player Icon URL")                                                 \
    X(CHARACTERISTIC_UUID_TRACK_CHANGED, characteristicTrackChangedAttUuid, 0x2B96, "Track Changed")                                                                       \
    X(CHARACTERISTIC_UUID_TRACK_TITLE, characteristicTrackTitleAttUuid, 0x2B97, "Track Title")                                                                             \
    X(CHARACTERISTIC_UUID_TRACK_DURATION, characteristicTrackDurationAttUuid, 0x2B98, "Track Duration")                                                                    \
    X(CHARACTERISTIC_UUID_TRACK_POSITION, characteristicTrackPositionAttUuid, 0x2B99, "Track Position")                                                                    \
    X(CHARACTERISTIC_UUID_PLAYBACK_SPEED, characteristicPlaybackSpeedAttUuid, 0x2B9A, "Playback Speed")                                                                    \
    X(CHARACTERISTIC_UUID_SEEKING_SPEED, characteristicSeekingSpeedAttUuid, 0x2B9B, "Seeking Speed")                                                                       \
    X(CHARACTERISTIC_UUID_CURRENT_TRACK_SEGMENTS_OBJECT_ID, characteristicCurrentTrackSegmentsObjectIdAttUuid, 0x2B9C, "Current Track Segments Object ID")                 \
    X(CHARACTERISTIC_UUID_CURRENT_TRACK_OBJECT_ID, characteristicCurrentTrackObjectIdAttUuid, 0x2B9D, "Current Track Object ID")                                           \
    X(CHARACTERISTIC_UUID_NEXT_TRACK_OBJECT_ID, characteristicNextTrackObjectIdAttUuid, 0x2B9E, "Next Track Object ID")                                                    \
    X(CHARACTERISTIC_UUID_PARENT_GROUP_OBJECT_ID, characteristicParentGroupObjectIdAttUuid, 0x2B9F, "Parent Group Object ID")                                              \
    X(CHARACTERISTIC_UUID_CURRENT_GROUP_OBJECT_ID, characteristicCurrentGroupObjectIdAttUuid, 0x2BA0, "Current Group Object ID")                                           \
    X(CHARACTERISTIC_UUID_PLAYING_ORDER, characteristicPlayingOrderAttUuid, 0x2BA1, "Playing Order")                                                                       \
    X(CHARACTERISTIC_UUID_PLAYING_ORDERS_SUPPORTED, characteristicPlayingOrdersSupportedAttUuid, 0x2BA2, "Playing Orders Supported")                                       \
    X(CHARACTERISTIC_UUID_MEDIA_STATE, characteristicMediaStateAttUuid, 0x2BA3, "Media State")                                                                             \
    X(CHARACTERISTIC_UUID_MEDIA_CONTROL_POINT, characteristicMediaControlPointAttUuid, 0x2BA4, "Media Control Point")                                                      \
    X(CHARACTERISTIC_UUID_MEDIA_CTRL_POINT_OP_SUPPORTED, characteristicMediaCtrlPointOpSupportedAttUuid, 0x2BA5, "Media Control Point Opcodes Supported")                  \
    X(CHARACTERISTIC_UUID_SEARCH_RESULTS_OBJECT_ID, characteristicSearchResultsObjectIdAttUuid, 0x2BA6, "Search Results Object ID")                                        \
    X(CHARACTERISTIC_UUID_SEARCH_CONTROL_POINT, characteristicSearchControlPointAttUuid, 0x2BA7, "Search Control Point")                                                   \
    X(CHARACTERISTIC_UUID_ENERGY_32, characteristicEnergy32AttUuid, 0x2BA8, "Energy 32")                                                                                   \
    X(CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_OBJECT_TYPE, characteristicMediaPlayerIconObjectTypeAttUuid, 0x2BA9, "Media Player Icon Object Type")                          \
    X(CHARACTERISTIC_UUID_TRACK_SEGMENTS_OBJECT_TYPE, characteristicTrackSegmentsObjectTypeAttUuid, 0x2BAA, "Track Segments Object Type")                                  \
    X(CHARACTERISTIC_UUID_TRACK_OBJECT_TYPE, characteristicTrackObjectTypeAttUuid, 0x2BAB, "Track Object Type")                                                            \
    X(CHARACTERISTIC_UUID_GROUP_OBJECT_TYPE, characteristicGroupObjectTypeAttUuid, 0x2BAC, "Group Object Type")                                                            \
    X(CHARACTERISTIC_UUID_CONSTANT_TONE_EXTENSION_ENABLE, characteristicConstantToneExtensionEnableAttUuid, 0x2BAD, "Constant Tone Extension Enable")                      \
    X(CHARACTERISTIC_UUID_ADVERTISING_CTE_MIN_LENGTH, characteristicAdvertisingCteMinLengthAttUuid, 0x2BAE, "Advertising Constant Tone Extension Minimum Length")          \
    X(CHARACTERISTIC_UUID_ADVERTISING_CTE_MIN_TRANSMIT_CNT, characteristicAdvertisingCteMinTransmitCntAttUuid, 0x2BAF,                                                     \
      "Advertising Constant Tone Extension Minimum Transmit Count")                                                                                                        \
    X(CHARACTERISTIC_UUID_ADVERTISING_CTE_TRAN_DURATION, characteristicAdvertisingCteTranDurationAttUuid, 0x2BB0, "Advertising Constant Tone Extension Transmit Duration") \
    X(CHARACTERISTIC_UUID_ADVERTISING_CTE_INTERVAL, characteristicAdvertisingCteIntervalAttUuid, 0x2BB1, "Advertising Constant Tone Extension Interval")                   \
    X(CHARACTERISTIC_UUID_ADVERTISING_CTE_PHY, characteristicAdvertisingCtePhyAttUuid, 0x2BB2, "Advertising Constant Tone Extension PHY")                                  \
    X(CHARACTERISTIC_UUID_BEARER_PROVIDER_NAME, characteristicBearerProviderNameAttUuid, 0x2BB3, "Bearer Provider Name")                                                   \
    X(CHARACTERISTIC_UUID_BEARER_UCI, characteristicBearerUciAttUuid, 0x2BB4, "Bearer UCI")                                                                                \
    X(CHARACTERISTIC_UUID_BEARER_TECHNOLOGY, characteristicBearerTechnologyAttUuid, 0x2BB5, "Bearer Technology")                                                           \
    X(CHARACTERISTIC_UUID_BEARER_URI_SCHEMES_SUPPORTED_LIST, characteristicBearerUriSchemesSupportedListAttUuid, 0x2BB6, "Bearer URI Schemes Supported List")              \
    X(CHARACTERISTIC_UUID_BEARER_SS, characteristicBearerSsAttUuid, 0x2BB7, "Bearer Signal Strength")                                                                      \
    X(CHARACTERISTIC_UUID_BEARER_SS_REPORTING_INTERVAL, characteristicBearerSsReportingIntervalAttUuid, 0x2BB8, "Bearer Signal Strength Reporting Interval")               \
    X(CHARACTERISTIC_UUID_BEARER_LIST_CURRENT_CALLS, characteristicBearerListCurrentCallsAttUuid, 0x2BB9, "Bearer List Current Calls")                                     \
    X(CHARACTERISTIC_UUID_CONTENT_CONTROL_ID, characteristicContentControlIdAttUuid, 0x2BBA, "Content Control ID")                                                         \
    X(CHARACTERISTIC_UUID_STATUS_FLAGS, characteristicStatusFlagsAttUuid, 0x2BBB, "Status Flags")                                                                          \
    X(CHARACTERISTIC_UUID_INCOMING_CALL_TARGET_BEARER_URI, characteristicIncomingCallTargetBearerUriAttUuid, 0x2BBC, "Incoming Call Target Bearer URI")                    \
    X(CHARACTERISTIC_UUID_CALL_STATE, characteristicCallStateAttUuid, 0x2BBD, "Call State")                                                                                \
    X(CHARACTERISTIC_UUID_CALL_CTRL_POINT, characteristicCallCtrlPointAttUuid, 0x2BBE, "Call Control Point")                                                               \
    X(CHARACTERISTIC_UUID_CALL_CTRL_POINT_OPTIONAL_OPCODES, characteristicCallCtrlPointOptionalOpcodesAttUuid, 0x2BBF, "Call Control Point Optional Opcodes")              \
    X(CHARACTERISTIC_UUID_TERMINATION_REASON, characteristicTerminationReasonAttUuid, 0x2BC0, "Termination Reason")                                                        \
    X(CHARACTERISTIC_UUID_INCOMING_CALL, characteristicIncomingCallAttUuid, 0x2BC1, "Incoming Call")                                                                       \
    X(CHARACTERISTIC_UUID_CALL_FRIENDLY_NAME, characteristicCallFriendlyNameAttUuid, 0x2BC2, "Call Friendly Name")                                                         \
    X(CHARACTERISTIC_UUID_MUTE, characteristicMuteAttUuid, 0x2BC3, "Mute")                                                                                                 \
    X(CHARACTERISTIC_UUID_SINK_ASE, characteristicSinkAseAttUuid, 0x2BC4, "Sink ASE")                                                                                      \
    X(CHARACTERISTIC_UUID_SOURCE_ASE, characteristicSourceAseAttUuid, 0x2BC5, "Source ASE")                                                                                \
    X(CHARACTERISTIC_UUID_ASE_CONTROL_POINT, characteristicAseControlPointAttUuid, 0x2BC6, "ASE Control Point")                                                            \
    X(CHARACTERISTIC_UUID_BAS_CONTROL_POINT, characteristicBasControlPointAttUuid, 0x2BC7, "Broadcast Audio Scan Control Point")                                           \
    X(CHARACTERISTIC_UUID_BROADCAST_RECEIVE_STATE, characteristicBroadcastReceiveStateAttUuid, 0x2BC8, "Broadcast Receive State")                                          \
    X(CHARACTERISTIC_UUID_SINK_PAC, characteristicSinkPacAttUuid, 0x2BC9, "Sink PAC")                                                                                      \
    X(CHARACTERISTIC_UUID_SINK_AUDIO_LOCATIONS, characteristicSinkAudioLocationsAttUuid, 0x2BCA, "Sink Audio Locations")                                                   \
    X(CHARACTERISTIC_UUID_SOURCE_PAC, characteristicSourcePacAttUuid, 0x2BCB, "Source PAC")                                                                                \
    X(CHARACTERISTIC_UUID_SOURCE_AUDIO_LOCATIONS, characteristicSourceAudioLocationsAttUuid, 0x2BCC, "Source Audio Locations")                                             \
    X(CHARACTERISTIC_UUID_AVAILABLE_AUDIO_CONTEXTS, characteristicAvailableAudioContextsAttUuid, 0x2BCD, "Available Audio Contexts")                                       \
    X(CHARACTERISTIC_UUID_SUPPORTED_AUDIO_CONTEXTS, characteristicSupportedAudioContextsAttUuid, 0x2BCE, "Supported Audio Contexts")                                       \
    X(CHARACTERISTIC_UUID_AMMONIA_CONC, characteristicAmmoniaConcAttUuid, 0x2BCF, "Ammonia Concentration")                                                                 \
    X(CHARACTERISTIC_UUID_CARBON_MONOXIDE_CONC, characteristicCarbonMonoxideConcAttUuid, 0x2BD0, "Carbon Monoxide Concentration")                                          \
    X(CHARACTERISTIC_UUID_METHANE_CONC, characteristicMethaneConcAttUuid, 0x2BD1, "Methane Concentration")                                                                 \
    X(CHARACTERISTIC_UUID_NITROGEN_DIOXIDE_CONC, characteristicNitrogenDioxideConcAttUuid, 0x2BD2, "Nitrogen Dioxide Concentration")                                       \
    X(CHARACTERISTIC_UUID_NON_METHANE_VOLATILE_OCS_CONC, characteristicNonMethaneVolatileOcsConcAttUuid, 0x2BD3, "Non-Methane Volatile Organic Compounds Concentration")   \
    X(CHARACTERISTIC_UUID_OZONE_CONC, characteristicOzoneConcAttUuid, 0x2BD4, "Ozone Concentration")                                                                       \
    X(CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM1_CONC, characteristicParticulateMatterPm1ConcAttUuid, 0x2BD5, "Particulate Matter-PM1 Concentration")                      \
    X(CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM2_5_CONC, characteristicParticulateMatterPm25ConcAttUuid, 0x2BD6, "Particulate Matter-PM2.5 Concentration")                 \
    X(CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM10_CONC, characteristicParticulateMatterPm10ConcAttUuid, 0x2BD7, "Particulate Matter-PM10 Concentration")                   \
    X(CHARACTERISTIC_UUID_SULFUR_DIOXIDE_CONC, characteristicSulfurDioxideConcAttUuid, 0x2BD8, "Sulfur Dioxide Concentration")                                             \
    X(CHARACTERISTIC_UUID_SULFUR_HEXAFLUORIDE_CONC, characteristicSulfurHexafluorideConcAttUuid, 0x2BD9, "Sulfur Hexafluoride Concentration")                              \
    X(CHARACTERISTIC_UUID_HEARING_AID_FEATURES, characteristicHearingAidFeaturesAttUuid, 0x2BDA, "Hearing Aid Features")                                                   \
    X(CHARACTERISTIC_UUID_HEARING_AID_PRESET_CONTROL_POINT, characteristicHearingAidPresetControlPointAttUuid, 0x2BDB, "Hearing Aid Preset Control Point")                 \
    X(CHARACTERISTIC_UUID_ACTIVE_PRESET_INDEX, characteristicActivePresetIndexAttUuid, 0x2BDC, "Active Preset Index")                                                      \
    X(CHARACTERISTIC_UUID_STORED_HEALTH_OBSERVATIONS, characteristicStoredHealthObservationsAttUuid, 0x2BDD, "Stored Health Observations")                                 \
    X(CHARACTERISTIC_UUID_FIXED_STRING_64, characteristicFixedString64AttUuid, 0x2BDE, "Fixed String 64")                                                                  \
    X(CHARACTERISTIC_UUID_HIGH_TEMPERATURE, characteristicHighTemperatureAttUuid, 0x2BDF, "High Temperature")                                                              \
    X(CHARACTERISTIC_UUID_HIGH_VOLTAGE, characteristicHighVoltageAttUuid, 0x2BE0, "High Voltage")                                                                          \
    X(CHARACTERISTIC_UUID_LIGHT_DISTRIBUTION, characteristicLightDistributionAttUuid, 0x2BE1, "Light Distribution")                                                        \
    X(CHARACTERISTIC_UUID_LIGHT_OUTPUT, characteristicLightOutputAttUuid, 0x2BE2, "Light Output")                                                                          \
    X(CHARACTERISTIC_UUID_LIGHT_SOURCE_TYPE, characteristicLightSourceTypeAttUuid, 0x2BE3, "Light Source Type")                                                            \
    X(CHARACTERISTIC_UUID_NOISE, characteristicNoiseAttUuid, 0x2BE4, "Noise")                                                                                              \
    X(CHARACTERISTIC_UUID_RELATIVE_RUNTIME2, characteristicRelativeRuntime2AttUuid, 0x2BE5, "Relative Runtime in a Correlated Color Temperature Range")                    \
    X(CHARACTERISTIC_UUID_TIME_SECOND_32, characteristicTimeSecond32AttUuid, 0x2BE6, "Time Second 32")                                                                     \
    X(CHARACTERISTIC_UUID_VOC_CONC, characteristicVocConcAttUuid, 0x2BE7, "VOC Concentration")                                                                             \
    X(CHARACTERISTIC_UUID_VOLTAGE_FREQUENCY, characteristicVoltageFrequencyAttUuid, 0x2BE8, "Voltage Frequency")                                                           \
    X(CHARACTERISTIC_UUID_BATTERY_CRITICAL_STATUS, characteristicBatteryCriticalStatusAttUuid, 0x2BE9, "Battery Critical Status")                                          \
    X(CHARACTERISTIC_UUID_BATTERY_HEALTH_STATUS, characteristicBatteryHealthStatusAttUuid, 0x2BEA, "Battery Health Status")                                                \
    X(CHARACTERISTIC_UUID_BATTERY_HEALTH_INFORMATION, characteristicBatteryHealthInformationAttUuid, 0x2BEB, "Battery Health Information")                                 \
    X(CHARACTERISTIC_UUID_BATTERY_INFORMATION, characteristicBatteryInformationAttUuid, 0x2BEC, "Battery Information")                                                     \
    X(CHARACTERISTIC_UUID_BATTERY_LEVEL_STATUS, characteristicBatteryLevelStatusAttUuid, 0x2BED, "Battery Level Status")                                                   \
    X(CHARACTERISTIC_UUID_BATTERY_TIME_STATUS, characteristicBatteryTimeStatusAttUuid, 0x2BEE, "Battery Time Status")                                                      \
    X(CHARACTERISTIC_UUID_ESTIMATED_SERVICE_DATE, characteristicEstimatedServiceDateAttUuid, 0x2BEF, "Estimated Service Date")                                             \
    X(CHARACTERISTIC_UUID_BATTERY_ENERGY_STATUS, characteristicBatteryEnergyStatusAttUuid, 0x2BF0, "Battery Energy Status")                                                \
    X(CHARACTERISTIC_UUID_OBSERVATION_SCHEDULE_CHANGED, characteristicObservationScheduleChangedAttUuid, 0x2BF1, "Observation Schedule Changed")                           \
    X(CHARACTERISTIC_UUID_CURRENT_ELAPSED_TIME, characteristicCurrentElapsedTimeAttUuid, 0x2BF2, "Current Elapsed Time")                                                   \
    X(CHARACTERISTIC_UUID_HEALTH_SENSOR_FEATURES, characteristicHealthSensorFeaturesAttUuid, 0x2BF3, "Health Sensor Features")                                             \
    X(CHARACTERISTIC_UUID_GHS_CONTROL_POINT, characteristicGhsControlPointAttUuid, 0x2BF4, "GHS Control Point")                                                            \
    X(CHARACTERISTIC_UUID_LE_GATT_SECURITY_LEVELS, characteristicLeGattSecurityLevelsAttUuid, 0x2BF5, "LE GATT Security Levels")                                           \
    X(CHARACTERISTIC_UUID_ESL_ADDRESS, characteristicEslAddressAttUuid, 0x2BF6, "ESL Address")                                                                             \
    X(CHARACTERISTIC_UUID_AP_SYNC_KEY_MATERIAL, characteristicApSyncKeyMaterialAttUuid, 0x2BF7, "AP Sync Key Material")                                                    \
    X(CHARACTERISTIC_UUID_ESL_RESPONSE_KEY_MATERIAL, characteristicEslResponseKeyMaterialAttUuid, 0x2BF8, "ESL Response Key Material")                                     \
    X(CHARACTERISTIC_UUID_ESL_CURRENT_ABSOLUTE_TIME, characteristicEslCurrentAbsoluteTimeAttUuid, 0x2BF9, "ESL Current Absolute Time")                                     \
    X(CHARACTERISTIC_UUID_ESL_DISPLAY_INFORMATION, characteristicEslDisplayInformationAttUuid, 0x2BFA, "ESL Display Information")                                          \
    X(CHARACTERISTIC_UUID_ESL_IMAGE_INFORMATION, characteristicEslImageInformationAttUuid, 0x2BFB, "ESL Image Information")                                                \
    X(CHARACTERISTIC_UUID_ESL_SENSOR_INFORMATION, characteristicEslSensorInformationAttUuid, 0x2BFC, "ESL Sensor Information")                                             \
    X(CHARACTERISTIC_UUID_ESL_LED_INFORMATION, characteristicEslLedInformationAttUuid, 0x2BFD, "ESL LED Information")                                                      \
    X(CHARACTERISTIC_UUID_ESL_CONTROL_POINT, characteristicEslControlPointAttUuid, 0x2BFE, "ESL Control Point")                                                            \
    X(CHARACTERISTIC_UUID_UDI_FOR_MEDICAL_DEVICES, characteristicUdiForMedicalDevicesAttUuid, 0x2BFF, "UDI for Medical Devices")                                           \
    X(CHARACTERISTIC_UUID_GMAP_ROLE, characteristicGmapRoleAttUuid, 0x2C00, "GMAP Role")                                                                                   \
    X(CHARACTERISTIC_UUID_UGG_FEATURES, characteristicUggFeaturesAttUuid, 0x2C01, "UGG Features")                                                                          \
    X(CHARACTERISTIC_UUID_UGT_FEATURES, characteristicUgtFeaturesAttUuid, 0x2C02, "UGT Features")                                                                          \
    X(CHARACTERISTIC_UUID_BGS_FEATURES, characteristicBgsFeaturesAttUuid, 0x2C03, "BGS Features")                                                                          \
    X(CHARACTERISTIC_UUID_BGR_FEATURES, characteristicBgrFeaturesAttUuid, 0x2C04, "BGR Features")                                                                          \
    X(CHARACTERISTIC_UUID_RAS_FEATURE, characteristicRasFeatureAttUuid, 0x2C05, "RAS feature")                                                                             \
    X(CHARACTERISTIC_UUID_REAL_TIME_PROCEDURE_DATA, characteristicRealTimeProcedureDataAttUuid, 0x2C06, "Live Ranging Data")                                               \
    X(CHARACTERISTIC_UUID_ON_DEMAND_PROCEDURE_DATA, characteristicOnDemandProcedureDataAttUuid, 0x2C07, "Stored Ranging Data")                                             \
    X(CHARACTERISTIC_UUID_CONTROL_POINT, characteristicControlPointAttUuid, 0x2C08, "Control Point")                                                                       \
    X(CHARACTERISTIC_UUID_RANGING_DATA_READY, characteristicRangingDataReadyAttUuid, 0x2C09, "Ranging Data Ready")                                                         \
    X(CHARACTERISTIC_UUID_RANGING_DATA_OVERWRITTEN, characteristicRangingDataOverwrittenAttUuid, 0x2C0A, "Ranging Data Overwritten")                                       \
    X(CHARACTERISTIC_UUID_ULL_HID_PROPERTIES, characteristicUllHidPropertiesAttUuid, 0x2C23, "Ultra Low Latency HID Properties")                                           \
    X(CHARACTERISTIC_UUID_LE_HID_OPERATION_MODE, characteristicLeHidOperationModeAttUuid, 0x2C24, "LE HID Operation mode")                                                 \
    X(CHARACTERISTIC_UUID_COORDINATED_SET_NAME, characteristicCoordinatedSetNameAttUuid, 0x2C1A, "Coordinated Set Name")

enum
{

#define X(macro, att_name, value, desc) macro = value,
    BLE_HOST_SERVICE_UUIDS BLE_HOST_DECLARATIONS_UUIDS BLE_HOST_DESCRIPTOR_UUIDS BLE_HOST_CHARACTERISTIC_UUIDS
#undef X
};

/**
*  @brief Definition for GATT Services UUID
*/
#define X(macro, att_name, value, desc) extern const struct att_uuid att_name;
BLE_HOST_SERVICE_UUIDS
BLE_HOST_DECLARATIONS_UUIDS
BLE_HOST_DESCRIPTOR_UUIDS
BLE_HOST_CHARACTERISTIC_UUIDS
#undef X

extern const struct att_uuid characteristicTest1AttUuid;
