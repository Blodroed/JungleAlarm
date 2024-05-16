#include <iostream>
#include "json.hpp"
#include "ISM43362.h"
#include "ipgeolocation_certificate.h"

#define JSON_NOEXCEPTION

class menu{
public:

    void sending_request();

    void longitude_latitude();

    void geolocation();

    void unix_epoch_time();
};