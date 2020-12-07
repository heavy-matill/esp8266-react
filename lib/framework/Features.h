#ifndef Features_h
#define Features_h

#define FT_ENABLED(feature) feature
#define FT_ENABLED2(feature) (feature == 1)

// project feature off by default
#ifndef FT_PROJECT
#define FT_PROJECT 0
#endif

// security feature on by default
#ifndef FT_SECURITY
#define FT_SECURITY 1
#endif

// mqtt feature on by default
#ifndef FT_MQTT
#define FT_MQTT 1
#endif

// ntp feature on by default
#ifndef FT_NTP
#define FT_NTP 1
#endif

// mqtt feature on by default
#ifndef FT_OTA
#define FT_OTA 1
#endif

// upload firmware feature off by default
#ifndef FT_UPLOAD_FIRMWARE
#define FT_UPLOAD_FIRMWARE 0
#endif

// custom RGB features
// matrix feature for e.g. displaying text 
// off by default
#ifndef FT_RGB_MATRIX
#define FT_RGB_MATRIX 0
#endif
// line feature for custom animations along line and custom fill algorithms along line_xy 
// on by default
#ifndef FT_RGB_LINE
#define FT_RGB_LINE 1
#endif

#endif
