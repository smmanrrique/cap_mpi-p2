CSV_NAME="broadcast_exp_new"
NUMBER_PACKETS=10

echo "EXPERIMENT,BCAST_TYPE,NPROC,PACKET_SIZE,N_PACKETS,N_BOUNCES,TYPE,NODE,PROCESS,SRC,DST,TAG,COM_TIME,RUNNING_TIME" >> $CSV_NAME"_"$NUMBER_PACKETS".csv"
# for fname in broadcast_test_*proc_*bcast_500size.csv
for fname in ./broadcast/broadcast_test_10experiments_*proc_*bcast_*size.csv
    do
        tail -n+2 $fname >> $CSV_NAME"_"$NUMBER_PACKETS".csv"
    done
