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
#define SERVICE_UUID_GENERIC_ACCESS                 0x1800 //Generic Access Service
#define SERVICE_UUID_GENERIC_ATTRIBUTE              0x1801 //Generic Attribute Service
#define SERVICE_UUID_IMMEDIATE_ALERT                0x1802 //Immediate Alert Service
#define SERVICE_UUID_LINK_LOSS                      0x1803 //Link Loss Service
#define SERVICE_UUID_TX_POWER                       0x1804 //Tx Power Service
#define SERVICE_UUID_CURRENT_TIME                   0x1805 //Current Time Service
#define SERVICE_UUID_REFERENCE_TIME_UPDATE          0x1806 //Reference Time Update Service
#define SERVICE_UUID_NEXT_DST_CHANGE                0x1807 //Next DST Change Service
#define SERVICE_UUID_GLUCOSE                        0x1808 //Glucose Service
#define SERVICE_UUID_HEALTH_THERMOMETER             0x1809 //Health Thermometer Service
#define SERVICE_UUID_DEVICE_INFORMATION             0x180A //Device Information Service
#define SERVICE_UUID_HEART_RATE                     0x180D //Heart Rate Service
#define SERVICE_UUID_PHONE_ALERT_STATUS             0x180E //Phone Alert Status Service
#define SERVICE_UUID_BATTERY                        0x180F //Battery Service
#define SERVICE_UUID_BLOOD_PRESSURE                 0x1810 //Blood Pressure Service
#define SERVICE_UUID_ALERT_NOTIFICATION             0x1811 //Alert Notification Service
#define SERVICE_UUID_HUMAN_INTERFACE_DEVICE         0x1812 //Human Interface Device Service
#define SERVICE_UUID_SCAN_PARAMETERS                0x1813 //Scan Parameters Service
#define SERVICE_UUID_RUNNING_SPEED_AND_CADENCE      0x1814 //Running Speed and Cadence Service
#define SERVICE_UUID_AUTOMATION_IO                  0x1815 //Automation IO Service
#define SERVICE_UUID_CYCLING_SPEED_AND_CADENCE      0x1816 //Cycling Speed and Cadence Service
#define SERVICE_UUID_CYCLING_POWER                  0x1818 //Cycling Power Service
#define SERVICE_UUID_LOCATION_AND_NAVIGATION        0x1819 //Location and Navigation Service
#define SERVICE_UUID_ENVIRONMENTAL_SENSING          0x181A //Environmental Sensing Service
#define SERVICE_UUID_BODY_COMPOSITION               0x181B //Body Composition Service
#define SERVICE_UUID_USER_DATA                      0x181C //User Data Service
#define SERVICE_UUID_WEIGHT_SCALE                   0x181D //Weight Scale Service
#define SERVICE_UUID_BOND_MANAGEMENT                0x181E //Bond Management Service
#define SERVICE_UUID_CONTINUOUS_GLUCOSE_MONITORING  0x181F //Continuous Glucose Monitoring Service
#define SERVICE_UUID_INTERNET_PROTOCOL_SUPPORT      0x1820 //Internet Protocol Support Service
#define SERVICE_UUID_INDOOR_POSITIONING             0x1821 //Indoor Positioning Service
#define SERVICE_UUID_PULSE_OXIMETER                 0x1822 //Pulse Oximeter Service
#define SERVICE_UUID_HTTP_PROXY                     0x1823 //HTTP Proxy Service
#define SERVICE_UUID_TRANSPORT_DISCOVERY            0x1824 //Transport Discovery Service
#define SERVICE_UUID_OBJECT_TRANSFER                0x1825 //Object Transfer Service
#define SERVICE_UUID_FITNESS_MACHINE                0x1826 //Fitness Machine Service
#define SERVICE_UUID_MESH_PROVISIONING              0x1827 //Mesh Provisioning Service
#define SERVICE_UUID_MESH_PROXY                     0x1828 //Mesh Proxy Service
#define SERVICE_UUID_RECONNECTION_CONFIGURATION     0x1829 //Reconnection Configuration Service
#define SERVICE_UUID_INSULIN_DELIVERY               0x183A //Insulin Delivery Service
#define SERVICE_UUID_BINARY_SENSOR                  0x183B //Binary Sensor Service
#define SERVICE_UUID_EMERGENCY_CONFIGURATION        0x183C //Emergency Configuration Service
#define SERVICE_UUID_AUTHORIZATION_CONTROL          0x183D //Authorization Control Service
#define SERVICE_UUID_PHYSICAL_ACTIVITY_MONITOR      0x183E //Physical Activity Monitor Service
#define SERVICE_UUID_ELAPSED_TIME                   0x183F //Elapsed Time Service
#define SERVICE_UUID_GENERIC_HEALTH_SENSOR          0x1840 //Generic Health Sensor Service
#define SERVICE_UUID_AUDIO_INPUT_CONTROL            0x1843 //Audio Input Control Service
#define SERVICE_UUID_VOLUME_CONTROL                 0x1844 //Volume Control Service
#define SERVICE_UUID_VOLUME_OFFSET_CONTROL          0x1845 //Volume Offset Control Service
#define SERVICE_UUID_COORDINATED_SET_IDENTIFICATION 0x1846 //Coordinated Set Identification Service
#define SERVICE_UUID_DEVICE_TIME                    0x1847 //Device Time Service
#define SERVICE_UUID_MEDIA_CONTROL                  0x1848 //Media Control Service
#define SERVICE_UUID_GENERIC_MEDIA_CONTROL          0x1849 //Generic Media Control Service
#define SERVICE_UUID_CONSTANT_TONE_EXTENSION        0x184A //Constant Tone Extension Service
#define SERVICE_UUID_TELEPHONE_BEARER               0x184B //Telephone Bearer Service
#define SERVICE_UUID_GENERIC_TELEPHONE_BEARER       0x184C //Generic Telephone Bearer Service
#define SERVICE_UUID_MICROPHONE_CONTROL             0x184D //Microphone Control Service
#define SERVICE_UUID_AUDIO_STREAM_CONTROL           0x184E //Audio Stream Control Service
#define SERVICE_UUID_BROADCAST_AUDIO_SCAN           0x184F //Broadcast Audio Scan Service
#define SERVICE_UUID_PUBLISHED_AUDIO_CAPABILITIES   0x1850 //Published Audio Capabilities Service
#define SERVICE_UUID_BASIC_AUDIO_ANNOUNCEMENT       0x1851 //Basic Audio Announcement Service
#define SERVICE_UUID_BROADCAST_AUDIO_ANNOUNCEMENT   0x1852 //Broadcast Audio Announcement Service
#define SERVICE_UUID_COMMON_AUDIO                   0x1853 //Common Audio Service
#define SERVICE_UUID_HEARING_ACCESS                 0x1854 //Hearing Access Service
#define SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO      0x1855 //Telephony and Media Audio Service
#define SERVICE_UUID_PUBLIC_BROADCAST_ANNOUNCEMENT  0x1856 //Public Broadcast Announcement Service
#define SERVICE_UUID_ELECTRONIC_SHELF_LABEL         0x1857 //Electronic Shelf Label Service
#define SERVICE_UUID_GAMING_AUDIO                   0x1858 //Gaming Audio Service
#define SERVICE_UUID_MESH_PROXY_SOLICITATION        0x1859 //Mesh Proxy Solicitation Service
//TODO: Ranging profile has not been confirmed yet. by junhui.hu
#define SERVICE_UUID_RANGING 0x185A //Ranging Service
//TODO: Ultra Low Latency HID service has not been confirmed yet. by qihang.mou
#define SERVICE_UUID_HID_OVER_ISO 0x185C //Ultra Low Latency HID Service

/**
 *  @brief Definition for Declaration UUID
 */
#define DECLARATIONS_UUID_PRIMARY_SERVICE   0x2800 //Primary Service
#define DECLARATIONS_UUID_SECONDARY_SERVICE 0x2801 //Secondary Service
#define DECLARATIONS_UUID_INCLUDE           0x2802 //Include
#define DECLARATIONS_UUID_CHARACTERISTIC    0x2803 //Characteristic

/**
 * @brief Definition for Descriptor UUID
 */
#define DESCRIPTOR_UUID_CHARACTERISTIC_EXTENDED_PROPERTIES    0x2900 //Characteristic Extended Properties
#define DESCRIPTOR_UUID_CHARACTERISTIC_USER_DESCRIPTION       0x2901 //Characteristic User Description
#define DESCRIPTOR_UUID_CLIENT_CHARACTERISTIC_CONFIGURATION   0x2902 //Client Characteristic Configuration
#define DESCRIPTOR_UUID_SERVER_CHARACTERISTIC_CONFIGURATION   0x2903 //Server Characteristic Configuration
#define DESCRIPTOR_UUID_CHARACTERISTIC_PRESENTATION_FORMAT    0x2904 //Characteristic Presentation Format
#define DESCRIPTOR_UUID_CHARACTERISTIC_AGGREGATE_FORMAT       0x2905 //Characteristic Aggregate Format
#define DESCRIPTOR_UUID_VALID_RANGE                           0x2906 //Valid Range
#define DESCRIPTOR_UUID_EXTERNAL_REPORT_REFERENCE             0x2907 //External Report Reference
#define DESCRIPTOR_UUID_REPORT_REFERENCE                      0x2908 //Report Reference
#define DESCRIPTOR_UUID_NUMBER_OF_DIGITALS                    0x2909 //Number of Digitals
#define DESCRIPTOR_UUID_VALUE_TRIGGER_SETTING                 0x290A //Value Trigger Setting
#define DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_CONFIGURATION   0x290B //Environmental Sensing Configuration
#define DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_MEASUREMENT     0x290C //Environmental Sensing Measurement
#define DESCRIPTOR_UUID_ENVIRONMENTAL_SENSING_TRIGGER_SETTING 0x290D //Environmental Sensing Trigger Setting
#define DESCRIPTOR_UUID_TIME_TRIGGER_SETTING                  0x290E //Time Trigger Setting
#define DESCRIPTOR_UUID_COMPLETE_BR_EDR_TRANSPORT_BLOCK_DATA  0x290F //Complete BR-EDR Transport Block Data
#define DESCRIPTOR_UUID_OBSERVATION_SCHEDULE                  0x2910 //Observation Schedule
#define DESCRIPTOR_UUID_VALID_RANGE_AND_ACCURACY              0x2911 //Valid Range and Accuracy


/**
 * @brief Definition for Characteristic UUID
 */
