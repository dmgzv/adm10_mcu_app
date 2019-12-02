#!/bin/sh

SLOT=1
if [[ "$#" -ge "1" ]]; then
        if [[ "$1" -ge "1" && "$1" -le "12" ]]; then
                SLOT=$1
        else
                echo "Invalid slot: 1 <= slot <= 12"
                exit
        fi
fi

ADDR=$(((((SLOT * 2) - 1) * 8) + 40))
IP="192.168.56.${ADDR}"
PORT=50000

echo "Slot: ${SLOT}"
echo "IP : ${IP}"
echo "Port : ${PORT}"

/usr/bin/horizon-device-api-client $IP $PORT get_param /__HIDDEN__,admin-state,service-state
/usr/bin/horizon-device-api-client $IP $PORT get_param /adm-10-2019-11-28,serial-number,vendor,part-number,hardware-number,software-number

/usr/bin/horizon-device-api-client $IP $PORT get_param /adm-10-2019-11-28/adm,admin-state,service-state

for i in $(seq 1 1 4)
do
        /usr/bin/horizon-device-api-client $IP $PORT get_param /adm-10-2019-11-28/adm/line[$i],admin-state,service-state,signal-type,module-type
        /usr/bin/horizon-device-api-client $IP $PORT get_param /adm-10-2019-11-28/adm/line[$i]/sfp,serial-number,vendor,part-number
done


for i in $(seq 1 1 16)
do
        /usr/bin/horizon-device-api-client $IP $PORT get_param /adm-10-2019-11-28/adm/client[$i],admin-state,service-state,signal-type,module-type
        /usr/bin/horizon-device-api-client $IP $PORT get_param /adm-10-2019-11-28/adm/client[$i]/sfp,serial-number,vendor,part-number
done
