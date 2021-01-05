CSV_NAME="broadcast"
NUMBER_PACKETS=1000

echo "BCAST_TYPE,NPROC,PACKET_SIZE,N_PACKETS,N_BOUNCES,TYPE,NODE,PROCESS,SRC,DST,TAG,COM_TIME,RUNNING_TIME" >> $CSV_NAME"_"$NUMBER_PACKETS"pkt.csv"
for fname in broadcast_test_*proc_*bcast_500size.csv
    do
        tail -n+2 $fname >> $CSV_NAME"_"$NUMBER_PACKETS"pkt.csv"
    done