#define CHARACTERISTIC_UUID_DEVICE_NAME                       0x2A00 //Device Name
#define CHARACTERISTIC_UUID_APPEARANCE                        0x2A01 //Appearance
#define CHARACTERISTIC_UUID_PERIPHERAL_PRIVACY_FLAG           0x2A02 //Peripheral Privacy Flag
#define CHARACTERISTIC_UUID_RECONNECTION_ADDRESS              0x2A03 //Reconnection Address
#define CHARACTERISTIC_UUID_PERIPHERAL_PREFERRED_CONN_PARAM   0x2A04 //Peripheral Preferred Connection Parameters
#define CHARACTERISTIC_UUID_SERVICE_CHANGED                   0x2A05 //Service Changed
#define CHARACTERISTIC_UUID_ALERT_LEVEL                       0x2A06 //Alert Level
#define CHARACTERISTIC_UUID_TX_POWER_LEVEL                    0x2A07 //Tx Power Level
#define CHARACTERISTIC_UUID_DATE_TIME                         0x2A08 //Date Time
#define CHARACTERISTIC_UUID_DAYOF_WEEK                        0x2A09 //Dayof Week
#define CHARACTERISTIC_UUID_DAY_DATE_TIME                     0x2A0A //Day Date Time
#define CHARACTERISTIC_UUID_EXACT_TIME_256                    0x2A0C //Exact Time 256
#define CHARACTERISTIC_UUID_DST_OFFSET                        0x2A0D //DST Offset
#define CHARACTERISTIC_UUID_TIME_ZONE                         0x2A0E //Time Zone
#define CHARACTERISTIC_UUID_LOCAL_TIME_INFORMATION            0x2A0F //Local Time Information
#define CHARACTERISTIC_UUID_TIME_WITH_DST                     0x2A11 //Time with DST
#define CHARACTERISTIC_UUID_TIME_ACCURACY                     0x2A12 //Time Accuracy
#define CHARACTERISTIC_UUID_TIME_SOURCE                       0x2A13 //Time Source
#define CHARACTERISTIC_UUID_REFERENCE_TIME_INFORMATION        0x2A14 //Reference Time Information
#define CHARACTERISTIC_UUID_TIME_UPDATE_CONTROL_POINT         0x2A16 //Time Update Control Point
#define CHARACTERISTIC_UUID_TIME_UPDATE_STATE                 0x2A17 //Time Update State
#define CHARACTERISTIC_UUID_GLUCOSE_MEASUREMENT               0x2A18 //Glucose Measurement
#define CHARACTERISTIC_UUID_BATTERY_LEVEL                     0x2A19 //Battery Level
#define CHARACTERISTIC_UUID_BATTERY_POWER_STATE               0x2A1A //Battery Power state
#define CHARACTERISTIC_UUID_TEMPERATURE_MEASUREMENT           0x2A1C //Temperature Measurement
#define CHARACTERISTIC_UUID_TEMPERATURE_TYPE                  0x2A1D //Temperature Type
#define CHARACTERISTIC_UUID_INTERMEDIATE_TEMPERATURE          0x2A1E //Intermediate Temperature
#define CHARACTERISTIC_UUID_MEASUREMENT_INTERVAL              0x2A21 //Measurement Interval
#define CHARACTERISTIC_UUID_BOOT_KEYBOARD_INPUT_REPORT        0x2A22 //Boot Keyboard Input Report
#define CHARACTERISTIC_UUID_SYSTEM_ID                         0x2A23 //System ID
#define CHARACTERISTIC_UUID_MODEL_NUMBER_STRING               0x2A24 //Model Number String
#define CHARACTERISTIC_UUID_SERIAL_NUMBER_STRING              0x2A25 //Serial Number String
#define CHARACTERISTIC_UUID_FIRMWARE_REVISION_STRING          0x2A26 //Firmware Revision String
#define CHARACTERISTIC_UUID_HARDWARE_REVISION_STRING          0x2A27 //Hardware Revision String
#define CHARACTERISTIC_UUID_SOFTWARE_REVISION_STRING          0x2A28 //Software Revision String
#define CHARACTERISTIC_UUID_MANUFACTURER_NAME_STRING          0x2A29 //Manufacturer Name String
#define CHARACTERISTIC_UUID_IEEE11073_20601_DATA_LIST         0x2A2A //IEEE11073-20601 Regulatory Certification Data List
#define CHARACTERISTIC_UUID_CURRENT_TIME                      0x2A2B //Current Time
#define CHARACTERISTIC_UUID_MAGNETIC_DECLINATION              0x2A2C //Magnetic Declination
#define CHARACTERISTIC_UUID_SCAN_REFRESH                      0x2A31 //Scan Refresh
#define CHARACTERISTIC_UUID_BOOT_KEYBOARD_OUTPUT_REPORT       0x2A32 //Boot Keyboard Output Report
#define CHARACTERISTIC_UUID_BOOT_MOUSE_INPUT_REPORT           0x2A33 //Boot Mouse Input Report
#define CHARACTERISTIC_UUID_GLUCOSE_MEASUREMENT_CONTEXT       0x2A34 //Glucose Measurement Context
#define CHARACTERISTIC_UUID_BLOOD_PRESSURE_MEASUREMENT        0x2A35 //Blood Pressure Measurement
#define CHARACTERISTIC_UUID_INTERMEDIATE_CUFF_PRESSURE        0x2A36 //Intermediate Cuff Pressure
#define CHARACTERISTIC_UUID_HEART_RATE_MEASUREMENT            0x2A37 //Heart Rate Measurement
#define CHARACTERISTIC_UUID_BODY_SENSOR_LOCATION              0x2A38 //Body Sensor Location
#define CHARACTERISTIC_UUID_HEART_RATE_CONTROL_POINT          0x2A39 //Heart Rate Control Point
#define CHARACTERISTIC_UUID_ALERT_STATUS                      0x2A3F //Alert Status
#define CHARACTERISTIC_UUID_RINGER_CONTROL_POINT              0x2A40 //Ringer Control Point
#define CHARACTERISTIC_UUID_RINGER_SETTING                    0x2A41 //Ringer Setting
#define CHARACTERISTIC_UUID_ALERT_CATEGORY_ID_BIT_MASK        0x2A42 //Alert Category ID Bit Mask
#define CHARACTERISTIC_UUID_ALERT_CATEGORY_ID                 0x2A43 //Alert Category ID
#define CHARACTERISTIC_UUID_ALERT_NOTIFICATION_CONTROL_POINT  0x2A44 //Alert Notification Control Point
#define CHARACTERISTIC_UUID_UNREAD_ALERT_STATUS               0x2A45 //Unread Alert Status
#define CHARACTERISTIC_UUID_NEW_ALERT                         0x2A46 //New Alert
#define CHARACTERISTIC_UUID_SUPPORTED_NEW_ALERT_CATEGORY      0x2A47 //Supported New Alert Category
#define CHARACTERISTIC_UUID_SUPPORTED_UNREAD_ALERT_CATEGORY   0x2A48 //Supported Unread Alert Category
#define CHARACTERISTIC_UUID_BLOOD_PRESSURE_FEATURE            0x2A49 //Blood Pressure Feature
#define CHARACTERISTIC_UUID_HID_INFORMATION                   0x2A4A //HID Information
#define CHARACTERISTIC_UUID_REPORT_MAP                        0x2A4B //Report Map
#define CHARACTERISTIC_UUID_HID_CONTROL_POINT                 0x2A4C //HID Control Point
#define CHARACTERISTIC_UUID_REPORT                            0x2A4D //Report
#define CHARACTERISTIC_UUID_PROTOCOL_MODE                     0x2A4E //Protocol Mode
#define CHARACTERISTIC_UUID_SCAN_INTERVAL_WINDOW              0x2A4F //Scan Interval Window
#define CHARACTERISTIC_UUID_PNP_ID                            0x2A50 //PnP ID
#define CHARACTERISTIC_UUID_GLUCOSE_FEATURE                   0x2A51 //Glucose Feature
#define CHARACTERISTIC_UUID_RECORD_ACCESS_CONTROL_POINT       0x2A52 //Record Access Control Point
#define CHARACTERISTIC_UUID_RSC_MEASUREMENT                   0x2A53 //RSC Measurement
#define CHARACTERISTIC_UUID_RSC_FEATURE                       0x2A54 //RSC Feature
#define CHARACTERISTIC_UUID_SC_CONTROL_POINT                  0x2A55 //SC Control Point
#define CHARACTERISTIC_UUID_AGGREGATE                         0x2A5A //Aggregate
#define CHARACTERISTIC_UUID_CSC_MEASUREMENT                   0x2A5B //CSC Measurement
#define CHARACTERISTIC_UUID_CSC_FEATURE                       0x2A5C //CSC Feature
#define CHARACTERISTIC_UUID_SENSOR_LOCATION                   0x2A5D //Sensor Location
#define CHARACTERISTIC_UUID_PLX_SPOT_CHECK_MEASUREMENT        0x2A5E //PLX Spot-Check Measurement
#define CHARACTERISTIC_UUID_PLX_CONTINUOUS_MEASUREMENT        0x2A5F //PLX Continuous Measurement
#define CHARACTERISTIC_UUID_PLX_FEATURES                      0x2A60 //PLX Features
#define CHARACTERISTIC_UUID_CYCLING_POWER_MEASUREMENT         0x2A63 //Cycling Power Measurement
#define CHARACTERISTIC_UUID_CYCLING_POWER_VECTOR              0x2A64 //Cycling Power Vector
#define CHARACTERISTIC_UUID_CYCLING_POWER_FEATURE             0x2A65 //Cycling Power Feature
#define CHARACTERISTIC_UUID_CYCLING_POWER_CONTROL_POINT       0x2A66 //Cycling Power Control Point
#define CHARACTERISTIC_UUID_LOCATION_AND_SPEED                0x2A67 //Location and Speed
#define CHARACTERISTIC_UUID_NAVIGATION                        0x2A68 //Navigation
#define CHARACTERISTIC_UUID_POSITION_QUALITY                  0x2A69 //Position Quality
#define CHARACTERISTIC_UUID_LN_FEATURE                        0x2A6A //LN Feature
#define CHARACTERISTIC_UUID_LN_CONTROL_POINT                  0x2A6B //LN Control Point
#define CHARACTERISTIC_UUID_ELEVATION                         0x2A6C //Elevation
#define CHARACTERISTIC_UUID_PRESSURE                          0x2A6D //Pressure
#define CHARACTERISTIC_UUID_TEMPERATURE                       0x2A6E //Temperature
#define CHARACTERISTIC_UUID_HUMIDITY                          0x2A6F //Humidity
#define CHARACTERISTIC_UUID_TRUE_WIND_SPEED                   0x2A70 //True Wind Speed
#define CHARACTERISTIC_UUID_TRUE_WIND_DIRECTION               0x2A71 //True Wind Direction
#define CHARACTERISTIC_UUID_APPARENT_WIND_SPEED               0x2A72 //Apparent Wind Speed
#define CHARACTERISTIC_UUID_APPARENT_WIND_DIRECTION           0x2A73 //Apparent Wind Direction
#define CHARACTERISTIC_UUID_GUST_FACTOR                       0x2A74 //Gust Factor
#define CHARACTERISTIC_UUID_POLLEN_CONC                       0x2A75 //Pollen Concentration
#define CHARACTERISTIC_UUID_UV_INDEX                          0x2A76 //UV Index
#define CHARACTERISTIC_UUID_IRRADIANCE                        0x2A77 //Irradiance
#define CHARACTERISTIC_UUID_RAINFALL                          0x2A78 //Rainfall
#define CHARACTERISTIC_UUID_WIND_CHILL                        0x2A79 //Wind Chill
#define CHARACTERISTIC_UUID_HEAT_INDEX                        0x2A7A //Heat Index
#define CHARACTERISTIC_UUID_DEW_POINT                         0x2A7B //Dew Point
#define CHARACTERISTIC_UUID_DESCRIPTOR_VALUE_CHANGED          0x2A7D //Descriptor Value Changed
#define CHARACTERISTIC_UUID_AEROBIC_HEART_RATE_LOWER_LIMIT    0x2A7E //Aerobic Heart Rate Lower Limit
#define CHARACTERISTIC_UUID_AEROBIC_THRESHOLD                 0x2A7F //Aerobic Threshold
#define CHARACTERISTIC_UUID_AGE                               0x2A80 //Age
#define CHARACTERISTIC_UUID_ANAEROBIC_HEART_RATE_LOWER_LIMIT  0x2A81 //Anaerobic Heart Rate Lower Limit
#define CHARACTERISTIC_UUID_ANAEROBIC_HEART_RATE_UPPER_LIMIT  0x2A82 //Anaerobic Heart Rate Upper Limit
#define CHARACTERISTIC_UUID_ANAEROBIC_THRESHOLD               0x2A83 //Anaerobic Threshold
#define CHARACTERISTIC_UUID_AEROBIC_HEART_RATE_UPPER_LIMIT    0x2A84 //Aerobic Heart Rate Upper Limit
#define CHARACTERISTIC_UUID_DATE_OF_BIRTH                     0x2A85 //Date of Birth
#define CHARACTERISTIC_UUID_DATE_OF_THRESHOLD_ASSESSMENT      0x2A86 //Date of Threshold Assessment
#define CHARACTERISTIC_UUID_EMAIL_ADDRESS                     0x2A87 //Email Address
#define CHARACTERISTIC_UUID_FAT_BURN_HEART_RATE_LOWER_LIMIT   0x2A88 //Fat Burn Heart Rate Lower Limit
#define CHARACTERISTIC_UUID_FAT_BURN_HEART_RATE_UPPER_LIMIT   0x2A89 //Fat Burn Heart Rate Upper Limit
#define CHARACTERISTIC_UUID_FIRST_NAME                        0x2A8A //First Name
#define CHARACTERISTIC_UUID_FIVE_ZONE_HEART_RATE_LIMITS       0x2A8B //Five Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_GENDER                            0x2A8C //Gender
#define CHARACTERISTIC_UUID_HEART_RATE_MAX                    0x2A8D //Heart Rate Max
#define CHARACTERISTIC_UUID_HEIGHT                            0x2A8E //Height
#define CHARACTERISTIC_UUID_HIP_CIRCUMFERENCE                 0x2A8F //Hip Circumference
#define CHARACTERISTIC_UUID_LAST_NAME                         0x2A90 //Last Name
#define CHARACTERISTIC_UUID_MAXIMUM_RECOMMENDED_HEART_RATE    0x2A91 //Maximum Recommended Heart Rate
#define CHARACTERISTIC_UUID_RESTING_HEART_RATE                0x2A92 //Resting Heart Rate
#define CHARACTERISTIC_UUID_SPORT_TYPE                        0x2A93 //Sport Type for Aerobic and Anaerobic Thresholds
#define CHARACTERISTIC_UUID_THREE_ZONE_HEART_RATE_LIMITS      0x2A94 //Three Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_TWO_ZONE_HEART_RATE_LIMITS        0x2A95 //Two Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_VO2_MAX                           0x2A96 //VO2 Max
#define CHARACTERISTIC_UUID_WAIST_CIRCUMFERENCE               0x2A97 //Waist Circumference
#define CHARACTERISTIC_UUID_WEIGHT                            0x2A98 //Weight
#define CHARACTERISTIC_UUID_DATABASE_CHANGE_INCREMENT         0x2A99 //Database Change Increment
#define CHARACTERISTIC_UUID_USER_INDEX                        0x2A9A //User Index
#define CHARACTERISTIC_UUID_BODY_COMPOSITION_FEATURE          0x2A9B //Body Composition Feature
#define CHARACTERISTIC_UUID_BODY_COMPOSITION_MEASUREMENT      0x2A9C //Body Composition Measurement
#define CHARACTERISTIC_UUID_WEIGHT_MEASUREMENT                0x2A9D //Weight Measurement
#define CHARACTERISTIC_UUID_WEIGHT_SCALE_FEATURE              0x2A9E //Weight Scale Feature
#define CHARACTERISTIC_UUID_USER_CONTROL_POINT                0x2A9F //User Control Point
#define CHARACTERISTIC_UUID_MAGNETIC_FLUX_DENSITY_2D          0x2AA0 //Magnetic Flux Density-2D
#define CHARACTERISTIC_UUID_MAGNETIC_FLUX_DENSITY_3D          0x2AA1 //Magnetic Flux Density-3D
#define CHARACTERISTIC_UUID_LANGUAGE                          0x2AA2 //Language
#define CHARACTERISTIC_UUID_BAROMETRIC_PRESSURE_TREND         0x2AA3 //Barometric Pressure Trend
#define CHARACTERISTIC_UUID_BOND_MANAGEMENT_CONTROL_POINT     0x2AA4 //Bond Management Control Point
#define CHARACTERISTIC_UUID_BOND_MANAGEMENT_FEATURE           0x2AA5 //Bond Management Feature
#define CHARACTERISTIC_UUID_CENTRAL_ADDRESS_RESOLUTION        0x2AA6 //Central Address Resolution
#define CHARACTERISTIC_UUID_CGM_MEASUREMENT                   0x2AA7 //CGM	Measurement
#define CHARACTERISTIC_UUID_CGM_FEATURE                       0x2AA8 //CGM	Feature
#define CHARACTERISTIC_UUID_CGM_STATUS                        0x2AA9 //CGM	Status
#define CHARACTERISTIC_UUID_CGM_SESSION_START_TIME            0x2AAA //CGM	Session Start Time
#define CHARACTERISTIC_UUID_CGM_SESSION_RUN_TIME              0x2AAB //CGM	Session Run Time
#define CHARACTERISTIC_UUID_CGM_SPECIFIC_OPS_CONTROL_POINT    0x2AAC //CGM	Specific Ops Control Point
#define CHARACTERISTIC_UUID_INDOOR_POSITIONING_CONFIGURATION  0x2AAD //Indoor Positioning Configuration
#define CHARACTERISTIC_UUID_LATITUDE                          0x2AAE //Latitude
#define CHARACTERISTIC_UUID_LONGITUDE                         0x2AAF //Longitude
#define CHARACTERISTIC_UUID_LOCAL_NORTH_COORDINATE            0x2AB0 //Local North Coordinate
#define CHARACTERISTIC_UUID_LOCAL_EAST_COORDINATE             0x2AB1 //Local East Coordinate
#define CHARACTERISTIC_UUID_FLOOR_NUMBER                      0x2AB2 //Floor Number
#define CHARACTERISTIC_UUID_ALTITUDE                          0x2AB3 //Altitude
#define CHARACTERISTIC_UUID_UNCERTAINTY                       0x2AB4 //Uncertainty
#define CHARACTERISTIC_UUID_LOCATION_NAME                     0x2AB5 //Location Name
#define CHARACTERISTIC_UUID_URI                               0x2AB6 //URI
#define CHARACTERISTIC_UUID_HTTP_HEADERS                      0x2AB7 //HTTP Headers
#define CHARACTERISTIC_UUID_HTTP_STATUS_CODE                  0x2AB8 //HTTP Status Code
#define CHARACTERISTIC_UUID_HTTP_ENTITY_BODY                  0x2AB9 //HTTP Entity Body
#define CHARACTERISTIC_UUID_HTTP_CONTROL_POINT                0x2ABA //HTTP Control Point
#define CHARACTERISTIC_UUID_HTTPS_SECURITY                    0x2ABB //HTTPS Security
#define CHARACTERISTIC_UUID_TDS_CONTROL_POINT                 0x2ABC //TDS Control Point
#define CHARACTERISTIC_UUID_OTS_FEATURE                       0x2ABD //OTS Feature
#define CHARACTERISTIC_UUID_OBJECT_NAME                       0x2ABE //Object Name
#define CHARACTERISTIC_UUID_OBJECT_TYPE                       0x2ABF //Object Type
#define CHARACTERISTIC_UUID_OBJECT_SIZE                       0x2AC0 //Object Size
#define CHARACTERISTIC_UUID_OBJECT_FIRST_CREATED              0x2AC1 //Object First-Created
#define CHARACTERISTIC_UUID_OBJECT_LAST_MODIFIED              0x2AC2 //Object Last-Modified
#define CHARACTERISTIC_UUID_OBJECT_ID                         0x2AC3 //Object ID
#define CHARACTERISTIC_UUID_OBJECT_PROPERTIES                 0x2AC4 //Object Properties
#define CHARACTERISTIC_UUID_OBJECT_ACTION_CONTROL_POINT       0x2AC5 //Object Action Control Point
#define CHARACTERISTIC_UUID_OBJECT_LIST_CONTROL_POINT         0x2AC6 //Object List Control Point
#define CHARACTERISTIC_UUID_OBJECT_LIST_FILTER                0x2AC7 //Object List Filter
#define CHARACTERISTIC_UUID_OBJECT_CHANGED                    0x2AC8 //Object Changed
#define CHARACTERISTIC_UUID_RESOLVABLE_PRIVATE_ADDRESS_ONLY   0x2AC9 //Resolvable Private Address Only
#define CHARACTERISTIC_UUID_FITNESS_MACHINE_FEATURE           0x2ACC //Fitness Machine Feature
#define CHARACTERISTIC_UUID_TREADMILL_DATA                    0x2ACD //Treadmill Data
#define CHARACTERISTIC_UUID_CROSS_TRAINER_DATA                0x2ACE //Cross Trainer Data
#define CHARACTERISTIC_UUID_STEP_CLIMBER_DATA                 0x2ACF //Step Climber Data
#define CHARACTERISTIC_UUID_STAIR_CLIMBER_DATA                0x2AD0 //Stair Climber Data
#define CHARACTERISTIC_UUID_ROWER_DATA                        0x2AD1 //Rower Data
#define CHARACTERISTIC_UUID_INDOOR_BIKE_DATA                  0x2AD2 //Indoor Bike Data
#define CHARACTERISTIC_UUID_TRAINING_STATUS                   0x2AD3 //Training Status
#define CHARACTERISTIC_UUID_SUPPORTED_SPEED_RANGE             0x2AD4 //Supported Speed Range
#define CHARACTERISTIC_UUID_SUPPORTED_INCLINATION_RANGE       0x2AD5 //Supported Inclination Range
#define CHARACTERISTIC_UUID_SUPPORTED_RESISTANCE_LEVEL_RANGE  0x2AD6 //Supported Resistance Level Range
#define CHARACTERISTIC_UUID_SUPPORTED_HEART_RATE_RANGE        0x2AD7 //Supported Heart Rate Range
#define CHARACTERISTIC_UUID_SUPPORTED_POWER_RANGE             0x2AD8 //Supported Power Range
#define CHARACTERISTIC_UUID_FITNESS_MACHINE_CONTROL_POINT     0x2AD9 //Fitness Machine Control Point
#define CHARACTERISTIC_UUID_FITNESS_MACHINE_STATUS            0x2ADA //Fitness Machine Status
#define CHARACTERISTIC_UUID_MESH_PROVISIONING_DATA_IN         0x2ADB //Mesh Provisioning Data In
#define CHARACTERISTIC_UUID_MESH_PROVISIONING_DATA_OUT        0x2ADC //Mesh Provisioning Data Out
#define CHARACTERISTIC_UUID_MESH_PROXY_DATA_IN                0x2ADD //Mesh Proxy Data In
#define CHARACTERISTIC_UUID_MESH_PROXY_DATA_OUT               0x2ADE //Mesh Proxy Data Out
#define CHARACTERISTIC_UUID_AVERAGE_CURRENT                   0x2AE0 //Average Current
#define CHARACTERISTIC_UUID_AVERAGE_VOLTAGE                   0x2AE1 //Average Voltage
#define CHARACTERISTIC_UUID_BOOLEAN                           0x2AE2 //Boolean
#define CHARACTERISTIC_UUID_CHROMATIC_DIST_FROM_PLANCKIAN     0x2AE3 //Chromatic Distance from Planckian
#define CHARACTERISTIC_UUID_CHROMATICITY_COORDINATES          0x2AE4 //Chromaticity Coordinates
#define CHARACTERISTIC_UUID_CHROMATICITY_IN_CCT_DUV_VALUES    0x2AE5 //Chromaticity in CCT and Duv Values
#define CHARACTERISTIC_UUID_CHROMATICITY_TOLERANCE            0x2AE6 //Chromaticity Tolerance
#define CHARACTERISTIC_UUID_CIE13_3_1995_COLOR_INDEX          0x2AE7 //CIE13.3-1995 Color Rendering Index
#define CHARACTERISTIC_UUID_COEFFICIENT                       0x2AE8 //Coefficient
#define CHARACTERISTIC_UUID_CORRELATED_COLOR_TEMPERATURE      0x2AE9 //Correlated Color Temperature
#define CHARACTERISTIC_UUID_COUNT_16                          0x2AEA //Count 16
#define CHARACTERISTIC_UUID_COUNT_24                          0x2AEB //Count 24
#define CHARACTERISTIC_UUID_COUNTRY_CODE                      0x2AEC //Country Code
#define CHARACTERISTIC_UUID_DATE_UTC                          0x2AED //Date UTC
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT                  0x2AEE //Electric Current
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT_RANGE            0x2AEF //Electric Current Range
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT_SPECIFICATION    0x2AF0 //Electric Current Specification
#define CHARACTERISTIC_UUID_ELECTRIC_CURRENT_STATISTICS       0x2AF1 //Electric Current Statistics
#define CHARACTERISTIC_UUID_ENERGY                            0x2AF2 //Energy
#define CHARACTERISTIC_UUID_ENERGY_IN_A_PERIOD_OF_DAY         0x2AF3 //Energy in a Period of Day
#define CHARACTERISTIC_UUID_EVENT_STATISTICS                  0x2AF4 //Event Statistics
#define CHARACTERISTIC_UUID_FIXED_STRING_16                   0x2AF5 //Fixed String 16
#define CHARACTERISTIC_UUID_FIXED_STRING_24                   0x2AF6 //Fixed String 24
#define CHARACTERISTIC_UUID_FIXED_STRING_36                   0x2AF7 //Fixed String 36
#define CHARACTERISTIC_UUID_FIXED_STRING_8                    0x2AF8 //Fixed String 8
#define CHARACTERISTIC_UUID_GENERIC_LEVEL                     0x2AF9 //Generic Level
#define CHARACTERISTIC_UUID_GLOBAL_TRADE_ITEM_NUMBER          0x2AFA //Global Trade Item Number
#define CHARACTERISTIC_UUID_ILLUMINANCE                       0x2AFB //Illuminance
#define CHARACTERISTIC_UUID_LUMINOUS_EFFICACY                 0x2AFC //Luminous Efficacy
#define CHARACTERISTIC_UUID_LUMINOUS_ENERGY                   0x2AFD //Luminous Energy
#define CHARACTERISTIC_UUID_LUMINOUS_EXPOSURE                 0x2AFE //Luminous Exposure
#define CHARACTERISTIC_UUID_LUMINOUS_FLUX                     0x2AFF //Luminous Flux
#define CHARACTERISTIC_UUID_LUMINOUS_FLUX_RANGE               0x2B00 //Luminous Flux Range
#define CHARACTERISTIC_UUID_LUMINOUS_INTENSITY                0x2B01 //Luminous Intensity
#define CHARACTERISTIC_UUID_MASS_FLOW                         0x2B02 //Mass Flow
#define CHARACTERISTIC_UUID_PERCEIVED_LIGHTNESS               0x2B03 //Perceived Lightness
#define CHARACTERISTIC_UUID_PERCENTAGE_8                      0x2B04 //Percentage_8
#define CHARACTERISTIC_UUID_POWER                             0x2B05 //Power
#define CHARACTERISTIC_UUID_POWER_SPECIFICATION               0x2B06 //Power Specification
#define CHARACTERISTIC_UUID_RELATIVE_RUNTIME_CURRENT_RANGE    0x2B07 //Relative Runtime in a Current Range
#define CHARACTERISTIC_UUID_RELATIVE_RUNTIME_GENERIC_LEVEL    0x2B08 //Relative Runtime in a Generic Level Range
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_VOLTAGE_RANGE      0x2B09 //Relative Value in a Voltage Range
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_ILLUMINANCE        0x2B0A //Relative Value in an Illuminance Range
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_PERIOD_OF_DAY      0x2B0B //Relative Value in a Period of Day
#define CHARACTERISTIC_UUID_RELATIVE_VALUE_TEMPERATURE_RANGE  0x2B0C //Relative Value in a Temperature Range
#define CHARACTERISTIC_UUID_TEMPERATURE_8                     0x2B0D //Temperature 8
#define CHARACTERISTIC_UUID_TEMPERATURE_8_IN_A_PERIOD_OF_DAY  0x2B0E //Temperature 8 in a Period of Day
#define CHARACTERISTIC_UUID_TEMPERATURE_8_STATISTICS          0x2B0F //Temperature 8 Statistics
#define CHARACTERISTIC_UUID_TEMPERATURE_RANGE                 0x2B10 //Temperature Range
#define CHARACTERISTIC_UUID_TEMPERATURE_STATISTICS            0x2B11 //Temperature Statistics
#define CHARACTERISTIC_UUID_TIME_DECIHOUR_8                   0x2B12 //Time Decihour 8
#define CHARACTERISTIC_UUID_TIME_EXPONENTIAL_8                0x2B13 //Time Exponential 8
#define CHARACTERISTIC_UUID_TIME_HOUR_24                      0x2B14 //Time Hour 24
#define CHARACTERISTIC_UUID_TIME_MILLISECOND_24               0x2B15 //Time Millisecond 24
#define CHARACTERISTIC_UUID_TIME_SECOND_16                    0x2B16 //Time Second1 6
#define CHARACTERISTIC_UUID_TIME_SECOND_8                     0x2B17 //Time Second 8
#define CHARACTERISTIC_UUID_VOLTAGE                           0x2B18 //Voltage
#define CHARACTERISTIC_UUID_VOLTAGE_SPECIFICATION             0x2B19 //Voltage Specification
#define CHARACTERISTIC_UUID_VOLTAGE_STATISTICS                0x2B1A //Voltage Statistics
#define CHARACTERISTIC_UUID_VOLUME_FLOW                       0x2B1B //Volume Flow
#define CHARACTERISTIC_UUID_CHROMATICITY_COORDINATE           0x2B1C //Chromaticity Coordinate
#define CHARACTERISTIC_UUID_RC_FEATURE                        0x2B1D //RC Feature
#define CHARACTERISTIC_UUID_RC_SETTINGS                       0x2B1E //RC Settings
#define CHARACTERISTIC_UUID_RECONN_CFG_CONTROL_POINT          0x2B1F //Reconnection Configuration Control Point
#define CHARACTERISTIC_UUID_IDD_STATUS_CHANGED                0x2B20 //IDD Status Changed
#define CHARACTERISTIC_UUID_IDD_STATUS                        0x2B21 //IDD Status
#define CHARACTERISTIC_UUID_IDD_ANNUNCIATION_STATUS           0x2B22 //IDD Annunciation Status
#define CHARACTERISTIC_UUID_IDD_FEATURES                      0x2B23 //IDD Features
#define CHARACTERISTIC_UUID_IDD_STATUS_READER_CONTROL_POINT   0x2B24 //IDD Status Reader Control Point
#define CHARACTERISTIC_UUID_IDD_COMMAND_CONTROL_POINT         0x2B25 //IDD Command Control Point
#define CHARACTERISTIC_UUID_IDD_COMMAND_DATA                  0x2B26 //IDD Command Data
#define CHARACTERISTIC_UUID_IDD_RECORD_ACCESS_CONTROL_POINT   0x2B27 //IDD Record Access Control Point
#define CHARACTERISTIC_UUID_IDD_HISTORY_DATA                  0x2B28 //IDD History Data
#define CHARACTERISTIC_UUID_CLIENT_SUPPORTED_FEATURES         0x2B29 //Client Supported Features
#define CHARACTERISTIC_UUID_DATABASE_HASH                     0x2B2A //Database Hash
#define CHARACTERISTIC_UUID_BSS_CONTROL_POINT                 0x2B2B //BSS Control Point
#define CHARACTERISTIC_UUID_BSS_RESPONSE                      0x2B2C //BSS Response
#define CHARACTERISTIC_UUID_EMERGENCY_ID                      0x2B2D //Emergency ID
#define CHARACTERISTIC_UUID_EMERGENCY_TEXT                    0x2B2E //Emergency Text
#define CHARACTERISTIC_UUID_ACS_STATUS                        0x2B2F //ACS Status
#define CHARACTERISTIC_UUID_ACS_DATA_IN                       0x2B30 //ACS Data In
#define CHARACTERISTIC_UUID_ACS_DATA_OUT_NOTIFY               0x2B31 //ACS Data Out Notify
#define CHARACTERISTIC_UUID_ACS_DATA_OUT_INDICATE             0x2B32 //ACS Data Out Indicate
#define CHARACTERISTIC_UUID_ACS_CONTROL_POINT                 0x2B33 //ACS Control Point
#define CHARACTERISTIC_UUID_ENHANCED_BP_MEASUREMENT           0x2B34 //Enhanced Blood Pressure Measurement
#define CHARACTERISTIC_UUID_ENHANCED_INTERM_CUFF_PRESSURE     0x2B35 //Enhanced Intermediate Cuff Pressure
#define CHARACTERISTIC_UUID_BLOOD_PRESSURE_RECORD             0x2B36 //Blood Pressure Record
#define CHARACTERISTIC_UUID_REGISTERED_USER                   0x2B37 //Registered User
#define CHARACTERISTIC_UUID_BR_EDR_HANDOVER_DATA              0x2B38 //BR-EDR Handover Data
#define CHARACTERISTIC_UUID_BLUETOOTH_SIG_DATA                0x2B39 //Bluetooth SIG Data
#define CHARACTERISTIC_UUID_SERVER_SUPPORTED_FEATURES         0x2B3A //Server Supported Features
#define CHARACTERISTIC_UUID_PHYSICAL_AM_FEATURES              0x2B3B //Physical Activity Monitor Features
#define CHARACTERISTIC_UUID_GENERAL_ACTIVITY_INST_DATA        0x2B3C //General Activity Instantaneous Data
#define CHARACTERISTIC_UUID_GENERAL_ACTIVITY_SUMMARY_DATA     0x2B3D //General Activity Summary Data
#define CHARACTERISTIC_UUID_CR_ACTIVITY_INSTANTANEOUS_DATA    0x2B3E //Cardio Respiratory Activity Instantaneous Data
#define CHARACTERISTIC_UUID_CR_ACTIVITY_SUMMARY_DATA          0x2B3F //Cardio Respiratory Activity Summary Data
#define CHARACTERISTIC_UUID_SC_ACTIVITY_SUMMARY_DATA          0x2B40 //Step Counter Activity Summary Data
#define CHARACTERISTIC_UUID_SLEEP_ACTIVITY_INST_DATA          0x2B41 //Sleep Activity Instantaneous Data
#define CHARACTERISTIC_UUID_SLEEP_ACTIVITY_SUMMARY_DATA       0x2B42 //Sleep Activity Summary Data
#define CHARACTERISTIC_UUID_PHYSICAL_AM_CONTROL_POINT         0x2B43 //Physical Activity Monitor Control Point
#define CHARACTERISTIC_UUID_ACTIVITY_CURRENT_SESSION          0x2B44 //Activity Current Session
#define CHARACTERISTIC_UUID_PHYSICAL_AS_DESCRIPTOR            0x2B45 //Physical Activity Session Descriptor
#define CHARACTERISTIC_UUID_PREFERRED_UNITS                   0x2B46 //Preferred Units
#define CHARACTERISTIC_UUID_HIGH_RESOLUTION_HEIGHT            0x2B47 //High Resolution Height
#define CHARACTERISTIC_UUID_MIDDLE_NAME                       0x2B48 //Middle Name
#define CHARACTERISTIC_UUID_STRIDE_LENGTH                     0x2B49 //Stride Length
#define CHARACTERISTIC_UUID_HANDEDNESS                        0x2B4A //Handedness
#define CHARACTERISTIC_UUID_DEVICE_WEARING_POSITION           0x2B4B //Device Wearing Position
#define CHARACTERISTIC_UUID_FOUR_ZONE_HEART_RATE_LIMITS       0x2B4C //Four Zone Heart Rate Limits
#define CHARACTERISTIC_UUID_HIGH_INTENSITY_EXERCISE_TH        0x2B4D //High Intensity Exercise Threshold
#define CHARACTERISTIC_UUID_ACTIVITY_GOAL                     0x2B4E //Activity Goal
#define CHARACTERISTIC_UUID_SEDENTARY_INTERVAL_NOTIFICATION   0x2B4F //Sedentary Interval Notification
#define CHARACTERISTIC_UUID_CALORIC_INTAKE                    0x2B50 //Caloric Intake
#define CHARACTERISTIC_UUID_TMAP_ROLE                         0x2B51 //TMAP Role
#define CHARACTERISTIC_UUID_AUDIO_INPUT_STATE                 0x2B77 //Audio Input State
#define CHARACTERISTIC_UUID_GAIN_SETTINGS_ATTRIBUTE           0x2B78 //Gain Settings Attribute
#define CHARACTERISTIC_UUID_AUDIO_INPUT_TYPE                  0x2B79 //Audio Input Type
#define CHARACTERISTIC_UUID_AICS_INPUT_STATUS                 0x2B7A //Audio Input Status
#define CHARACTERISTIC_UUID_AUDIO_INPUT_CONTROL_POINT         0x2B7B //Audio Input Control Point
#define CHARACTERISTIC_UUID_AUDIO_INPUT_DESCRIPTION           0x2B7C //Audio Input Description
#define CHARACTERISTIC_UUID_VOLUME_STATE                      0x2B7D //Volume State
#define CHARACTERISTIC_UUID_VOLUME_CONTROL_POINT              0x2B7E //Volume Control Point
#define CHARACTERISTIC_UUID_VOLUME_FLAGS                      0x2B7F //Volume Flags
#define CHARACTERISTIC_UUID_VOLUME_OFFSET_STATE               0x2B80 //Volume Offset State
#define CHARACTERISTIC_UUID_AUDIO_LOCATION                    0x2B81 //Audio Location
#define CHARACTERISTIC_UUID_VOLUME_OFFSET_CONTROL_POINT       0x2B82 //Volume Offset Control Point
#define CHARACTERISTIC_UUID_AUDIO_OUTPUT_DESCRIPTION          0x2B83 //Audio Output Description
#define CHARACTERISTIC_UUID_SET_IDENTITY_RESOLVING_KEY        0x2B84 //Set Identity Resolving Key
#define CHARACTERISTIC_UUID_COORDINATED_SET_SIZE              0x2B85 //Coordinated Set Size
#define CHARACTERISTIC_UUID_SET_MEMBER_LOCK                   0x2B86 //Set Member Lock
#define CHARACTERISTIC_UUID_SET_MEMBER_RANK                   0x2B87 //Set Member Rank
#define CHARACTERISTIC_UUID_ENCRYPTED_DATA_KEY_MATERIAL       0x2B88 //Encrypted Data Key Material
#define CHARACTERISTIC_UUID_APPARENT_ENERGY_32                0x2B89 //Apparent Energy 32
#define CHARACTERISTIC_UUID_APPARENT_POWER                    0x2B8A //Apparent Power
#define CHARACTERISTIC_UUID_LIVE_HEALTH_OBSERVATIONS          0x2B8B //Live Health Observations
#define CHARACTERISTIC_UUID_CO_CONC                           0x2B8C //CO\{}textsubscript{2} Concentration
#define CHARACTERISTIC_UUID_COSINE_OF_THE_ANGLE               0x2B8D //Cosine of the Angle
#define CHARACTERISTIC_UUID_DEVICE_TIME_FEATURE               0x2B8E //Device Time Feature
#define CHARACTERISTIC_UUID_DEVICE_TIME_PARAMETERS            0x2B8F //Device Time Parameters
#define CHARACTERISTIC_UUID_DEVICE_TIME                       0x2B90 //Device Time
#define CHARACTERISTIC_UUID_DEVICE_TIME_CONTROL_POINT         0x2B91 //Device Time Control Point
#define CHARACTERISTIC_UUID_TIME_CHANGE_LOG_DATA              0x2B92 //Time Change Log Data
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_NAME                 0x2B93 //Media Player Name
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_OBJECT_ID       0x2B94 //Media Player Icon Object ID
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_URL             0x2B95 //Media Player Icon URL
#define CHARACTERISTIC_UUID_TRACK_CHANGED                     0x2B96 //Track Changed
#define CHARACTERISTIC_UUID_TRACK_TITLE                       0x2B97 //Track Title
#define CHARACTERISTIC_UUID_TRACK_DURATION                    0x2B98 //Track Duration
#define CHARACTERISTIC_UUID_TRACK_POSITION                    0x2B99 //Track Position
#define CHARACTERISTIC_UUID_PLAYBACK_SPEED                    0x2B9A //Playback Speed
#define CHARACTERISTIC_UUID_SEEKING_SPEED                     0x2B9B //Seeking Speed
#define CHARACTERISTIC_UUID_CURRENT_TRACK_SEGMENTS_OBJECT_ID  0x2B9C //Current Track Segments Object ID
#define CHARACTERISTIC_UUID_CURRENT_TRACK_OBJECT_ID           0x2B9D //Current Track Object ID
#define CHARACTERISTIC_UUID_NEXT_TRACK_OBJECT_ID              0x2B9E //Next Track Object ID
#define CHARACTERISTIC_UUID_PARENT_GROUP_OBJECT_ID            0x2B9F //Parent Group Object ID
#define CHARACTERISTIC_UUID_CURRENT_GROUP_OBJECT_ID           0x2BA0 //Current Group Object ID
#define CHARACTERISTIC_UUID_PLAYING_ORDER                     0x2BA1 //Playing Order
#define CHARACTERISTIC_UUID_PLAYING_ORDERS_SUPPORTED          0x2BA2 //Playing Orders Supported
#define CHARACTERISTIC_UUID_MEDIA_STATE                       0x2BA3 //Media State
#define CHARACTERISTIC_UUID_MEDIA_CONTROL_POINT               0x2BA4 //Media Control Point
#define CHARACTERISTIC_UUID_MEDIA_CTRL_POINT_OP_SUPPORTED     0x2BA5 //Media Control Point Opcodes Supported
#define CHARACTERISTIC_UUID_SEARCH_RESULTS_OBJECT_ID          0x2BA6 //Search Results Object ID
#define CHARACTERISTIC_UUID_SEARCH_CONTROL_POINT              0x2BA7 //Search Control Point
#define CHARACTERISTIC_UUID_ENERGY_32                         0x2BA8 //Energy 32
#define CHARACTERISTIC_UUID_MEDIA_PLAYER_ICON_OBJECT_TYPE     0x2BA9 //Media Player Icon Object Type
#define CHARACTERISTIC_UUID_TRACK_SEGMENTS_OBJECT_TYPE        0x2BAA //Track Segments Object Type
#define CHARACTERISTIC_UUID_TRACK_OBJECT_TYPE                 0x2BAB //Track Object Type
#define CHARACTERISTIC_UUID_GROUP_OBJECT_TYPE                 0x2BAC //Group Object Type
#define CHARACTERISTIC_UUID_CONSTANT_TONE_EXTENSION_ENABLE    0x2BAD //Constant Tone Extension Enable
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_MIN_LENGTH        0x2BAE //Advertising Constant Tone Extension Minimum Length
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_MIN_TRANSMIT_CNT  0x2BAF //Advertising Constant Tone Extension Minimum Transmit Count
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_TRAN_DURATION     0x2BB0 //Advertising Constant Tone Extension Transmit Duration
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_INTERVAL          0x2BB1 //Advertising Constant Tone Extension Interval
#define CHARACTERISTIC_UUID_ADVERTISING_CTE_PHY               0x2BB2 //Advertising Constant Tone Extension PHY
#define CHARACTERISTIC_UUID_BEARER_PROVIDER_NAME              0x2BB3 //Bearer Provider Name
#define CHARACTERISTIC_UUID_BEARER_UCI                        0x2BB4 //Bearer UCI
#define CHARACTERISTIC_UUID_BEARER_TECHNOLOGY                 0x2BB5 //Bearer Technology
#define CHARACTERISTIC_UUID_BEARER_URI_SCHEMES_SUPPORTED_LIST 0x2BB6 //Bearer URI Schemes Supported List
#define CHARACTERISTIC_UUID_BEARER_SS                         0x2BB7 //Bearer Signal Strength
#define CHARACTERISTIC_UUID_BEARER_SS_REPORTING_INTERVAL      0x2BB8 //Bearer Signal Strength Reporting Interval
#define CHARACTERISTIC_UUID_BEARER_LIST_CURRENT_CALLS         0x2BB9 //Bearer List Current Calls
#define CHARACTERISTIC_UUID_CONTENT_CONTROL_ID                0x2BBA //Content Control ID
#define CHARACTERISTIC_UUID_STATUS_FLAGS                      0x2BBB //Status Flags
#define CHARACTERISTIC_UUID_INCOMING_CALL_TARGET_BEARER_URI   0x2BBC //Incoming Call Target Bearer URI
#define CHARACTERISTIC_UUID_CALL_STATE                        0x2BBD //Call State
#define CHARACTERISTIC_UUID_CALL_CTRL_POINT                   0x2BBE //Call Control Point
#define CHARACTERISTIC_UUID_CALL_CTRL_POINT_OPTIONAL_OPCODES  0x2BBF //Call Control Point Optional Opcodes
#define CHARACTERISTIC_UUID_TERMINATION_REASON                0x2BC0 //Termination Reason
#define CHARACTERISTIC_UUID_INCOMING_CALL                     0x2BC1 //Incoming Call
#define CHARACTERISTIC_UUID_CALL_FRIENDLY_NAME                0x2BC2 //Call Friendly Name
#define CHARACTERISTIC_UUID_MUTE                              0x2BC3 //Mute
#define CHARACTERISTIC_UUID_SINK_ASE                          0x2BC4 //Sink ASE
#define CHARACTERISTIC_UUID_SOURCE_ASE                        0x2BC5 //Source ASE
#define CHARACTERISTIC_UUID_ASE_CONTROL_POINT                 0x2BC6 //ASE Control Point
#define CHARACTERISTIC_UUID_BAS_CONTROL_POINT                 0x2BC7 //Broadcast Audio Scan Control Point
#define CHARACTERISTIC_UUID_BROADCAST_RECEIVE_STATE           0x2BC8 //Broadcast Receive State
#define CHARACTERISTIC_UUID_SINK_PAC                          0x2BC9 //Sink PAC
#define CHARACTERISTIC_UUID_SINK_AUDIO_LOCATIONS              0x2BCA //Sink Audio Locations
#define CHARACTERISTIC_UUID_SOURCE_PAC                        0x2BCB //Source PAC
#define CHARACTERISTIC_UUID_SOURCE_AUDIO_LOCATIONS            0x2BCC //Source Audio Locations
#define CHARACTERISTIC_UUID_AVAILABLE_AUDIO_CONTEXTS          0x2BCD //Available Audio Contexts
#define CHARACTERISTIC_UUID_SUPPORTED_AUDIO_CONTEXTS          0x2BCE //Supported Audio Contexts
#define CHARACTERISTIC_UUID_AMMONIA_CONC                      0x2BCF //Ammonia Concentration
#define CHARACTERISTIC_UUID_CARBON_MONOXIDE_CONC              0x2BD0 //Carbon Monoxide Concentration
#define CHARACTERISTIC_UUID_METHANE_CONC                      0x2BD1 //Methane Concentration
#define CHARACTERISTIC_UUID_NITROGEN_DIOXIDE_CONC             0x2BD2 //Nitrogen Dioxide Concentration
#define CHARACTERISTIC_UUID_NON_METHANE_VOLATILE_OCS_CONC     0x2BD3 //Non-Methane Volatile Organic Compounds Concentration
#define CHARACTERISTIC_UUID_OZONE_CONC                        0x2BD4 //Ozone Concentration
#define CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM1_CONC       0x2BD5 //Particulate Matter-PM1 Concentration
#define CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM2_5_CONC     0x2BD6 //Particulate Matter-PM2.5 Concentration
#define CHARACTERISTIC_UUID_PARTICULATE_MATTER_PM10_CONC      0x2BD7 //Particulate Matter-PM10 Concentration
#define CHARACTERISTIC_UUID_SULFUR_DIOXIDE_CONC               0x2BD8 //Sulfur Dioxide Concentration
#define CHARACTERISTIC_UUID_SULFUR_HEXAFLUORIDE_CONC          0x2BD9 //Sulfur Hexafluoride Concentration
#define CHARACTERISTIC_UUID_HEARING_AID_FEATURES              0x2BDA //Hearing Aid Features
#define CHARACTERISTIC_UUID_HEARING_AID_PRESET_CONTROL_POINT  0x2BDB //Hearing Aid Preset Control Point
#define CHARACTERISTIC_UUID_ACTIVE_PRESET_INDEX               0x2BDC //Active Preset Index
#define CHARACTERISTIC_UUID_STORED_HEALTH_OBSERVATIONS        0x2BDD //Stored Health Observations
#define CHARACTERISTIC_UUID_FIXED_STRING_64                   0x2BDE //Fixed String 64
#define CHARACTERISTIC_UUID_HIGH_TEMPERATURE                  0x2BDF //High Temperature
#define CHARACTERISTIC_UUID_HIGH_VOLTAGE                      0x2BE0 //High Voltage
#define CHARACTERISTIC_UUID_LIGHT_DISTRIBUTION                0x2BE1 //Light Distribution
#define CHARACTERISTIC_UUID_LIGHT_OUTPUT                      0x2BE2 //Light Output
#define CHARACTERISTIC_UUID_LIGHT_SOURCE_TYPE                 0x2BE3 //Light Source Type
#define CHARACTERISTIC_UUID_NOISE                             0x2BE4 //Noise
#define CHARACTERISTIC_UUID_RELATIVE_RUNTIME2                 0x2BE5 //Relative Runtime in a Correlated Color Temperature Range
#define CHARACTERISTIC_UUID_TIME_SECOND_32                    0x2BE6 //Time Second 32
#define CHARACTERISTIC_UUID_VOC_CONC                          0x2BE7 //VOC Concentration
#define CHARACTERISTIC_UUID_VOLTAGE_FREQUENCY                 0x2BE8 //Voltage Frequency
#define CHARACTERISTIC_UUID_BATTERY_CRITICAL_STATUS           0x2BE9 //Battery Critical Status
#define CHARACTERISTIC_UUID_BATTERY_HEALTH_STATUS             0x2BEA //Battery Health Status
#define CHARACTERISTIC_UUID_BATTERY_HEALTH_INFORMATION        0x2BEB //Battery Health Information
#define CHARACTERISTIC_UUID_BATTERY_INFORMATION               0x2BEC //Battery Information
#define CHARACTERISTIC_UUID_BATTERY_LEVEL_STATUS              0x2BED //Battery Level Status
#define CHARACTERISTIC_UUID_BATTERY_TIME_STATUS               0x2BEE //Battery Time Status
#define CHARACTERISTIC_UUID_ESTIMATED_SERVICE_DATE            0x2BEF //Estimated Service Date
#define CHARACTERISTIC_UUID_BATTERY_ENERGY_STATUS             0x2BF0 //Battery Energy Status
#define CHARACTERISTIC_UUID_OBSERVATION_SCHEDULE_CHANGED      0x2BF1 //Observation Schedule Changed
#define CHARACTERISTIC_UUID_CURRENT_ELAPSED_TIME              0x2BF2 //Current Elapsed Time
#define CHARACTERISTIC_UUID_HEALTH_SENSOR_FEATURES            0x2BF3 //Health Sensor Features
#define CHARACTERISTIC_UUID_GHS_CONTROL_POINT                 0x2BF4 //GHS Control Point
#define CHARACTERISTIC_UUID_LE_GATT_SECURITY_LEVELS           0x2BF5 //LE GATT Security Levels
#define CHARACTERISTIC_UUID_ESL_ADDRESS                       0x2BF6 //ESL Address
#define CHARACTERISTIC_UUID_AP_SYNC_KEY_MATERIAL              0x2BF7 //AP Sync Key Material
#define CHARACTERISTIC_UUID_ESL_RESPONSE_KEY_MATERIAL         0x2BF8 //ESL Response Key Material
#define CHARACTERISTIC_UUID_ESL_CURRENT_ABSOLUTE_TIME         0x2BF9 //ESL Current Absolute Time
#define CHARACTERISTIC_UUID_ESL_DISPLAY_INFORMATION           0x2BFA //ESL Display Information
#define CHARACTERISTIC_UUID_ESL_IMAGE_INFORMATION             0x2BFB //ESL Image Information
#define CHARACTERISTIC_UUID_ESL_SENSOR_INFORMATION            0x2BFC //ESL Sensor Information
#define CHARACTERISTIC_UUID_ESL_LED_INFORMATION               0x2BFD //ESL LED Information
#define CHARACTERISTIC_UUID_ESL_CONTROL_POINT                 0x2BFE //ESL Control Point
#define CHARACTERISTIC_UUID_UDI_FOR_MEDICAL_DEVICES           0x2BFF //UDI for Medical Devices
#define CHARACTERISTIC_UUID_GMAP_ROLE                         0x2C00 //GMAP Role
#define CHARACTERISTIC_UUID_UGG_FEATURES                      0x2C01 //UGG Features
#define CHARACTERISTIC_UUID_UGT_FEATURES                      0x2C02 //UGT Features
#define CHARACTERISTIC_UUID_BGS_FEATURES                      0x2C03 //BGS Features
#define CHARACTERISTIC_UUID_BGR_FEATURES                      0x2C04 //BGR Features
//TODO: Ranging profile has not been confirmed yet. by junhui.hu
#define CHARACTERISTIC_UUID_RAS_FEATURE              0x2C05 //RAS feature
#define CHARACTERISTIC_UUID_REAL_TIME_PROCEDURE_DATA 0x2C06 //Live Ranging Data
#define CHARACTERISTIC_UUID_ON_DEMAND_PROCEDURE_DATA 0x2C07 //Stored Ranging Data
#define CHARACTERISTIC_UUID_CONTROL_POINT            0x2C08 //Control Point
#define CHARACTERISTIC_UUID_RANGING_DATA_READY       0x2C09 //Ranging Data Ready
#define CHARACTERISTIC_UUID_RANGING_DATA_OVERWRITTEN 0x2C0A //Ranging Data Overwritten
//TODO: Ultra Low Latency HID service has not been confirmed yet. by qihang.mou
#define CHARACTERISTIC_UUID_ULL_HID_PROPERTIES    0x2C23 //Ultra Low Latency HID Properties
#define CHARACTERISTIC_UUID_LE_HID_OPERATION_MODE 0x2C24 //LE HID Operation mode
//TODO: Coordinated Set Name Characteristic has not been confirmed yet.
#define CHARACTERISTIC_UUID_COORDINATED_SET_NAME 0x7FE6 //Coordinated Set Name

