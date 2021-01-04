
PROGRAM="latency.out"
CSV_FILE="latency_test.csv"
HOSTFILE="host_lab004.txt"
PACKET_SIZES=(50)
NUMBER_BOUNCES=(2)
NUMBER_PROCCESS=(8)


for p in "${NUMBER_PROCCESS[@]}"
    do
        echo "PACKET_SIZE,N_BOUNCES,TYPE,SRC,DST,TAG,BOUNCE_TIME" > ""$p"proc_"$CSV_FILE"";
        for j in "${NUMBER_BOUNCES[@]}"
            do
                for i in "${PACKET_SIZES[@]}" 
                    do
                        # echo "mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$i" "$j"";
                        mpirun -n "$p" --hostfile "$HOSTFILE" -mca btl_tcp_if_include br0 "$PROGRAM" "$i" "$j" >> ""$p"proc_"$CSV_FILE"";
                    done
            done
    done