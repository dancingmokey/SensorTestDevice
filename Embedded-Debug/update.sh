#~/bin/bash

killall SensorTestDevice

sftp liulei@192.168.2.112 << EOF


cd ~/Workspace/SensorTestDevice/Embedded-Debug
get SensorTestDevice

exit

EOF

cd /bin 
. setqt4env

cd /work
./SensorTestDevice -qws