extern const unsigned char serviceGenericAccessUuid[];
extern const unsigned char serviceGenericAttributeUuid[];
extern const unsigned char serviceImmediateAlertUuid[];
extern const unsigned char serviceLinkLossUuid[];
extern const unsigned char serviceTxPowerUuid[];
extern const unsigned char serviceCurrentTimeUuid[];
extern const unsigned char serviceReferenceTimeUpdateUuid[];
extern const unsigned char serviceNextDstChangeUuid[];
extern const unsigned char serviceGlucoseUuid[];
extern const unsigned char serviceHealthThermometerUuid[];
extern const unsigned char serviceDeviceInformationUuid[];
extern const unsigned char serviceHeartRateUuid[];
extern const unsigned char servicePhoneAlertStatusUuid[];
extern const unsigned char serviceBatteryUuid[];
extern const unsigned char serviceBloodPressureUuid[];
extern const unsigned char serviceAlertNotificationUuid[];
extern const unsigned char serviceHumanInterfaceDeviceUuid[];
extern const unsigned char serviceScanParametersUuid[];
extern const unsigned char serviceRunningSpeedAndCadenceUuid[];
extern const unsigned char serviceAutomationIoUuid[];
extern const unsigned char serviceCyclingSpeedAndCadenceUuid[];
extern const unsigned char serviceCyclingPowerUuid[];
extern const unsigned char serviceLocationAndNavigationUuid[];
extern const unsigned char serviceEnvironmentalSensingUuid[];
extern const unsigned char serviceBodyCompositionUuid[];
extern const unsigned char serviceUserDataUuid[];
extern const unsigned char serviceWeightScaleUuid[];
extern const unsigned char serviceBondManagementUuid[];
extern const unsigned char serviceContinuousGlucoseMonitoringUuid[];
extern const unsigned char serviceInternetProtocolSupportUuid[];
extern const unsigned char serviceIndoorPositioningUuid[];
extern const unsigned char servicePulseOximeterUuid[];
extern const unsigned char serviceHttpProxyUuid[];
extern const unsigned char serviceTransportDiscoveryUuid[];
extern const unsigned char serviceObjectTransferUuid[];
extern const unsigned char serviceFitnessMachineUuid[];
extern const unsigned char serviceMeshProvisioningUuid[];
extern const unsigned char serviceMeshProxyUuid[];
extern const unsigned char serviceReconnectionConfigurationUuid[];
extern const unsigned char serviceInsulinDeliveryUuid[];
extern const unsigned char serviceBinarySensorUuid[];
extern const unsigned char serviceEmergencyConfigurationUuid[];
extern const unsigned char serviceAuthorizationControlUuid[];
extern const unsigned char servicePhysicalActivityMonitorUuid[];
extern const unsigned char serviceElapsedTimeUuid[];
extern const unsigned char serviceGenericHealthSensorUuid[];
extern const unsigned char serviceAudioInputControlUuid[];
extern const unsigned char serviceVolumeControlUuid[];
extern const unsigned char serviceVolumeOffsetControlUuid[];
extern const unsigned char serviceCoordinatedSetIdentificationUuid[];
extern const unsigned char serviceDeviceTimeUuid[];
extern const unsigned char serviceMediaControlUuid[];
extern const unsigned char serviceGenericMediaControlUuid[];
extern const unsigned char serviceConstantToneExtensionUuid[];
extern const unsigned char serviceTelephoneBearerUuid[];
extern const unsigned char serviceGenericTelephoneBearerUuid[];
extern const unsigned char serviceMicrophoneControlUuid[];
extern const unsigned char serviceAudioStreamControlUuid[];
extern const unsigned char serviceBroadcastAudioScanUuid[];
extern const unsigned char servicePublishedAudioCapabilitiesUuid[];
extern const unsigned char serviceBasicAudioAnnouncementUuid[];
extern const unsigned char serviceBroadcastAudioAnnouncementUuid[];
extern const unsigned char serviceCommonAudioUuid[];
extern const unsigned char serviceHearingAccessUuid[];
extern const unsigned char serviceTelephonyAndMediaAudioUuid[];
extern const unsigned char servicePublicBroadcastAnnouncementUuid[];
extern const unsigned char serviceElectronicShelfLabelUuid[];
extern const unsigned char serviceGamingAudioUuid[];
extern const unsigned char serviceMeshProxySolicitationUuid[];
//TODO: Ranging profile has not been confirmed yet. by junhui.hu
extern const unsigned char serviceRangingUuid[];
//TODO: Ultra Low Latency HID service has not been confirmed yet. by qihang.mou
extern const unsigned char serviceUllhidUuid[];

