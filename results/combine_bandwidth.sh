# head -n1 file1.txt > combined.txt
echo "BCAST_TYPE,PACKET_SIZE,N_PACKETS,N_BOUNCES,NODE,PROCESS,SRC,DST,TAG,COM_TIME,RUNNING_TIME" > bandwidth.csv
for fname in bandwidth_test_8*.csv 
do
    tail -n+2 $fname >> bandwidth.csv
done