extern const unsigned char declarationsPrimaryServiceUuid[];
extern const unsigned char declarationsSecondaryServiceUuid[];
extern const unsigned char declarationsIncludeUuid[];
extern const unsigned char declarationsCharacteristicUuid[];
extern const unsigned char descriptorCharacteristicExtendedPropertiesUuid[];
extern const unsigned char descriptorCharacteristicUserDescriptionUuid[];
extern const unsigned char descriptorClientCharacteristicConfigurationUuid[];
extern const unsigned char descriptorServerCharacteristicConfigurationUuid[];
extern const unsigned char descriptorCharacteristicPresentationFormatUuid[];
extern const unsigned char descriptorCharacteristicAggregateFormatUuid[];
extern const unsigned char descriptorValidRangeUuid[];
extern const unsigned char descriptorExternalReportReferenceUuid[];
extern const unsigned char descriptorReportReferenceUuid[];
extern const unsigned char descriptorNumberOfDigitalsUuid[];
extern const unsigned char descriptorValueTriggerSettingUuid[];
extern const unsigned char descriptorEnvironmentalSensingConfigurationUuid[];
extern const unsigned char descriptorEnvironmentalSensingMeasurementUuid[];
extern const unsigned char descriptorEnvironmentalSensingTriggerSettingUuid[];
extern const unsigned char descriptorTimeTriggerSettingUuid[];
extern const unsigned char descriptorCompleteBrEdrTransportBlockDataUuid[];
extern const unsigned char descriptorObservationScheduleUuid[];
extern const unsigned char descriptorValidRangeAndAccuracyUuid[];
extern const unsigned char characteristicDeviceNameUuid[];
extern const unsigned char characteristicAppearanceUuid[];
extern const unsigned char characteristicPeripheralPrivacyFlagUuid[];
extern const unsigned char characteristicReconnectionAddressUuid[];
extern const unsigned char characteristicPeripheralPreferredConnParamUuid[];
extern const unsigned char characteristicServiceChangedUuid[];
extern const unsigned char characteristicAlertLevelUuid[];
extern const unsigned char characteristicTxPowerLevelUuid[];
extern const unsigned char characteristicDateTimeUuid[];
extern const unsigned char characteristicDayofWeekUuid[];
extern const unsigned char characteristicDayDateTimeUuid[];
extern const unsigned char characteristicExactTime256Uuid[];
extern const unsigned char characteristicDstOffsetUuid[];
extern const unsigned char characteristicTimeZoneUuid[];
extern const unsigned char characteristicLocalTimeInformationUuid[];
extern const unsigned char characteristicTimeWithDstUuid[];
extern const unsigned char characteristicTimeAccuracyUuid[];
extern const unsigned char characteristicTimeSourceUuid[];
extern const unsigned char characteristicReferenceTimeInformationUuid[];
extern const unsigned char characteristicTimeUpdateControlPointUuid[];
extern const unsigned char characteristicTimeUpdateStateUuid[];
extern const unsigned char characteristicGlucoseMeasurementUuid[];
extern const unsigned char characteristicBatteryLevelUuid[];
extern const unsigned char characteristicBatteryPowerStateUuid[];
extern const unsigned char characteristicTemperatureMeasurementUuid[];
extern const unsigned char characteristicTemperatureTypeUuid[];
extern const unsigned char characteristicIntermediateTemperatureUuid[];
extern const unsigned char characteristicMeasurementIntervalUuid[];
extern const unsigned char characteristicBootKeyboardInputReportUuid[];
extern const unsigned char characteristicSystemIdUuid[];
extern const unsigned char characteristicModelNumberStringUuid[];
extern const unsigned char characteristicSerialNumberStringUuid[];
extern const unsigned char characteristicFirmwareRevisionStringUuid[];
extern const unsigned char characteristicHardwareRevisionStringUuid[];
extern const unsigned char characteristicSoftwareRevisionStringUuid[];
extern const unsigned char characteristicManufacturerNameStringUuid[];
extern const unsigned char characteristicIEEE_11073_20601DataListUuid[];
extern const unsigned char characteristicCurrentTimeUuid[];
extern const unsigned char characteristicMagneticDeclinationUuid[];
extern const unsigned char characteristicScanRefreshUuid[];
extern const unsigned char characteristicBootKeyboardOutputReportUuid[];
extern const unsigned char characteristicBootMouseInputReportUuid[];
extern const unsigned char characteristicGlucoseMeasurementContextUuid[];
extern const unsigned char characteristicBloodPressureMeasurementUuid[];
extern const unsigned char characteristicIntermediateCuffPressureUuid[];
extern const unsigned char characteristicHeartRateMeasurementUuid[];
extern const unsigned char characteristicBodySensorLocationUuid[];
extern const unsigned char characteristicHeartRateControlPointUuid[];
extern const unsigned char characteristicAlertStatusUuid[];
extern const unsigned char characteristicRingerControlPointUuid[];
extern const unsigned char characteristicRingerSettingUuid[];
extern const unsigned char characteristicAlertCategoryIdBitMaskUuid[];
extern const unsigned char characteristicAlertCategoryIdUuid[];
extern const unsigned char characteristicAlertNotificationControlPointUuid[];
extern const unsigned char characteristicUnreadAlertStatusUuid[];
extern const unsigned char characteristicNewAlertUuid[];
extern const unsigned char characteristicSupportedNewAlertCategoryUuid[];
extern const unsigned char characteristicSupportedUnreadAlertCategoryUuid[];
extern const unsigned char characteristicBloodPressureFeatureUuid[];
extern const unsigned char characteristicHidInformationUuid[];
extern const unsigned char characteristicReportMapUuid[];
extern const unsigned char characteristicHidControlPointUuid[];
extern const unsigned char characteristicReportUuid[];
extern const unsigned char characteristicProtocolModeUuid[];
extern const unsigned char characteristicScanIntervalWindowUuid[];
extern const unsigned char characteristicPnpIdUuid[];
extern const unsigned char characteristicGlucoseFeatureUuid[];
extern const unsigned char characteristicRecordAccessControlPointUuid[];
extern const unsigned char characteristicRscMeasurementUuid[];
extern const unsigned char characteristicRscFeatureUuid[];
extern const unsigned char characteristicScControlPointUuid[];
extern const unsigned char characteristicAggregateUuid[];
extern const unsigned char characteristicCscMeasurementUuid[];
extern const unsigned char characteristicCscFeatureUuid[];
extern const unsigned char characteristicSensorLocationUuid[];
extern const unsigned char characteristicPlxSpotCheckMeasurementUuid[];
extern const unsigned char characteristicPlxContinuousMeasurementUuid[];
extern const unsigned char characteristicPlxFeaturesUuid[];
extern const unsigned char characteristicCyclingPowerMeasurementUuid[];
extern const unsigned char characteristicCyclingPowerVectorUuid[];
extern const unsigned char characteristicCyclingPowerFeatureUuid[];
extern const unsigned char characteristicCyclingPowerControlPointUuid[];
extern const unsigned char characteristicLocationAndSpeedUuid[];
extern const unsigned char characteristicNavigationUuid[];
extern const unsigned char characteristicPositionQualityUuid[];
extern const unsigned char characteristicLnFeatureUuid[];
extern const unsigned char characteristicLnControlPointUuid[];
extern const unsigned char characteristicElevationUuid[];
extern const unsigned char characteristicPressureUuid[];
extern const unsigned char characteristicTemperatureUuid[];
extern const unsigned char characteristicHumidityUuid[];
extern const unsigned char characteristicTrueWindSpeedUuid[];
extern const unsigned char characteristicTrueWindDirectionUuid[];
extern const unsigned char characteristicApparentWindSpeedUuid[];
extern const unsigned char characteristicApparentWindDirectionUuid[];
extern const unsigned char characteristicGustFactorUuid[];
extern const unsigned char characteristicPollenConcUuid[];
extern const unsigned char characteristicUvIndexUuid[];
extern const unsigned char characteristicIrradianceUuid[];
extern const unsigned char characteristicRainfallUuid[];
extern const unsigned char characteristicWindChillUuid[];
extern const unsigned char characteristicHeatIndexUuid[];
extern const unsigned char characteristicDewPointUuid[];
extern const unsigned char characteristicDescriptorValueChangedUuid[];
extern const unsigned char characteristicAerobicHeartRateLowerLimitUuid[];
extern const unsigned char characteristicAerobicThresholdUuid[];
extern const unsigned char characteristicAgeUuid[];
extern const unsigned char characteristicAnaerobicHeartRateLowerLimitUuid[];
extern const unsigned char characteristicAnaerobicHeartRateUpperLimitUuid[];
extern const unsigned char characteristicAnaerobicThresholdUuid[];
extern const unsigned char characteristicAerobicHeartRateUpperLimitUuid[];
extern const unsigned char characteristicDateOfBirthUuid[];
extern const unsigned char characteristicDateOfThresholdAssessmentUuid[];
extern const unsigned char characteristicEmailAddressUuid[];
extern const unsigned char characteristicFatBurnHeartRateLowerLimitUuid[];
extern const unsigned char characteristicFatBurnHeartRateUpperLimitUuid[];
extern const unsigned char characteristicFirstNameUuid[];
extern const unsigned char characteristicFiveZoneHeartRateLimitsUuid[];
extern const unsigned char characteristicGenderUuid[];
extern const unsigned char characteristicHeartRateMaxUuid[];
extern const unsigned char characteristicHeightUuid[];
extern const unsigned char characteristicHipCircumferenceUuid[];
extern const unsigned char characteristicLastNameUuid[];
extern const unsigned char characteristicMaximumRecommendedHeartRateUuid[];
extern const unsigned char characteristicRestingHeartRateUuid[];
extern const unsigned char characteristicSportTypeUuid[];
extern const unsigned char characteristicThreeZoneHeartRateLimitsUuid[];
extern const unsigned char characteristicTwoZoneHeartRateLimitsUuid[];
extern const unsigned char characteristicVo2MaxUuid[];
extern const unsigned char characteristicWaistCircumferenceUuid[];
extern const unsigned char characteristicWeightUuid[];
extern const unsigned char characteristicDatabaseChangeIncrementUuid[];
extern const unsigned char characteristicUserIndexUuid[];
extern const unsigned char characteristicBodyCompositionFeatureUuid[];
extern const unsigned char characteristicBodyCompositionMeasurementUuid[];
extern const unsigned char characteristicWeightMeasurementUuid[];
extern const unsigned char characteristicWeightScaleFeatureUuid[];
extern const unsigned char characteristicUserControlPointUuid[];
extern const unsigned char characteristicMagneticFluxDensity2dUuid[];
extern const unsigned char characteristicMagneticFluxDensity3dUuid[];
extern const unsigned char characteristicLanguageUuid[];
extern const unsigned char characteristicBarometricPressureTrendUuid[];
extern const unsigned char characteristicBondManagementControlPointUuid[];
extern const unsigned char characteristicBondManagementFeatureUuid[];
extern const unsigned char characteristicCentralAddressResolutionUuid[];
extern const unsigned char characteristicCgmMeasurementUuid[];
extern const unsigned char characteristicCgmFeatureUuid[];
extern const unsigned char characteristicCgmStatusUuid[];
extern const unsigned char characteristicCgmSessionStartTimeUuid[];
extern const unsigned char characteristicCgmSessionRunTimeUuid[];
extern const unsigned char characteristicCgmSpecificOpsControlPointUuid[];
extern const unsigned char characteristicIndoorPositioningConfigurationUuid[];
extern const unsigned char characteristicLatitudeUuid[];
extern const unsigned char characteristicLongitudeUuid[];
extern const unsigned char characteristicLocalNorthCoordinateUuid[];
extern const unsigned char characteristicLocalEastCoordinateUuid[];
extern const unsigned char characteristicFloorNumberUuid[];
extern const unsigned char characteristicAltitudeUuid[];
extern const unsigned char characteristicUncertaintyUuid[];
extern const unsigned char characteristicLocationNameUuid[];
extern const unsigned char characteristicUriUuid[];
extern const unsigned char characteristicHttpHeadersUuid[];
extern const unsigned char characteristicHttpStatusCodeUuid[];
extern const unsigned char characteristicHttpEntityBodyUuid[];
extern const unsigned char characteristicHttpControlPointUuid[];
extern const unsigned char characteristicHttpsSecurityUuid[];
extern const unsigned char characteristicTdsControlPointUuid[];
extern const unsigned char characteristicOtsFeatureUuid[];
extern const unsigned char characteristicObjectNameUuid[];
extern const unsigned char characteristicObjectTypeUuid[];
extern const unsigned char characteristicObjectSizeUuid[];
extern const unsigned char characteristicObjectFirstCreatedUuid[];
extern const unsigned char characteristicObjectLastModifiedUuid[];
extern const unsigned char characteristicObjectIdUuid[];
extern const unsigned char characteristicObjectPropertiesUuid[];
extern const unsigned char characteristicObjectActionControlPointUuid[];
extern const unsigned char characteristicObjectListControlPointUuid[];
extern const unsigned char characteristicObjectListFilterUuid[];
extern const unsigned char characteristicObjectChangedUuid[];
extern const unsigned char characteristicResolvablePrivateAddressOnlyUuid[];
extern const unsigned char characteristicFitnessMachineFeatureUuid[];
extern const unsigned char characteristicTreadmillDataUuid[];
extern const unsigned char characteristicCrossTrainerDataUuid[];
extern const unsigned char characteristicStepClimberDataUuid[];
extern const unsigned char characteristicStairClimberDataUuid[];
extern const unsigned char characteristicRowerDataUuid[];
extern const unsigned char characteristicIndoorBikeDataUuid[];
extern const unsigned char characteristicTrainingStatusUuid[];
extern const unsigned char characteristicSupportedSpeedRangeUuid[];
extern const unsigned char characteristicSupportedInclinationRangeUuid[];
extern const unsigned char characteristicSupportedResistanceLevelRangeUuid[];
extern const unsigned char characteristicSupportedHeartRateRangeUuid[];
extern const unsigned char characteristicSupportedPowerRangeUuid[];
extern const unsigned char characteristicFitnessMachineControlPointUuid[];
extern const unsigned char characteristicFitnessMachineStatusUuid[];
extern const unsigned char characteristicMeshProvisioningDataInUuid[];
extern const unsigned char characteristicMeshProvisioningDataOutUuid[];
extern const unsigned char characteristicMeshProxyDataInUuid[];
extern const unsigned char characteristicMeshProxyDataOutUuid[];
extern const unsigned char characteristicAverageCurrentUuid[];
extern const unsigned char characteristicAverageVoltageUuid[];
extern const unsigned char characteristicBooleanUuid[];
extern const unsigned char characteristicChromaticDistFromPlanckianUuid[];
extern const unsigned char characteristicChromaticityCoordinatesUuid[];
extern const unsigned char characteristicChromaticityinCctDuvValuesUuid[];
extern const unsigned char characteristicChromaticityToleranceUuid[];
extern const unsigned char characteristicCie1331995ColorIndexUuid[];
extern const unsigned char characteristicCoefficientUuid[];
extern const unsigned char characteristicCorrelatedColorTemperatureUuid[];
extern const unsigned char characteristicCount16Uuid[];
extern const unsigned char characteristicCount24Uuid[];
extern const unsigned char characteristicCountryCodeUuid[];
extern const unsigned char characteristicDateUtcUuid[];
extern const unsigned char characteristicElectricCurrentUuid[];
extern const unsigned char characteristicElectricCurrentRangeUuid[];
extern const unsigned char characteristicElectricCurrentSpecificationUuid[];
extern const unsigned char characteristicElectricCurrentStatisticsUuid[];
extern const unsigned char characteristicEnergyUuid[];
extern const unsigned char characteristicEnergyInAPeriodOfDayUuid[];
extern const unsigned char characteristicEventStatisticsUuid[];
extern const unsigned char characteristicFixedString16Uuid[];
extern const unsigned char characteristicFixedString24Uuid[];
extern const unsigned char characteristicFixedString36Uuid[];
extern const unsigned char characteristicFixedString8Uuid[];
extern const unsigned char characteristicGenericLevelUuid[];
extern const unsigned char characteristicGlobalTradeItemNumberUuid[];
extern const unsigned char characteristicIlluminanceUuid[];
extern const unsigned char characteristicLuminousEfficacyUuid[];
extern const unsigned char characteristicLuminousEnergyUuid[];
extern const unsigned char characteristicLuminousExposureUuid[];
extern const unsigned char characteristicLuminousFluxUuid[];
extern const unsigned char characteristicLuminousFluxRangeUuid[];
extern const unsigned char characteristicLuminousIntensityUuid[];
extern const unsigned char characteristicMassFlowUuid[];
extern const unsigned char characteristicPerceivedLightnessUuid[];
extern const unsigned char characteristicPercentage8Uuid[];
extern const unsigned char characteristicPowerUuid[];
extern const unsigned char characteristicPowerSpecificationUuid[];
extern const unsigned char characteristicRelativeRuntimeCurrentRangeUuid[];
extern const unsigned char characteristicRelativeRuntimeGenericLevelUuid[];
extern const unsigned char characteristicRelativeValueVoltageRangeUuid[];
extern const unsigned char characteristicRelativeValueIlluminanceUuid[];
extern const unsigned char characteristicRelativeValuePeriodOfDayUuid[];
extern const unsigned char characteristicRelativeValueTemperatureRangeUuid[];
extern const unsigned char characteristicTemperature8Uuid[];
extern const unsigned char characteristicTemperature8InAPeriodOfDayUuid[];
extern const unsigned char characteristicTemperature8StatisticsUuid[];
extern const unsigned char characteristicTemperatureRangeUuid[];
extern const unsigned char characteristicTemperatureStatisticsUuid[];
extern const unsigned char characteristicTimeDecihour8Uuid[];
extern const unsigned char characteristicTimeExponential8Uuid[];
extern const unsigned char characteristicTimeHour24Uuid[];
extern const unsigned char characteristicTimeMillisecond24Uuid[];
extern const unsigned char characteristicTimeSecond16Uuid[];
extern const unsigned char characteristicTimeSecond8Uuid[];
extern const unsigned char characteristicVoltageUuid[];
extern const unsigned char characteristicVoltageSpecificationUuid[];
extern const unsigned char characteristicVoltageStatisticsUuid[];
extern const unsigned char characteristicVolumeFlowUuid[];
extern const unsigned char characteristicChromaticityCoordinateUuid[];
extern const unsigned char characteristicRcFeatureUuid[];
extern const unsigned char characteristicRcSettingsUuid[];
extern const unsigned char characteristicReconnCfgControlPointUuid[];
extern const unsigned char characteristicIddStatusChangedUuid[];
extern const unsigned char characteristicIddStatusUuid[];
extern const unsigned char characteristicIddAnnunciationStatusUuid[];
extern const unsigned char characteristicIddFeaturesUuid[];
extern const unsigned char characteristicIddStatusReaderControlPointUuid[];
extern const unsigned char characteristicIddCommandControlPointUuid[];
extern const unsigned char characteristicIddCommandDataUuid[];
extern const unsigned char characteristicIddRecordAccessControlPointUuid[];
extern const unsigned char characteristicIddHistoryDataUuid[];
extern const unsigned char characteristicClientSupportedFeaturesUuid[];
extern const unsigned char characteristicDatabaseHashUuid[];
extern const unsigned char characteristicBssControlPointUuid[];
extern const unsigned char characteristicBssResponseUuid[];
extern const unsigned char characteristicEmergencyIdUuid[];
extern const unsigned char characteristicEmergencyTextUuid[];
extern const unsigned char characteristicAcsStatusUuid[];
extern const unsigned char characteristicAcsDataInUuid[];
extern const unsigned char characteristicAcsDataOutNotifyUuid[];
extern const unsigned char characteristicAcsDataOutIndicateUuid[];
extern const unsigned char characteristicAcsControlPointUuid[];
extern const unsigned char characteristicEnhancedBpMeasurementUuid[];
extern const unsigned char characteristicEnhancedIntermCuffPressureUuid[];
extern const unsigned char characteristicBloodPressureRecordUuid[];
extern const unsigned char characteristicRegisteredUserUuid[];
extern const unsigned char characteristicBrEdrHandoverDataUuid[];
extern const unsigned char characteristicBluetoothSigDataUuid[];
extern const unsigned char characteristicServerSupportedFeaturesUuid[];
extern const unsigned char characteristicPhysicalAmFeaturesUuid[];
extern const unsigned char characteristicGeneralActivityInstDataUuid[];
extern const unsigned char characteristicGeneralActivitySummaryDataUuid[];
extern const unsigned char characteristicCrActivityInstantaneousDataUuid[];
extern const unsigned char characteristicCrActivitySummaryDataUuid[];
extern const unsigned char characteristicScActivitySummaryDataUuid[];
extern const unsigned char characteristicSleepActivityInstDataUuid[];
extern const unsigned char characteristicSleepActivitySummaryDataUuid[];
extern const unsigned char characteristicPhysicalAmControlPointUuid[];
extern const unsigned char characteristicActivityCurrentSessionUuid[];
extern const unsigned char characteristicPhysicalAsDescriptorUuid[];
extern const unsigned char characteristicPreferredUnitsUuid[];
extern const unsigned char characteristicHighResolutionHeightUuid[];
extern const unsigned char characteristicMiddleNameUuid[];
extern const unsigned char characteristicStrideLengthUuid[];
extern const unsigned char characteristicHandednessUuid[];
extern const unsigned char characteristicDeviceWearingPositionUuid[];
extern const unsigned char characteristicFourZoneHeartRateLimitsUuid[];
extern const unsigned char characteristicHighIntensityExerciseThUuid[];
extern const unsigned char characteristicActivityGoalUuid[];
extern const unsigned char characteristicSedentaryIntervalNotificationUuid[];
extern const unsigned char characteristicCaloricIntakeUuid[];
extern const unsigned char characteristicTmapRoleUuid[];
extern const unsigned char characteristicAudioInputStateUuid[];
extern const unsigned char characteristicGainSettingsAttributeUuid[];
extern const unsigned char characteristicAudioInputTypeUuid[];
extern const unsigned char characteristicAudioInputStatusUuid[];
extern const unsigned char characteristicAudioInputControlPointUuid[];
extern const unsigned char characteristicAudioInputDescriptionUuid[];
extern const unsigned char characteristicVolumeStateUuid[];
extern const unsigned char characteristicVolumeControlPointUuid[];
extern const unsigned char characteristicVolumeFlagsUuid[];
extern const unsigned char characteristicVolumeOffsetStateUuid[];
extern const unsigned char characteristicAudioLocationUuid[];
extern const unsigned char characteristicVolumeOffsetControlPointUuid[];
extern const unsigned char characteristicAudioOutputDescriptionUuid[];
extern const unsigned char characteristicSetIdentityResolvingKeyUuid[];
extern const unsigned char characteristicCoordinatedSetSizeUuid[];
extern const unsigned char characteristicSetMemberLockUuid[];
extern const unsigned char characteristicSetMemberRankUuid[];
extern const unsigned char characteristicCoordinatedSetNameUuid[];
extern const unsigned char characteristicEncryptedDataKeyMaterialUuid[];
extern const unsigned char characteristicApparentEnergy32Uuid[];
extern const unsigned char characteristicApparentPowerUuid[];
extern const unsigned char characteristicLiveHealthObservationsUuid[];
extern const unsigned char characteristicCoConcUuid[];
extern const unsigned char characteristicCosineOfTheAngleUuid[];
extern const unsigned char characteristicDeviceTimeFeatureUuid[];
extern const unsigned char characteristicDeviceTimeParametersUuid[];
extern const unsigned char characteristicDeviceTimeUuid[];
extern const unsigned char characteristicDeviceTimeControlPointUuid[];
extern const unsigned char characteristicTimeChangeLogDataUuid[];
extern const unsigned char characteristicMediaPlayerNameUuid[];
extern const unsigned char characteristicMediaPlayerIconObjectIdUuid[];
extern const unsigned char characteristicMediaPlayerIconUrlUuid[];
extern const unsigned char characteristicTrackChangedUuid[];
extern const unsigned char characteristicTrackTitleUuid[];
extern const unsigned char characteristicTrackDurationUuid[];
extern const unsigned char characteristicTrackPositionUuid[];
extern const unsigned char characteristicPlaybackSpeedUuid[];
extern const unsigned char characteristicSeekingSpeedUuid[];
extern const unsigned char characteristicCurrentTrackSegmentsObjectIdUuid[];
extern const unsigned char characteristicCurrentTrackObjectIdUuid[];
extern const unsigned char characteristicNextTrackObjectIdUuid[];
extern const unsigned char characteristicParentGroupObjectIdUuid[];
extern const unsigned char characteristicCurrentGroupObjectIdUuid[];
extern const unsigned char characteristicPlayingOrderUuid[];
extern const unsigned char characteristicPlayingOrdersSupportedUuid[];
extern const unsigned char characteristicMediaStateUuid[];
extern const unsigned char characteristicMediaControlPointUuid[];
extern const unsigned char characteristicMediaCtrlPointOpSupportedUuid[];
extern const unsigned char characteristicSearchResultsObjectIdUuid[];
extern const unsigned char characteristicSearchControlPointUuid[];
extern const unsigned char characteristicEnergy32Uuid[];
extern const unsigned char characteristicMediaPlayerIconObjectTypeUuid[];
extern const unsigned char characteristicTrackSegmentsObjectTypeUuid[];
extern const unsigned char characteristicTrackObjectTypeUuid[];
extern const unsigned char characteristicGroupObjectTypeUuid[];
extern const unsigned char characteristicConstantToneExtensionEnableUuid[];
extern const unsigned char characteristicAdvertisingCteMinLengthUuid[];
extern const unsigned char characteristicAdvertisingCteMinTransmitCntUuid[];
extern const unsigned char characteristicAdvertisingCteTranDurationUuid[];
extern const unsigned char characteristicAdvertisingCteIntervalUuid[];
extern const unsigned char characteristicAdvertisingCtePhyUuid[];
extern const unsigned char characteristicBearerProviderNameUuid[];
extern const unsigned char characteristicBearerUciUuid[];
extern const unsigned char characteristicBearerTechnologyUuid[];
extern const unsigned char characteristicBearerUriSchemesSuppListUuid[];
extern const unsigned char characteristicBearerSsUuid[];
extern const unsigned char characteristicBearerSsReportingIntervalUuid[];
extern const unsigned char characteristicBearerListCurrentCallsUuid[];
extern const unsigned char characteristicContentControlIdUuid[];
extern const unsigned char characteristicStatusFlagsUuid[];
extern const unsigned char characteristicIncomingCallTargetBearerUriUuid[];
extern const unsigned char characteristicCallStateUuid[];
extern const unsigned char characteristicCallCtrlPointUuid[];
extern const unsigned char characteristicCallCtrlPointOptionalOpcodesUuid[];
extern const unsigned char characteristicTerminationReasonUuid[];
extern const unsigned char characteristicIncomingCallUuid[];
extern const unsigned char characteristicCallFriendlyNameUuid[];
extern const unsigned char characteristicMuteUuid[];
extern const unsigned char characteristicSinkAseUuid[];
extern const unsigned char characteristicSourceAseUuid[];
extern const unsigned char characteristicAseControlPointUuid[];
extern const unsigned char characteristicBasControlPointUuid[];
extern const unsigned char characteristicBroadcastReceiveStateUuid[];
extern const unsigned char characteristicSinkPacUuid[];
extern const unsigned char characteristicSinkAudioLocationsUuid[];
extern const unsigned char characteristicSourcePacUuid[];
extern const unsigned char characteristicSourceAudioLocationsUuid[];
extern const unsigned char characteristicAvailableAudioContextsUuid[];
extern const unsigned char characteristicSupportedAudioContextsUuid[];
extern const unsigned char characteristicAmmoniaConcUuid[];
extern const unsigned char characteristicCarbonMonoxideConcUuid[];
extern const unsigned char characteristicMethaneConcUuid[];
extern const unsigned char characteristicNitrogenDioxideConcUuid[];
extern const unsigned char characteristicNonMethaneVolatileOcsConcUuid[];
extern const unsigned char characteristicOzoneConcUuid[];
extern const unsigned char characteristicParticulateMatterPm1ConcUuid[];
extern const unsigned char characteristicParticulateMatterPm25ConcUuid[];
extern const unsigned char characteristicParticulateMatterPm10ConcUuid[];
extern const unsigned char characteristicSulfurDioxideConcUuid[];
extern const unsigned char characteristicSulfurHexafluorideConcUuid[];
extern const unsigned char characteristicHearingAidFeaturesUuid[];
extern const unsigned char characteristicHearingAidPresetControlPointUuid[];
extern const unsigned char characteristicActivePresetIndexUuid[];
extern const unsigned char characteristicStoredHealthObservationsUuid[];
extern const unsigned char characteristicFixedString64Uuid[];
extern const unsigned char characteristicHighTemperatureUuid[];
extern const unsigned char characteristicHighVoltageUuid[];
extern const unsigned char characteristicLightDistributionUuid[];
extern const unsigned char characteristicLightOutputUuid[];
extern const unsigned char characteristicLightSourceTypeUuid[];
extern const unsigned char characteristicNoiseUuid[];
extern const unsigned char characteristicRelativeRuntime2Uuid[];
extern const unsigned char characteristicTimeSecond32Uuid[];
extern const unsigned char characteristicVocConcUuid[];
extern const unsigned char characteristicVoltageFrequencyUuid[];
extern const unsigned char characteristicBatteryCriticalStatusUuid[];
extern const unsigned char characteristicBatteryHealthStatusUuid[];
extern const unsigned char characteristicBatteryHealthInformationUuid[];
extern const unsigned char characteristicBatteryInformationUuid[];
extern const unsigned char characteristicBatteryLevelStatusUuid[];
extern const unsigned char characteristicBatteryTimeStatusUuid[];
extern const unsigned char characteristicEstimatedServiceDateUuid[];
extern const unsigned char characteristicBatteryEnergyStatusUuid[];
extern const unsigned char characteristicObservationScheduleChangedUuid[];
extern const unsigned char characteristicCurrentElapsedTimeUuid[];
extern const unsigned char characteristicHealthSensorFeaturesUuid[];
extern const unsigned char characteristicGhsControlPointUuid[];
extern const unsigned char characteristicLeGattSecurityLevelsUuid[];
extern const unsigned char characteristicEslAddressUuid[];
extern const unsigned char characteristicApSyncKeyMaterialUuid[];
extern const unsigned char characteristicEslResponseKeyMaterialUuid[];
extern const unsigned char characteristicEslCurrentAbsoluteTimeUuid[];
extern const unsigned char characteristicEslDisplayInformationUuid[];
extern const unsigned char characteristicEslImageInformationUuid[];
extern const unsigned char characteristicEslSensorInformationUuid[];
extern const unsigned char characteristicEslLedInformationUuid[];
extern const unsigned char characteristicEslControlPointUuid[];
extern const unsigned char characteristicUdiForMedicalDevicesUuid[];
extern const unsigned char characteristicGmapRoleUuid[];
extern const unsigned char characteristicUggFeaturesUuid[];
extern const unsigned char characteristicUgtFeaturesUuid[];
extern const unsigned char characteristicBgsFeaturesUuid[];
extern const unsigned char characteristicBgrFeaturesUuid[];
//TODO: Ranging profile has not been confirmed yet. by junhui.hu
extern const unsigned char characteristicRasFeatureUuid[];
extern const unsigned char characteristicLiveRangingDataUuid[];
extern const unsigned char characteristicStoredRangingDataUuid[];
extern const unsigned char characteristicControlPointUuid[];
extern const unsigned char characteristicRangingDataReadyUuid[];
extern const unsigned char characteristicRangingDataOverwrittenUuid[];
//TODO: Ultra Low Latency HID service has not been confirmed yet. by qihang.mou
extern const unsigned char characteristicUllHidPropertiesUuid[];
extern const unsigned char characteristicLeHidOperationModeUuid[];

